package cardiffOxfordDataTransfer;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.security.NoSuchAlgorithmException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.TreeMap;
import java.util.Vector;

import javax.xml.stream.FactoryConfigurationError;
import javax.xml.stream.XMLOutputFactory;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamWriter;


/** 
 * Keeps track of the acknowledgements and general health of the XML as we parse it. 
 * This also generates the final Acknowledgement file which is to be sent back to Cardiff.
 */
public class AcknowledgeXML
{
	private final boolean DEFINE_ALLOW_COMMENTS = false;
		
	private final String STATUS_STRING = "status";
	private final String PERSONID_STRING = "partid";
	private final String SESSIONID_STRING = "sessionid";
	private final String SECTIONID_STRING = "sectionid";
	private final String ERR_STRING = "err";
	
	// Somewhere to keep these Acknowledgements till ready to send
	Vector m_Acknowledgements = new Vector();
	//Collect the parts of each user session. So we can check for duplicates and full sets of parts
	HashMap m_RosettaCreationList = new HashMap();
	String m_outputId;
	Vector m_comment = new Vector();
	boolean hasCriticalError = false;
	/**
	 * @return The HashMap which tracks participant^session and their corresponding parts successfully passed first pass parse check.
	 */
/*	
	public HashMap<String, Integer> get_RosettaCreationList()
	{
		return m_RosettaCreationList;
	}
*/
	/**
	 * @param outPutID	This is the outputID in the cardat.xml we are importing from Cardif. It is used for Cardif to match up our returning Acknowledge file to their XML deposit.
	 */
	public void setOutPutID(String outPutID)
	{
		m_outputId = outPutID;
	}

	/**
	 * @return the number of Acknowledgements we have collected.
	 */
	public int getNumberOfAcks()
	{
		return m_Acknowledgements.size();
	}
	
	public boolean hasCriticalError()
	{
		return hasCriticalError;
	}
	
	/**
	 * @return true of at least one entery has the status of 1
	 */
	public boolean hasValidDate()
	{
		Iterator it = m_Acknowledgements.iterator();
		while (it.hasNext())
		{
			TreeMap data = (TreeMap) it.next();
			if (((String)data.get(STATUS_STRING)).compareTo("1") == 0)
					return true;
		}
		return false;
	}

	/**
	 * @param PersonID The participant identification string to check against  
	 * @param Sessionid The SessionID to check against
	 * @param SectionName The SectionID to check against
	 * @return True if all the params match and it has a status of 1, which means it was parsed OK.
	 */
	public boolean isSessionValid(String PersonID, String Sessionid, String SectionName)
	{
		Iterator it = m_Acknowledgements.iterator();
		while (it.hasNext())
		{
			TreeMap data = (TreeMap) it.next();
			if ((((String)(data.get(PERSONID_STRING))).compareTo(PersonID) == 0) && (((String)data.get(SESSIONID_STRING)).compareTo(Sessionid) == 0) && (((String)data.get(SECTIONID_STRING)).compareTo(SectionName) == 0))
			{
				if (((String)data.get(STATUS_STRING)).compareTo("1") == 0)
					return true;
				return false;
			}
		}
		return false;
	}

	/**
	 * This is to be used when there is a problem with the structure of the XML.
	 * Stop parsing, and report back the issue so Cardif can fix it and re-submit.
	 * 
	 * @param partid 	The participantid where the issue was caused, use null if not possible to give.
	 * @param sessionid The sessionid where the issue was caused, use null if not possible to give. 
	 * @param sectionid The section where the issue was caused, use null if not possible to give.
	 * @param Err 		The message to give to Cardif so they can locate the issue.
	 */
	public void setCritical(String partid, String sessionid, String sectionid, String Err)
	{
		if (partid == null)
			partid = "";
		if (sessionid == null)
			sessionid = "";
		if (sectionid == null)
			sectionid = "";
		m_Acknowledgements.clear();
		TreeMap newAck = new TreeMap();
		newAck.put(PERSONID_STRING, partid);
		newAck.put(SESSIONID_STRING, sessionid);
		newAck.put(SECTIONID_STRING, sectionid);
		newAck.put(STATUS_STRING, "3"); // STATUS IS ERROR <> 1. 2 = Error, 3 = Critical
		newAck.put(ERR_STRING, Err);
		m_Acknowledgements.add(newAck);
		hasCriticalError = true;
		
		if (DEFINE_ALLOW_COMMENTS)
			m_comment.add("ALERT: Critical parsing error detected, could not complete the parsing of the XML data.");
	}

	/**
	 * Call this every time we find a problem with Cardif data or we have sucesfully parsed a section. The program will continue to parse if we are submitting an error. 
	 * The errors are kept to minor problems, like it fails MD5 or perhaps a data field is out of range. If there is an issue with structure of the XML use setCritical().
	 * @param partid	The participantid where the issue was caused.
	 * @param sessionid The sessionid where the issue was caused
	 * @param sectionid The sectionid where the issue was caused
	 * @param status	The status, 1 is sucessfully parsed, <> 1 means there is a problem.
	 * @param Err		Used when there is a problem i.e. status <> 1.
	 */
	public void add(String partid, String sessionid, String sectionid, String status, String Err)
	{
		//if part is OK, add so the list to create Rosetta's from later
		if (status.compareTo("1") == 0)
		{
			String PartSession = partid+":"+sessionid;
			Integer sectionList = new Integer(0);
			if (m_RosettaCreationList.containsKey(PartSession))
			{
				sectionList = (Integer) m_RosettaCreationList.get(PartSession);
				m_RosettaCreationList.remove(PartSession);
			}

			int SectionID = Integer.parseInt(sectionid);
						
			if ((sectionList.intValue() & (1<<SectionID))==1) //DUPLICATE!
				addComment("Duplicate sectionid detected. Partid:" + partid + ", SessionId:" + sessionid + ",Sectionid:"+ sectionid );
			else
				sectionList = new Integer(sectionList.intValue() + (1<<SectionID));
			
			m_RosettaCreationList.put(PartSession,sectionList);
		}
		//add to the acknowledgement list	
		TreeMap newAck = new TreeMap();
		newAck.put(PERSONID_STRING, partid);
		newAck.put(SESSIONID_STRING, sessionid);
		newAck.put(SECTIONID_STRING, sectionid);
		newAck.put(STATUS_STRING, status);
		if (Err != null)
			newAck.put(ERR_STRING, Err);
		m_Acknowledgements.add(newAck);
	}

	/**
	 * For reporting back general remarks about the submitted XML. Either an issue which they should know about but can't be attributed to 
	 * a section or if it's unknown if it's an issue or not. Like if there a duplicates in the submitted data. 
	 * @param Comment The comment to add to the Acknowledge file
	 */
	public void addComment(String Comment)
	{
		m_comment.add("WARNING:" + Comment);
	}

	/**
	 * Creates the acknowledgement file, it will still need encrypting.
	 * @param fileAndPath	Path and filename of the file to be created
	 * @throws FactoryConfigurationError	Can't create XMLOutputFactory.
	 * @throws Exception	Throws if we can't delete the old acknowledgement file.
	 */
	public void generateAckFile(String fileAndPath) throws FactoryConfigurationError, Exception
	{
		// delete old file
		File f = new File(fileAndPath);

		if (f.exists())
		{
			if (!f.delete())
				throw new Exception("Can't delete old Ack File, is it locked?");
		}

		// anything new to create?
		if (m_Acknowledgements.size() == 0)
			return;

		OutputStream outputStream = new FileOutputStream(new File(fileAndPath));
		XMLStreamWriter out = XMLOutputFactory.newInstance().createXMLStreamWriter(new OutputStreamWriter(outputStream, "utf-8"));
		generateStartXMLAck(out, m_Acknowledgements.size(), m_outputId);
		Iterator it = m_Acknowledgements.iterator();
		while (it.hasNext())
			generateXMLAck(out, (TreeMap) it.next());
		generateEndXMLAct(out);
		out.close();
		m_comment.clear();
		hasCriticalError = false;
		// m_Acknowledgements.clear();
	}

	private void generateStartXMLAck(XMLStreamWriter out, int count, String outputId) throws XMLStreamException
	{
		if (outputId == null) // when parsing problems, this may not be set.
			outputId = "";

		Calendar rightNow = Calendar.getInstance();

		Date now = rightNow.getTime();
		String formatted = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss").format(now);

		out.writeStartDocument("UTF-8", "1.0");
		if (DEFINE_ALLOW_COMMENTS)
		{
			for (int i = 0; i < m_comment.size(); i++)
				out.writeComment((String) m_comment.get(i));
		}
		out.writeStartElement("ackfile");
		out.writeAttribute("outputId", outputId);
		out.writeAttribute("timeFileGenerated", formatted);
		out.writeAttribute("numAcks", Integer.toString(count));
	}

	private void generateXMLAck(XMLStreamWriter out, TreeMap ackData) throws XMLStreamException, NoSuchAlgorithmException
	{
		String md5 = md5Util.generateMD5(ackData);

		out.writeStartElement("ack");
		out.writeAttribute("partid", (String) ackData.get(PERSONID_STRING));
		out.writeAttribute("sessionid", (String) ackData.get(SESSIONID_STRING));
		out.writeAttribute("sectionid", (String) ackData.get(SECTIONID_STRING));
		out.writeAttribute("status", (String) ackData.get(STATUS_STRING));
		if (ackData.get("err") != null)
			out.writeAttribute("err", (String) ackData.get(ERR_STRING));

		out.writeAttribute("checkMD5", md5);
		out.writeEndElement();
		
	}

	private void generateEndXMLAct(XMLStreamWriter out) throws XMLStreamException
	{
		out.writeEndElement();
		out.writeEndDocument();
	}
}
