package ctsu.pacs.test;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Calendar;
import java.util.Iterator;
import java.util.TreeMap;
import java.util.Vector;

public class PACSkeyword
{
	static Vector<PACSkeyword> m_keywords = new Vector<PACSkeyword>();
	
	private long m_keywordPK;
	private String m_keyword;
	private String m_type;
	
	public long getKeywordPK()
	{
		return m_keywordPK;
	}
	
	public String getKeyword()
	{
		return m_keyword;
	}
	
	public static PACSkeyword instance(long keywordPK,String keywordword[],String Type)
	{
		return new PACSkeyword(keywordPK,keywordword,Type);
	}

	public static PACSkeyword instance(long keywordPK,String keywordword,String Type)
	{
		return new PACSkeyword(keywordPK,keywordword,Type);
	}
	
	private PACSkeyword(long keywordPK,String keywordword[],String Type)
	{
		// NOT SURE HOW TO STORE THIS ARRAY..COMBINE IT TO A SINGLE STRING? ROSETTA? DOES IT MATTER IF IT'S A STRING BLOCK?
		//Block it up.. see how it works.
		String keywordcombined = encodeKeywords(keywordword);
		m_keyword = keywordcombined;
		m_keywordPK = keywordPK;
		m_type = Type;
	}

	private PACSkeyword(long keywordPK,String keywordwordcombined,String Type)
	{
		m_keyword = keywordwordcombined;
		m_keywordPK = keywordPK;
		m_type = Type;
	}
	
	public static void addKey(database db,long keywordPK,String keywordword[],String Type) throws Exception 
	{
		PACSkeyword k = PACSkeyword.instance(keywordPK, keywordword,Type);
		k.save(db);
		m_keywords.add(k);
	}
	
	public boolean save(database db) throws Exception
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
	
	static PACSkeyword find(long keywordPK)
	{
		Iterator<PACSkeyword> it = m_keywords.iterator();
		while (it.hasNext())
		{
			PACSkeyword k = it.next();
			if (k.m_keywordPK == keywordPK)
				return k;
		}
		return null;
	}

	static PACSkeyword find(String type, String[] keywordWord)
	{
		String combinekeys = encodeKeywords(keywordWord);
		Iterator<PACSkeyword> it = m_keywords.iterator();
		while (it.hasNext())
		{
			PACSkeyword k = it.next();
			if ((k.m_keyword.compareTo(combinekeys) == 0) && (k.m_type.compareTo(type) == 0))
				return k;
		}
		return null;	
	}
	
	static void load(database db) throws SQLException
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
				m_keywords.add(PACSkeyword.instance(keywordpk, keywordword,type));
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}
	}	
	
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
}
