package cardiffOxfordDataTransfer;

import java.security.NoSuchAlgorithmException;
import cardiffOxfordDataTransfer.globals;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.ParseException;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.TreeMap;

/**
 * @author jonathanp
 *
 *
CLEAR DOWN DB
-------------
delete from cardat;
delete from rawdata_bool;
delete from rawdata_ints;
delete from rawdata_date;
delete from rawdata_chars;
 *
 */
public class databaseAccess
{
	private static String     	Host = "localhost";
	private static String    	 Port = "II7";
	private static String       vnode = "bb5.ctsu.ox.ac.uk";  
	private static String 		dbname = "webfu_result_live";

//to connect to a VNODE	
//ingres://localhost:II7/vnode::dbname
	private static String    demoDbUrl2 = "jdbc:ingres://" +  Host + ":" + Port + "/" + vnode + "::" + dbname; 
	
    private Connection m_conn = null;
    private Statement  m_stmt = null;
    
    varValidation m_Validatior;
	
	/**
	 * @param Validatior
	 * @throws InstantiationException
	 * @throws IllegalAccessException
	 * @throws ClassNotFoundException
	 * @throws SQLException
	 */
	public databaseAccess(varValidation Validatior) throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException
	{
		m_Validatior = Validatior;

        Class.forName( "com.ingres.jdbc.IngresDriver" ).newInstance();
        m_conn = DriverManager.getConnection( demoDbUrl2 );
        m_stmt = m_conn.createStatement();
        
        m_conn.setAutoCommit(false);
	}
	
	/**
	 *
	 * @param Persionid
	 * @param Sessionid
	 * @param Sectioname
	 * @return			-1 if no such row, or rec_id for the row if it is found.
	 * @throws SQLException 
	 */
	public int contains(String Persionid,String Sessionid,String Sectioname) throws SQLException
	{
        String query = "SELECT rec_id FROM cardat WHERE partid = '"+Persionid+"' AND sessionid = '"+Sessionid+"' AND sectionid = '"+Sectioname+"'";
        ResultSet result = m_stmt.executeQuery(query);
        if (!result.first())
        	return -1; //no rows
        int num = result.getInt(1);
       	return num;
	}
	

	/**
	 * @param sectionData
	 * @param RawXML 
	 * @return	the record_id of the new record
	 * @throws SQLException 
	 * @throws NoSuchAlgorithmException 
	 * @throws ParseException 
	 */
	public int addSectionInformation(HashMap sectionData, String RawXML) throws SQLException, NoSuchAlgorithmException, ParseException
	{
		String partid = (String) sectionData.get(globals.DEFINE_PARTID_NAME);
		String sessionid = (String) sectionData.get(globals.DEFINE_SESSIONID_NAME);
		String sectionid = (String) sectionData.get(globals.DEFINE_SECTION_ID_NAME); 
		String dateCompleted = (String) sectionData.get(globals.DEFINE_DATECOMPLETED_NAME); 
		String MD5 = (String) sectionData.get(globals.DEFINE_CHECKDM5_NAME);
		java.sql.Timestamp completedDate = null;
		try
		{
			completedDate = new java.sql.Timestamp(Utils.ISODateStringToDate(dateCompleted).getTime());
		}
		catch (ParseException e1)
		{
			throw new SQLException("Error converting date: " + dateCompleted);
		}
		catch (NullPointerException e2)
		{
			throw new SQLException("Error converting date: " + dateCompleted);
		}
		
        String query = "INSERT INTO cardat (partid, sessionid, sectionid, checkmd5,result,cksum_row,datecompleted)";
        query += " VALUES (?,?,?,?,?,?,?)";
            
		PreparedStatement pstmt = m_conn.prepareStatement(query);
		pstmt.setString(1, partid);
		pstmt.setString(2, sessionid);
		pstmt.setString(3, sectionid);
		pstmt.setString(4, MD5);
		pstmt.setString(5, RawXML);
		pstmt.setString(6, "");
		pstmt.setTimestamp(7, completedDate);

		try
		{		
			pstmt.execute();
		}
		catch(java.sql.SQLIntegrityConstraintViolationException e)
		{
			throw new SQLException("Error executing statment : " + e.getMessage());
		}
		finally
		{
			pstmt.close();
		}
        
        int rec_id =  contains(partid,sessionid,sectionid);
                
        //now add checksum
        TreeMap MD5Data = new TreeMap();
        MD5Data.put("rec_id", String.valueOf(rec_id));
        MD5Data.put(globals.DEFINE_PARTID_NAME,partid);
        MD5Data.put(globals.DEFINE_SESSIONID_NAME,sessionid);
        MD5Data.put(globals.DEFINE_SECTION_ID_NAME,sectionid);
        MD5Data.put(globals.DEFINE_DATECOMPLETED_NAME,dateCompleted);
        MD5Data.put(globals.DEFINE_CHECKDM5_NAME,MD5);
        MD5Data.put("result",RawXML);

        String md5 = md5Util.getSignature(MD5Data);
        //add this
        
        query = "UPDATE cardat SET cksum_row = '" + md5 + "' WHERE rec_id = '" + rec_id + "'";
        m_stmt.executeUpdate( query );
		return rec_id;
 	}
	
	/**
	 * @throws SQLException
	 */
	public void commit() throws SQLException
	{
		m_conn.commit();
	}
	
	/**
	 * @throws SQLException
	 */
	public void rollback() throws SQLException
	{
		m_conn.rollback();
	}
	
	/**
	 * @param rec_id
	 * @param sectionData
	 * @param SectionName
	 * @throws SQLException
	 * @throws ParseException 
	 */
	public void addVaribleInformation(int rec_id, HashMap sectionData, String SectionName) throws SQLException, ParseException
	{		
		Object o = null;
		String rec_id_string = String.valueOf(rec_id);
		//add the control fields
		Iterator it = sectionData.keySet().iterator();
		while (it.hasNext())
		{
			final String key = (String) it.next();
			final String val = (String) sectionData.get(key);
			
			try
			{
				o = m_Validatior.getVarType(key,SectionName);
			}
			catch (final XMLParseException e1)
			{
				//no section exists. This would have been picked up in the first pass. Something has really gone wrong if this
				//ever happens	
				ErrorReporting.reportCritical(e1); //This needs fixing, alert CTUS
			}
			
			if (o instanceof Boolean)
			{
				if ((val.toLowerCase().compareTo("true")==0) || (val.compareTo("1")==0))
				{
			        String query = "INSERT INTO rawdata_bool (rec_id,field_name,value)";
			        query += " VALUES ('" + rec_id_string + "','" + key + "','" + true + "')";
			        
//			        System.out.println( "Query: " + query );
			        m_stmt.executeUpdate( query );
			       
				}
				else if  ((val.toLowerCase().compareTo("false")==0) || (val.compareTo("0")==0))
				{
			        String query = "INSERT INTO rawdata_bool (rec_id,field_name,value)";
			        query += " VALUES ('" + rec_id_string + "','" + key + "','" + false + "')";
			        
//			        System.out.println( "Query: " + query );
			        m_stmt.executeUpdate( query );
				}
				else
				{
					final XMLParseException e = new XMLParseException("UNKNOWN BOOL TYPE:" + val);		
					ErrorReporting.reportCritical(e); //This needs fixing, alert CTUS
				}
			}
			else if (o instanceof Integer)
			{
		        String query = "INSERT INTO rawdata_ints (rec_id,field_name,value)";
		        query += " VALUES ('" + rec_id_string + "','" + key + "','" + val + "')";
		        
//		        System.out.println( "Query: " + query );
		        m_stmt.executeUpdate( query );
			}	
			else if (o instanceof Date)
			{
				java.sql.Timestamp IgresDate  = new java.sql.Timestamp(Utils.ISODateStringToDate(val).getTime());
		        String query = "INSERT INTO rawdata_date (rec_id,field_name,value)";
		        query += " VALUES ('" + rec_id_string + "','" + key + "','" + IgresDate + "')";
		        
//		        System.out.println( "Query: " + query );
		        m_stmt.executeUpdate( query );
			}				
			else if (o instanceof Float)
			{
				final XMLParseException e = new XMLParseException("FLOAT NOT SUPPORTED");		
				ErrorReporting.reportCritical(e); //This needs fixing, alert CTUS
			}	
			else if (o instanceof String)
			{
				String varType = "";
				try
				{
					varType =  m_Validatior.getTypeString(key,SectionName);
				}
				catch (final XMLParseException e1)
				{
					//no section exists. This would have been picked up in the first pass. Something has really gone wrong if this
					//ever happens	
					ErrorReporting.reportCritical(e1); //This needs fixing, alert CTUS
				}
		        String query = "INSERT INTO rawdata_chars (rec_id,field_name,value,char_type)";
		        query += " VALUES ('" + rec_id_string + "','" + key + "','" + val + "','" + varType + "')";
		        
//		        System.out.println( "Query: " + query );
		        m_stmt.executeUpdate( query );
			}
		}
	}
}
