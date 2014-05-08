package uk.ac.ox.ctsu.syngo.client;

import java.io.File;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Calendar;
import java.util.Vector;

import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.DicomStudyKeywords;
import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.KeywordInformation;

import uk.ac.ox.ctsu.syngo.client.pdfwriter.Paragraph;

/*
 * This is the java process which is responsible for collecting the ICE questionnaire, create a PDF and uploading
 * it to the study in question. These studies are in defined AU's, currently BIOBANK_PSI and 1K_SCANS.
 */
public class uploadClinical
{	
	/************************************************************************************
	*                                       MAIN FUNCTION								*
	************************************************************************************/		
	public static void main(String[] args)
	{		
		String[] AUList = /*{"BIOBANK_PSI"};*/ {"IF_ABDO","IF_CARDIAC","IF_DEXA","IF_NEURO"};
		//THE PK OF THE KEYWORD, WHICH TRIGGERS THE CREATION OF A PDF WHEN WE DON'T HAVE A FULL SET OF DATA
		int IGNORE_INCOMPLETE_ICE_DATA = -87; //Temporally - a void key PK
		
		//make sure we aren't already running
		singleAppInstance.instance(singleAppInstance.CMDINCIDENT);
	
		//setup the Syngo.share comms, database and PDF creator
		pdfwriter pdf = new pdfwriter();
		PACSComms PC = new PACSComms();
		database m_db = null;
		try
		{
			m_db = database.instancebb5_extract_pacs_live(false);
			if (!PC.signin())
				throw new Exception("Error signing into PACS");
			//update the last time this CRON job was ran, this is visible from the web site status page	
			m_db.updateRunTime("CLINICAL", true);
		}
		catch (Exception e)
		{
			logs.logfile(logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Initalising error: " + e.getMessage(),null,null );
			return;
		}	
						
		//get all the dicom in the database, status 1, get keywords and save data.
			/*
			 * "BIOBANK" - download data and keywords
			 */
/***********************************************************************************************\		
 		//TESTING, print to the debug console the contents of a clinical report
		questionaire q;
 		try
 		{
 			q = new questionaire("182431106");
 			q.generateReport();		
 		}
 		catch (Exception e4)
 		{
 			// TODO Auto-generated catch block
 			e4.printStackTrace();
 		}
//***********************************************************************************************/
		
		Vector<Long> studypkVector = new Vector<Long>();
		Vector<pair<Integer,String>> quizVector = new Vector<pair<Integer,String>>();
		try
		{
			getStudyPKsFromAUs(m_db, studypkVector,AUList);

			for (int i=0;i<studypkVector.size();i++)
			{
				db_pacsstudy sr = null;
				try
				{
					sr = db_pacsstudy.instance(m_db,studypkVector.get(i));
				}
				catch (Exception e2)
				{
					logs.log(m_db,logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Study record not found for StudyPK ["+studypkVector.get(i)+"], this needs investigating: " + e2.getMessage(),null,null);					
					continue;
				}
				//try and find his/her quiz answers
				int iParticitpentID = 0;
				if (sr.getConfirmedPatientID().length() == 0)
				{
					try
					{
						String []patientID = new String[2];
						String []firstname = new String[2];
						patientID[0] = sr.getPatientID();
						firstname[0] = sr.getName();
						if (patientID[0] == null)
							patientID[0] = "";
						if (firstname[0] == null)
							firstname[0] = "";
						iParticitpentID = m_db.hasMatchOnPatientID(patientID,firstname);
	
						if (patientID[1].isEmpty())
						{
							checkAndSendAlert(m_db, sr);
							continue;
						}
						sr.setConfirmedPatientID(patientID[1]);
						sr.setConfirmedName(firstname[1]);
						sr.update();
					}
					catch(Exception e)
					{
						m_db.rollback(); 
						logs.log(m_db, logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.DATABASE, "Error adding new dicom record: " + e.getMessage(),null,null );
						continue;
					}
					finally
					{ //COMMIT AS WE DO ALONG..
						m_db.commit();					
					}
				}
				else
					iParticitpentID = m_db.getPatientPid(sr.getConfirmedPatientID(),sr.getConfirmedName());
			
				if (iParticitpentID == 0)
				{
					logs.log(m_db, logs.LOG_LEVEL.INFO, logs.LOG_TYPE.GENERAL, "No ICE Pid found",sr.getConfirmedPatientID(),sr.getStudyDate().getTime() );
					continue;
				}
				
				String particitpentID = String.valueOf(iParticitpentID);
				
				
				quizVector.clear();
				try
				{
					questionaire q1;
					q1 = new questionaire(particitpentID);
					if (!q1.hasCompleteData())
					{
						boolean hasProcessAnyWaykeyword = false;
						//IF KEYWORD "PARTICIPANT COMPLETE" or what ever it will be called.. PROCESS ANYWAY
						DicomStudyKeywords[] PACSkeywords = PC.getForPatientStudyKeyWords(sr.getStudyPK());
						
						if (PACSkeywords != null)
						{
							for (int i1=0;i1<PACSkeywords.length;i1++)
							{
								//kwi is holding the keyword information for this study.
								//to find the text string to the keywordPK, use kwi[i2].setKeywordPath() and step
								//though till you find the one you are looking for. Of course, tagging the study with in 
								//BIOBANK_PSI is the first step, so it can be found. 
								KeywordInformation[] kwi = PACSkeywords[i1].getKeywordInformation();
								for (int i2=0;i2<kwi.length;i2++)
								{									
									long Keypk = kwi[i2].getKeywordPk();
									if (Keypk == IGNORE_INCOMPLETE_ICE_DATA)
										hasProcessAnyWaykeyword = true;
								}
							}
						}
						if (!hasProcessAnyWaykeyword)
						{
							checkAndSendAlert(m_db, sr);
							continue;
						}
					}
					quizVector = q1.getReportStrings(sr);
				}
				catch (SQLException e1)
				{
					logs.log(m_db,logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.DATABASE, "Error getting quiz for patient: " + e1.getMessage(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());					
					continue;
				}
				if (quizVector.size() == 0)
				{
					logs.log(m_db,logs.LOG_LEVEL.INFO, logs.LOG_TYPE.GENERAL, "No Quiz found for PatientID",sr.getConfirmedPatientID(),sr.getStudyDate().getTime());					
					continue;
				}
				
				StringBuilder patientQuizFilenameBuilder = new StringBuilder();
				patientQuizFilenameBuilder.append(sr.getConfirmedPatientID());
				patientQuizFilenameBuilder.append("_");
				patientQuizFilenameBuilder.append(studypkVector.get(i));
				patientQuizFilenameBuilder.append("_Report.pdf");	
								
			 	try
				{		
					pdf.Open(particitpentID,sr.getConfirmedPatientID(),sr.getConfirmedName());
					for (int ii=0;ii<quizVector.size();ii++)
					{
						Paragraph p1 = pdf. new Paragraph(quizVector.get(ii).left + 20, 700-(ii*12), quizVector.get(ii).right);
						pdf.write(p1);
					}
					pdf.SaveClose(patientQuizFilenameBuilder.toString());
				}
				catch (Exception e)
				{
					logs.log(m_db,logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.FILESYSTEM, "Error generating/saving PDF (" + patientQuizFilenameBuilder.toString() + "): " + e.getMessage(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());
					continue;
				}		

			 	try
				{			 		
					//this should throw if it fails.. according to the docs
					PC.importFile(patientQuizFilenameBuilder.toString(), sr.getPatientID(), sr.getName(),patientQuizFilenameBuilder.toString() , patientQuizFilenameBuilder.toString(),sr.getAU(),sr.getPatientPK());
				}
				catch (Exception e)
				{
					logs.log(m_db,logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error uploading PDF to PACS: " + e.getMessage(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());
					continue;
				}
				//now delete the PDF
				File pdffile = new File(patientQuizFilenameBuilder.toString());
				if (!pdffile.delete())
					logs.log(m_db,logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Failed to delete local PDF report: " + patientQuizFilenameBuilder.toString(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());

				try
				{
					sr.setComplete(); //set the status to two.. COMPLETE
					if (!sr.update())
						throw new Exception("No rows updated");
					m_db.commit();
				}
				catch (Exception e)
				{
					m_db.rollback();
					logs.log(m_db,logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Error updating pacsstudy:" + e.getMessage(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());
					continue;
				}
				
				logs.log(m_db, logs.LOG_LEVEL.INFO, logs.LOG_TYPE.GENERAL, "Uploaded quiz PDF to PACS",sr.getConfirmedPatientID(),sr.getStudyDate().getTime());
			}	
			m_db.updateRunTime("CLINICAL", false);
		}
		catch (Exception e3)
		{
			logs.log(m_db, logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Error while getting studyPKs from db:" + e3.getMessage(),null,null);
		}
	}

	/************************************************************************************
	*                                       PRIVATE FUNCTIONS							
	 * @throws Exception *
	************************************************************************************/
	private static void checkAndSendAlert(database m_db, db_pacsstudy sr)
	{
		if (sr.getLastAlert() != null)
		{
			//Is older then 23 hours. 
			Calendar LastAlert = (Calendar) sr.getLastAlert().clone();
			LastAlert.add(Calendar.DAY_OF_MONTH, 1);
			LastAlert.add(Calendar.HOUR_OF_DAY, -1);
			if (LastAlert.after(Calendar.getInstance()))
			{
				//we have already sent out an alert for this today...
				return;
			}
		}
		
		//if over 24 hours past the series found in BIOBANK_PSI. Send out an alert						
		Calendar C = (Calendar) sr.getStudyDate().clone();
		C.add(Calendar.DAY_OF_MONTH, 1);
		
		if (C.before(Calendar.getInstance()))
		{ //this should be a rare enough event for them not needing to be batched.
			logs.sendIncompleteICEDataAlert(m_db,sr);
			sr.setLastAlertToNow();
			try
			{
				sr.update();
			}
			catch (Exception e)
			{
				//failed to save to database, no need to do anything, as the alert will be repeated next time.
			}
		}
	}

	private static void getStudyPKsFromAUs(database m_db, Vector<Long> studypkVector,String[] AUList) throws SQLException
	{
		StringBuilder sql = new StringBuilder();
		sql.append("SELECT studypk FROM pacsstudy WHERE");
		
		sql.append(" status = 1 and (");

		for (int i=0;i<AUList.length;i++)
		{
			if (i>0)
				sql.append(" OR ");
			sql.append(" au ='" + AUList[i] + "'");			
		}
		sql.append(" )");
		
		ResultSet result = null;
		try
		{
			result = m_db.m_stmt.executeQuery(sql.toString());
			while (result.next())
			{	
				long keywordpk = result.getLong("studypk");
				studypkVector.add(keywordpk);
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}
	}

	
/////////////////////////TEST////////////////////////////////		
/*		String ParticipentID = "903162640";
		Vector<pair<Integer,String>> quizVector2 = new Vector<pair<Integer,String>>();
		try
		{
//			getQuiz(m_db, sr.getConfirmedPatientID(),quizVector);
			questionaire q1 = null;
			try
			{
				q1 = new questionaire(ParticipentID);
			}
			catch (InstantiationException e)
			{
				e.printStackTrace();
			}
			catch (IllegalAccessException e)
			{
				e.printStackTrace();
			}
			catch (ClassNotFoundException e)
			{
				e.printStackTrace();
			}
			quizVector2 = q1.getReportStrings();
		}
		catch (SQLException e1)
		{
		}
		
	 	try
		{		
			pdf.Open(ParticipentID);
			for (int ii=0;ii<quizVector2.size();ii++)
			{
				Paragraph p1 = pdf. new Paragraph( quizVector2.get(ii).left + 20, 800-(ii*12), quizVector2.get(ii).right);
				pdf.write(p1);
			}
			pdf.SaveClose("c:\\temp\\temp.pdf");
		}
		catch (Exception e)
		{
			e=e;
		}	
		*/	
/*
		try
		{
			questionaire q1;
			
			q1 = new questionaire("900900909");
			
		}
		catch (SQLException e4)
		{
			e4.printStackTrace();
		}
		catch (InstantiationException e)
		{
			e.printStackTrace();
		}
		catch (IllegalAccessException e)
		{
			e.printStackTrace();
		}
		catch (ClassNotFoundException e)
		{
			e.printStackTrace();
		}
*/		

	//TEMP TESTING
/*
 	try
	{		
		pdf.Open();
		
		String testp1 = "Titanfall went on sale in the US overnight and while us Brits have to wait until Friday to get our hands on the game of 2014, we thought we will fill you in on all the juicy details and get you up to speed on what all the fuss is about.";

		String testp2 = "Stateside, the reviews are pouring in and, as expected, theyre pumping with positivity. Its Metacritic score of 87 means only FIFA 14 has reviewed better on Microsofts next-gen machine. Respected gaming sites Polygon, Gamespot, Joystiq and TotalXbox all gave the game a 90% rating.";
		Paragraph p1 = pdf. new Paragraph(5, 800, testp1);
		Paragraph p2 = pdf. new Paragraph(5, 800, testp2);
		
		pdf.write(p1);
		pdf.write(p2);
		pdf.SaveClose("c:\\temp\\temp.pdf");
	}
	catch (IOException e4)
	{
		e4.printStackTrace();
	}
	catch (Exception e)
	{
		e.printStackTrace();
	}		
*/		
	
	
	
}
