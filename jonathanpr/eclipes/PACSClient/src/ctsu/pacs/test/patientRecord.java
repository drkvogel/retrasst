package ctsu.pacs.test;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Calendar;
import java.util.Date;

//INSERT INTO patientrecord (id,name,address,dob) VALUES ('test','Mr Test','12 newroad\nheadington\nox23 2by','01/01/1970 00:00:00')

public class patientRecord
{
	private database m_db;
	
	private String m_patientid;
	private String m_name;
	private String m_address;
	private Date m_dob;
	private String m_phone;
	
	public static patientRecord instance(database db,String patientID) throws SQLException
	{
		return new patientRecord(db,patientID);
	}
	
	private patientRecord(database db,String patientID) throws SQLException
	{
		m_db = db;
		m_patientid = patientID;
		load();
	}
	
	public String[] getPatientInfo()
	{
		String []text = new String[5];
		text[0] = "PatientID:" + m_patientid;
		text[1] = "Name:" + m_name;
		text[2] = "Address:" + m_address;
		text[3] = "Date of birth:" + m_dob;
		text[4] = "Phone:" + m_phone;			
		return text;
	}
	
	private void load() throws SQLException
	{
		String sql = "select name,address,phone,dob from patient where id ='"+m_patientid+ "'";
		ResultSet result = null;
		try
		{
			result = m_db.m_stmt.executeQuery(sql);
			if (result.next())
			{	
				m_name = result.getString("name");
				m_address = result.getString("address");
				m_dob = result.getDate("dob");
				m_phone = result.getString("phone");
			}
			else
				throw new SQLException("Failed to load " + m_patientid);
		}
		finally
		{
			if (result!=null)
				result.close();		
		}	
	}
}
