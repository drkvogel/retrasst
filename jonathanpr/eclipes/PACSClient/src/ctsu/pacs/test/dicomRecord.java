package ctsu.pacs.test;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.DateFormat;
import java.util.Calendar;

import javarosetta.Rosetta;

public class dicomRecord
{	
	final int FINDINGSTATUS_NONE = 1;
	final int FINDINGSTATUS_DICOMUPLOADED = 2;
	final int FINDINGSTATUS_RESULT_ALL_CLEAR = 3;
	final int FINDINGSTATUS_RESULT_NEEDS_ESCALATING = 4;
	final int FINDINGSTATUS_QA_PASS = 5;
	final int FINDINGSTATUS_QA_FAIL = 6;
	
	final int STATUS_OK = 1;
	final int STATUS_INVALID = 99;

	private database m_db;	
	private Calendar m_studydate;
	private String m_PACSURL;
	private String m_patientID;
	private String m_studyID;
	private int m_hasAlertFlag;  //in fact this is a boolean, just ingres don't line boolean
	private String m_radiographerReport;
	private int m_status;  
	private int m_findingsStatus; 
	
	public static dicomRecord instance(database db,String patientID, String Study_UID,Calendar createWhen) throws Exception
	{
		dicomRecord pr = new dicomRecord(db,patientID,Study_UID, createWhen);
		return pr;
	}
	
	public static dicomRecord instance(String patientID, String Study_UID,Calendar createWhen)
	{
		dicomRecord pr = new dicomRecord(patientID,Study_UID, createWhen);
		return pr;
	}
	
	private dicomRecord(String patientID, String Study_UID,Calendar createWhen)
	{
		m_db = null;
		m_patientID = patientID;
		m_studyID = Study_UID;	
		m_hasAlertFlag = 0;
		m_radiographerReport = "";
		m_status = STATUS_OK;
		m_findingsStatus = FINDINGSTATUS_NONE;
		m_PACSURL = "";
		m_studydate = createWhen;
	}
	
	private dicomRecord(database db,String patientID, String Study_UID,Calendar createWhen) throws Exception
	{
		this(patientID, Study_UID,createWhen);
		m_db = db;
		loadDB();
	}
	
	public boolean hasPDFsUploaded()
	{
		return (m_findingsStatus != FINDINGSTATUS_NONE);
	}
		
	public boolean hasPSIAlert()
	{
		return m_hasAlertFlag == 1;
	}
	
	public String[] getAttachedFindingsReport()
	{
		return m_radiographerReport.split("\\r?\\n");	
	}
	
	public void setPSIAlert(String report) throws Exception
	{
		m_hasAlertFlag = 1;
		m_radiographerReport = report;
		save();
	}
	
	public void updateFindingsStatus_QAPass() throws Exception
	{
		m_findingsStatus = FINDINGSTATUS_QA_PASS;
		save();
	}

	public void updateFindingsStatus_QAFail() throws Exception
	{
		m_findingsStatus = FINDINGSTATUS_QA_FAIL;
		save();
	}
	
	public void updateFindingsStatus_dicomUpLoaded(String pacsURL) throws Exception 
	{
		m_findingsStatus = FINDINGSTATUS_DICOMUPLOADED;
		m_PACSURL = pacsURL;
		save();
	}

	private void save() throws Exception
	{
		if (m_db!=null)
		{
			if (!updateDB())
			{
				if (!insertDB())
					throw new Exception("Problem adding to the database");
			}
		}
		else
			saveToCGI();
	}	
	
	private void loadDB() throws Exception
	{
//		String sql = "SELECT * FROM dicomrecord WHERE patientid = '"+m_patientID+"' AND studyid = '"+m_studyID+"'";
		String sql = "SELECT * FROM dicompacs WHERE patientid = '"+m_patientID+"' AND studyid = '"+m_studyID+"'";
			
		ResultSet result = null;
		try
		{
			result = m_db.m_stmt.executeQuery(sql);
			if (result.next())
			{	
				m_hasAlertFlag = result.getInt("alertflag");
//				m_radiographerReport = result.getString("radiographerreport");
//				m_radiographerReport = result.getClob("radiographerreport").toString();
				m_status = result.getInt("status");
				m_findingsStatus = result.getInt("findingsstatus");
				m_PACSURL = result.getString("pacsurl");
				m_studydate.setTime(result.getTimestamp("studydate"));
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}
	}
	
	private void saveToCGI() throws Exception
	{
		Rosetta r = new Rosetta();
		r.setString("patientid", m_patientID);
		r.setString("studyid", m_studyID);
		r.setString("radiographerreport", m_radiographerReport);
		r.setInt("alertflag", m_hasAlertFlag);
		r.setInt("status", m_status);
		r.setInt("findingsstatus", m_findingsStatus);
		r.setString("pacsurl", m_PACSURL);
		r.setDate("studydate", m_studydate.getTime());
		
		Rosetta rContiner = new Rosetta();
		rContiner.setRosetta("insertDicomRecord", r);
				
		String seralisedRosetta = rContiner.serializeOut();		
		//HEX IT
		
		StringBuffer hexString = new StringBuffer();
		for (int i=0;i<seralisedRosetta.length();i++) 
		{
			String hex = Integer.toHexString(0xFF & seralisedRosetta.charAt(i));
			if(hex.length()==1) 
				hexString.append('0');
			hexString.append(hex);
		}		
		seralisedRosetta = "rosetta="+hexString;
		
		//now send to CGI
		String hexresponse = httpPost.sendGetRequest(httpPost.CGIURL, seralisedRosetta);
		System.out.println(hexresponse);
		
		if (!(hexresponse.startsWith("<HTML><BODY>") && hexresponse.endsWith("</BODY></HTML>")))
		{
			//grabage back from CGI call... 
			throw new Exception("garbage");
		}

		//get the data between body> and </body
		hexresponse = hexresponse.substring(12, hexresponse.length() - 14);
							
		//UNHEX response
		StringBuilder sb = new StringBuilder();
		StringBuilder temp = new StringBuilder();
	 
		for( int i=0; i<hexresponse.length()-1; i+=2 )
		{			 
		     //grab the hex in pairs
			String output = hexresponse.substring(i, (i + 2));
			//convert hex to decimal
			int decimal = Integer.parseInt(output, 16);
			//convert the decimal to character
		    sb.append((char)decimal);
	 
		    temp.append(decimal);
		}
	
		String res = sb.toString();
			
		Rosetta in = new Rosetta();
		in.serializeIn(res);
		  
		if ((in.isRosetta("ERROR") || (!in.isRosetta("OK"))))
		{
			//Badness...
			throw new Exception("garbage");
		}		
	}
	
	private boolean updateDB() throws Exception
	{
		
//		String query = "UPDATE dicomrecord SET alertflag = ?, radiographerreport = ?, pacsurl = ?, status = ?, findingsstatus = ?, studydate = ?";
		String query = "UPDATE dicompacs SET alertflag = ?, radiographerreport = ?, pacsurl = ?, status = ?, findingsstatus = ?, studydate = ?";
		query += " WHERE patientid = '" + m_patientID + "' AND studyid = '" + m_studyID + "'";
		    
		PreparedStatement pstmt =  m_db.m_conn.prepareStatement(query);
		pstmt.setInt(1, m_hasAlertFlag);
		pstmt.setString(2, m_radiographerReport);
		pstmt.setString(3, m_PACSURL);
		pstmt.setInt(4, m_status);
		pstmt.setInt(5, m_findingsStatus);
		pstmt.setTimestamp(6, new java.sql.Timestamp(m_studydate.getTime().getTime()));
		
		try
		{		
			pstmt.execute();
			return (pstmt.getUpdateCount()>=1);
		}
		catch(Exception e)
		{
			m_db.rollback();
			throw new Exception("Error executing statment : " + e.getMessage());
		}
		finally
		{
			m_db.commit();
			pstmt.close();
		}	
	}
	
	private boolean insertDB() throws Exception
	{
		String query = "INSERT INTO dicompacs (patientid,studyid,alertflag,radiographerreport,pacsurl,status,findingsstatus,studydate) VALUES( ?, ?, ?, ?, ?, ?, ?, ?)";
				
		PreparedStatement pstmt =  m_db.m_conn.prepareStatement(query);		
		pstmt.setString(1, m_patientID);
		pstmt.setString(2, m_studyID);
		pstmt.setInt(3, m_hasAlertFlag);
		pstmt.setString(4, m_radiographerReport);
		pstmt.setString(5, m_PACSURL);
		pstmt.setInt(6, m_status);
		pstmt.setInt(7, m_findingsStatus);
		pstmt.setTimestamp(8, new java.sql.Timestamp(m_studydate.getTime().getTime()));
		
		try
		{		
			pstmt.execute();
			return (pstmt.getUpdateCount()>=1);
		}
		catch(Exception e)
		{
			m_db.rollback();
			throw new Exception("Error executing statment : " + e.getMessage());
		}
		finally
		{
			m_db.commit();
			pstmt.close();
		}	
	}
}
