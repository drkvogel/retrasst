package ctsu.pacs.test;

import java.security.NoSuchAlgorithmException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Savepoint;
import java.sql.Statement;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.TreeMap;
import java.util.Vector;

/**
 * @author jonathanp
 *
 */

public class database
{
//	private static String       vnode = "brat_cp";  
//	private static String 		dbname = "jonathan_dicom";
//	private static String       vnode = "red";  	
	private static String     	Host_JonMachine = "localhost";
	private static String    	Port = "II7";
/*
	private static String     	bb8_Host_Linux = "bb8.ctsu.ox.ac.uk";
	private static String       bb8_vnode = "biocore_bb8";  
	
	private static String 		bb8_db_biocore_pacsclinicdata_test = "biocore_pacsclinicdata_test";
	
	private static String    	bb8_biocore_pacsclinicdata_test_JonMachine = "jdbc:ingres://" +  Host_JonMachine + ":" + Port + "/" + bb8_vnode + "::" + bb8_db_biocore_pacsclinicdata_test; 
	private static String    	bb8_biocore_pacsclinicdata_test_linuxBox = "jdbc:ingres://" +  bb8_Host_Linux + ":" + Port + "/" + bb8_db_biocore_pacsclinicdata_test; 
*/			
	private static String     	bb5_Host_Linux = "bb5.ctsu.ox.ac.uk";
	private static String       bb5_vnode_biocore = "biocore_bb5";
	private static String       bb5_vnode_extract = "bb5_extract";
	
	private static String 		bb5_db_ace_ice_live = "ace_ice_live";
	private static String 		bb5_db_ace_trove_live = "ace_trove_live";
	private static String 		bb5_db_extract_pacs_live = "biocore_pacs_live";
	private static String 		bb5_db_extract_pacs_ddb = "biocore_pacs_ddb/star";
	
	private static String    	bb5_ace_ice_live_JonMachine = "jdbc:ingres://" +  Host_JonMachine + ":" + Port + "/" + bb5_vnode_biocore + "::" + bb5_db_ace_ice_live; 
	private static String    	bb5_ace_trove_live_JonMachine = "jdbc:ingres://" +  Host_JonMachine + ":" + Port + "/" + bb5_vnode_biocore + "::" + bb5_db_ace_trove_live; 
	private static String    	bb5_extract_pacs_live_JonMachine = "jdbc:ingres://" +  Host_JonMachine + ":" + Port + "/" + bb5_vnode_extract + "::" + bb5_db_extract_pacs_live; 
	private static String    	bb5_extract_pacs_ddb_JonMachine = "jdbc:ingres://" +  Host_JonMachine + ":" + Port + "/" + bb5_vnode_extract + "::" + bb5_db_extract_pacs_ddb; 

	private static String    	bb5_ace_ice_live_linuxBox = "jdbc:ingres://" +  bb5_Host_Linux + ":" + Port + "/" + bb5_db_ace_ice_live; 
	private static String    	bb5_ace_trove_live_linuxBox = "jdbc:ingres://" +  bb5_Host_Linux + ":" + Port + "/" + bb5_db_ace_trove_live; 
	private static String    	bb5_extract_pacs_live_linuxBox = "jdbc:ingres://" +  bb5_Host_Linux + ":" + Port + "/" + bb5_db_extract_pacs_live; 
	private static String    	bb5_extract_pacs_ddb_linuxBox = "jdbc:ingres://" +  bb5_Host_Linux + ":" + Port + "/" + bb5_db_extract_pacs_ddb; 
    
    public Connection m_conn = null;
    public Statement  m_stmt = null;
    
    public ASEncryption m_aes;	
 	private static long overLapCheckMS = 1000*0; 
 	
	static database m_bb5_ace_trove = null;
	static database m_bb5_ace_ice_live = null;
	static database m_bb5_extract_pacs_live = null;
	static database m_bb5_extract_pacs_ddb = null;
	
	static boolean m_AutoCommit;	   
	static final boolean LINUX = false;
	
 	public static database instancebb5_extract_pacs_ddb() throws Exception
 	{
 		if (m_bb5_extract_pacs_ddb!=null)
 			return m_bb5_extract_pacs_ddb;
 		if (LINUX)
 	 		return m_bb5_extract_pacs_ddb = new database(bb5_extract_pacs_ddb_linuxBox);
 		return m_bb5_extract_pacs_ddb = new database(bb5_extract_pacs_ddb_JonMachine);
 	}

	
 	public static database instancebb5_ace_trove() throws Exception
 	{
 		if (m_bb5_ace_trove!=null)
 			return m_bb5_ace_trove;
 		if (LINUX)
 	 		return m_bb5_ace_trove = new database(bb5_ace_trove_live_linuxBox);
 		return m_bb5_ace_trove = new database(bb5_ace_trove_live_JonMachine);
 	}
 	
 	public static database instancebb5_ace_ice_live() throws Exception
 	{
 		if (m_bb5_ace_ice_live!=null)
 			return m_bb5_ace_ice_live;
 		if (LINUX)
 	 		return m_bb5_ace_ice_live = new database(bb5_ace_ice_live_linuxBox);
 		return m_bb5_ace_ice_live = new database(bb5_ace_ice_live_JonMachine);
 	}
 		
 	public static database instancebb5_extract_pacs_live(boolean autoCommit) throws Exception
 	{
 		m_AutoCommit = autoCommit;
 		if (m_bb5_extract_pacs_live!=null)
 			return m_bb5_extract_pacs_live;
 		if (LINUX)
 	 		m_bb5_extract_pacs_live = new database(bb5_extract_pacs_live_linuxBox);
 		else
 			m_bb5_extract_pacs_live = new database(bb5_extract_pacs_live_JonMachine);

 		m_bb5_extract_pacs_live.m_conn.setAutoCommit(autoCommit);
 		
		try
		{
			m_bb5_extract_pacs_live.m_aes = new ASEncryption(new encryptionKey().getDBEncKey());
		}
		catch (Exception e)
		{
			e.printStackTrace();
			throw new Exception("Encryption error", e);
		}           		
 		return m_bb5_extract_pacs_live;
 	}
 	
	private database(String Url) throws Exception
	{
        try
		{
			Class.forName( "com.ingres.jdbc.IngresDriver" ).newInstance();
	        m_conn = DriverManager.getConnection(Url);
	    	m_stmt = m_conn.createStatement();  
		}
		catch (InstantiationException e)
		{
			e.printStackTrace();
			throw new Exception("Database error", e);
		}
        catch (IllegalAccessException e)
		{
			e.printStackTrace();
			throw new Exception("Database error", e);
		}
        catch (ClassNotFoundException e)
		{
			e.printStackTrace();
			throw new Exception("Database error", e);
		} 
        catch (SQLException e)
		{
			e.printStackTrace();
			throw new Exception("Database error", e);
		}	       
    }

	public void commit() throws SQLException
	{
		m_conn.commit();
	}
	
	public void rollback() throws SQLException
	{
		m_conn.rollback();
	}
	
	public void rollback(Savepoint savepoint) throws SQLException
	{
		m_conn.rollback(savepoint);
	}
	
	public Savepoint getSavepoint() throws SQLException
	{
		return m_conn.setSavepoint();
	}
	
	public void releaseSavePoint(Savepoint savepoint) throws SQLException
	{
		m_conn.releaseSavepoint(savepoint);
	}

	
	//**** THIS STUFF BELOW NEEDS MOVING OUT TO A DIFFERENT CLASS NOW ****//
	
	public patientRecord getPatientRecord(String PatientID) throws SQLException
	{
		return patientRecord.instance(this, PatientID);
	}
	
	public boolean hasKeywords(long seriespk) throws Exception
	{
		String sql = "SELECT count(*) as count FROM dicombiobank_keyword WHERE seriespk = " + seriespk; 
		
		ResultSet result = null;
		try
		{
			result = m_stmt.executeQuery(sql);
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
	
	public boolean savekeywords(Vector<Long> keywordids, long seriespk) throws Exception
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
			m_stmt.executeUpdate(sqlss.toString());
			return true;
		}
		catch(Exception e)
		{
			rollback();
			throw e;	
		}		
		finally
		{
			commit();
			m_stmt.close();
		}			
	}
	
	public Calendar getLastUpdate() throws SQLException
	{
		Calendar lastUpdate = null;
		String sql = "SELECT lastdownload from dicomdownloader";
		ResultSet result = null;
		try
		{
			result = m_stmt.executeQuery(sql);
			if (result.next())
			{	
				long last = result.getTimestamp("lastdownload").getTime();
				lastUpdate = Calendar.getInstance();				
				lastUpdate.setTimeInMillis(last-overLapCheckMS);
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}	
		return lastUpdate;
	}
	
	public int lookupRealPatientID(String dicomPID) throws Exception
	{
		String sql = "SELECT pid from biobankparticipant where dicomparticipantid ='" + dicomPID + "'";
		ResultSet result = null;
		try
		{
			result = m_stmt.executeQuery(sql);
			if (result.next())	
				return result.getInt("pid");
		}
		finally
		{
			if (result!=null)
				result.close();
		}	
		return -1;
	}
	
	//CHECKED
	public boolean updateRunTime(String cronName, boolean start) throws Exception
	{
		String query = "UPDATE cronstatus set ";
		
		if (start)
			query += " started = ? WHERE name = ?";
		else
			query += " finished = ? WHERE name = ?";
		
		PreparedStatement pstmt = null;    
		
		try
		{		
 			pstmt =  m_conn.prepareStatement(query);
			pstmt.setTimestamp(1, new java.sql.Timestamp(Calendar.getInstance().getTime().getTime()));	
			pstmt.setString(2,cronName);	
			pstmt.execute();
			return (pstmt.getUpdateCount()>=1);
		}
		catch(Exception e)
		{
			if (!m_AutoCommit)
				rollback();
			throw new Exception(e);	
		}
		finally
		{
			if (pstmt != null)
			{
				if (!m_AutoCommit)
					commit();
				pstmt.close();
			}
		}
	}	
	
	public boolean updateLastUpdate(Calendar lastUpdate) throws SQLException
	{
		String query = "UPDATE dicomdownloader set lastdownload = ?"; 				
		PreparedStatement pstmt = null;    
		
		try
		{		
			pstmt =  m_conn.prepareStatement(query);
			pstmt.setTimestamp(1, new java.sql.Timestamp(lastUpdate.getTime().getTime()));	
			pstmt.execute();
			return (pstmt.getUpdateCount()>=1);
		}
		catch(Exception e)
		{
			rollback();
			return false;	
		}
		finally
		{
			if (pstmt != null)
			{
				commit();
				pstmt.close();
			}
		}
	}
	
	public boolean insertLastUpdate(Calendar lastUpdate) throws SQLException
	{
		String query = "INSERT INTO dicomdownloader (lastdownload) VALUES (?)"; 				
		PreparedStatement pstmt = null;    
		
		try
		{		
			pstmt =  m_conn.prepareStatement(query);
			pstmt.setTimestamp(1, new java.sql.Timestamp(lastUpdate.getTime().getTime()));	
			pstmt.execute();
			return (pstmt.getUpdateCount()>=1);
		}
		catch(Exception e)
		{
			rollback();
			return false;	
		}
		finally
		{
			if (pstmt != null)
			{
				commit();
				pstmt.close();
			}
		}
	
	}
	/*
	 * THIS IS NO LONGER REQUIRED! IT WORKS, COMMENTING OUT SO NOT CALLED
	 *	
	public int testGetDistanceMatch(String rawPatientID, String rawFirstName, String dbPatientID, String dbFirstName)
	{
		if (dbPatientID.compareToIgnoreCase(rawPatientID) == 0)
			return 0;
		if (dbFirstName.compareToIgnoreCase(rawFirstName) == 0)
			return 0;
		
		int pidDist = stringUtils.getLevenshteinDistance(rawPatientID,dbPatientID); 
		int fnDist = stringUtils.getLevenshteinDistance(rawFirstName,dbFirstName); 
		
		if ((pidDist > 2) && (fnDist > 2))
			return -1;
		
		int dist = pidDist + fnDist;				
		return dist;
	}
	

	public boolean tryExtensiveMatchOnPatientID(String[] PatientID, String[] FirstName, Calendar dicomDate) throws SQLException
	{ 
		if (((PatientID[0] == null) || (PatientID[0].isEmpty())) && ((FirstName[0] == null) || (FirstName[0].isEmpty())))  
			return false;
		
		
		//get all firstnames and patient IDS due to be scanned for that day.
		//make both upper, if same.. YEAH!
		
		//see if any of them are close..both have to have NO MORE THEN 1 or two miss typed letters MAX!
		
		Vector<String> patientids = new Vector<String>(); 
		Vector<String> firstnames = new Vector<String>(); 
		Vector<Integer> piddistance = new Vector<Integer>(); 
		Vector<Integer> fnddistance = new Vector<Integer>(); 
																//2014-01-28
		SimpleDateFormat dateformatBuilding = new SimpleDateFormat("yyyy-MM-dd");
		StringBuilder dateString = new StringBuilder( dateformatBuilding.format( dicomDate.getTime() ) );
		
		String sql = "SELECT patientid,firstname,char(scandate) AS scandate FROM patientquiz WHERE char(scandate) LIKE '" + dateString.toString() + "%'";
		ResultSet result = null;
		try
		{
			result = m_stmt.executeQuery(sql);
			while (result.next())
			{	
				String pid = result.getString("patientid");
				String fn = result.getString("firstname");
				
				if (pid.compareToIgnoreCase(PatientID[0]) == 0)
				{ //we have a match!
					PatientID[1] = pid;
					FirstName[1] = fn;
					return true;
				}
				if (fn.compareToIgnoreCase(FirstName[0]) == 0)
				{ //we have a match!
					PatientID[1] = pid;
					FirstName[1] = fn;
					return true;
				}
				
				int pidDist = stringUtils.getLevenshteinDistance(PatientID[0],pid); 
				int fnDist = stringUtils.getLevenshteinDistance(FirstName[0],fn); 
				
				if ((pidDist > 2) && (fnDist > 2))
					continue; //more then 2 changes.. too many..

				patientids.add(pid);
				firstnames.add(fn); 
				piddistance.add(pidDist);
				fnddistance.add(fnDist);				
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}
		
		if (patientids.size() == 0)
			return false;
		
		if (patientids.size() == 1)
		{
			PatientID[1] = patientids.firstElement();
			FirstName[1] = firstnames.firstElement();
		}
		//if two have the same score, then give up.. as there is no way of knowing
		int lowestscore = Integer.MAX_VALUE;
		boolean duplicate = false;
		int lowestIndex = -1;
		for (int i=0;i<patientids.size();i++)
		{
			int totalscore = piddistance.elementAt(i) + fnddistance.elementAt(i); 
			if (lowestscore == totalscore)
				duplicate = true;
			else if (lowestscore > totalscore)
			{
				lowestIndex = i;
				duplicate = false;
				lowestscore = totalscore;
			}
		}
		
		if (duplicate)
			return false;
		
		PatientID[1] = patientids.get(lowestIndex);
		FirstName[1] = firstnames.get(lowestIndex);
		return true;
	}
*/
	//checks to see if we have exact matches on either patientid and/or firstname
	public void MatchOnPatientID(String []PatientID, String []FirstName) throws Exception
	{	
		//if both are empty or null go
		if (((PatientID[0] == null) || (PatientID[0].isEmpty())) && ((FirstName[0] == null) || (FirstName[0].isEmpty())))  
			return;
		
		//strip out white space
		PatientID[0] = PatientID[0].replaceAll("\\s","");
		FirstName[0] = FirstName[0].replaceAll("\\s","");

		//if neither are 8 long.. quit
		if ((PatientID[0].length() != 8) && (FirstName[0].length() != 8))
			return;
// no longer reversed		
//		FirstName = new StringBuilder(FirstName).reverse().toString();
		if (PatientID[1] == null) PatientID[1] = "";
		if (FirstName[1] == null) FirstName[1] = "";

		
		String sql = "SELECT dicomparticipantid,firstnameid FROM biobankparticipant WHERE";
		
		if (PatientID[0].isEmpty())
			sql += " firstnameid = '" + FirstName[0] + "'";
		else if (FirstName[0].isEmpty())
			sql += " dicomparticipantid = '" + PatientID[0] + "'";
		else
			sql += " dicomparticipantid = '" + PatientID[0] + "' OR firstnameid = '" + FirstName[0] + "'";
		
		ResultSet result = null;
		try
		{
			result = m_stmt.executeQuery(sql);
			if (result.next())
			{	
				PatientID[1] = result.getString("dicomparticipantid");
				FirstName[1] = result.getString("firstnameid");
				
				if (PatientID[1] == null) PatientID[1] = "";
				if (FirstName[1] == null) FirstName[1] = "";
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}	
		return;
	}
}

