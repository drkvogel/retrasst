package uk.ac.ox.ctsu.syngo.client;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Timestamp;
import java.util.Calendar;

/*
 * pacsstudy table is filled up the CRON job which is responsible for scanning the PACS for new data and adds them to this table.
 * This class is the access to that table.
 */
public class db_pacsstudy
{	
	//the row status
	static final int STATUS_INVALID = 0; //DUFF, SHOULD NEVER HAPPEN, IF WE DO GET A ZERO STATUS, THEN THE SYSTEM IS BROKEN
	static final int STATUS_OK = 1; //DICOM IN THE SYSTEM
	static final int STATUS_COMPLETE = 2; //FINISHED WITH THIS, NO LONGER NEEDS ANY TASKS PREFORMED ON IT
	static final int STATUS_INGORE = 3; //SYSTEM SHOULD IGNORE THIS SCAN.

	//a mirror of the row columns
	private long m_studyPK;
	private long m_patientPK;
	private String m_patientID;
	private String m_firstName;
	private String m_confirmedPatientID;
	private Calendar m_studyDate;	
	private Calendar m_insert;
	private Calendar m_lastalert;
	private String m_studyUid;
	private String m_aet;
	private String m_au;
	private String m_modalities;
	private int m_status;  
	private String m_vi;
		
	private database m_db;	
	
	/************************************************************************************
	*                                       PUBLIC FUNCTIONS							*
	*************************************************************************************/
	
	//Initialisation: returns a db_pacsstudy, which is loaded from database
	public static db_pacsstudy instance(database db,long studyPK) throws Exception
	{
		return new db_pacsstudy(db,studyPK);
	}
	
	//Initialisation: a new db_pacsstudy, call insert to save it.
	public static db_pacsstudy instance(database db,long studyPK,long patientPK, String patientID, String firstName, String confirmedPatientID, Calendar studyDate, String studyUid,String aet, String au, String modalities)//, Vector<Long> keywords)
	{
		return new db_pacsstudy(db,studyPK, patientPK,patientID, confirmedPatientID, firstName,studyDate, studyUid,aet, au, modalities);
	}
	
	//returns true of the pacsstudy table already contains an entry with the same studyPK and AU as supplied.
	public static boolean contains(database db,long studyPK,String AU) throws Exception
	{
		String sql = "SELECT count(*) AS count FROM pacsstudy WHERE studypk = "+ studyPK + " AND au = '"+AU+"' AND status <> " + STATUS_INVALID;			
		ResultSet result = null;
		try
		{
			result = db.m_stmt.executeQuery(sql);
			if (result.next())
			{
				int count = result.getInt("count");
				return (count > 0);
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}
		return false;
	}
	
	// updates all this data in the database.
	public boolean update() throws Exception
	{
		String query = "UPDATE pacsstudy SET patientid = ?, confirmedpatientid = ?,firstname = ?, patientpk = ?, studydate = ?, studyuid_enc = ?, aet = ?, au = ?, modalities = ?, status = ?, update = ?,statusenc = ?, aesvi = ?, lastalert = ?"; 				
		query += " WHERE studyPK = '" + m_studyPK + "'";
		    
		m_db.m_aes.setEncrypt();
        String vi = m_db.m_aes.getIV();
        PreparedStatement pstmt =  m_db.m_conn.prepareStatement(query);
			
		pstmt.setString(1,m_patientID);
		pstmt.setString(2,m_confirmedPatientID);
		pstmt.setString(3, m_firstName);
		pstmt.setLong(4, m_patientPK);
		pstmt.setTimestamp(5, new java.sql.Timestamp(m_studyDate.getTime().getTime()));	
		pstmt.setString(6,m_db.m_aes.encrypt(m_studyUid));
		pstmt.setString(7,m_aet);
		pstmt.setString(8,m_au);
		pstmt.setString(9,m_modalities);
		pstmt.setInt(10,m_status);				
		pstmt.setTimestamp(11, new java.sql.Timestamp(Calendar.getInstance().getTime().getTime()));				
		pstmt.setLong(12,1);	//status 1 which is encrypted
		pstmt.setString(13,vi);	//the vi	
		if (m_lastalert == null)
			pstmt.setTimestamp(14, null);
		else
			pstmt.setTimestamp(14, new java.sql.Timestamp(m_lastalert.getTime().getTime()));
		
		try
		{		
			pstmt.execute();
			return (pstmt.getUpdateCount()>=1);
		}
		finally
		{
			pstmt.close();
		}	
	}
	
	// inserts a new row with all this data into the database.
	public boolean insert() throws Exception
	{
		String query = "INSERT INTO pacsstudy (patientid,patientpk,confirmedpatientid, firstname, studydate,studyuid_enc,aet,au,modalities,status,update,insert,studypk,statusenc,aesvi) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";				
		m_db.m_aes.setEncrypt();
        String vi = m_db.m_aes.getIV();  	
		PreparedStatement pstmt =  m_db.m_conn.prepareStatement(query);		

		pstmt.setString(1,m_patientID);
		pstmt.setLong(2, m_patientPK);
		pstmt.setString(3,m_confirmedPatientID);
		pstmt.setString(4,m_firstName);
		pstmt.setTimestamp(5, new java.sql.Timestamp(m_studyDate.getTime().getTime()));	
		pstmt.setString(6,m_db.m_aes.encrypt(m_studyUid));
		pstmt.setString(7,m_aet);
		pstmt.setString(8,m_au);
		pstmt.setString(9,m_modalities);
		pstmt.setInt(10,m_status);				
		pstmt.setTimestamp(11, new java.sql.Timestamp(Calendar.getInstance().getTime().getTime()));				
		pstmt.setTimestamp(12, new java.sql.Timestamp(Calendar.getInstance().getTime().getTime()));				
		pstmt.setLong(13,m_studyPK);				
		pstmt.setLong(14,1);	//status 1 which is encrypted			
		pstmt.setString(15,vi);	//the vi			
			
		try
		{		
			pstmt.execute();
			return (pstmt.getUpdateCount()>=1);
		}
		finally
		{
			pstmt.close();
		}	
	}
	
	/***********************************  Get/Set functions  ***********************************/
	public long getStudyPK()
	{
		return m_studyPK;
	}

	public long getPatientPK()
	{
		return m_patientPK;
	}
	
	public String getConfirmedPatientID()
	{
		return m_confirmedPatientID;
	}
	
	public void setConfirmedPatientID(String ConfirmedPatientID)
	{
		m_confirmedPatientID = ConfirmedPatientID;
	}
	
	public String getPatientID()
	{
		return m_patientID;
	}
	
	public String getFirstName()
	{
		return m_firstName;
	}
	
	public void setFirstName(String firstName)
	{
		m_firstName = firstName;
	}
	
	public Calendar getStudyDate()
	{
		return m_studyDate;
	}
	
	public Calendar getLastAlert()
	{
		return m_lastalert;
	}
	
	public Calendar setLastAlertToNow()
	{
		return m_lastalert = Calendar.getInstance();
	}
	
	public void setComplete()
	{
		m_status = STATUS_COMPLETE;
	}
	
	public Calendar getInsertDate()
	{
		return m_insert;
	}
	
	public String getAET() 
	{
		return m_aet;
	}
	
	public String getAU()
	{
		return m_au;
	}	
	
	/************************************************************************************
	*                                       PRIVATE FUNCTIONS							*
	*************************************************************************************/	
	private db_pacsstudy(database db,long studyPK, long patientPK, String patientID,String firstName, String confirmedPatientID, Calendar studyDate, String studyUid,String aet, String au, String modalities)//, Vector<Long> keywords)  
	{
		m_db = db;
		m_firstName = firstName.replaceAll("\\s",""); //make sure there are no spaces;
		m_studyPK = studyPK; 
		m_patientPK = patientPK;
		m_confirmedPatientID = confirmedPatientID;
		m_patientID = patientID.replaceAll("\\s",""); //make sure there are no spaces	
		m_studyDate = studyDate; 
		m_studyUid = studyUid;
		m_aet = aet; 
		if (m_aet == null)
			m_aet = "";
		m_au = au; 
		if (m_au == null)
			m_au = "";
		m_modalities = modalities;
		m_status = 1;
		m_insert = Calendar.getInstance();	
		m_lastalert = null;
	}
	
	private db_pacsstudy(database db,long studyPK) throws Exception
	{
		m_studyPK = studyPK; 
		m_db = db;
		m_studyDate = Calendar.getInstance();
		m_insert = Calendar.getInstance();
		loadDB();
	}
	
	//takes a column name, if there is another column with the same name, but is encrypted (_enc appended), then it 
	//decrypts the entry and returns the value.
	private String DecryptResult(ResultSet result, String varName) throws Exception
	{
		String newString = result.getString(varName + "_enc");
		if ((newString != null) && (!newString.isEmpty()))
			newString = m_db.m_aes.decrypt(newString);
		else
			newString = result.getString(varName);	
		return newString;
	}
	
	//loads in the class data from the database
	private void loadDB() throws Exception
	{
		String sql = "SELECT * FROM pacsstudy WHERE studypk = " + m_studyPK + " AND status = " + STATUS_OK;			
		ResultSet result = null;
		try
		{
			result = m_db.m_stmt.executeQuery(sql);
			if (result.next())
			{	
				m_vi = result.getString("aesvi");
				m_db.m_aes.setDecrypt(m_vi);
				
				m_firstName = result.getString("firstname");
				m_patientID = result.getString("patientid");
				m_confirmedPatientID = result.getString("confirmedpatientid");
				m_studyUid = DecryptResult(result,"studyuid");
				m_modalities = result.getString("modalities");
				m_insert.setTime(result.getTimestamp("insert"));							
				m_patientPK = result.getLong("patientpk");
				m_studyDate.setTime(result.getTimestamp("studydate"));	
				m_aet = result.getString("aet");
				m_au = result.getString("au");
				m_status = result.getInt("status");	
				Timestamp ts = result.getTimestamp("lastalert");
				if (ts != null)
				{
					m_lastalert = Calendar.getInstance();
					m_lastalert.setTime(ts);
				}
			}
			else
				throw new Exception("No such record");
		}
		finally
		{
			if (result!=null)
				result.close();
		}
	}
}
