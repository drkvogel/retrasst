package uk.ac.ox.ctsu.syngo.client;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.rmi.RemoteException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.InvalidParameterSpecException;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.TreeMap;
import java.util.Vector;

import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.xml.stream.XMLStreamException;

import uk.ac.ox.ctsu.javarosetta.Rosetta;

import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.DicomStudyKeywords;
import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.KeywordInformation;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomDocumentFuzzySearchResult;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomImage;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomSeries;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomStudy;

//Downloads all dicom in "BIOBANK", based on inserttime
public class queryICEQuestionnaires
{
	static database m_dbPacsclinicdata = null;	
	static database m_dbAceIceLive = null;
	static database m_dbextract_pacs_ddb = null;
	/************************************************************************************
	*                                       PUBLIC FUNCTIONS							*
	************************************************************************************/
	public class NewQuestionData
	{
		public int cid;
		public int trid;
		public int pid;
		public Date StudyDate;
		public String dicomid;
		public String firstname;
	}
	
	
	public static void main(String[] args)
	{	
		//make sure we aren't already running
		singleAppInstance.instance(singleAppInstance.CMDQUERYQUESTIONNAIRE);

		//setup the database 
		try
		{
			m_dbextract_pacs_ddb = database.instancebb5_extract_pacs_ddb();
			m_dbPacsclinicdata = database.instancebb5_extract_pacs_live(true);	
			m_dbAceIceLive = database.instancebb5_ace_ice_live();
		}
		catch (Exception e)
		{
			logs.logfile(logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Initalising database error: " + e.getMessage(),null,null );
			return;
		}                  
			
		try
		{
			//update the last time this CRON job was ran, this is visible from the web site status page	
			m_dbPacsclinicdata.updateRunTime("QUERYICE", true);
		}
		catch (Exception e)
		{
			logs.log(m_dbPacsclinicdata, logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Writing updateRunTime database error: " + e.getMessage(),null,null );
			return;
		}

			
	//	BELOW IS THE DISTRIBUTED SQL		
		StringBuilder bigQuerySql = new StringBuilder();
		// 61 is "come into the clinic"(?)  - no definitive list of gtypes - ask Alan(!)
		bigQuerySql.append("SELECT pid FROM gobz WHERE gtyp = 61 AND status = 1000 AND pid NOT IN (SELECT pid FROM biobankparticipant)");
		ResultSet result = null;

		Vector<Integer> newPids = new Vector<Integer>(); 
		try
		{
			result = m_dbextract_pacs_ddb.m_stmt.executeQuery(bigQuerySql.toString());
			while (result.next())
				newPids.add(result.getInt("pid"));
			if (result!=null)
				result.close();
		}
		catch (SQLException e)
		{
			logs.log(m_dbPacsclinicdata, logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Exception getting new pid list: " + e.getMessage(),null,null );
			return;			
		}

		Vector<NewQuestionData> newQuestionDataList = new Vector<NewQuestionData>();
		queryICEQuestionnaires x = new queryICEQuestionnaires();
		try
		{
			String query = "SELECT cid,trid,pid,tsig,xub_isr FROM gobz WHERE gtyp = 61 AND status = 1000 AND pid = ?";				
			PreparedStatement pstmt =  m_dbAceIceLive.m_conn.prepareStatement(query);
						
			Iterator<Integer> itr = newPids.iterator();
			while (itr.hasNext())
			{
				pstmt.setInt(1,itr.next());	
				ResultSet res = pstmt.executeQuery();
				while (res.next())
				{
					NewQuestionData question = x.new NewQuestionData();
					question.cid = res.getInt(1);
					question.trid = res.getInt(2); 
					question.pid = res.getInt(3);
					question.StudyDate = res.getDate(4);
					String RossettaData = res.getString(5);
					question.firstname = "";
					question.dicomid = "";
					
					Rosetta R = new Rosetta();
					R.serializeIn(RossettaData);
					try
					{
						R = R.getRosetta("data");
						R = R.getRosetta("ipermit");
						try
						{
							question.dicomid = R.getString("idid"); // this is definitely right(!)
							question.dicomid = question.dicomid.replaceAll("\\s",""); //strip out white space
						}
						catch (NoSuchFieldException e) { }
						try
						{
							question.firstname = R.getString("nmnm");  // Alan is supposed to call it this, should be pulling out the first name
							question.firstname = question.dicomid.replaceAll("\\s",""); //strip out white space
						}
						catch (NoSuchFieldException e) { }
						
						if ((question.firstname.isEmpty()) && (question.dicomid.isEmpty()))
							throw new NoSuchFieldException("No such field");
					}
					catch (NoSuchFieldException e)
					{
						//not in this Rosetta, report...
						logs.log(m_dbPacsclinicdata, logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.GENERAL, "ipermit.idid not present for pid: " + question.pid ,null,null );
						continue;
					}
					newQuestionDataList.add(question);
				}
			}
			pstmt.close();
			pstmt = null;  	
			//this list can be freed
			newPids.clear();
		}
		catch (SQLException e)
		{
			logs.log(m_dbPacsclinicdata, logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Exception getting gob data from ice: " + e.getMessage(),null,null );
			return;						
		}	
		
		try
		{
			String query = "INSERT INTO biobankparticipant (pid,dicomparticipantid,firstnameid, scandate,cid,trid) VALUES(?,?,?,?,?,?)";
			PreparedStatement pstmt =  m_dbPacsclinicdata.m_conn.prepareStatement(query);
		
			try
			{
				Iterator<NewQuestionData> itr2 = newQuestionDataList.iterator();
		 		while (itr2.hasNext())
				{
		 			NewQuestionData NQD = itr2.next();
		 			
					pstmt.setInt(1,NQD.pid);	
					pstmt.setString(2,NQD.dicomid);	
					pstmt.setString(3, NQD.firstname);
					pstmt.setDate(4,new java.sql.Date(NQD.StudyDate.getTime()));	
					pstmt.setInt(5,NQD.cid);	
					pstmt.setInt(6,NQD.trid);	
					pstmt.execute();
					
					if (pstmt.getUpdateCount() != 1)
					{
						//not sure on what kind of error this returns false, and doesn't assert. Or even how you find out why...
						logs.log(m_dbPacsclinicdata, logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Failed to insert into biobankparticipant",null,null );
						return;											
					}
				}
			}
			finally
			{
				pstmt.close();
			}
		}
		catch (SQLException e)
		{
			logs.log(m_dbPacsclinicdata, logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Exception trying to insert into biobankparticipant: " + e.getMessage(),null,null );
			return;						
		}	
		
		try
		{
			m_dbPacsclinicdata.updateRunTime("QUERYICE", false);
		}
		catch (Exception e)
		{
			logs.log(m_dbPacsclinicdata, logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Writing updateRunTime database error: " + e.getMessage(),null,null );
			return;
		}			
	}
		
}





