package uk.ac.ox.ctsu.syngo.client;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Vector;

public class cmdLineDecryptDB
{
	/************************************************************************************
	*                                       PUBLIC FUNCTIONS							*
	************************************************************************************/
	public static void main(String[] args)
	{		
		//make sure we aren't already running
		singleAppInstance.instance(singleAppInstance.CMDDECRYPT);
		
		System.setProperty("javax.net.ssl.trustStore","cacerts");
		System.setProperty("javax.net.ssl.trustStorePassword", "changeit");

		//setup the database
		database m_db = null;
		try
		{
			m_db = database.instancebb5_extract_pacs_live(false);
			//update the last time this CRON job was ran, this is visible from the web site status page	
			m_db.updateRunTime("DECRYPT", true);
			decryptTable(m_db, "dicombiobank");
			decryptTable(m_db, "pacsstudy");
			m_db.updateRunTime("DECRYPT", false);
		}
		catch (Exception e)
		{
			logs.logfile(logs.LOG_LEVEL.CRITICAL, logs.LOG_TYPE.DATABASE, "Initalising error: " + e.getMessage(),null,null );
			return;
		}
	}
	/************************************************************************************
	*                                       PRIVATE FUNCTIONS							*
	************************************************************************************/	
	private static void decryptTable(database m_db, String tablename) throws Exception
	{
		Vector<String> encryptedColumns = new Vector<String>();
		ResultSet rsColumns = m_db.m_conn.getMetaData().getColumns(null, null, tablename, null);
		encryptedColumns.add("studypk"); //we want a pk
		encryptedColumns.add("aesvi"); //we want the vi
		while (rsColumns.next()) 
		{
			String columnName = rsColumns.getString("COLUMN_NAME");
		    if (columnName.endsWith("_enc"))
		    	encryptedColumns.add(columnName);
		}
		rsColumns.close();
		
		String sql = new String();
		for (int i=0;i<encryptedColumns.size();i++)
		{
			if (sql.length() > 0)
				sql += ",";
			sql += encryptedColumns.elementAt(i);
		}		
		sql = "SELECT " + sql + " FROM " + tablename + " WHERE status > 0 AND statusenc = 1";
		
		HashMap<Long,HashMap<String,String>> insertback = new HashMap<Long,HashMap<String,String>>();
		
		ResultSet result = null;
		try
		{
			result = m_db.m_stmt.executeQuery(sql);
			while (result.next())
			{	
				Long pk = result.getLong(encryptedColumns.elementAt(0));	      
				
				String iv = result.getString(encryptedColumns.elementAt(1));
				m_db.m_aes.setDecrypt(iv);
				HashMap<String,String> newRow = new HashMap<String,String>();
				for (int i=2;i<encryptedColumns.size();i++)
				{
					String encrypted = result.getString(encryptedColumns.elementAt(i));
					String clear = m_db.m_aes.decrypt(encrypted);
					newRow.put(encryptedColumns.elementAt(i).substring(0, encryptedColumns.elementAt(i).length()-4), clear);
				}
				insertback.put(pk,newRow);
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}
			
		//now insert the clear data	
		Iterator<Long> keysert =  insertback.keySet().iterator();
		while (keysert.hasNext())
		{			
			Long key = keysert.next();			
			HashMap<String,String> row = insertback.get(key);
			
			StringBuilder sqlBulder = new StringBuilder();
			sqlBulder.append("UPDATE ");
			sqlBulder.append(tablename);
			sqlBulder.append(" SET ");
						
			Iterator<String> recordkeys = row.keySet().iterator();
			Iterator<String> recordvalues = row.values().iterator();
		
			int j=0;
			while (recordkeys.hasNext())
			{
				if (j>0)
				{
					sqlBulder.append(",");
				}
				sqlBulder.append(recordkeys.next());
				sqlBulder.append(" = ? ");
				j++;
			}			
			sqlBulder.append(", statusenc = 0 ");			
			sqlBulder.append(" WHERE studypk = ");
			sqlBulder.append(key);
			
			PreparedStatement pstmt =  m_db.m_conn.prepareStatement(sqlBulder.toString());
			
			j=1;
			while (recordvalues.hasNext())
			{
				pstmt.setString(j, recordvalues.next());
				j++;
			}
			
			try
			{	
				pstmt.execute();
			}
			catch(Exception e)
			{
				m_db.rollback();
			}
			finally
			{
				if (pstmt != null)
				{
					m_db.commit();
					pstmt.close();
				}
			}			
		}
	}
}
