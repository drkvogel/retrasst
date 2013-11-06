package cardiffOxfordDataTransfer;

import java.io.File;
import cardiffOxfordDataTransfer.globals;
import java.io.IOException;
import java.security.NoSuchAlgorithmException;
import java.text.ParseException;
import java.util.Calendar;
import java.util.Date;
import java.util.TreeMap;

import javax.xml.XMLConstants;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import org.xml.sax.Attributes;
import org.xml.sax.ErrorHandler;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.DefaultHandler;


/**
 * @author jonathanp
 *
 */
public class ValidationXMLParser extends DefaultHandler
{
		
	TreeMap m_SectionData = new TreeMap();		
	TreeMap m_HeaderData = new TreeMap();		
	String m_SectionMD5;
	
	varValidation m_varValidation;
	String m_CurrentSectionID; //section->SectionId;
	String m_SectorfilePartId; //section->PartId;
	String m_SectorfileSessionId; //section->SessionId;
		
	int m_XMLnumSections = 0;
	
	String m_SchemaFile;
	String m_XMLFile;
	
	duplicateCheckVar m_duplicateChecker; //checking for duplicate variables
	duplicateCheckVar m_sourceVaribleChecker; //checking each section does contain ALL variables it should.
	AcknowledgeXML m_AckXML;
	
	static boolean m_inCarfilePart;
	
	public ValidationXMLParser()
	{
		m_duplicateChecker = new duplicateCheckVar();
		m_sourceVaribleChecker = new duplicateCheckVar();
		m_inCarfilePart = true;
	}

	/**
	 * @return the number of sections found in the XML
	 */
	public int getXMLnumSections()
	{
		return m_XMLnumSections;
	}
	
	/**
	 * @param XMLFile	Path and file name of the XML file to parse. i.e. the Cardif unencrypted data
	 * @param SchemaFile Path and file name of the Schema file to validate the XML against
	 * @param Ack		What to store the Ackowledgements so we can report back.
	 * @param varValidator	What we use to validate the incoming XML data against.
	 */
	public void setUp(String XMLFile,String SchemaFile, AcknowledgeXML Ack,varValidation varValidator)
	{
		m_AckXML = Ack;
		m_XMLFile = XMLFile;
		m_SchemaFile = SchemaFile;
		m_varValidation =  varValidator;
	}
	
	/**
	 * We get some structural or other major issue where we should stop parsing and report back to Cardiff.
	 * @param ErrReport 	The Report to send back.
	 */
	public void addAckXMLCriticalError(String ErrReport)
	{
		m_AckXML.setCritical(m_SectorfilePartId,m_SectorfileSessionId, m_CurrentSectionID,ErrReport);
	}
	
	/**
	 * Kicks the parser off.
	 * 
	 * @throws IOException
	 * @throws ParserConfigurationException
	 * @throws SAXException
	 * @throws XMLParseException
	 */
	public void parse() throws IOException, ParserConfigurationException, SAXException, XMLParseException 
	{
		m_CurrentSectionID = "-1";			
	
		SAXParserFactory spf = SAXParserFactory.newInstance();
		spf.setNamespaceAware(true);
		
	 	SAXParser saxParser = spf.newSAXParser();
		XMLReader xmlReader = saxParser.getXMLReader();
		xmlReader.setContentHandler(this);
		xmlReader.setErrorHandler(new MyErrorHandler(m_AckXML,this));	
				
		xmlReader.parse(new InputSource(m_XMLFile));
	}

	public void startDocument()
	{
	} 

	public void startElement(String namespaceURI, String localName, String qName, Attributes atts) 
	{					
		if (localName.compareTo(globals.DEFINE_CARFILE_NAME) == 0)
		{
			m_HeaderData.clear();
			m_inCarfilePart = true;
			for (int i=0;i<atts.getLength();i++)
			{
				if (atts.getLocalName(i).compareTo(globals.DEFINE_CARFILE_OUTPUTID) == 0)
				{
					m_AckXML.setOutPutID(atts.getValue(i));			
				}
				else if (atts.getLocalName(i).compareTo(globals.DEFINE_CARFILE_NUMSECTIONS)==0)
				{
					m_XMLnumSections = Integer.parseInt(atts.getValue(i));
				}
				m_HeaderData.put(atts.getLocalName(i),atts.getValue(i)); //FOR header validating
			}	
			
			//report any header issues to the comment section
			validateHeader();
		}		
		else if (localName.compareTo(globals.DEFINE_SECTION_NAME)==0)
		{
			m_inCarfilePart = false;
			m_CurrentSectionID = "?"; //section->SectionId;
			m_SectorfilePartId = "?"; //section->PartId;
			m_SectorfileSessionId = "?"; //section->SessionId;
			m_SectionMD5 = "?";
			
			//reset the duplicate checker, as this is a new section
			m_duplicateChecker.ClearForNewSet(); 
			m_sourceVaribleChecker.ClearForNewSet();
			for (int i=0;i<atts.getLength();i++)
			{
				if (atts.getLocalName(i).compareTo(globals.DEFINE_CHECKDM5_NAME) == 0)
					m_SectionMD5 = atts.getValue(i);
				else if (atts.getLocalName(i).compareTo(globals.DEFINE_DATECOMPLETED_NAME) != 0) 
				{
					   //IN HERE SHOULD BE EVERY HEADER VARIABLE APART FROM DATE_COMPLETE AND CHECKSUM	
 					m_SectionData.put(atts.getLocalName(i),atts.getValue(i)); //FOR mD5
				}
				//save for ack generation.
				if (atts.getLocalName(i).compareTo(globals.DEFINE_PARTID_NAME) == 0)
					m_SectorfilePartId = atts.getValue(i);
				else if (atts.getLocalName(i).compareTo(globals.DEFINE_SESSIONID_NAME) == 0)
					m_SectorfileSessionId = atts.getValue(i);			
				else if (atts.getLocalName(i).compareTo(globals.DEFINE_SECTION_ID_NAME)==0)
					m_CurrentSectionID = atts.getValue(i); 		
			}	
			
			if (m_CurrentSectionID.compareTo("?") == 0)
				ErrorReporting.m_AckErrorString += "sectionid is not defined;";	
			if (m_SectorfilePartId.compareTo("?") == 0)
				ErrorReporting.m_AckErrorString += "partid is not defined;";	
			if (m_SectorfileSessionId.compareTo("?") == 0)
				ErrorReporting.m_AckErrorString += "sessionid is not defined;";	
			if (m_SectionMD5.compareTo("?") == 0)
				ErrorReporting.m_AckErrorString += "checkMD5 is not defined;";
			
			if (ErrorReporting.m_AckErrorString.length() == 0)
			{
				//check participant id is in the valid range
				try
				{
					int partID = Integer.parseInt(m_SectorfilePartId);
					if ((partID <800000000) || (partID >899999999))
					{							
						ErrorReporting.m_AckErrorString += "Partid out side of valid range;";	
					}
				}
				catch (NumberFormatException e)
				{
					ErrorReporting.m_AckErrorString += "Partid not valid;";	
				}
				//check section id is in the valid range
				try
				{
					int sectionID = Integer.parseInt(m_CurrentSectionID);
					if ((sectionID <1) || (sectionID >9))
					{
						ErrorReporting.m_AckErrorString += "sectionid out side of valid range;";	
					}
				}
				catch (NumberFormatException e)
				{
					ErrorReporting.m_AckErrorString += "sectionid not valid;";	
				}
			
				try
				{
					Integer.parseInt(m_SectorfileSessionId);
				}
				catch (NumberFormatException e)
				{
					ErrorReporting.m_AckErrorString += "SessionID not valid;";	
				}
			}
		}
		else if (localName.compareTo(globals.DEFINE_QUESTION_RESPONSE_NAME) == 0)
		{
			if (ErrorReporting.m_AckErrorString.length() > 0)
				return;

			if (!IgnoreList.shouldIgnore(atts.getValue("qid")))
			{
				m_SectionData.put(atts.getValue("qid"),atts.getValue("resp"));
				
				boolean res = false;
				try
				{
		 			res = m_varValidation.validateVar(atts.getValue("qid"),atts.getValue("resp"),m_CurrentSectionID);
				}
				catch (Exception e)
				{
					e.printStackTrace();
					ErrorReporting.reportCritical(e); //can't rethrow. so deal with it here.
				}
				
				try
				{
					m_sourceVaribleChecker.isNotDuplicate(m_varValidation.getVarRegExCheckString(atts.getValue("qid"), m_CurrentSectionID));
				}
				catch (XMLParseException e)
				{
					//if this throws as it can't find the variable, or something, it would have thrown also above.
					//so we can ignore.
				}
					
				if (!res)
				{
					ErrorReporting.m_AckErrorString += atts.getValue("qid") +":" + atts.getValue("resp") + ";";
				}
				
				if (!m_duplicateChecker.isNotDuplicate(atts.getValue("qid")))
				{
					ErrorReporting.m_AckErrorString += atts.getValue("qid") + " duplicate;";
	//				System.out.println("FAIL, " + atts.getValue("qid") + " is a duplicate");		
				}
			}
		}	
	}

	private void validateHeader()
	{
		//validate header
		//check attribs exists, and are within parameters				
		String outid = (String) m_HeaderData.get(globals.DEFINE_CARFILE_OUTPUTID);
		if (outid == null)
			m_AckXML.addComment("not defined carfile outputid");			
		else
		{
			try
			{
				Integer.parseInt(outid);
			}
			catch(NumberFormatException e)
			{
				m_AckXML.addComment("carfile outputid looks suspect");				
			}
		}
		
		outid = (String) m_HeaderData.get(globals.DEFINE_CARFILE_NUMSECTIONS);
		if (outid == null)
			m_AckXML.addComment("carfile numSections is not defined");			
		else
		{
			try
			{
				Integer.parseInt(outid);
			}
			catch(NumberFormatException e)
			{
				m_AckXML.addComment("carfile numSections looks suspect");				
			}
		}
		
		outid = (String)m_HeaderData.get(globals.DEFINE_CARFILE_TIMEFILEGENERATED);
		if (outid == null)
			m_AckXML.addComment("carfile timeFileGenerated is not defined");			
		else
		{
			try
			{
				Date dt = Utils.ISODateStringToDate(outid);
				Calendar c = Calendar.getInstance();
				c.set(2013, 8, 1, 1, 1); //sanity check, make sure not before Sept 2013
				Date dateSept2013 = c.getTime();
				if (dt.before(dateSept2013))
				{
					m_AckXML.addComment("carfile timeFileGenerated is before Sept 2013, invalid");
				}
				else
				{
					c.set(2017, 8, 1, 1, 1); //sanity check, make sure not after Sept 2017
					dateSept2013 = c.getTime();
					if (dt.after(dateSept2013))
						m_AckXML.addComment("carfile timeFileGenerated is after Sept 2017, invalid");
				}
			}
			catch (ParseException e)
			{
				m_AckXML.addComment("carfile timeFileGenerated is not a valid date");			
			}
		}
	}
	
	public void endElement(String namespaceURI, String localName, String qname)
	{
		if (localName.compareTo(globals.DEFINE_SECTION_NAME)==0)
		{			
			//validate start and end dates of this section

/*	WHEN CONFIRMED WE DON'T NEED THIS DATE. REMOVE THIS CODE, Validate start and end date in the var section
 *
 * 			@SuppressWarnings("unused")
			Date EndDate = null;
 * 
 * 		try
			{
				if (m_SectorfileDateCompleted == null)
				{
					m_AckXML.addComment("SessionID["+m_SectorfileSessionId+"]-CompleteDate is null");			
				}
				else
					EndDate = Utils.ISODateStringToDate(m_SectorfileDateCompleted);
			}
			catch (ParseException e1)
			{
				m_AckXML.addComment("SessionID["+m_SectorfileSessionId+"]-EndDate can't be validated");
			}
*/				
/*			
			boolean passedTest = true;
			if (m_CurrentSectionID.compareTo("?") == 0)
			{
				passedTest = false;
				ErrorReporting.m_AckErrorString += "sectionid is not defined;";	
			}
			if (m_SectorfilePartId.compareTo("?") == 0)
			{
				passedTest = false;
				ErrorReporting.m_AckErrorString += "partid is not defined;";	
			}
			if (m_SectorfileSessionId.compareTo("?") == 0)
			{
				passedTest = false;
				ErrorReporting.m_AckErrorString += "sessionid is not defined;";	
			}
			if (m_SectionMD5.compareTo("?") == 0)
			{
				passedTest = false;
				ErrorReporting.m_AckErrorString += "checkMD5 is not defined;";	
			}
			
			//check participant id is in the valid range
			try
			{
				int partID = Integer.parseInt(m_SectorfilePartId);
				if ((partID <800000000) || (partID >899999999))
				{
					passedTest = false;
					ErrorReporting.m_AckErrorString += "Partid out side of valid range;";	
				}
			}
			catch (NumberFormatException e)
			{
				passedTest = false;
				ErrorReporting.m_AckErrorString += "Partid not valid;";	
			}
			//check section id is in the valid range
			try
			{
				int sectionID = Integer.parseInt(m_CurrentSectionID);
				if ((sectionID <1) || (sectionID >9))
				{
					passedTest = false;
					ErrorReporting.m_AckErrorString += "sectionid out side of valid range;";	
				}
			}
			catch (NumberFormatException e)
			{
				passedTest = false;
				ErrorReporting.m_AckErrorString += "sectionid not valid;";	
			}
		
			try
			{
				Integer.parseInt(m_SectorfileSessionId);
			}
			catch (NumberFormatException e)
			{
				passedTest = false;
				ErrorReporting.m_AckErrorString += "SessionID not valid;";	
			}
*/
			if (ErrorReporting.m_AckErrorString.length() == 0)
			{
				try
				{
					if (!md5Util.checkMD5(m_SectionMD5, m_SectionData))
						ErrorReporting.m_AckErrorString += "Failed MD5 check;";
				}
				catch (NoSuchAlgorithmException e)
				{
					e.printStackTrace();
					ErrorReporting.reportCritical(e); //deal with it here.
					return;
				}
			}		
			
			if (ErrorReporting.m_AckErrorString.length() == 0)
			{
				try
				{
					String result = m_varValidation.validateSectionData(m_CurrentSectionID,m_SectionData);
					if (result.compareTo("OK")!=0)
					{
						ErrorReporting.m_AckErrorString += "failed the validation ["+ result +"];";
					}
				}
				catch (XMLParseException e1)
				{ //PARSING AN INT WITH IN VALIDATESECTION.. should never happen....
					e1.printStackTrace();
					ErrorReporting.reportCritical(e1); //deal with it here.
				}
		
				if (ErrorReporting.m_AckErrorString.length() == 0)
				{ //only check if no errors
					if (!m_duplicateChecker.sectionHasData())
					{
						ErrorReporting.m_AckErrorString += "Section has no data;";
					}
				}
									
				try
				{
					if (m_sourceVaribleChecker.size() != m_varValidation.getNumberInSection(m_CurrentSectionID))
					{
						ErrorReporting.m_AckErrorString += "Looks like we are missing some varaibles("+m_sourceVaribleChecker.size()+"/"+m_varValidation.getNumberInSection(m_CurrentSectionID)+");";	
					}
				}
				catch (XMLParseException e)
				{
					//this really should never happen.
					// but if it did, it's caught above and report added so Cardiff will know there is a problem.
				}
			}	
			
			m_SectionData.clear();
			
			if (ErrorReporting.m_AckErrorString.length()!=0) //do have have an error?
				m_AckXML.add(m_SectorfilePartId,m_SectorfileSessionId, m_CurrentSectionID,globals.DEFINE_ACK_ERROR_STATUS,ErrorReporting.m_AckErrorString);
			else
				m_AckXML.add(m_SectorfilePartId,m_SectorfileSessionId, m_CurrentSectionID,globals.DEFINE_ACK_NOERROR_STATUS, null);
				
			//clear any errors, ready for next section
			ErrorReporting.m_AckErrorString = "";
		}
	}

	public void endDocument() throws SAXException
	{
	}
	
	private static class MyErrorHandler implements ErrorHandler
	{
		private ValidationXMLParser m_SectionXMLData;
		private AcknowledgeXML m_AckXML;

		MyErrorHandler(AcknowledgeXML AckXML, ValidationXMLParser SectionXMLData)
		{
			m_AckXML = AckXML;
			m_SectionXMLData = SectionXMLData; 
		}

		private String getParseExceptionInfo(SAXParseException spe)
		{
			String systemId = spe.getSystemId();

			if (systemId == null)
			{
				systemId = "null";
			}
			String info = /*"URI=" + systemId +*/ "Line=" + spe.getLineNumber() + ": " + spe.getMessage();
			return info;
		}

		public void warning(SAXParseException spe) 
		{
			if (m_inCarfilePart) //if in carfile section, enter as a comment
				m_AckXML.addComment(getParseExceptionInfo(spe));
			else
				ErrorReporting.m_AckErrorString += getParseExceptionInfo(spe);
		}

		public void error(SAXParseException spe)
		{
			if (m_inCarfilePart) //if in carfile section, enter as a comment
				m_AckXML.addComment(getParseExceptionInfo(spe));
			else
				ErrorReporting.m_AckErrorString += getParseExceptionInfo(spe);
		}

		public void fatalError(SAXParseException spe) 
		{ //THIS IS CORRECT. NO EMAIL TO CTSU, A REPORT TO CARDIF IS NEEDED. IT'S A PARSING ERROR ON THEIR XML
			ErrorReporting.m_AckErrorString += getParseExceptionInfo(spe);
			m_SectionXMLData.addAckXMLCriticalError(ErrorReporting.m_AckErrorString);
			ErrorReporting.m_AckErrorString = "";
		}
	}
}
