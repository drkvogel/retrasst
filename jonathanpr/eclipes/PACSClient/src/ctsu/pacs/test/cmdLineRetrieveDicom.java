package ctsu.pacs.test;

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

//Downloads all dicom, based on inserttime
public class cmdLineRetrieveDicom
{
	static database m_db = null;
	static String destination = "dicom/";
	static PACSComms m_PC;
	
	public static void main(String[] args)
	{	
		singleAppInstance.instance(singleAppInstance.CMDDOWNLOAD);
		
		try
		{
			m_db = database.instancebb5_extract_pacs_live(false);
			m_PC = new PACSComms();
			if (!m_PC.signin())
				throw new Exception("Error signing into PACS");
			
			m_db.updateRunTime("DOWNLOADDI", true);

			PACSkeyword.load(m_db);
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
			Vector<Long> studypkVector = retreavePKList("studypk","pacsstudy","au ='BIOBANK' and status = 1");

			studyRecord sr = null;
			for (int i=0;i<studypkVector.size();i++)
			{
				try
				{
					if ((sr == null) || (sr.getStudyPK() != studypkVector.get(i)))
						sr = studyRecord.instance(m_db,studypkVector.get(i));				
					
					if (sr.getConfirmedPatientID().length() == 0)
					{
						try{
							
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
					
					/** RETREVE CID, TRID from biobankparticipant. These are used the zip filename**/
					String tridcid = retreavetridcid(sr);
					if (tridcid.isEmpty())
					{
						logs.log(m_db, logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error while trying to get trid and cid values",sr.getConfirmedPatientID(),null);
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
			m_db.updateRunTime("DOWNLOADDI",false);
		}
		catch (Exception e)
		{
			logs.log(m_db, logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error while trying to download from PACS:" + e.getMessage(),null,null);
		}
	}

	private static String retreavetridcid(studyRecord sr) throws SQLException
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
		if (aet == null)
			aet = "UNKNOWN"; //this only happens when dicom is uploaded by hand (generally my fault)
		int numimages = dicomseries.getNumImages();
		String Modality = dicomseries.getModality();
			
		String query = "INSERT INTO dicombiobank (studypk,aet,numimages,modality,seriespk,patientid,studyid_enc,seriesid_enc,zipmd5,studydate,status,orginalsize,compressedsize,aesvi,filename) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
		
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
		pstmt.setString(15,filename);	//the vi
		
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
	
	private static boolean downloadStudy(Vector<Long> completeSeriespk,studyRecord sr, final String destination, PACSComms PC, String tridcid) throws Exception
	{	
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
				
		StringBuilder manifest_csv = new StringBuilder();
		for (int i = 0; i < DicomSeriesResults.length; i++)
		{
			//see if we have already downloaded this series..
			if (completeSeriespk.contains(DicomSeriesResults[i].getId()))
					continue;
			
				//if there is a problem with saving the keywords, it throws an exception, so will
			//abort downloadStudy
			downloadKeywords(DicomSeriesResults[i].getId(),sr);
						
			manifest_csv.delete(0,manifest_csv.length());
			manifest_csv.append("filename,patientid,studyid,study date,modality,aet,uncompressed size (in bytes))");
			DicomImage[] DicomResults = null;
			try
			{
				DicomResults = PC.searchForDicomInSeries(DicomSeriesResults[i].getId());
			}
			catch (Exception e)
			{
				logs.log(m_db, logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error while trying to get Series in study detils" + e.getMessage(),patientID,studydate.getTime());
				return false;
			}
			
			int seriesImageCount = 0;
			String directory = destination + /*patientID +"/" + StudyDetails.getStudyInstanceUid() +*/ "/"+ DicomSeriesResults[i].getSeriesInstanceUid();
			for (int j = 0; j < DicomResults.length; j++)
			{
				try
				{
					String FileName = PC.ExportDicomImage(DicomResults[j].getId(),directory + "/");
	
					if (FileName == null)
						return false;
					
					File F = new File(FileName);
					manifest_csv.append("\n"+F.getName()+",");
					manifest_csv.append(patientID+","+StudyDetails.getStudyInstanceUid()+","+DicomSeriesResults[i].getModificationDate().getTime().toLocaleString()+","+DicomSeriesResults[i].getModality()+","+ StudyDetails.getCallingAet()+",");
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
						sr.setComplete();
						sr.update();
						m_db.commit();
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
		return true;
	}

	private static boolean downloadKeywords(long seriespk,studyRecord sr) throws Exception
	{
		//todo: check if no keywords attached to this study.. see if there are any
		if (m_db.hasKeywords(seriespk))
			return true;
		
		Vector<Long> studykeyword = new Vector<Long>();
		
		DicomSeriesKeywords[] PACSkeywords = null;
		try
		{
			PACSkeywords = m_PC.getForPatientSeriesKeyWords(seriespk);
		}
		catch (Exception e2)
		{
			logs.log(m_db,logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error retreving keywords: " + e2.getMessage(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());					
			return false;						
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

					if (PACSkeyword.find(Keypk) == null)
					{
						try
						{
							//this commits to DB
							PACSkeyword.addKey(m_db,Keypk,path, type);
						}
						catch (Exception e)
						{
							logs.log(m_db,logs.LOG_LEVEL.ERROR, logs.LOG_TYPE.PACS, "Error retreving keywords: " + e.getMessage(),sr.getConfirmedPatientID(),sr.getStudyDate().getTime());					
							return false;						
						}
					}
					studykeyword.add(Keypk);
				}
			}
		}
		if (studykeyword.size() > 0)
		{
			//save to db, this commits or rolls back and throws if there is an issue.
			m_db.savekeywords(studykeyword,seriespk);
		}
		return true;
	}
}





