package cardiffOxfordDataTransfer;

import java.util.Properties;

import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;

/**
 * A way of keeping track of errors. Emails the designated contact in CTSU, stops execution on Critical Error reports.
 *
 */
public class ErrorReporting
{
	static String m_AckErrorString = new String();
	/**
	 * 
	 */
	public static String m_PWD;
	
	/**
	 * 
	 */
	public ErrorReporting()
	{

	}
	
	private static String generateMessageString(Exception e)
	{
		String ErrorText = e.getMessage();
		StackTraceElement []stTrace = e.getStackTrace();
		for (int i=0;i<stTrace.length;i++)
		{
			ErrorText += stTrace[i].toString();
		}			
		return ErrorText;
	}
	
	/**
	 * @param e
	 */
	public static void reportWarning(Exception e)
	{
		String s = generateMessageString(e);
		reportError(s,false);
	}
	
	/**
	 * @param e
	 */
	public static void reportCritical(Exception e)
	{
		String s = "Unknown Error";
		if (e!=null)
			s = generateMessageString(e);
		reportError(s,true);
		System.exit(-1);
	}	
	
	static void reportError(String ErrorMessage, boolean bCritical)
	{		
		String to = "jonathan.price@ctsu.ox.ac.uk";
		String from = "jonathan.price@ctsu.ox.ac.uk";

		String host = "pat.ctsu.ox.ac.uk";
		String usrName = "jonathan.price@ctsu.ox.ac.uk";
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
			message.addRecipient(Message.RecipientType.TO, new InternetAddress(to));

			if (bCritical)
				message.setSubject("CRITICAL ERROR: Cardiff - Oxford Data Transfer");
			else
				message.setSubject("WARNING: Cardiff - Oxford Data Transfer");			
			message.setText(ErrorMessage);

			Transport.send(message);
			System.out.println("Error message sent to:" + to);
		}
		catch (MessagingException mex)
		{
			mex.printStackTrace();
		}
	}
}
