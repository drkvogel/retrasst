package uk.ac.ox.ctsu.syngo.client;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Savepoint;
import java.sql.Statement;
import java.util.Calendar;

/**
 * Used to open up the connections to the various databases used.
 * Also some basic commonly used database related functions which are used in all the java programs.
 */

public class database
{	

	//bb5/local settings!!!!!!!!!!
/*
	static final boolean LINUX = true; // true on bb5, false if desktop machine 
*/
	//Standard Ingres Port
	private static String    	Port = "II7"; // "IJ7" for sharon, "II7" for Jonathan/bb5
//	private static String    	Port = "IJ7"; // "IJ7" for sharon, "II7" for Jonathan/bb5

	static final boolean 		LINUX = false; // true on bb5, false if desktop machine 

	private static String     	Host_JonMachine = "localhost";
	private static String     	bb5_Host_Linux = "bb5.ctsu.ox.ac.uk";


//VNode names on my local machine to BB5, users extract (biocore_pacs table) and biocore (ice tables)	
	private static String       bb5_vnode_biocore = "biocore_bb5";
	
//The databases
	private static String 		bb5_db_ace_ice_live = "ace_ice_live";
	private static String 		bb5_db_ace_trove_live = "ace_trove_live";
	private static String 		bb5_db_extract_pacs_live = "biocore_pacs_live";
	private static String 		bb5_db_extract_pacs_ddb = "biocore_pacs_ddb/star";

//The combined connection information used on my local machine	
 	private static String    	bb5_ace_ice_live_JonMachine = "jdbc:ingres://" +  Host_JonMachine + ":" + Port + "/" + bb5_vnode_biocore + "::" + bb5_db_ace_ice_live; 
	private static String    	bb5_ace_trove_live_JonMachine = "jdbc:ingres://" +  Host_JonMachine + ":" + Port + "/" + bb5_vnode_biocore + "::" + bb5_db_ace_trove_live; 
	private static String    	bb5_extract_pacs_live_JonMachine = "jdbc:ingres://" +  Host_JonMachine + ":" + Port + "/" + bb5_vnode_biocore + "::" + bb5_db_extract_pacs_live; 
	private static String    	bb5_extract_pacs_ddb_JonMachine = "jdbc:ingres://" +  Host_JonMachine + ":" + Port + "/" + bb5_vnode_biocore + "::" + bb5_db_extract_pacs_ddb; 

//The combined connection information used on the linux server BB5
	private static String    	bb5_ace_ice_live_linuxBox = "jdbc:ingres://" +  bb5_Host_Linux + ":" + Port + "/" + bb5_db_ace_ice_live; 
	private static String    	bb5_ace_trove_live_linuxBox = "jdbc:ingres://" +  bb5_Host_Linux + ":" + Port + "/" + bb5_db_ace_trove_live; 
	private static String    	bb5_extract_pacs_live_linuxBox = "jdbc:ingres://" +  bb5_Host_Linux + ":" + Port + "/" + bb5_db_extract_pacs_live; 
	private static String    	bb5_extract_pacs_ddb_linuxBox = "jdbc:ingres://" +  bb5_Host_Linux + ":" + Port + "/" + bb5_db_extract_pacs_ddb; 
    
    public Connection m_conn = null;
    public Statement  m_stmt = null;
    
//This is used to encrypted any data in the database.    
    public ASEncryption m_aes;	
  	
//The database connections.    
	static database m_bb5_ace_trove = null;
	static database m_bb5_ace_ice_live = null;
	static database m_bb5_extract_pacs_live = null;
	static database m_bb5_extract_pacs_ddb = null;
	
//Are we AutoCommiting to the database.	
	static boolean m_AutoCommit;	   
	
	/************************************************************************************
	*                                       PUBLIC FUNCTIONS							*
	************************************************************************************/
	
// Produce an instance of the database connection to extract_pacs_ddb on BB5	
 	public static database instancebb5_extract_pacs_ddb() throws Exception
 	{
 		if (m_bb5_extract_pacs_ddb!=null)
 			return m_bb5_extract_pacs_ddb;
 		if (LINUX)
 	 		return m_bb5_extract_pacs_ddb = new database(bb5_extract_pacs_ddb_linuxBox);
 		return m_bb5_extract_pacs_ddb = new database(bb5_extract_pacs_ddb_JonMachine);
 	}
	
 //Instance the database ace_trove on BB5	
 	public static database instancebb5_ace_trove() throws Exception
 	{
 		if (m_bb5_ace_trove!=null)
 			return m_bb5_ace_trove;
 		if (LINUX)
 	 		return m_bb5_ace_trove = new database(bb5_ace_trove_live_linuxBox);
 		return m_bb5_ace_trove = new database(bb5_ace_trove_live_JonMachine);
 	}
 	
//Instance the database ace_live on BB5	
  	public static database instancebb5_ace_ice_live() throws Exception
 	{
 		if (m_bb5_ace_ice_live!=null)
 			return m_bb5_ace_ice_live;
 		if (LINUX)
 	 		return m_bb5_ace_ice_live = new database(bb5_ace_ice_live_linuxBox);
 		return m_bb5_ace_ice_live = new database(bb5_ace_ice_live_JonMachine);
 	}
 		
//Instance the database pacse_live on BB5	
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
			throw new Exception("Encryption error: " + e.getMessage(), e);
		}           		
 		return m_bb5_extract_pacs_live;
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
	
	//Helper function to update the lastrun time of one of these CRON jobs.
	public boolean updateRunTime(String cronName, boolean start) throws Exception
	{
		String query = "UPDATE cronstatus set ";
		
		if (start)
			query += " started = ? WHERE name = ?";  // ? is like %s in printf,
		                                             // look up prepareStatement,
		                                             // useful for running similar SQL statements
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
	
	//Helper function to update the lastrun time of the CRON job which scans the PACS for new scans.
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
	
	//Helper function (used by all the cron programs) to insert an entry into the lastrun time of the CRON job which scans the PACS for new scans.
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
	
	// (wrt dicom key pair, used in four or so programs)
	
	//checks to see if we have exact matches on either patientid and/or firstname from the DICOM into the table biobankparticipant.
	//biobankparticipant gets it's information from ICE.
	//pass in two arrays of two strings, first entry in each array filled with the required field.
	//returns the confirmed patientid and firstname in the second part of the passed in array. 
	//It also returns an int which is the true ID of this participant in ICE, which can be used to look up clinical information.
	public int hasMatchOnPatientID(String []PatientID, String []FirstName) throws Exception
	{	
		String NoSpacePatientID = "";
		String NoSpaceName = "";
		PatientID[1] = "";
		FirstName[1] = "";
		//if both are empty or null don't even bother looking anything up.
		if (((PatientID[0] == null) || (PatientID[0].isEmpty())) && ((FirstName[0] == null) || (FirstName[0].isEmpty())))  
			return 0;
		
		//The initial test data came through with spaces between every other letter. These need stripping out.
		NoSpacePatientID = PatientID[0].replaceAll("\\s","");
		NoSpaceName = FirstName[0].replaceAll("\\s","");

		//if neither are 8 in length, then we know there won't be any matches
		if ((NoSpacePatientID.length() != 8) && (NoSpaceName.length() != 8))
			return 0;

//  Firstname was the reverse of the patientid, this seams no longer the case, left commented out, as this may change	
//		FirstName = new StringBuilder(FirstName).reverse().toString();
//		if (PatientID[1] == null) PatientID[1] = "";
//		if (FirstName[1] == null) FirstName[1] = "";
		
		String sql = "SELECT dicomparticipantid,firstnameid,pid FROM biobankparticipant WHERE";
		
		if (PatientID[0].isEmpty())
			sql += " firstnameid = '" + NoSpaceName + "'";
		else if (FirstName[0].isEmpty())
			sql += " dicomparticipantid = '" + NoSpacePatientID + "'";
		else
			sql += " dicomparticipantid = '" + NoSpacePatientID + "' OR firstnameid = '" + NoSpaceName + "'";
		
		ResultSet result = null;
		int realPID = 0;
		try
		{
			result = m_stmt.executeQuery(sql);
			if (result.next())
			{	
				PatientID[1] = result.getString("dicomparticipantid");
				FirstName[1] = result.getString("firstnameid");
				realPID = result.getInt("pid"); 
				
				if (PatientID[1] == null) PatientID[1] = "";
				if (FirstName[1] == null) FirstName[1] = "";				
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}	
		return realPID;
	}
	
	public int getPatientPid(String PatientID, String FirstName) throws Exception
	{	
		//The initial test data came through with spaces between every other letter. These need stripping out.
		PatientID = PatientID.replaceAll("\\s","");
		FirstName = FirstName.replaceAll("\\s","");

		String sql = "SELECT pid FROM biobankparticipant WHERE";
		
		if (PatientID.isEmpty())
			sql += " firstnameid = '" + FirstName + "'";
		else if (FirstName.isEmpty())
			sql += " dicomparticipantid = '" + PatientID + "'";
		else
			sql += " dicomparticipantid = '" + PatientID + "' OR firstnameid = '" + FirstName + "'";
		
		ResultSet result = null;
		int realPID = 0;
		try
		{
			result = m_stmt.executeQuery(sql);
			if (result.next())
			{	
				realPID = result.getInt("pid"); 
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}	
		return realPID;
	}
	
	/************************************************************************************
	*                                       PRIVATE FUNCTIONS							*
	*************************************************************************************/	
// Does the work and creates the connections for the database
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
			throw new Exception("Database error" + e.getMessage(), e);
		}
        catch (IllegalAccessException e)
		{
			e.printStackTrace();
			throw new Exception("Database error" + e.getMessage(), e);
		}
        catch (ClassNotFoundException e)
		{
			e.printStackTrace();
			throw new Exception("Database error" + e.getMessage(), e);
		} 
        catch (SQLException e)
		{
			e.printStackTrace();
			throw new Exception("Database error" + e.getMessage(), e);
		}	       
    }
}

