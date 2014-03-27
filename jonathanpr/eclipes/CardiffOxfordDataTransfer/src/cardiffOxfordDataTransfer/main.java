package cardiffOxfordDataTransfer;

import java.io.FileNotFoundException;
import java.io.IOException;

import org.xml.sax.SAXException;

import cardiffOxfordDataTransfer.Utils;
/**
 * 
 *GENERAL INFORMATION
 *-------------------
 *
 *ON BADLY FORMED XML, IT STOPS PARSING AND REPORTS THAT ERROR. AFTER ALL WE MAKE SURE MAKE SURE WE ARE 100% SURE WHAT WE ARE LOOKING AT.
 *NO FAIL SCHEMA CHECK OR VARIABLE OUT OF BOUNDS, IT CONTIUNES AND REPORTS THOSE ERRORS, AND REJECTS THAT SECTION. OTHER SECTIONS IT HAS
 *NO PROBLEMS WITH IT WILL ADD TO THE DATABASE. 
 *
 */

public class main
{
	public static void main(String[] args) 
	{
		try
		{
			final String decrptedXML_FILENAME = "cardat.xml";
			final String ack_FILENAME = "carack.xml";
		
			//TEMP CODE TO SET THE EMAIL PASSWORD
			try
			{
				ErrorReporting.m_PWD = Utils.ReadPWDFromFile("pwd.txt");
			}
			catch (IOException e2)
			{
				e2.printStackTrace();
				ErrorReporting.reportCritical(e2);
			}
						
			AcknowledgeXML AckXML = new AcknowledgeXML();
			ValidationXMLParser XML = new ValidationXMLParser();
			ToDatabaseXMLParser DatabaseParser = new ToDatabaseXMLParser();
			varValidation varValidator = new varValidation();
			
			try
			{
				varValidator.LoadValidationFile("varvalidation.txt");
			}
			catch (XMLParseException e1)
			{
				e1.printStackTrace();
				ErrorReporting.reportCritical(e1);
				return;
			}
			catch (IOException e1)
			{
				e1.printStackTrace();
				ErrorReporting.reportCritical(e1);
				return;
			}
			
			try
			{
				XML.setUp(decrptedXML_FILENAME, AckXML,varValidator);
				XML.parse();
			}
			catch (SAXException s)
			{
				//parse exception.. lets report back what we know to Cardiff. Run on.
			}
			catch (FileNotFoundException fnfe)
			{ //if no cardat.xml, relax. no problem.. go back to sleep
				return;			
			}
			catch (Exception e)
			{	
				//something bigger happened. Better upgrade the problem to CTSU
				e.printStackTrace();
				ErrorReporting.reportCritical(e);
				return;
			}
/*			
			try
			{
				DatabaseParser.setUp(decrptedXML_FILENAME, AckXML,varValidator);
				//store the XML in the database.			
				DatabaseParser.parse();
			}
			catch (SAXException s)
			{
				//parse exception.. lets report back what we know to Cardiff. Run on.
			}
			catch (IOException IOE)
			{
				if (IOE.getMessage().compareTo("Buffer too small") == 0)
				{
					//File is of zero size.
					//Lets do nothing
					return;
				}
			}
			catch (Exception e1)
			{
				e1.printStackTrace();
				ErrorReporting.reportCritical(e1);			
			}
	*/
			//check the number of sections are correct,
			if (AckXML.getNumberOfAcks() != XML.getXMLnumSections())
			{
				if (!AckXML.hasCriticalError())
					AckXML.addComment("The number of sections is reported to be " + XML.getXMLnumSections() + " but we found " + AckXML.getNumberOfAcks());	
			}
			try
			{
				AckXML.generateAckFile(ack_FILENAME);
			}
			catch (Exception e) //FileNotFoundException | UnsupportedEncodingException | NoSuchAlgorithmException | XMLStreamException | FactoryConfigurationError
			{	
				//What to do? Their XML parsed fine...
				//a problem with the ACKNOLLAGE FILE!
				//REPORT IT!, RE-PARSE IT PLEASE ONCE ISSUE IS LOOKED AT.
				e.printStackTrace();
				ErrorReporting.reportCritical(e);
			}
		}
		catch (Exception eee)
		{
			//Anything uncought. Email it off...
			eee.printStackTrace();
			ErrorReporting.reportCritical(eee);
		}
	}	

}
