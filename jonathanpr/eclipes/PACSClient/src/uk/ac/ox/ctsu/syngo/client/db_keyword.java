package uk.ac.ox.ctsu.syngo.client;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Iterator;
import java.util.Vector;

/*
 * keyword table keeps track of all the keywords found attached to series. These are collected as DICOM is retrieved from the PACS system.
 * This class is the access to that table.
 */
public class db_keyword
{
	//a mirror of the row columns
	private long m_keywordPK;
	private String m_keyword;
	private String m_type;
	
	static Vector<db_keyword> m_keywords = new Vector<db_keyword>();

	/************************************************************************************
	*                                       PUBLIC FUNCTIONS							*
	*************************************************************************************/
	
	//Adds a new key
	public static void addKey(database db,long keywordPK,String keywordword[],String Type) throws Exception 
	{
		db_keyword k = db_keyword.instance(keywordPK, keywordword,Type);
		k.save(db);
		m_keywords.add(k);
	}

	//returns the key with the keywordPK
	public static db_keyword find(long keywordPK)
	{
		Iterator<db_keyword> it = m_keywords.iterator();
		while (it.hasNext())
		{
			db_keyword k = it.next();
			if (k.m_keywordPK == keywordPK)
				return k;
		}
		return null;
	}

	//loads all the keys from the database
	static public void load(database db) throws SQLException
	{
		m_keywords.clear();
		String sql = "SELECT * FROM keyword where status = 1";
		
		ResultSet result = null;
		try
		{
			result = db.m_stmt.executeQuery(sql);
			while (result.next())
			{	
				long keywordpk = result.getLong("keywordpk");
				String keywordword = result.getString("keyword");
				String type = result.getString("type");
				m_keywords.add(db_keyword.instance(keywordpk, keywordword,type));
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}
	}	
	
/***********************************  Get/Set functions  ***********************************/
	public long getKeywordPK()
	{
		return m_keywordPK;
	}
	
	public String getKeyword()
	{
		return m_keyword;
	}
	
	/************************************************************************************
	*                                       PRIVATE FUNCTIONS							*
	*************************************************************************************/
	private db_keyword(long keywordPK,String keywordword[],String Type)
	{
		String keywordcombined = encodeKeywords(keywordword);
		m_keyword = keywordcombined;
		m_keywordPK = keywordPK;
		m_type = Type;
	}

	private db_keyword(long keywordPK,String keywordwordcombined,String Type)
	{
		m_keyword = keywordwordcombined;
		m_keywordPK = keywordPK;
		m_type = Type;
	}
	
	//saves the keyword into the database
	private boolean save(database db) throws Exception
	{	
		String query = "INSERT INTO keyword (keywordpk,Keyword,type) VALUES( ?, ?,?)";	
		PreparedStatement pstmt =  db.m_conn.prepareStatement(query);		
		pstmt.setLong(1,m_keywordPK);
		pstmt.setString(2,m_keyword);  
		pstmt.setString(3,m_type);
		
		try
		{		
			pstmt.execute();
			return (pstmt.getUpdateCount()>=1);
		}
		catch(Exception e)
		{
			db.rollback();
			throw new Exception("Error executing statment : " + e.getMessage());
		}
		finally
		{
			db.commit();
			pstmt.close();
		}			
	}

	//Keys can have multiple levels, this function combines them for storage
	private static String encodeKeywords(String keywordword[])
	{
		String keywordcombined = "";
		for (int i=0;i<keywordword.length;i++)
		{
			if (keywordcombined.length() > 0)
				keywordcombined += "^";
			keywordcombined += keywordword[i];
		}
		return keywordcombined;
	}
	
	//Internal initialisation
	private static db_keyword instance(long keywordPK,String keywordword[],String Type)
	{
		return new db_keyword(keywordPK,keywordword,Type);
	}

	//Internal initialisation
	private static db_keyword instance(long keywordPK,String keywordword,String Type)
	{
		return new db_keyword(keywordPK,keywordword,Type);
	}
}
