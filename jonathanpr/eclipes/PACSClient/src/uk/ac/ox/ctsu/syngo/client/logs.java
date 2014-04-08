package uk.ac.ox.ctsu.syngo.client;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Properties;
import java.util.logging.FileHandler;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.logging.SimpleFormatter;

import javax.mail.MessagingException;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;

/*
 * Logging and alerts 
 * 
 * Outputs short reports to file, database and can send alert emails
 * 
 * The log system's reports are categorised into 5 areas
 * 		DATABASE 	- Message about the database system, can't connect, sql error..etc
 * 		FILESYSTEM 	- Report on the filesystem, can't open file, can write to file system..etc
 * 		PACS 		- Information on the syngo.share PACS system. 
 * 		GENERAL 	- Anything which doesn't fit in to any of the others
 *  
 * logs are also sub divided how serious they are 
 * 		DEBUG		- Developer information
 * 		INFO		- General information about the state of the system
 * 		ERROR		- Problem in the system, something to keep an eye on.
 * 		CRITICAL	- Something serious is going wrong, should be investigated. Generates an email to CTSU_CONTACT_EMAIL.
 * 		ACTIONREQUIRED	- User action is required, Generates an email to CTSU_CONTACT_EMAIL and BIOBANK_CLINIC_EMAIL.
 */
public class logs
{
	private static String m_PWD = null;
	public static enum LOG_LEVEL {DEBUG,INFO,ERROR,CRITICAL,ACTIONREQUIRED}
	public static enum LOG_TYPE  {GENERAL,DATABASE, FILESYSTEM,PACS}
	
	private static Logger m_logger = null;
	private static FileHandler m_fh = null;
    
    private static String CTSU_CONTACT_EMAIL = "jonathan.price@ctsu.ox.ac.uk"; //Technical help email
    private static String CTSU_CONTACT_EMAIL2 = "sharon.curtis@ctsu.ox.ac.uk"; //Technical help email
    private static String BIOBANK_CLINIC_EMAIL = "jonathan.price@ctsu.ox.ac.uk"; //Biobank administrator

/************************************************************************************
*                                       PUBLIC FUNCTIONS							*
*************************************************************************************/	
	//General logging interface
	public static void log(database db,logs.LOG_LEVEL level, logs.LOG_TYPE type, String msg, String patientID, Date StudyDate)
	{
		logdb(db,level, type, msg,patientID, StudyDate);
		if (level == LOG_LEVEL.ERROR || level == LOG_LEVEL.CRITICAL)
			logfile(level, type,msg,patientID, StudyDate);
	}
	
	//use if you only want to log to the log file, not the database or email
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

	//Called to warn BIOBANK Clinical staff that we have scans over 24 hours old on syngo.share we can't match.
	public static void sendIncompleteICEDataAlert(database db,db_pacsstudy sr)
	{
		SimpleDateFormat dfDate  = new SimpleDateFormat("dd/MMM/yyyy");
		String[] Recipients = new String[3];
		Recipients[0] = CTSU_CONTACT_EMAIL;
		Recipients[1] = CTSU_CONTACT_EMAIL2;
		Recipients[2] = BIOBANK_CLINIC_EMAIL;
		
		String Subject = "Incomplete participent results";
		String Message = "A partipiant in BIOBANK_PSI doesn't yet have enough data in BIOBANK system to generate the clinical information upload. The scan was performed over 24 hours ago. The following details are know about this scan:\r\n\r\n";
		Message += "Participant ID: " + sr.getConfirmedPatientID() +"   " + sr.getFirstName() + "\r\n";
		Message += "Producer: " + sr.getAU() + "\r\n";
		Message += "Sender AET: " + sr.getAET() + "\r\n";		
		Message += "Scan Date: " + dfDate.format(sr.getStudyDate().getTime()) + "\r\n"; 
		Message += "\r\n" + "Please check this partipiant, if everthing looks correct, please email jonathan.price@ctsu.ox.ac.uk so it can be further investigated\r\n";		
		generateEmail(db,Recipients,Subject,Message);
	}
	
	//Called to warn BIOBANK Clinical staff that we have incomplete data in a series.
	public static void sendIncompleteSeriesAlert(database db,db_pacsstudy sr,String description)
	{
		SimpleDateFormat dfDate  = new SimpleDateFormat("dd/MMM/yyyy");
		String[] Recipients = new String[3];
		Recipients[0] = CTSU_CONTACT_EMAIL;
		Recipients[1] = CTSU_CONTACT_EMAIL2;
		Recipients[2] = BIOBANK_CLINIC_EMAIL;
		
		String Subject = "Possible incomplete series found";
		String Message = "Please check the following series in BIOBANK, it may not contain a full image count:\r\n\r\n";
		Message += "Participant ID: " + sr.getConfirmedPatientID() +"   " + sr.getFirstName() + "\r\n";
		Message += "Producer: " + sr.getAU() + "\r\n";
		Message += "Sender AET: " + sr.getAET() + "\r\n";		
		Message += "Scan Date: " + dfDate.format(sr.getStudyDate().getTime()) + "\r\n"; 
		Message += "Series Discription: " + description + "\r\n"; 
		Message += "\r\n" + "To correct this there are severl options:";
		Message += "\r\n" + "   1) If the missing images in the series can be found, push them on to singo.share and make sure they get moved into BIOBANK.";
		Message += "\r\n" + "   2) If the image count can't be corrected, tag the series with the keyword XXXXX.";
		Message += "\r\n" + "   3) If this is a false alert, contact jonathan.price@ctsu.ox.ac.uk so it can be investiaged.";
		generateEmail(db,Recipients,Subject,Message);
	}
	
	/************************************************************************************
	*                                       PRIVATE FUNCTIONS							*
	*************************************************************************************/		
	private static void logdb(database db,logs.LOG_LEVEL level, logs.LOG_TYPE type, String msg, String patientID, Date StudyDate)
	{
		//Writes the log report to the database and may send out an email, depending on log level.
		
		//currently the message length in the database is 256.. this may need changing.
		String crop256Msg = msg;
		if (crop256Msg.length() >= 256)
			crop256Msg = crop256Msg.substring(0, 255);
		
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

		//if we have the log level of ACTIONREQUIRED or CRITICAL, we sent out an email
		String subject = "";
		String[] Recipients = null;
		if (level == LOG_LEVEL.ACTIONREQUIRED)
		{
			Recipients = new String[3];
			Recipients[0] = CTSU_CONTACT_EMAIL;
			Recipients[1] = BIOBANK_CLINIC_EMAIL; 
			Recipients[2] = CTSU_CONTACT_EMAIL2;

			subject = "PACS CRONS: Action required Alert";
		}
		else if (level == LOG_LEVEL.CRITICAL)
		{
			Recipients = new String[2];
			Recipients[0] = CTSU_CONTACT_EMAIL;
			Recipients[1] = CTSU_CONTACT_EMAIL2;
			subject = "PACS CRONS: Critial Alert";
		}

		if (!subject.isEmpty())
			sendGeneralAlert(db,subject,msg,Recipients);
	}
	
	private static void sendGeneralAlert(database db,String Subject,String Message,String[] Recipients)
	{
		String emailText = "Administrator\r\n\r\nAuto generated message:\r\n";
		generateEmail(db,Recipients,Subject,emailText+Message);
	}
	
	private static void generateEmail(database db,String[] Recipients,String Subject,String Message)
	{
		try
		{
			String emailText = Message;			
			String from = "jonathan.price@ctsu.ox.ac.uk";
	
			String host = "pat.ctsu.ox.ac.uk";
			String usrName = "jonathan.price@ctsu.ox.ac.uk";
		
			if (m_PWD == null)
				m_PWD = ReadPWDFromFile("pwd.dat");
//			if (m_PWD.isEmpty())
//				return;
			String usrPass = m_PWD;
	
			// Get system properties
			Properties properties = System.getProperties();
	
			// Setup mail server
			properties.setProperty("mail.smtp.host", host);
			properties.setProperty("mail.user", usrName);
			properties.setProperty("mail.password", usrPass);
	
			// Get the default Session object.
			Session session = Session.getDefaultInstance(properties);
	
			try
			{
				MimeMessage message = new MimeMessage(session);
				message.setFrom(new InternetAddress(from));
				for (int i=0;i<Recipients.length;i++)
					message.addRecipient(MimeMessage.RecipientType.TO, new InternetAddress(Recipients[i]));	
				message.setSubject(Subject);
				message.setText(emailText);
	
				Transport.send(message);
			}
			catch (MessagingException mex)
			{
				if (db != null)
					logs.log(db, logs.LOG_LEVEL.DEBUG, logs.LOG_TYPE.GENERAL, "Exception sending email: " + mex.getMessage(),null,null );
			}
		}
		catch (Exception e)
		{
			if (db != null)
				logs.log(db, logs.LOG_LEVEL.DEBUG, logs.LOG_TYPE.GENERAL, "Exception getting password for email: " + e.getMessage(),null,null );
		}
	}

	//Opens the log file
	private static void setup()
	{
    	m_logger = Logger.getLogger("MyLog");
		try
		{
			m_fh = new FileHandler("LogFile.log", true);
		}
		catch (SecurityException e)
		{
			String[] Recipients = new String[1];
			Recipients[0] = CTSU_CONTACT_EMAIL;
			String subject = "PACS CRONS: Critial Alert";
			String msg = "Problem with log file: " + e.getMessage();
			sendGeneralAlert(null,subject,msg,Recipients);
			return;
		}
		catch (IOException e)
		{//can't open the file. report error
			String[] Recipients = new String[1];
			Recipients[0] = CTSU_CONTACT_EMAIL;
			String subject = "PACS CRONS: Critial Alert";
			String msg = "Problem with log file: " + e.getMessage();
			sendGeneralAlert(null,subject,msg,Recipients);
			return;
		}
		m_logger.addHandler(m_fh);
		m_logger.setLevel(Level.ALL);
		SimpleFormatter formatter = new SimpleFormatter();
		m_fh.setFormatter(formatter);
   }
	
	//Opens a file and reads and returns the first line
	private static String ReadPWDFromFile(String filename) throws Exception
	{
		String line = "PWDNOTFOUND";
		BufferedReader in = null;
		try
		{
			in = new BufferedReader(new FileReader(filename));			
			if ((line = in.readLine()) != null)
				return line;
		}
		catch (IOException e)
		{
			throw new Exception("Password file error: " + e.getMessage());
		}
		finally
		{
			if (in!= null)
				in.close();
		}
	    return "PWDNOTFOUND";
	}
}
