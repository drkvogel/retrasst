package ctsu.pacs.test;

import java.io.IOException;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.Calendar;
import java.util.Date;
import java.util.logging.FileHandler;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.logging.SimpleFormatter;

public class logs
{
	static public enum LOG_LEVEL {DEBUG,INFO,ERROR,CRITICAL,ACTIONREQUIRED}
	static public enum LOG_TYPE  {GENERAL,DATABASE, FILESYSTEM,PACS}
	
	static Logger m_logger = null;
    static FileHandler m_fh = null;

    private static void setup()
    {
    	m_logger = Logger.getLogger("MyLog");
        try
		{
			m_fh = new FileHandler("LogFile.log", true);
		}
		catch (SecurityException e)
		{
			e.printStackTrace();
			return;
		}
        catch (IOException e)
		{//can't open the file. report error
			e.printStackTrace();
			return;
		}
        m_logger.addHandler(m_fh);
        m_logger.setLevel(Level.ALL);
        SimpleFormatter formatter = new SimpleFormatter();
        m_fh.setFormatter(formatter);
    }
	
	public static void log(database db,logs.LOG_LEVEL level, logs.LOG_TYPE type, String msg, String patientID, Date StudyDate)
	{
		logdb(db,level, type, msg,patientID, StudyDate);
		if (level == LOG_LEVEL.ERROR || level == LOG_LEVEL.CRITICAL)
			logfile(level, type,msg,patientID, StudyDate);
	}
	
	private static void logdb(database db,logs.LOG_LEVEL level, logs.LOG_TYPE type, String msg, String patientID, Date StudyDate)
	{
		//currently the message length in the database is 256.. this may need changing.
		String crop256Msg = msg;
		if (crop256Msg.length() >= 256)
			crop256Msg = crop256Msg.substring(0, 256);
		
		PreparedStatement pstmt = null;
		String query = "INSERT INTO logs (loglevel,logtype,message";
		if (patientID!=null)	
			query += ",patientid";
		if (StudyDate!=null)	
			query += ",studydate";
		query += ") VALUES (?,?,?";
		
				
		if (patientID!=null)	
			query += ",?";
		if (StudyDate!=null)	
			query += ",?";
		query += ")";		
		
		try
		{		
			pstmt = db.m_conn.prepareStatement(query);	
			pstmt.setInt(1, level.ordinal());
			pstmt.setInt(2, type.ordinal());
			pstmt.setString(3, crop256Msg);
			int c = 4;
			if (patientID!=null)	
				pstmt.setString(c++, patientID);
			if (StudyDate!=null)	
				pstmt.setTimestamp(c++,new java.sql.Timestamp(StudyDate.getTime()));
			pstmt.execute();
		}
		catch(Exception e)
		{
			logfile(LOG_LEVEL.CRITICAL, LOG_TYPE.DATABASE, "Error in logging to database: " + e.getMessage(),patientID,StudyDate);
			try
			{
				db.rollback();
			}
			catch (SQLException e1)
			{
				logfile(LOG_LEVEL.CRITICAL, LOG_TYPE.DATABASE, "Error in logging (rolling back): " + e.getMessage(),patientID,StudyDate);
			}
		}
		finally
		{
			try
			{
				db.commit();
				if (pstmt!=null)
					pstmt.close();
			}
			catch (SQLException e)
			{
				logfile(LOG_LEVEL.CRITICAL, LOG_TYPE.DATABASE, "Error in logging (commit or close): " + e.getMessage(),patientID,StudyDate);
			}
		}
	}

	public static void logfile(logs.LOG_LEVEL level, logs.LOG_TYPE type, String msg, String patientID, Date StudyDate)
	{
		if ((m_logger==null) || (m_fh == null))
			setup();
		if ((m_logger==null) || (m_fh == null))
			return; //problems setting up the log file.. it will output errors to the console.. no much else we can do.
			
		Level theirLevel = Level.INFO;
		if (level == LOG_LEVEL.ERROR)
			theirLevel = Level.WARNING;
		else if (level == LOG_LEVEL.CRITICAL)
			theirLevel = Level.SEVERE;		
	
		String theirType = "GENERAL ";
		if (type == LOG_TYPE.DATABASE)
			theirType = "DATABASE ";
		else if (type == LOG_TYPE.FILESYSTEM)
			theirType = "FILESYSTEM ";
		else if (type == LOG_TYPE.PACS)
			theirType = "PACS ";
				
		String details = "";
		if (patientID!=null)
			details = "[" + patientID + "]";
		if (StudyDate!=null)
			details += "[" + StudyDate + "]";
		
		// the following statement is used to log any messages   
        m_logger.log(theirLevel,theirType + ":" + details + " " + msg);
	}
}
