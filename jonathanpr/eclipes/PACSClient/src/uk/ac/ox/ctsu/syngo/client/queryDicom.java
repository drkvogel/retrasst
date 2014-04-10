package uk.ac.ox.ctsu.syngo.client;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.rmi.RemoteException;
import java.security.InvalidKeyException;
import java.security.KeyStore;
import java.security.KeyStoreException;
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
import java.util.Enumeration;
import java.util.List;
import java.util.TreeMap;
import java.util.Vector;

import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.xml.stream.XMLStreamException;

import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.DicomStudyKeywords;
import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.KeywordInformation;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomDocumentFuzzySearchResult;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomImage;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomSeries;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomStudy;

//The java program queries syngo.share by looking in a defined set of Authority Units for new studies.
//It finds out when it last did a scan, then retrieves ALL dicom studies from two weeks previous, adding
//all new studies to the database. 

public class queryDicom
{
// The AU's to look at for new studies
// "BIOBANK" 		- Where studies are placed once QA checked and is ready for downloading.
// "BIOBANK_PSI" 	- Where studies are copied if something which needs following up.
// "1K_SCANS" 		- Once the name is finalised, and in the system, it can be put in the list below
	private static String[] AUList = {"BIOBANK","BIOBANK_PSI"};//,"1K_SCANS"};

	private static database m_db = null;
	private static String m_patientID = null;
	private static String m_studyDate = null;

	private static String destination = "download/"; //location to where the DICOM data is downloaded too
	
	/************************************************************************************
	*                                       PUBLIC FUNCTIONS							*
	************************************************************************************/
	public static void main(String[] args)
	{	
		//make sure we aren't already running
		singleAppInstance.instance(singleAppInstance.CMDQUERY);
				
		PACSComms m_PC;
		//set lasttime so if it's not in the database, it will search far back
		Calendar m_lastDownload = null;
		
		//setup the Syngo.share comms, database and PDF creator
		try
		{
			m_db = database.instancebb5_extract_pacs_live(false);
			m_PC = new PACSComms();
			if (!m_PC.signin())
				throw new Exception("Error signing into PACS");
		}
		catch (Exception e)
		{
			logs.logfile(logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Initalising database error: " + e.getMessage(),null,null );
			return;
		}

		try
		{
		//update the last time this CRON job was ran, this is visible from the web site status page	
			m_db.updateRunTime("QUERY", true);
			
			//Retrieve from the database the last time this was successfully run, so we can limit the overlap slightly when we query the PACS
			m_lastDownload = getLastUpdate();
			//if no value, start from the start of the year.
			if (m_lastDownload == null) 
			{	
				m_lastDownload = Calendar.getInstance();
				m_lastDownload.set(2014, 0, 1);
			}
		}
		catch (Exception e)
		{
			logs.log(m_db, logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Reading from database error: " + e.getMessage(),null,null );
			return;
		}
						
		try
		{
			Calendar toNow = Calendar.getInstance();

			//Get a list of all scans between last time (-2 weeks) and now, which are in the specified AUs.
			DicomDocumentFuzzySearchResult[] DicomStudyResults = m_PC.searchForPatientStudies(m_lastDownload,toNow,AUList);
			if ((DicomStudyResults != null)	&& (DicomStudyResults.length > 0))
			{
				for (int i = 0; i < DicomStudyResults.length; i++)
				{
					long studyPK = DicomStudyResults[i].getDicomStudy().getId();
					String AU = DicomStudyResults[i].getOrganizationalUnit().getName();		
					//if we already have this scan in the database, then skip it.
					if (db_pacsstudy.contains(m_db, studyPK,AU))
						continue;
					try
					{
						//try and match the dicom id's to what we are expecting. If we do, filled in the confirmed
						//id, either way, we add it to the pacsstudy database.
						String []patientID = new String[2];
						String []firstname = new String[2];
						
						patientID[0] = "";
						firstname[0] = "";
						patientID[1] = "";
						firstname[1] = "";
											
						patientID[0] = DicomStudyResults[i].getPatient().getPatientId();
						firstname[0] = DicomStudyResults[i].getPatient().getFirstName();
						
						if (patientID[0] == null) //these above return null if no value :(
							patientID[0] = "";
						if (firstname[0] == null) //these above return null if no value :(
							firstname[0] = "";
						
						if (firstname[0].isEmpty())
							firstname[0] = DicomStudyResults[i].getPatient().getLastName();
						if (firstname[0] == null) //these above return null if no value :(
							firstname[0] = "";
						
						m_db.hasMatchOnPatientID(patientID,firstname);
						
						if (!patientID[1].isEmpty()) 
							firstname[0]= firstname[1]; 			
							
						Long patientpk = DicomStudyResults[i].getPatient().getId();
						Calendar StudyDate = DicomStudyResults[i].getDocument().getDocumentCreatedWhen(); //check this	
						String studyUid = DicomStudyResults[i].getDicomStudy().getStudyInstanceUid();
						String AET = DicomStudyResults[i].getDicomStudy().getCallingAet();

						String Modalities = DicomStudyResults[i].getDicomStudy().getAllModalities();			
											
						db_pacsstudy sr = db_pacsstudy.instance(m_db, studyPK, patientpk, patientID[0], patientID[1], firstname[0],StudyDate, studyUid, AET, AU, Modalities);
						sr.insert();
					}
					catch(Exception e)
					{
						m_db.rollback(); 
						logs.log(m_db, logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.DATABASE, "Error adding new dicom record: " + e.getMessage(),null,null );
						throw e;
					}
					finally
					{ //COMMIT AS WE DO ALONG..
						m_db.commit();					
					}
				}			
			}
//			update last downloaded value as we are complete
			try
			{
				if (!m_db.updateLastUpdate(toNow))
				{
					if (!m_db.insertLastUpdate(toNow))
						throw new SQLException("failed to update lastdownload");
				}
//				update last time this CRON job was run to completion
				m_db.updateRunTime("QUERY", false);
			}
			catch (SQLException e)
			{
				//if this fails, it's fine, as it means we pick up others we have already checked.
				logs.log(m_db, logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Writing to database error: " + e.getMessage(),null,null );
				return;
			}			
		}
		catch (Exception e)
		{
			logs.log(m_db, logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.PACS, "Error while querying PACS:" + e.getMessage(),m_patientID,m_lastDownload.getTime());
		}
	}
	
	/************************************************************************************
	*                                       PRIVATE FUNCTIONS							*
	************************************************************************************/
	private static Calendar getLastUpdate() throws SQLException
	{
		Calendar lastUpdate = null;
		String sql = "SELECT lastdownload from dicomdownloader";
		ResultSet result = null;
		try
		{
			result = m_db.m_stmt.executeQuery(sql);
			if (result.next())
			{	
				long last = result.getTimestamp("lastdownload").getTime();
				lastUpdate = Calendar.getInstance();				
				lastUpdate.setTimeInMillis(last);
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}	
		return lastUpdate;
	}
}





