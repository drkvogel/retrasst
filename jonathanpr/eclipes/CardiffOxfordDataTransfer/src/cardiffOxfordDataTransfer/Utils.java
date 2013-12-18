package cardiffOxfordDataTransfer;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.nio.charset.Charset;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * @author jonathanp
 *
 */
public class Utils
{
	/**
	 * Takes a date string in ISO format (an ISO format, may need to add the extended version if we get those as well).
	 * @param ISODateString we are expecting YYYY-MM-DDTHH:MM:SS
	 * @return The date object
	 * @throws ParseException
	 */
	public static Date ISODateStringToDate(String ISODateString) throws ParseException
	{
		SimpleDateFormat date = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss");
		date.setLenient(false);
		return date.parse(ISODateString);
	}
	
	/**
	 * Takes a date string in ISO format (an ISO format, may need to add the extended version if we get those as well).
	 * @param ISODateString we are expecting YYYY-MM-DDTHH:MM:SS
	 * @return	A date string which Ingres will like
	 * @throws ParseException
	 */
	public static String ISODateStringToIngresDateString(String ISODateString) throws ParseException
	{
		Date d = ISODateStringToDate(ISODateString);
		SimpleDateFormat dateformat = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss");
        StringBuilder nowYYYYMMDD = new StringBuilder(dateformat.format(d));
        return nowYYYYMMDD.toString();
	}
	
	/**
	 * VERY TEMP CODE, TO READ EMAIL SYSTEM PASSWORD
	 * @param filename  Takes the first line of this file and turns it into a String
	 * @return The String found
	 */
	
	static public String ReadPWDFromFile(String filename) throws IOException
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
		}
		finally
		{
			if (in!= null)
				in.close();
		}
	    return "PWDNOTFOUND";
	}
/*	
	public static String ReadPWDFromFile(String Dir,String filename)
	{
		StringBuilder StringBuild = new StringBuilder();
		String line = ""; 
		BufferedReader in = null;
		try
		{
			in = new BufferedReader(new FileReader(filename));
			
			while ((line = in.readLine()) != null)
				StringBuild.append(line);
		}
		catch (IOException e)
		{
		}
		finally
		{
			if (in != null)
				in.close();
		}
		return StringBuild.toString();		
	*/	
/*		
		String []s = new String[1];
		s[0] = filename;	
		
		Path path = Paths.get(Dir,s);
		
		Charset charset = Charset.forName("US-ASCII");
		BufferedReader reader = null;
		String line = null;
		try
		{
			reader = Files.newBufferedReader(path,charset);
			if ((line = reader.readLine()) != null)
			{
			    return line;
			}
		}
		catch (IOException e)
		{
		}
	    return "PWDNOTFOUND";
*/	//}
}
