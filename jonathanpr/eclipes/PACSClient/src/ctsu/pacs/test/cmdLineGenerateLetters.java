package ctsu.pacs.test;

import java.io.File;
import java.io.IOException;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Vector;

import org.apache.pdfbox.exceptions.COSVisitorException;

import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.DicomStudyKeywords;
import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.KeywordInformation;

public class cmdLineGenerateLetters
{
	/*
	 * UNTESTED, UNFINISHED, NOT TO BE USED
	 * 
	 * SO COMMENTED OUT
	 */
	
	public static void main(String[] args)
	{		
/*		singleAppInstance.instance(singleAppInstance.CMDLETTERS);
		System.setProperty("javax.net.ssl.trustStore","cacerts");
		System.setProperty("javax.net.ssl.trustStorePassword", "changeit");
		
		pdfwriter m_pdf = new pdfwriter();
		database m_db = null;
		PACSComms m_PC = new PACSComms();
		
		try
		{
			m_db = new database();
			if (!m_PC.signin())
				throw new Exception("Error signing into PACS");
			m_db.updateRunTime("LETTERS", true);
		}
		catch (Exception e)
		{
			logs.logfile(logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Initalising error: " + e.getMessage(),null,null );
			return;
		}
		
		//get all the dicom in the database, status 1, get keywords, if need a letter, process and update status accordingly

		Vector<Long> studypkVector = new Vector<Long>();
		Vector<String> patientDetailsVector = new Vector<String>();
		try
		{
			getStudyPKsFromREPORTED(m_db, studypkVector);

			for (int i=0;i<studypkVector.size();i++)
			{
				studyRecord sr = null;
				try
				{
					sr = studyRecord.instance(m_db,studypkVector.get(i));
				}
				catch (Exception e2)
				{
					logs.log(m_db,logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Study record not found for StudyPK ["+studypkVector.get(i)+"], this needs investigating",null,null);					
					continue;
				}
				
				if (sr.getConfirmedPatientID().length() == 0)
				{
					continue;
				}
				
				//get keywords, does it have "LETTERS_REQUIRED"
				Vector<Long> studykeyword = new Vector<Long>();
				
				
				DicomStudyKeywords[] PACSkeywords = null;
				try
				{
					PACSkeywords = m_PC.getForPatientStudyKeyWords(sr.getStudyPK());
				}
				catch (Exception e2)
				{
					logs.log(m_db,logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error retreving keywords: " + e2.getMessage(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());					
					continue;						
				}
				if (PACSkeywords != null)
				{
					for (int i1=0;i1<PACSkeywords.length;i1++)
					{
						KeywordInformation[] kwi = PACSkeywords[i1].getKeywordInformation();
						for (int i2=0;i2<kwi.length;i2++)
						{
							String type = kwi[i2].getFieldTypeName();
							String path[] = kwi[i2].getKeywordPath();
							long Keypk = kwi[i2].getKeywordPk();

							// not even too sure if we should now be saving these to the db
							// as we aren't saving the keywords in the db...
							if (PACSkeyword.find(Keypk) == null)
							{
								try
								{
									PACSkeyword.addKey(m_db,Keypk,path, type);
								}
								catch (Exception e)
								{
									logs.log(m_db,logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error retreving keywords: " + e.getMessage(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());					
									continue;						
								}
							}
							studykeyword.add(Keypk);
						}
					}
				}

				//IS SHOULD EVENTALLY BE A SINGLE TAG "LETTER REQUIRED"
				sr.setKeywordList(studykeyword);
				String [] thestrings = {"Diagnosis made - referral needed"};
				if (!sr.hasKeyword("Result of any review", thestrings))
					continue; //no keyword.. leave it alone

			//patient details
				patientDetailsVector.clear();
				try
				{
					getPatientDetails(m_db, sr.getConfirmedPatientID(),patientDetailsVector);
				}
				catch (SQLException e1)
				{
					logs.log(m_db,logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.DATABASE, "Error getting quiz for patient: " + e1.getMessage(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());					
					continue;
				}
				if (patientDetailsVector.size() == 0)
				{
					logs.log(m_db,logs.LOG_LEVEL.INFO, logs.LOG_TYPE.GENERAL, "No Quiz found for PatientID",sr.getConfirmedPatientID(),sr.getStudyDate().getTime());					
					continue;
				}
				
				StringBuilder patientLetterFilenameBuilder = new StringBuilder();
				patientLetterFilenameBuilder.append(sr.getConfirmedPatientID()); patientLetterFilenameBuilder.append("_");
				patientLetterFilenameBuilder.append(studypkVector.get(i));
				patientLetterFilenameBuilder.append("_patientletter.pdf");		

				StringBuilder DocLetterFilenameBuilder = new StringBuilder();
				DocLetterFilenameBuilder.append(sr.getConfirmedPatientID()); DocLetterFilenameBuilder.append("_");
				DocLetterFilenameBuilder.append(studypkVector.get(i));
				DocLetterFilenameBuilder.append("_docletter.pdf");		

				//todo: can the pdf writer fill in a template?
				Vector<String> patientLetter = new Vector<String>();
				Vector<String> docLetter = new Vector<String>();
//				try
//				{
//					m_pdf.writePDF(patientLetter, patientLetterFilenameBuilder.toString());
//					m_pdf.writePDF(docLetter, DocLetterFilenameBuilder.toString());
//				}
//				catch (COSVisitorException e)
//				{
//					logs.log(m_db,logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.FILESYSTEM, "Error generating/saving PDF: " + e.getMessage(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());
//					continue;
//				}
//				catch (IOException e)
//				{
//					logs.log(m_db,logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.FILESYSTEM, "Error generating/saving PDF: " + e.getMessage(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());
//					continue;
//				}

				//DO SOMETHING WITH THE PDFS.. EMAIL? STORE TO A LOCATION?

				//now delete the PDF
				File pdffile = new File(patientLetterFilenameBuilder.toString());
				if (!pdffile.delete())
					logs.log(m_db,logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Failed to delete local PDF report:" + patientLetterFilenameBuilder.toString(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());
	
				pdffile = new File(DocLetterFilenameBuilder.toString());
				if (!pdffile.delete())
					logs.log(m_db,logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Failed to delete local PDF report:" + DocLetterFilenameBuilder.toString(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());

				//todo: UPDATE PACS KEYWORD ON THIS STUDY? TO "LETTER SEND"
				
				sr.setComplete(); //set the status to two..
				try
				{
					//this will update the complete and if there was a confirmed patientid found, also update this
					if (!sr.update())
						throw new Exception("No rows updated");
				}
				catch (Exception e)
				{
					logs.log(m_db,logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Error updating pacsstudy:" + e.getMessage(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());
					continue;
				}
				logs.log(m_db, logs.LOG_LEVEL.INFO, logs.LOG_TYPE.GENERAL, "Success creating letters",sr.getConfirmedPatientID(),sr.getStudyDate().getTime());
			}
			m_db.updateRunTime("LETTERS", false);
		}
		catch (Exception e3)
		{
			logs.log(m_db, logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Error while getting studyPKs from db:" + e3.getMessage(),null,null);
		}
		*/
	}
/*
	private static void getStudyPKsFromREPORTED(database m_db, Vector<Long> studypkVector) throws SQLException
	{
		String sql = "SELECT studypk FROM pacsstudy WHERE au ='REPORTED' and status = 1";
		
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
	
	private static void getPatientDetails(database m_db, String confirmedpatientid,Vector<String> patientdetils) throws SQLException
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
				patientdetils.add(keywordpk);
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}
	}*/
}
