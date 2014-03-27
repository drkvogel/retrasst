package ctsu.pacs.test;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.DateFormat;
import java.util.Calendar;
import java.util.Iterator;
import java.util.Vector;

import javarosetta.Rosetta;

public class studyRecord
{	
	static final int STATUS_INVALID = 0; //DUFF, SHOULD NEVER HAPPEN, IF WE DO GET A ZERO STATUS, THEN THE SYSTEM IS BROKEN
	static final int STATUS_OK = 1; //DICOM IN THE SYSTEM
	static final int STATUS_COMPLETE = 2; //FINISHED WITH THIS, NO LONGER NEEDS ANY TASKS PREFORMED ON IT
	static final int STATUS_INGORE = 3; //SYSTEM SHOULD IGNORE THIS SCAN.

	private database m_db;	
	
	private long m_studyPK;
	private long m_patientPK;
	private String m_patientID;
	private String m_firstName;
	private String m_confirmedPatientID;
	private Calendar m_studyDate;	
	private String m_studyUid;
	private String m_aet;
	private String m_au;
	private String m_modalities;
	private int m_status;  
	private Vector<Long> m_keywords;
	private String m_vi;
	
	public static studyRecord instance(database db,long studyPK) throws Exception
	{
		return new studyRecord(db,studyPK);
	}
	
	public static studyRecord instance(database db,long studyPK,long patientPK, String patientID, String firstName, String confirmedPatientID, Calendar studyDate, String studyUid,String aet, String au, String modalities)//, Vector<Long> keywords)
	{
		return new studyRecord(db,studyPK, patientPK,patientID, confirmedPatientID, firstName,studyDate, studyUid,aet, au, modalities);
	}
	
	private studyRecord(database db,long studyPK, long patientPK, String patientID,String firstName, String confirmedPatientID, Calendar studyDate, String studyUid,String aet, String au, String modalities)//, Vector<Long> keywords)  
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
		m_keywords = new Vector<Long>();
		m_status = 1;
	}
	
	private studyRecord(database db,long studyPK) throws Exception
	{
		m_studyPK = studyPK; 
		m_db = db;
		m_studyDate = Calendar.getInstance();
		m_keywords = new Vector<Long>();
		loadDB();
	}
	
	public boolean hasKeyword(long keyPK)
	{
		Iterator<Long> it = m_keywords.iterator();
		while (it.hasNext())
		{
			Long id = (Long) it.next();
			if (id == keyPK)
				return true;
		}
		return false;	
	}
	
	public boolean hasKeyword(String type,String []keyword) 
	{
		PACSkeyword key = PACSkeyword.find(type,keyword);
		if (key == null)
			return false;		
		long keyPK = key.getKeywordPK();
		return hasKeyword(keyPK);
	}
	
	
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
	
	private String DecryptResult(ResultSet result, String varName) throws Exception
	{
		String newString = result.getString(varName + "_enc");
		if ((newString != null) && (!newString.isEmpty()))
			newString = m_db.m_aes.decrypt(newString);
		else
			newString = result.getString(varName);	
		return newString;
	}
	
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
											
				m_patientPK = result.getLong("patientpk");
				m_studyDate.setTime(result.getTimestamp("studydate"));	
				m_aet = result.getString("aet");
				m_au = result.getString("au");
				m_status = result.getInt("status");		
			}
			else
				throw new Exception("No such record");
		}
		finally
		{
			if (result!=null)
				result.close();
		}
//now load in keys
		m_keywords.clear();
	}
	
	public boolean update() throws Exception
	{
		String query = "UPDATE pacsstudy SET patientid = ?, confirmedpatientid = ?,firstname = ?, patientpk = ?, studydate = ?, studyuid_enc = ?, aet = ?, au = ?, modalities = ?, status = ?, update = ?,statusenc = ?, aesvi = ?"; 				
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

	public void setKeywordList(Vector<Long> keywords)
	{
		m_keywords = keywords;
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
	
	public void setComplete()
	{
		m_status = STATUS_COMPLETE;
	}
}
