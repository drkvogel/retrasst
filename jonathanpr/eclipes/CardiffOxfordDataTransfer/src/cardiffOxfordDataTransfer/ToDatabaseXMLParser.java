package cardiffOxfordDataTransfer;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.security.NoSuchAlgorithmException;
import java.sql.SQLException;
import java.text.ParseException;
import java.util.HashMap;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.Attributes;
import org.xml.sax.ErrorHandler;
import org.xml.sax.InputSource;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.DefaultHandler;

import cardiffOxfordDataTransfer.globals;

/**
 * @author jonathanp
 *
 */
public class ToDatabaseXMLParser extends DefaultHandler
{		
	HashMap m_SectionData = new HashMap();		
	HashMap m_RespData = new HashMap();		
	HashMap m_CarFileData = new HashMap();		
	String m_XMLFile;
	AcknowledgeXML m_ValidatedAckXML;
	varValidation m_Validatior;
	databaseAccess m_databaseAccess;
	int m_xml_id;
	//To extract RAW XML
	SaxInputStream m_sis = null; 
	private Locator m_locator;
	int m_SectionStartLine = 0;

	public ToDatabaseXMLParser()
	{
	}
	
	public void setDocumentLocator(Locator locator)
	{
		this.m_locator = locator;
	}
	
	/**
	 * Call this to setup the parsing.
	 * @param XMLFile  	The XML to be parsed
	 * @param Ack		The Acknowledgement file creation class
	 * @param validator	The class used to validate the data against
	 * @throws Exception 
	 * @throws SQLException 
	 * @throws ClassNotFoundException 
	 * @throws IllegalAccessException 
	 * @throws InstantiationException 
	 */
	public void setUp(String XMLFile, AcknowledgeXML Ack, varValidation validator) throws Exception
	{
		m_ValidatedAckXML = Ack;
		m_XMLFile = XMLFile;
		m_Validatior = validator;
		m_databaseAccess = new databaseAccess(m_Validatior);
	}
	
	/**
	 * Calling this sets off the parsing
	 * @throws IOException
	 * @throws ParserConfigurationException
	 * @throws SAXException
	 * @throws XMLParseException
	 */
	public void parse() throws IOException, ParserConfigurationException, SAXException, XMLParseException 
	{
		final SAXParserFactory spf = SAXParserFactory.newInstance();
		spf.setNamespaceAware(true);
		
		File f = new File(m_XMLFile);
		FileInputStream fis = new FileInputStream(f);
		m_sis = new SaxInputStream(fis);
		InputSource is = new InputSource(m_sis);
		 
	 	final SAXParser saxParser = spf.newSAXParser();
		final XMLReader xmlReader = saxParser.getXMLReader();
		xmlReader.setContentHandler(this);
		xmlReader.setErrorHandler(new MyErrorHandler(this));		
		xmlReader.parse(is);
	}
	
	public void startElement(String namespaceURI, String localName, String qName, Attributes atts) 
	{			
		if (localName.compareTo(globals.DEFINE_CARFILE_NAME) == 0)
		{
			for (int i=0;i<atts.getLength();i++)
			{
				m_CarFileData.put(atts.getLocalName(i),atts.getValue(i));
			}				
		}		
		else if (localName.compareTo(globals.DEFINE_SECTION_NAME)==0)
		{		
			m_SectionStartLine = m_locator.getLineNumber();
			m_RespData.clear();
			//reset the duplicate checker, as this is a new section
			for (int i=0;i<atts.getLength();i++)
			{
				m_SectionData.put(atts.getLocalName(i),atts.getValue(i)); //FOR mD5
			}				
		}
		else if (localName.compareTo(globals.DEFINE_QUESTION_RESPONSE_NAME) == 0)
		{
			if (!IgnoreList.shouldIgnore(atts.getValue("qid")))
				m_RespData.put(atts.getValue("qid"),atts.getValue("resp"));			
		}
	}
	
	public void endElement(String namespaceURI, String localName, String qname)
	{
		if (localName.compareTo(globals.DEFINE_SECTION_NAME)==0)
		{		
			final String SectionName = (String) m_SectionData.get(globals.DEFINE_SECTION_ID_NAME);
			final String SessionID = (String) m_SectionData.get(globals.DEFINE_SESSIONID_NAME);
			final String PersonID = (String) m_SectionData.get(globals.DEFINE_PARTID_NAME);
		
			//is this in the ACK file as status 1? i.e. is OK to put in the db		
			if (!m_ValidatedAckXML.isSessionValid(PersonID,SessionID,SectionName))
				return; //not a valid section.
	
			//Is not already in DB? lets see
			int rec_id = -1;
			try
			{
				if (m_databaseAccess.contains(PersonID,SessionID,SectionName)!=-1)
					return;
				
				int LN = m_locator.getLineNumber();			
				
//				System.out.println("From:" + LN + " to " + m_SectionStartLine + " MAX:" );
				
				String RawXML = m_sis.getLines(m_SectionStartLine, LN);
				RawXML = RawXML.trim();
			
				rec_id = m_databaseAccess.addSectionInformation(m_SectionData,RawXML);
				if (rec_id == -1)
				{
					throw new SQLException("Could not add a row to the database");
				}
				
				m_databaseAccess.addVaribleInformation(rec_id,m_RespData,SectionName);
			}
			catch (SQLException e1)
			{
				dealWithExecption(e1);
			}			
			catch (NoSuchAlgorithmException e1)
			{
				dealWithExecption(e1);
			}			
			catch (ParseException e1)
			{
				dealWithExecption(e1);
			}			
			m_SectionData.clear();		
		}
	}
	
	public void endDocument() throws SAXException
	{
		try
		{
			m_databaseAccess.commit();	
		}
		catch (SQLException e1)
		{
			dealWithExecption(e1);
		}			
	}
	
	private void dealWithExecption(Exception e1)
	{
		e1.printStackTrace();
		final XMLParseException e = new XMLParseException("ERROR ADDING TO DATABASE  :" + e1.getMessage());		
		try
		{
			m_databaseAccess.rollback();
		}
		catch (SQLException e2)
		{
			e2.printStackTrace();
			final XMLParseException e4 = new XMLParseException("ERROR ROLLING BACK DATABASE AFTER ERROR ADDING TO DB: " + e1.getMessage() + ":" + e2.getMessage());		
			ErrorReporting.reportCritical(e4); //This needs fixing, alert CTUS
		}
		ErrorReporting.reportCritical(e); //This needs fixing, alert CTUS		
	}

	private static class MyErrorHandler implements ErrorHandler
	{
		MyErrorHandler(ToDatabaseXMLParser SectionXMLData)
		{ 
		}

		private String getParseExceptionInfo(SAXParseException spe)
		{
			String systemId = spe.getSystemId();

			if (systemId == null)
			{
				systemId = "null";
			}
			final String info = /*"URI=" + systemId +*/ "Line=" + spe.getLineNumber() + ": " + spe.getMessage();
			return info;
		}

		public void warning(SAXParseException spe) 
		{
			//we know all this, from the previous run through.. so ignore
		}

		public void error(SAXParseException spe)
		{
			//we know all this, from the previous run through.. so ignore
		}

		public void fatalError(SAXParseException spe) //throws SAXException 
		{//We could get this error, but we don't care as if it got this far, it is going to be reported. 
//			final String message = "Error: " + getParseExceptionInfo(spe);
//			throw new SAXException(message); 			
		}
	}
}
