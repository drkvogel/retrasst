package ctsu.pacs.test;

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

//Downloads all dicom in "BIOBANK", based on inserttime


//java -Djavax.net.debug=ssl -Djavax.net.ssl.keyStore=cacerts -Djavax.net.ssl.keyStorePassword=changeit -Djavax.net.ssl.trustStorePath="C:\Program Files (x86)\Java\jdk1.6.0_26\jre\lib\security\cacerts" -Djavax.net.ssl.trustStorePassword=changeit -jar queryPACS.jar
public class cmdLineQueryPACS
{
	static database m_db = null;
	static String m_patientID = null;
	static String m_studyDate = null;

	static String destination = "download/";

	public static void main(String[] args)
	{	
		singleAppInstance.instance(singleAppInstance.CMDQUERY);
				
		PACSComms m_PC;
		//set lasttime so if it's not in the database, it will search far back
		Calendar m_lastDownload = null;
		
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

/*	TEMP CODE TO UPLOAD A DICOM FILE TO THE PACS	
		try
		{
			m_PC.importDicomToPatient("C:\\Users\\jonathanp\\Desktop\\1.3.6.1.4.1.9784.10.2.26.2.22603.dcm");
		}
		catch (Exception e1)
		{
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
*/		
		/*
		int a1 = m_db.testGetDistanceMatch("PATIENTI", "FIRSTNAM", "PATIENTI", "FIRSTNAM");
		int a2 = m_db.testGetDistanceMatch("PATIENTI", "firstnam", "PATIENTI", "FIRSTNAM");
		int a3 = m_db.testGetDistanceMatch("PatientTI", "FIRSTNAM", "PATIENTI", "FIRSTNAM");
		int a4 = m_db.testGetDistanceMatch("CATIENTI", "SIRSTNAM", "PATIENTI", "FIRSTNAM");
		int a5 = m_db.testGetDistanceMatch("PATIENTI23", "FIRSTNAMdd", "PATIENTI", "FIRSTNAM");
		int a6 = m_db.testGetDistanceMatch("PIENTI", "FISTNAM", "PATIENTI", "FIRSTNAM");
		int a7 = m_db.testGetDistanceMatch("PATI", "FIRS", "PATIENTI", "FIRSTNAM");
		int a8 = m_db.testGetDistanceMatch("P22222222", "R4444444", "PATIENTI", "FIRSTNAM");
		int a9 = m_db.testGetDistanceMatch("????????", "??????????", "PATIENTI", "FIRSTNAM");
*/
		
		
		try
		{
			m_db.updateRunTime("QUERY", true);
			m_lastDownload = m_db.getLastUpdate();
			if (m_lastDownload == null) //if no value, start from the start of the year.
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
						
		//now we have the time of the last DICOM successfully down loaded, subtract a small amount, for somekind of overlap. and search from that point
		try
		{
			//
			 // "BIOBANK" - download data and keywords
			 // "BIOBANK_PSI" - Upload quiz answers
			// "REPORTED" - if keyword says it needs referring, generate patient and doc letter. Optionally tag it as letter created. 
			 // "SCAN_REVIEW" - Upload quiz answers 
			 //
			String[] AUList = {"BIOBANK","REPORTED","BIOBANK_PSI"};//,"1K_SCANS"};
			Calendar toNow = Calendar.getInstance();
			
			DicomDocumentFuzzySearchResult[] DicomStudyResults = m_PC.searchForPatientStudies(m_lastDownload,toNow,AUList);
			if ((DicomStudyResults != null)	&& (DicomStudyResults.length > 0))
			{
				for (int i = 0; i < DicomStudyResults.length; i++)
				{
					long studyPK = DicomStudyResults[i].getDicomStudy().getId();
					String AU = DicomStudyResults[i].getOrganizationalUnit().getName();					
					if (studyRecord.contains(m_db, studyPK,AU))
						continue;
					try
					{
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
						
						m_db.MatchOnPatientID(patientID,firstname);
						
						if (!patientID[1].isEmpty()) //correct first name
							firstname[0]= firstname[1]; 			
							
						Long patientpk = DicomStudyResults[i].getPatient().getId();
						Calendar StudyDate = DicomStudyResults[i].getDocument().getDocumentCreatedWhen(); //check this	
						String studyUid = DicomStudyResults[i].getDicomStudy().getStudyInstanceUid();
						String AET = DicomStudyResults[i].getDicomStudy().getCallingAet();

						String Modalities = DicomStudyResults[i].getDicomStudy().getAllModalities();			
						
						studyRecord sr = studyRecord.instance(m_db, studyPK, patientpk, patientID[0], patientID[1], firstname[0],StudyDate, studyUid, AET, AU, Modalities);
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
//			update last downloaded
			try
			{
				if (!m_db.updateLastUpdate(toNow))
				{
					if (!m_db.insertLastUpdate(toNow))
						throw new SQLException("failed to update lastdownload");
				}
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
			e.printStackTrace();
			logs.log(m_db, logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error while querying PACS:" + e.getMessage(),m_patientID,m_lastDownload.getTime());
		}
	}
}





