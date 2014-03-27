package ctsu.pacs.test;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.rmi.RemoteException;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Savepoint;
import java.util.Calendar;
import java.util.Date;
import java.util.Properties;
import java.util.Vector;

import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;
import javax.xml.stream.XMLStreamException;

import org.apache.pdfbox.exceptions.COSVisitorException;

import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.GenericImportResult;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomDocumentFuzzySearchResult;

import ctsu.pacs.test.pdfwriter.Paragraph;

public class cmdLineProcessIncidents
{
	static String m_PWD = "";
	
	
	/*
	 *  java -jar -Djavax.net.ssl.keyStore=~/.keystore queryPACS.jar
	 * 
	 */
	
	
	public static void main(String[] args)
	{		
		singleAppInstance.instance(singleAppInstance.CMDINCIDENT);
	
		pdfwriter pdf = new pdfwriter();
		PACSComms PC = new PACSComms();
		database m_db = null;
		try
		{
			m_db = database.instancebb5_extract_pacs_live(false);
			if (!PC.signin())
				throw new Exception("Error signing into PACS");
			m_db.updateRunTime("UPLOADQUES", true);
		}
		catch (Exception e)
		{
			logs.logfile(logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Initalising error: " + e.getMessage(),null,null );
			return;
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
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			catch (IllegalAccessException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			catch (ClassNotFoundException e)
			{
				// TODO Auto-generated catch block
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
			
			q1 = new questionaire("903162640");
			 q1 = new questionaire("992498361");
			 q1 = new questionaire("937408640");
			 q1 = new questionaire("925513579");
			 q1 = new questionaire("961576793");
			 q1 = new questionaire("930179127");
			 q1 = new questionaire("948866012");
			 q1 = new questionaire("945123484");
			 q1 = new questionaire("951517501");	
			 q1 = new questionaire("913113887");
			 q1 = new questionaire("934631987");
			 q1 = new questionaire("987654321");
			 q1 = new questionaire("952560857");
			 q1 = new questionaire("954178263");
			 q1 = new questionaire("955655613");
			 q1 = new questionaire("929551907");
			 q1 = new questionaire("987237190");
			 q1 = new questionaire("994756739");
			 q1 = new questionaire("996987926");
			 q1 = new questionaire("996198164");
			 q1 = new questionaire("989524536");
			 q1 = new questionaire("900900909");
			 q1 = new questionaire("900900908");	
		}
		catch (SQLException e4)
		{
			// TODO Auto-generated catch block
			e4.printStackTrace();
		}
		catch (InstantiationException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (IllegalAccessException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (ClassNotFoundException e)
		{
			// TODO Auto-generated catch block
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
		// TODO Auto-generated catch block
		e4.printStackTrace();
	}
	catch (Exception e)
	{
		// TODO Auto-generated catch block
		e.printStackTrace();
	}		
		
*/		
				
		//get all the dicom in the database, status 1, get keywords and save data.
			/*
			 * "BIOBANK" - download data and keywords
			 */
		Vector<Long> studypkVector = new Vector<Long>();
		Vector<pair<Integer,String>> quizVector = new Vector<pair<Integer,String>>();
		try
		{
			getStudyPKsFromBIOBANK_PSI(m_db, studypkVector);

			for (int i=0;i<studypkVector.size();i++)
			{
				studyRecord sr = null;
				try
				{
					sr = studyRecord.instance(m_db,studypkVector.get(i));
				}
				catch (Exception e2)
				{
					logs.log(m_db,logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Study record not found for StudyPK ["+studypkVector.get(i)+"], this needs investigating: " + e2.getMessage(),null,null);					
					continue;
				}
				//try and find his/her quiz answers
				
				if (sr.getConfirmedPatientID().length() == 0)
				{
					try
					{
						String []patientID = new String[2];
						String []firstname = new String[2];
						patientID[0] = sr.getPatientID();
						firstname[0] = sr.getFirstName();
						if (patientID[0] == null)
							patientID[0] = "";
						if (firstname[0] == null)
							firstname[0] = "";
						m_db.MatchOnPatientID(patientID,firstname);
	
						if (patientID[1].isEmpty())
							continue;
	
						sr.setConfirmedPatientID(patientID[1]);
						sr.setFirstName(firstname[1]);
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
				
				int iParticitpentID = m_db.lookupRealPatientID(sr.getConfirmedPatientID());
				
				if (iParticitpentID == -1)
				{
					logs.log(m_db,logs.LOG_LEVEL.INFO, logs.LOG_TYPE.GENERAL, "No Biobank paticipient id for: " + sr.getPatientID(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());					
					continue;				
				}
				
				String particitpentID = String.valueOf(iParticitpentID);
				
				quizVector.clear();
				try
				{
					questionaire q1;
					q1 = new questionaire(particitpentID);
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
					pdf.Open(particitpentID,sr.getConfirmedPatientID());
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
					PC.importFile(patientQuizFilenameBuilder.toString(), sr.getPatientID(),patientQuizFilenameBuilder.toString() , patientQuizFilenameBuilder.toString(),sr.getPatientPK());
				}
				catch (Exception e)
				{
					logs.log(m_db,logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error uploading PDF to PACS" + e.getMessage(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());
					continue;
				}
				//now delete the PDF
				File pdffile = new File(patientQuizFilenameBuilder.toString());
				if (!pdffile.delete())
					logs.log(m_db,logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Failed to delete local PDF report:" + patientQuizFilenameBuilder.toString(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());

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
			m_db.updateRunTime("UPLOADQUES", false);
		}
		catch (Exception e3)
		{
			logs.log(m_db, logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Error while getting studyPKs from db:" + e3.getMessage(),null,null);
		}
	}

	private static void getStudyPKsFromBIOBANK_PSI(database m_db, Vector<Long> studypkVector) throws SQLException
	{
		String sql = "SELECT studypk FROM pacsstudy WHERE au ='BIOBANK_PSI' and status = 1";
		
		ResultSet result = null;
		try
		{
			result = m_db.m_stmt.executeQuery(sql);
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
/*	
	private static void getQuiz(database m_db, String confirmedpatientid,Vector<String> quizlines) throws SQLException
	{
		String sql = "SELECT quiz FROM patientquiz WHERE patientid ='" + confirmedpatientid + "'";
		
		ResultSet result = null;
		try
		{
			result = m_db.m_stmt.executeQuery(sql);
			if (result.next())
			{	
				String keywordpk = result.getString("quiz");
				//todo: process this, could be a rosetta or delimited in some way
				quizlines.add(keywordpk);
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}
	}
	
	private static void generateEmail(String PatientID,String modality,String PACSurl, String BIOBankurl)
	{
		String emailText = "Review Administrator\r\n\r\nWe have a suspect scan. Link to the BIOBANK PACS: ";
		emailText += PACSurl + ".\r\n";
		emailText += "\r\nYou can report your findings: " + BIOBankurl + "\r\n\r\n";
		emailText += "BioBank";
		
		String to = "jonathan.price@ctsu.ox.ac.uk";
		String from = "jonathan.price@ctsu.ox.ac.uk";

		String host = "pat.ctsu.ox.ac.uk";
		String usrName = "jonathan.price@ctsu.ox.ac.uk";
		String usrPass = m_PWD;

		// Get system properties
		Properties properties = System.getProperties();

		// Setup mail server
		properties.setProperty("mail.smtp.host", host);
		properties.setProperty("mail.user", usrName);
		properties.setProperty("mail.password", usrPass);

		// Get the default Session object.
		Session session = Session.getDefaultInstance(properties);

		try
		{
			MimeMessage message = new MimeMessage(session);
			message.setFrom(new InternetAddress(from));
			message.addRecipient(Message.RecipientType.TO, new InternetAddress(to));

			message.setSubject("BioBank incidental finding");
			message.setText(emailText);

			Transport.send(message);
			System.out.println("email message sent to:" + to);
		}
		catch (MessagingException mex)
		{
			mex.printStackTrace();
		}
	}
	
	static public String ReadPWDFromFile(String filename) throws Exception
	{
		String line = "PWDNOTFOUND";
		BufferedReader in = null;
		try
		{
			in = new BufferedReader(new FileReader(filename));
			
			if ((line = in.readLine()) != null)
				return line;
		}
		catch (IOException e)
		{
			throw new Exception("Password file error", e);
		}
		finally
		{
			if (in!= null)
				in.close();
		}
	    return "PWDNOTFOUND";
	}
	*/
}
