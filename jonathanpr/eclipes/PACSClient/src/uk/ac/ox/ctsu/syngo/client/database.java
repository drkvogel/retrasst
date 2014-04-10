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
	//SET THIS TO TRUE IF YOU ARE RUNNING FROM YOU LOCAL MACHINE. SET TO FALSE WHEN COMPILING AND RUNNING FROM LINUX.
	static final boolean LINUX = false;

	private static String     	Host_JonMachine = "localhost";
	private static String     	bb5_Host_Linux = "bb5.ctsu.ox.ac.uk";

//Standard Ingres Port
	private static String    	Port = "II7";

//VNode names on my local machine to BB5, users extract (biocore_pacs table) and biocore (ice tables)	
	private static String       bb5_vnode_biocore = "biocore_bb5";
	private static String       bb5_vnode_extract = "bb5_extract";
	
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
	
//Instance the database extract_pacs_ddb on BB5	
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
	
	//Helper function to insert an entry into the lastrun time of the CRON job which scans the PACS for new scans.
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
	
	//checks to see if we have exact matches on either patientid and/or firstname from the DICOM into the table biobankparticipant.
	//biobankparticipant gets it's information from ICE.
	//pass in two arrays of two strings, first entry in each array filled with the required field.
	//returns the confirmed patientid and firstname in the second part of the passed in array. 
	//It also returns an int which is the true ID of this participant in ICE, which can be used to look up clinical information.
	public int hasMatchOnPatientID(String []PatientID, String []FirstName) throws Exception
	{	
		PatientID[1] = "";
		FirstName[1] = "";
		//if both are empty or null don't even bother looking anything up.
		if (((PatientID[0] == null) || (PatientID[0].isEmpty())) && ((FirstName[0] == null) || (FirstName[0].isEmpty())))  
			return 0;
		
		//The initial test data came through with spaces between every other letter. These need stripping out.
		PatientID[0] = PatientID[0].replaceAll("\\s","");
		FirstName[0] = FirstName[0].replaceAll("\\s","");

		//if neither are 8 in length, then we know there won't be any matches
		if ((PatientID[0].length() != 8) && (FirstName[0].length() != 8))
			return 0;

//  Firstname was the reverse of the patientid, this seams no longer the case, left commented out, as this may change	
//		FirstName = new StringBuilder(FirstName).reverse().toString();

		if (PatientID[1] == null) PatientID[1] = "";
		if (FirstName[1] == null) FirstName[1] = "";
		
		String sql = "SELECT dicomparticipantid,firstnameid,pid FROM biobankparticipant WHERE";
		
		if (PatientID[0].isEmpty())
			sql += " firstnameid = '" + FirstName[0] + "'";
		else if (FirstName[0].isEmpty())
			sql += " dicomparticipantid = '" + PatientID[0] + "'";
		else
			sql += " dicomparticipantid = '" + PatientID[0] + "' OR firstnameid = '" + FirstName[0] + "'";
		
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
	
	/************************************************************************************
	*                                       PRIVATE FUNCTIONS							*
	*************************************************************************************/	
 //Does the work and creates the database
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

