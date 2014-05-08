package uk.ac.ox.ctsu.syngo.client;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.rmi.RemoteException;
import java.security.NoSuchAlgorithmException;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Savepoint;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.TreeMap;
import java.util.Vector;

import javax.xml.stream.XMLStreamException;

import com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.Keyword;
import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.DicomSeriesKeywords;
import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.DicomStudyKeywords;
import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.KeywordInformation;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomDocumentFuzzySearchResult;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomImage;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomSeries;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomStudy;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GenericContainer;

// Not currently active.

//Downloads dicom, based on inserttime
public class retrieveDicom
{
	static database m_db = null;
	static String destination = "dicom/";
	static PACSComms m_PC;
	
	//if we find this appending to the end of a series discription, it means we need to check we have
	//the full image count in the series.
	static String SERIES_COUNT_STRING = "_count:";   // i.e.  brain_scan_count:128   (so should be at least 128 images in this series)
	static int IGNORE_SERIES_COUNT_KEYWORD_PK = -777787; //An invalid keyword PK, once this is know, we can put the real one in.
	/************************************************************************************
	*                                       PUBLIC FUNCTIONS							*
	************************************************************************************/
	public static void main(String[] args)
	{	
		//make sure we aren't already running
		singleAppInstance.instance(singleAppInstance.CMDDOWNLOAD);
		
		//setup the connections to Syngo.share comms & database
		try
		{
			m_db = database.instancebb5_extract_pacs_live(false);
			m_PC = new PACSComms();
			if (!m_PC.signin())
				throw new Exception("Error signing into PACS");
			
			//update the last time this CRON job was ran, this is visible from the web site status page	
			m_db.updateRunTime("GETDICOM", true);

			db_keyword.load(m_db);
			//load in the keywords
		}
		catch (Exception e)
		{
			logs.logfile(logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Initalising database error: " + e.getMessage(),null,null );
			return;
		}
		
		//get all the dicom in the database, status 1, get keywords and save data.
		try
		{	
			/*
			 * "BIOBANK" - download data and keywords
			 */
			Vector<Long> studypkVector = retreavePKList("studypk","pacsstudy","au ='BIOBANK' and status = 1 ORDER BY studyinsertdate ASC");

			db_pacsstudy sr = null;
			for (int i=0;i<studypkVector.size();i++)
			{
				try
				{
					if ((sr == null) || (sr.getStudyPK() != studypkVector.get(i)))
						sr = db_pacsstudy.instance(m_db,studypkVector.get(i));				
					
					if (sr.getConfirmedPatientID().length() == 0)
					{
						try{
							
							String []patientID = new String[2];
							String []name = new String[2];
							patientID[0] = sr.getPatientID();
							name[0] = sr.getName();
							if (patientID[0] == null)
								patientID[0] = "";
							if (name[0] == null)
								name[0] = "";
							m_db.hasMatchOnPatientID(patientID,name);
		
							if (patientID[1].isEmpty())
							{
								checkAndSendAlert(m_db, sr);
								continue;
							}
		
							sr.setConfirmedPatientID(patientID[1]);
							sr.setConfirmedName(name[1]);
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
					
					/** RETREVE CID, TRID from biobankparticipant. These are used the zip filename**/
					String tridcid = retreavetridcid(sr);
					if (tridcid.isEmpty())
					{
						logs.log(m_db, logs.LOG_LEVEL.INFO, logs.LOG_TYPE.PACS, "Error while trying to get trid and cid values, is not in biobankparticipent table",sr.getConfirmedPatientID(),null);
						continue;
					}
														
					Vector<Long> completeSeriespk = retreavePKList("seriespk","dicombiobank","studypk = " + studypkVector.get(i) + " AND status = 1");
										
					try
					{
						downloadStudy(completeSeriespk,sr, destination,m_PC,tridcid);
					}
					catch (Exception e)
					{
						logs.log(m_db, logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error while trying to download study: " + e.getMessage(),sr.getPatientID(),Calendar.getInstance().getTime());
					}
				}
				catch (Exception e)
				{
					logs.log(m_db, logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error while trying to download study: " + e.getMessage(),sr.getPatientID(),Calendar.getInstance().getTime());
				}
			}	
			// this is for the web page status of last run. has no effect on the 'system'
			m_db.updateRunTime("GETDICOM",false);
		}
		catch (Exception e)
		{
			logs.log(m_db, logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error while trying to download from PACS:" + e.getMessage(),null,null);
		}
	}
	
	/************************************************************************************
	*                                       PRIVATE FUNCTIONS							*
	************************************************************************************/
	private static void checkAndSendAlert(database m_db, db_pacsstudy sr)
	{
		if (sr.getLastAlert() != null)
		{
			Calendar LastAlert = (Calendar) sr.getLastAlert().clone();
			LastAlert.add(Calendar.DAY_OF_MONTH, 1);
			if (LastAlert.before(Calendar.getInstance()))
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

	private static String retreavetridcid(db_pacsstudy sr) throws SQLException
	{
		String tridcid = "";		
		try
		{
		//retreve cid and trid, used in the filename of the zip.
		String sql = "SELECT cid,trid FROM biobankparticipant WHERE dicomparticipantid = '" + sr.getConfirmedPatientID() + "'";
		Vector<Long> pkVector = new Vector<Long>();		
		ResultSet result = null;
		try
		{
			result = m_db.m_stmt.executeQuery(sql);
			if (result.next())
			{	
				int cid = result.getInt("cid");
				int trid = result.getInt("trid");
				tridcid = String.valueOf(trid) +"_" + String.valueOf(cid);
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}
		}
		catch (Exception e)
		{
			return "";
		}
		return tridcid;
	}

	private static Vector<Long> retreavePKList(String pkName, String table, String Where) throws SQLException
	{
		String sql = "SELECT " + pkName + " FROM " + table + " WHERE " + Where;
		Vector<Long> pkVector = new Vector<Long>();		
		ResultSet result = null;
		try
		{
			result = m_db.m_stmt.executeQuery(sql);
			while (result.next())
			{	
				long keywordpk = result.getInt(pkName);
				pkVector.add(keywordpk);
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}
		return pkVector;
	}

	private static void insert(String patientid,DicomSeries dicomseries,DicomStudy dicomstudy,String zipmd5,Calendar studydate,int status,int orginalsize,int compressedsize, String filename) throws Exception
	{
		long seriesPK = dicomseries.getId();
		Long studypk = dicomstudy.getId();
		String studyid = dicomstudy.getStudyInstanceUid();
		String seriesid = dicomseries.getSeriesInstanceUid();
		String aet = dicomstudy.getCallingAet();
		String hostname = dicomstudy.getCallingHost();
		if (aet == null)
			aet = "UNKNOWN"; //this only happens when dicom is uploaded by hand (generally my fault)
		if (hostname == null)
			hostname = "UNKNOWN";
		int numimages = dicomseries.getNumImages();
		String Modality = dicomseries.getModality();
			
		String query = "INSERT INTO dicombiobank (studypk,aet,numimages,modality,seriespk,patientid,studyid_enc,seriesid_enc,zipmd5,studydate,status,orginalsize,compressedsize,aesvi,filename,seriesdescription,hostname) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
		
 		m_db.m_aes.setEncrypt();
        String vi = m_db.m_aes.getIV();
 		
		PreparedStatement pstmt =  m_db.m_conn.prepareStatement(query);
		pstmt.setLong(1,studypk);		
		pstmt.setString(2,aet);
		pstmt.setLong(3,numimages);
		pstmt.setString(4,Modality);		
		pstmt.setLong(5,seriesPK);
		pstmt.setString(6,patientid);
		pstmt.setString(7,m_db.m_aes.encrypt(studyid));
		pstmt.setString(8,m_db.m_aes.encrypt(seriesid));
		pstmt.setString(9, zipmd5);
		pstmt.setTimestamp(10,new java.sql.Timestamp(studydate.getTime().getTime()), studydate);
		pstmt.setInt(11,status);
		pstmt.setInt(12,orginalsize);
		pstmt.setInt(13,compressedsize);
		pstmt.setString(14,vi);	//the vi
		pstmt.setString(15,filename);	
		pstmt.setString(16, dicomseries.getSeriesDescription());
		pstmt.setString(17, hostname);
		
		try
		{		
			pstmt.execute();
		}
		catch(Exception e)
		{
			throw new SQLException("Error executing statment ["+patientid+":"+studyid+":"+seriesid+"] " + e.getMessage());
		}
		finally
		{
			pstmt.close();
		}
	}
	
	private static boolean downloadStudy(Vector<Long> completeSeriespk,db_pacsstudy sr, final String destination, PACSComms PC, String tridcid) throws Exception
	{	
		boolean hasDownloadedAllSeries = true;
		final long studypk = sr.getStudyPK();
		final String patientID = sr.getConfirmedPatientID();
		final Calendar studydate = sr.getStudyDate();
		
		DicomSeries[] DicomSeriesResults = null;
		DicomStudy StudyDetails = null;
		try
		{
			StudyDetails = PC.getDicomStudy(studypk);
			DicomSeriesResults = PC.searchForPatientSeries(studypk);			
		}
		catch (Exception e)
		{
			logs.log(m_db, logs.LOG_LEVEL.INFO, logs.LOG_TYPE.PACS, "Error while trying to get study detils: " + e.getMessage(),patientID,studydate.getTime());
			return false;
		}
		
//		GenericContainer Generticfile = PC.getGenericDocument(StudyDetails..getDocumentPk());
		
		Calendar ThirtyMinsAgo = Calendar.getInstance();
		ThirtyMinsAgo.add(Calendar.MINUTE, -60); //older then an hour on the system, for when they use copy which does take 30 mins or more to complete.
		
		if (sr.getStudyInsertDate().after(ThirtyMinsAgo))
			return false;
		
		for (int i = 0; i < DicomSeriesResults.length; i++)
		{					
			//see if we have already downloaded this series..
			if (completeSeriespk.contains(DicomSeriesResults[i].getId()))
					continue;
						
			//get discription, if it has _count:X at the end, we have an image count to check
			String discription = DicomSeriesResults[i].getSeriesDescription();
			//if the series discription has an image count in the description, check the series has the full count.		
						
			boolean ImageCountOK = true;
			Vector<Long> serieskeywords = downloadKeywords(DicomSeriesResults[i].getId(),sr);
			if (discription.toLowerCase().contains(SERIES_COUNT_STRING))
			{
				int pos = discription.toLowerCase().lastIndexOf(SERIES_COUNT_STRING);
				if (pos != -1)
				{
					String count = discription.substring(pos + SERIES_COUNT_STRING.length(), discription.length());
					if (count.length() > 0)
					{
						try
						{
							int icount = Integer.valueOf(count);
							if (DicomSeriesResults[i].getNumImages() < icount)
							{
								ImageCountOK = false;
								//OK, we may not have the full set of images on the PACS. 
								//Check to see if we have it tagged as missing images, so we can download it anyway.
								for (int seriesCount=0;seriesCount<serieskeywords.size();seriesCount++)
								{
									if (serieskeywords.get(seriesCount) == IGNORE_SERIES_COUNT_KEYWORD_PK)
									{
										ImageCountOK = true;
										break;
									}
								}
							}
						}
						catch (NumberFormatException e)
						{
							//not a valid int.. so lets carry on, drop a line in the logs so can be checked.
							logs.log(m_db, logs.LOG_LEVEL.DEBUG, logs.LOG_TYPE.GENERAL, "Non int on series count: " + discription,patientID,studydate.getTime());
						}
					}
				}
			}
			if (!ImageCountOK)
			{
				logs.sendIncompleteSeriesAlert(m_db,sr,discription);
				hasDownloadedAllSeries = false; //so we don't set the study as complete. 
				continue;
			}
			
			//save the keywords against this series.
			saveKeywords(DicomSeriesResults[i].getId(),serieskeywords);
			
//lets build the manifest file for this series			
			StringBuilder manifest_csv = new StringBuilder();
			manifest_csv.delete(0,manifest_csv.length());
			manifest_csv.append("filename,patientid,series discription,studyid,study date,modality,aet,hostname,uncompressed size (in bytes))");
			DicomImage[] DicomResults = null;
			try
			{
				DicomResults = PC.searchForDicomInSeries(DicomSeriesResults[i].getId());
			}
			catch (Exception e)
			{
				logs.log(m_db, logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error while trying to get Series in study detils: " + e.getMessage(),patientID,studydate.getTime());
				return false;
			}
			
			int seriesImageCount = 0;
			String directory = destination + "/"+ DicomSeriesResults[i].getSeriesInstanceUid();
			for (int j = 0; j < DicomResults.length; j++)
			{
				try
				{
					String FileName = PC.ExportDicomImage(DicomResults[j].getId(),directory + "/");
	
					if (FileName == null)
						return false;
										
					File F = new File(FileName);
					manifest_csv.append("\n"+F.getName()+",");
					manifest_csv.append(patientID+","+DicomSeriesResults[i].getSeriesDescription()+","+StudyDetails.getStudyInstanceUid()+","+DicomSeriesResults[i].getModificationDate().getTime().toLocaleString()+","+DicomSeriesResults[i].getModality()+","+ StudyDetails.getCallingAet()+","+ StudyDetails.getCallingHost() +",");
					manifest_csv.append(F.length());
					seriesImageCount++;
				}
				catch (Exception e)
				{
					logs.log(m_db, logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.PACS, "Error while trying to export DICOM, exiting" + e.getMessage(),patientID,studydate.getTime());
					return false;
				}
			}
			
			if (seriesImageCount != DicomSeriesResults[i].getNumImages())
			{
				logs.log(m_db, logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Failed to download all images in series, downloaded:" + seriesImageCount + " expected:" + DicomSeriesResults[i].getNumImages() ,patientID,studydate.getTime());
				return false;				
			}
			
			try
			{
				try
				{
					FileOutputStream fos = new FileOutputStream(directory + "//manifest.cvs"); 
					OutputStreamWriter out = new OutputStreamWriter(fos, "UTF-8"); 
					out.write(manifest_csv.toString());
					out.flush();
					out.close();
					
					String filename = DicomSeriesResults[i].getModality() +"_" +tridcid +"_" + String.valueOf(sr.getStudyPK()) +"_" + String.valueOf(DicomSeriesResults[i].getId()) + ".zip";
					String zipFileName = destination + "/"+filename;
					
					int originalsize = fileUtils.zipDir(directory, zipFileName);
					String MD5String;
					MD5String = fileUtils.checksumFile(zipFileName);
					int compressedLength =(int) fileUtils.getFileSize(zipFileName);
					
					if (compressedLength == -1)
						throw new IOException("can't get length of zip");
					
					File file = new File(directory);
					fileUtils.deleteFileRecursive(file);
					
					try
					{
						insert(patientID,DicomSeriesResults[i],StudyDetails,MD5String,studydate,1,originalsize,compressedLength,filename);
					}
					catch (SQLException e)
					{ //this is OK, as it will re-try next time.
						m_db.rollback();
						logs.log(m_db, logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.DATABASE, "Error while trying to write to biobank table" + e.getMessage(),patientID,studydate.getTime());
						throw new SQLException(e);
					}
				}
				catch (IOException e1)
				{
					logs.log(m_db, logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.FILESYSTEM, "Error while trying MD5, Zip or Delete from file system:" + e1.getMessage(),patientID,studydate.getTime());
					return false;
				}
			}
			catch (NoSuchAlgorithmException e)
			{ //BIG PROBLEMS WITH MD5. NO POINT TRYING ANYTHING ELSE, LOG! AND ABORT! 
				logs.log(m_db, logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.PACS, "Problem with MD5:" + e.getMessage(),null,null);
				throw new Exception(e);
			}
		}
		if (hasDownloadedAllSeries)
			sr.setComplete();
		sr.update();
		m_db.commit();
		return true;
	}

	private static Vector<Long> downloadKeywords(long seriespk,db_pacsstudy sr) throws Exception
	{
		//check if no keywords attached to this series..
		Vector<Long> studykeyword = new Vector<Long>();
		
		if (hasKeywords(seriespk))
			return studykeyword;
				
		DicomSeriesKeywords[] PACSkeywords = null;
		try
		{
			PACSkeywords = m_PC.getForPatientSeriesKeyWords(seriespk);
		}
		catch (Exception e2)
		{
			logs.log(m_db,logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error retreving keywords: " + e2.getMessage(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());					
			return null;						
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

					if (db_keyword.find(Keypk) == null)
					{
						try
						{
							//this commits to DB
							db_keyword.addKey(m_db,Keypk,path, type);
						}
						catch (Exception e)
						{
							logs.log(m_db,logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error retreving keywords: " + e.getMessage(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());					
							return null;						
						}
					}
					studykeyword.add(Keypk);
				}
			}
		}
/*		
		if (studykeyword.size() > 0)
		{
			//save to db, this commits or rolls back and throws if there is an issue.
			savekeywords(studykeyword,seriespk);
		}
*/		return studykeyword;
	}
	
	private static void saveKeywords(long seriespk,Vector<Long> studykeyword) throws Exception
	{
		//anything to save?
		if (studykeyword.size() == 0)
			return;
		//have we already saved the keywords?
		if (hasKeywords(seriespk))
			return;

		//save to db, this commits or rolls back and throws if there is an issue.
		savekeywords(studykeyword,seriespk);
	}
	
	static public boolean hasKeywords(long seriespk) throws Exception
	{
		String sql = "SELECT count(*) as count FROM dicombiobank_keyword WHERE seriespk = " + seriespk; 
		
		ResultSet result = null;
		try
		{
			result = m_db.m_stmt.executeQuery(sql);
			if (result.next())
				return (result.getInt("count") > 0);
			return false;
		}
		finally
		{
			if (result!=null)
				result.close();
		}	
	}
	
	static public boolean savekeywords(Vector<Long> keywordids, long seriespk) throws Exception
	{
		StringBuilder sqlss = new StringBuilder(); 
		sqlss.append("INSERT INTO dicombiobank_keyword ( seriespk, keywordpk ) VALUES ");

		for (int i=0;i<keywordids.size();i++)
		{
			if (i>0)
				sqlss.append(",");
			sqlss.append(" (");
			sqlss.append(seriespk);
			sqlss.append(", ");
			sqlss.append(keywordids.elementAt(i));
			sqlss.append(")");
		}
		
		try
		{		
			m_db.m_stmt.executeUpdate(sqlss.toString());
			return true;
		}
		catch(Exception e)
		{
			m_db.rollback();
			throw e;	
		}		
		finally
		{
			m_db.commit();
			m_db.m_stmt.close();
		}			
	}
}


