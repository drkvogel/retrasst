package uk.ac.ox.ctsu.syngo.client;

import java.io.InputStream;
import java.math.BigDecimal;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.Iterator;
import java.util.ListIterator;
import java.util.TreeMap;
import java.util.Vector;

import uk.ac.ox.ctsu.javarosetta.Rosetta;
import uk.ac.ox.ctsu.syngo.client.questionaire.eventDetails;


/*
 * In BB5, ace_ice_live.gobz
 * 
 * pid = REAL participent id, so will need to look up our random id in another table to get this.
 * 
 * See patientQuestions.doc for more info.
 * 
 * get answer string select ivalue, meaning from code_int where encoding_id = 100295   or from code_string
 * 
 * ace_trove_live
 * emplyment     select * from code_int where encoding_id = 100295
 * smoke1        select meaning from code_int where encoding_id = 100347 AND ivalue = ? 
 * smoke2		select * from code_int where encoding_id = 100348
 * ethnic		select * from code_int where encoding_id = 100422
 * 
 * ill select * from code_string where encoding_id = 1006 AND svalue = ?
 * cancer select * from code_string where encoding_id = 1003 AND svalue = ?
 */

public class questionaire
{
	final int INVALIDVALUE = -9999971;   //AN INVALID VALUE TO SHOW UNKNWON VALUE;
	final String UNKNOWNVALUE = "Unknown";
	//keep to strings as we are going to print these to pdf
	String m_age;
	Date m_dob;
	String m_sex;  	
	String m_PID;
	
	int m_EthnicGroup;
	int m_currentSmoker;
	int m_pastSmoker;

	String m_weightKG;
	String m_hstand;
	double m_fweightKG;
	double m_fhstand;
	
	Vector<eventDetails> m_illnessesAndCancersList;
	Vector<eventDetails> m_NonCancerList;
	Vector<eventDetails> m_OperationList;
	
	int m_currentEmploymentStatus;
	int m_alcohol;
	int m_systolicBP;
	int m_diastolicBP;
	
	boolean m_hasCompleteData;
	
	Rosetta m_xub_osr;
	Rosetta m_xub_isr;
	
	private static database m_ace_trove_live_db = null;
	private static database m_ace_ice_live_db =null;

/*			
	private static String     	Host = "localhost";
	private static String    	Port = "II7";  
	private static String       vnode = "bb5_extract";
	
	private static String 		dbname_ace_ice_live = "ace_ice_live";
	private static String    	demoDbUrl_JonMachine_ace_ice_live = "jdbc:ingres://" +  Host + ":" + Port + "/" + vnode + "::" + dbname_ace_ice_live; 
//	private static String    	demoDbUrl_linuxBox_ace_ice_live = "jdbc:ingres://" +  Host + ":" + Port + "/" + dbname_ace_ice_live; 
    private static Connection 	m_conn_ace_ice_live = null;
    private static Statement  	m_stmt_ace_ice_live = null;
    
	private static String 		dbname_ace_trove_live = "ace_trove_live";
	private static String    	demoDbUrl_JonMachine_ace_trove_live = "jdbc:ingres://" +  Host + ":" + Port + "/" + vnode + "::" + dbname_ace_trove_live; 
//	private static String    	demoDbUrl_linuxBox_ace_trove_live = "jdbc:ingres://" +  Host + ":" + Port + "/" + dbname_ace_trove_live; 
    private static Connection 	m_conn_ace_trove_live = null;
    private static Statement  	m_stmt_ace_trove_live = null;
  */   
    private static TreeMap<Integer,String> m_employmentStringList;
    private static TreeMap<Integer,String> m_smokeCurrentStringList;
    private static TreeMap<Integer,String> m_smokePostStringList;
    private static TreeMap<Integer,String> m_ethnicStringList;
    private static TreeMap<Integer,String> m_drinkerStringList;
	

        
    static void openDB() throws Exception
    {
    	m_ace_trove_live_db = database.instancebb5_ace_trove();
    	m_ace_ice_live_db = database.instancebb5_ace_ice_live();
    }
    
	public class eventDetails
	{
		String c;
		int w;
		String d;
	}	
	
	
	public boolean hasCompleteData()
	{
		return m_hasCompleteData;
	}

	questionaire(String PID) throws Exception 
	{
		m_PID = PID;
		m_age = UNKNOWNVALUE;
		m_sex = UNKNOWNVALUE;  
		m_illnessesAndCancersList = new Vector<eventDetails>();
		m_NonCancerList = new Vector<eventDetails>();
		m_OperationList = new Vector<eventDetails>();
		m_currentEmploymentStatus = INVALIDVALUE;
		m_EthnicGroup = INVALIDVALUE;
		m_currentSmoker = INVALIDVALUE;
		m_pastSmoker = INVALIDVALUE;
		m_alcohol = INVALIDVALUE;
		m_systolicBP = INVALIDVALUE;
		m_diastolicBP = INVALIDVALUE;
		
		m_weightKG = UNKNOWNVALUE;
		m_hstand = UNKNOWNVALUE;
		
		m_hasCompleteData = false;
		
		m_xub_osr =  new Rosetta();
		m_xub_isr = new Rosetta();
	
		if (m_ace_trove_live_db==null)
		{
			questionaire.openDB();
			loadStringsFromDB();
		}
		
		//use DB ACE_ICE_LIVE
		String sql = "SELECT xub_osr,xub_isr,gtyp,status FROM gobz WHERE pid = "+PID+" AND Gtyp IN (11,22,31,34,51)";

		Vector <Integer> m_gtypList = new Vector <Integer>();
		ResultSet result = null;
		try
		{
			result = m_ace_ice_live_db.m_stmt.executeQuery(sql);
			
			while (result.next())
			{					
				int Gtyp = result.getInt("gtyp");
				
				if (Gtyp == 51)
				{
					m_hasCompleteData = true;
					continue; //this one signifies the participant is fully processed.
				}
				else
					m_gtypList.add(Gtyp);
				
				int Status = result.getInt("status");
				if (Status != 1000)
					continue; //skipped as this is skipped (500) 
				
				String osr = result.getString("xub_osr");
				String isr = result.getString("xub_isr");
				
				m_xub_osr.serializeIn(osr);
				m_xub_isr.serializeIn(isr);
				
				switch (Gtyp)
				{ 
					case 11:
					{ //age, sex
						try
						{
							m_dob = m_xub_osr.getDate("dob");
							Calendar currentDay = Calendar.getInstance();
							Calendar dob = Calendar.getInstance();
							dob.setTime(m_dob);
							int years = currentDay.get(Calendar.YEAR) - dob.get(Calendar.YEAR);
							m_age = String.valueOf(years);
						} 
						catch (NoSuchFieldException e) { }
						try
						{
							if (m_xub_osr.getBoolean("male"))
								m_sex = "Male";
							else
								m_sex = "Female";
						} 
						catch (NoSuchFieldException e) {}						
					}
					break;
					case 22:
					{ //Ethnic, smoker
						Rosetta subR;
						
						//emplyment
						try
						{
							subR = m_xub_isr.getRosetta("answer");
							subR = subR.getRosetta("a");
							subR = subR.getRosetta("D9");
							subR = subR.getRosetta("ans");

							int counter=0;
							while(true) //this will throw when we have got the lastest one
							{
								m_currentEmploymentStatus = subR.getInt("ans"+counter);
								counter++;
							}
						}
						catch (NoSuchFieldException e){}
						
						
						try
						{
							subR = m_xub_isr.getRosetta("answer");
							subR = subR.getRosetta("a");
							try
							{
								m_EthnicGroup = subR.getRosetta("YE1").getInt("ans");
							}
							catch (NoSuchFieldException e){}
							try
							{
								m_currentSmoker = subR.getRosetta("S1").getInt("ans");
							}
							catch (NoSuchFieldException e){}
							try
							{
								m_alcohol = subR.getRosetta("A1").getInt("ans");
							}
							catch (NoSuchFieldException e){}
							try
							{
								m_pastSmoker = subR.getRosetta("S2").getInt("ans");
							}
							catch (NoSuchFieldException e){}
						}
						catch (NoSuchFieldException e){}					
					}
					break;
					case 31:
					{ //Systolic BP, Diastolic BP, Ill and ops
						//Weight, Hight
						Rosetta subR;
						Rosetta subsubR;
						try
						{
							subR = m_xub_isr.getRosetta("data");
							
//							System.out.println(subR.serializeOut());
							try
							{  //reported illness
								subsubR = subR.getRosetta("Vcanc");
								int count = subsubR.getInt("count");
								for (int i=0;i<count;i++)
								{
									eventDetails e = new eventDetails();
									e.c = subsubR.getString("c"+Integer.toString(i));
									e.w = subsubR.getInt("w"+Integer.toString(i));
									e.d = subsubR.getString("d"+Integer.toString(i));
									m_illnessesAndCancersList.add(e);
								}									
							}
							catch (NoSuchFieldException e){}
							try
							{  //ops
								subsubR = subR.getRosetta("Vnonc");
								int count = subsubR.getInt("count");
								for (int i=0;i<count;i++)
								{
									eventDetails e = new eventDetails();
									e.c = subsubR.getString("c"+Integer.toString(i));
									e.w = subsubR.getInt("w"+Integer.toString(i));										
									e.d = subsubR.getString("d"+Integer.toString(i));
									m_NonCancerList.add(e);
									
								}																		
							}
							catch (NoSuchFieldException e){}
							
							try
							{  //ops
								subsubR = subR.getRosetta("Voper");
								String ss = subsubR.serializeOut();
								int count = subsubR.getInt("count");
								for (int i=0;i<=count;i++)
								{
									eventDetails e = new eventDetails();
									e.c = subsubR.getString("c"+Integer.toString(i));
									e.w = subsubR.getInt("w"+Integer.toString(i));										
									e.d = subsubR.getString("d"+Integer.toString(i));
									m_OperationList.add(e);
								}																		
							}
							catch (NoSuchFieldException e){}
							
							int counter=0;
							while(true) //this will throw when we have got the lastest one
							{
								try
								{  //Systolic BP
									m_systolicBP = subR.getInt("bp" + Integer.toString(counter) + "_sys");
									counter++;
									break;
								}
								catch (NoSuchFieldException e1) {break; }									
							}
							counter=0;
							while(true) //this will throw when we have got the lastest one
							{
								try
								{  //Systolic BP
									m_diastolicBP = subR.getInt("bp" + Integer.toString(counter) + "_dia");
									counter++;
									break;
								}
								catch (NoSuchFieldException e1) {break; }									
							}
							
							if (m_systolicBP == INVALIDVALUE) 
							{ //check the sub rossettas
								try
								{
									int count = 0;
									while (true)
									{
										subsubR = subR.getRosetta("bp" + Integer.toString(count));
										count++;
										try
										{
											if (m_systolicBP == INVALIDVALUE)
												m_systolicBP = subsubR.getInt("sys");
										}
										catch (NoSuchFieldException e1) {break; }									
									}								
								}								
								catch (NoSuchFieldException e1) { }		
							}
							
							if (m_diastolicBP == INVALIDVALUE)
							{ //check the sub rossettas
								try
								{
									int count = 0;
									while (true)
									{
										subsubR = subR.getRosetta("bp" + Integer.toString(count));	
										count++;
										try
										{
											if (m_diastolicBP == INVALIDVALUE)
												m_diastolicBP = subsubR.getInt("dia");
										}
										catch (NoSuchFieldException e1) {break; }									
									}								
								}								
								catch (NoSuchFieldException e1) { }		
							}							
						}
						catch (NoSuchFieldException e1) { }									
					}
					break;
					case 34:
					{ //Weight, Height
						Rosetta subR;
						try
						{
							subR = m_xub_isr.getRosetta("data");
							try
							{
								m_fweightKG = subR.getReal("weight");
								m_weightKG = toValueDoubleToString(m_fweightKG,"KG");					
							}
							catch (NoSuchFieldException e)
							{
								try
								{
									m_fweightKG = Double.parseDouble(subR.getRosetta("imped").getString("wgt"));
									m_weightKG = toValueDoubleToString(m_fweightKG,"KG");
								}
								catch (NoSuchFieldException e2){}
							}
								
							try
							{
								m_fhstand = subR.getReal("hstand");
								m_hstand = toValueDoubleToString(subR.getReal("hstand"),"cm");
							}
							catch (NoSuchFieldException e){}						
						}
						catch (NoSuchFieldException e){}
					}
					break;
				}
			}
		}
		finally
		{
			if (result!=null)
				result.close();
		}	
		
		
// Lets check we have all the gtyps we need.
		if (m_gtypList.size() == 4)
		{
			m_hasCompleteData = true; //make sure this is set, 
		}
	}
	
	private String toValueIntToString(int value, String Append)
	{
		if (value == INVALIDVALUE)
			return UNKNOWNVALUE;
		return String.valueOf(value) + " " + Append;
	}
	
	private String toValueDoubleToString(double value, String Append)
	{
		if (value == 0.0)
			return UNKNOWNVALUE;
		return String.valueOf(value) + " " + Append;
	}
	
	private String toEthnicGroupString(int group)
	{
		if (group == INVALIDVALUE)
			return UNKNOWNVALUE;
		return m_ethnicStringList.get(group);
	}
	
	private String toCurrentSmokingString(int group)
	{
		if (group == INVALIDVALUE)
			return UNKNOWNVALUE;
		return m_smokeCurrentStringList.get(group);
	}
	
	private String toPastSmokingString(int group)
	{
		if (group == INVALIDVALUE)
			return UNKNOWNVALUE;
		return m_smokePostStringList.get(group);
	}	
	
	private String toDrinkerString(int group)
	{
		if (group == INVALIDVALUE)
			return UNKNOWNVALUE;
		return m_drinkerStringList.get(group);
	}
	
	private String toEmploymentString(int group)
	{
		if (group == INVALIDVALUE)
			return UNKNOWNVALUE;
		return m_employmentStringList.get(group);
	}
	
	public String getNonCancerString(eventDetails ed) throws SQLException
	{
		try
		{
			Integer.parseInt(ed.c);
			//if ed.c is a number
			String sql = "select meaning from code_string where encoding_id = 1006 AND svalue = " + ed.c;
			
			ResultSet result = null;
			try
			{					
				result = m_ace_trove_live_db.m_stmt.executeQuery(sql);
				if (result.next())
					return result.getString("meaning");
			}
			finally
			{
				if (result!=null)
					result.close();
			}
		}
		catch(NumberFormatException e)
		{

		}
		return ed.d;
	}
	
	public String getCancerString(eventDetails ed) throws SQLException
	{
		try
		{
			Integer.parseInt(ed.c);
			String sql = "select meaning from code_string where encoding_id = 1003 AND svalue = " + ed.c;		
			ResultSet result = null;
			try
			{					
				result = m_ace_trove_live_db.m_stmt.executeQuery(sql);
				if (result.next())
					return result.getString("meaning");
			}
			finally
			{
				if (result!=null)
					result.close();
			}	
		}
		catch(NumberFormatException e)
		{

		}
		return ed.d;
	}
	
	public String getOperationString(eventDetails ed) throws SQLException
	{
		try
		{
			Integer.parseInt(ed.c);
			String sql = "select meaning from code_string where encoding_id = 1005 AND svalue = " + ed.c;		
			ResultSet result = null;
			try
			{					
				result = m_ace_trove_live_db.m_stmt.executeQuery(sql);
				if (result.next())
					return result.getString("meaning");
			}
			finally
			{
				if (result!=null)
					result.close();
			}	
		}
		catch(NumberFormatException e)
		{

		}
		return ed.d;
	}	
	
	public static void loadStringsFromDB() throws SQLException 
	{
		final String STRINGID_EMPLOYMENT 	= "100295";
		final String STRINGID_SMOKECURRENT 	= "100347";
		final String STRINGID_SMOKEPAST 	= "100348";
		final String STRINGID_ETHNIC 		= "100422";
		final String STRINGID_DRINKER		= "100402";
		
		m_employmentStringList = new TreeMap<Integer,String>();
		m_smokeCurrentStringList = new TreeMap<Integer,String>();
		m_smokePostStringList = new TreeMap<Integer,String>();
		m_ethnicStringList = new TreeMap<Integer,String>();
		m_drinkerStringList = new TreeMap<Integer,String>();
			
		String sql = "select meaning,ivalue from code_int where encoding_id = ";
		
		for (int i=0;i<5;i++)
		{
			ResultSet result = null;
			try
			{
				String fullSQL = sql + STRINGID_EMPLOYMENT;
				if (i==1)
					fullSQL = sql + STRINGID_SMOKECURRENT;
				else if (i==2)
					fullSQL = sql + STRINGID_SMOKEPAST;
				else if (i==3)
					fullSQL = sql + STRINGID_ETHNIC;
				else if (i==4)
					fullSQL = sql + STRINGID_DRINKER;
									
				result = m_ace_trove_live_db.m_stmt.executeQuery(fullSQL);
				while (result.next())
				{					
					String meaning = result.getString("meaning");
					int value = result.getInt("ivalue");
					if (i==0)
						m_employmentStringList.put(value, meaning);
					else if (i==1)
						m_smokeCurrentStringList.put(value, meaning);
					else if (i==2)
						m_smokePostStringList.put(value, meaning);
					else if (i==3)
						m_ethnicStringList.put(value, meaning);	
					else if (i==4)
						m_drinkerStringList.put(value, meaning);	
				}
			}
			finally
			{
				if (result!=null)
					result.close();
			}	
		}
	}
	
	Vector<pair<Integer,String>> getReportStrings(db_pacsstudy sr) throws SQLException
	{
		Vector<pair<Integer,String>> returnString = new Vector<pair<Integer,String>>();
		
		returnString.add(pair.of(0,"Participant identifier:" +sr.getConfirmedPatientID() + " " + sr.getFirstName()));		
		returnString.add(pair.of(0,""));  
		
//		Calendar currentDay = Calendar.getInstance();
//		Calendar dob = Calendar.getInstance();
//		dob.setTime(m_dob);
//		SimpleDateFormat dfDate  = new SimpleDateFormat("dd/MMM/yyyy");
//		int years = currentDay.get(Calendar.YEAR) - dob.get(Calendar.YEAR);
		returnString.add(pair.of(0,"Age: " + m_age));//String.valueOf(years)));
		returnString.add(pair.of(0,"Sex: " + m_sex));  
		returnString.add(pair.of(0,"Blood pressure, systolic: " + toValueIntToString(m_systolicBP," mmHg")));
		returnString.add(pair.of(0,"Blood pressure, diastolic: " + toValueIntToString(m_diastolicBP," mmHg")));	
		returnString.add(pair.of(0,"Weight: " + m_weightKG));
		returnString.add(pair.of(0,"Height: " + m_hstand));
		
		if ((m_weightKG != UNKNOWNVALUE) && (m_hstand != UNKNOWNVALUE))
		{
		 	double BMI = m_fhstand/100;
		 	BMI = m_fweightKG / (BMI * BMI);
		 	
		    BigDecimal bd = new BigDecimal(BMI);
		    BigDecimal rounded = bd.setScale(1, BigDecimal.ROUND_DOWN);
		    		 	
			returnString.add(pair.of(0,"BMI: " + Double.toString(rounded.doubleValue())));
		}
		
		returnString.add(pair.of(0,""));
		
		returnString.add(pair.of(0,"What is your ethnic group? " +  toEthnicGroupString(m_EthnicGroup)));
		returnString.add(pair.of(0,"Do you smoke tobacco now? " + toCurrentSmokingString(m_currentSmoker)));
		returnString.add(pair.of(0,"In the past, how often have you smoked tobacco? " + toPastSmokingString(m_pastSmoker)));
		returnString.add(pair.of(0,"About how often do you drink alcohol? " + toDrinkerString(m_alcohol)));
		returnString.add(pair.of(0,"Which of the following describes your current employment situation? " + toEmploymentString(m_currentEmploymentStatus)));			
		returnString.add(pair.of(0,""));  

		ListIterator<eventDetails> itr = m_illnessesAndCancersList.listIterator();
		if (itr.hasNext())
		{
			returnString.add(pair.of(0,""));  
			returnString.add(pair.of(0,"[year of diagnosis]  Verbal interview Cancer History:"));  

			while (itr.hasNext())
			{
				eventDetails ed = itr.next();
				if (ed.w < 150) 
				{ //it's an age and not a year... find the year from this age.
					final GregorianCalendar calendar = new GregorianCalendar();
					calendar.setTime(m_dob);
					calendar.add(Calendar.YEAR, ed.w);
					ed.w = calendar.get(Calendar.YEAR);
				}
				returnString.add(pair.of(30,"      [" + ed.w + "]          " + getCancerString(ed)));  
			}
		}
	
		itr = m_NonCancerList.listIterator();
		if (itr.hasNext())
		{
			returnString.add(pair.of(0,""));  
			returnString.add(pair.of(0,"[year of diagnosis]  Verbal interview Non-cancer History:"));  
			while (itr.hasNext())
			{
				eventDetails ed = itr.next();
				if (ed.w < 150) 
				{ //it's an age and not a year... find the year from this age.
					final GregorianCalendar calendar = new GregorianCalendar();
					calendar.setTime(m_dob);
					calendar.add(Calendar.YEAR, ed.w);
					ed.w = calendar.get(Calendar.YEAR);
				}
				returnString.add(pair.of(30,"      [" + ed.w + "]          " + getNonCancerString(ed)));  
			}
		}		
		
		itr = m_OperationList.listIterator();
		if (itr.hasNext())
		{
			returnString.add(pair.of(0,""));  
			returnString.add(pair.of(0,"[year of diagnosis]  Verbal interview Operation History:"));  
			while (itr.hasNext())
			{
				eventDetails ed = itr.next();
				if (ed.w < 150) 
				{ //it's an age and not a year... find the year from this age.
					final GregorianCalendar calendar = new GregorianCalendar();
					calendar.setTime(m_dob);
					calendar.add(Calendar.YEAR, ed.w);
					ed.w = calendar.get(Calendar.YEAR);
				}
				returnString.add(pair.of(30,"      [" + ed.w + "]          " + getOperationString(ed)));  
			}
		}			
		
		return returnString;
	}
		/*
	String generateReport(String Pid) throws SQLException
	{
		System.out.println();
		Vector<pair<Integer,String>> report = getReportStrings(Pid);
		
		int pos = 0;
		int size = report.size();
		while (pos < size)
		{
			System.out.println(report.get(pos).right);
			pos++;
		}
		return "";		
	}*/
}
