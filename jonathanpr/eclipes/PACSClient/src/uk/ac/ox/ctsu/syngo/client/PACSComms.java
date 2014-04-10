package uk.ac.ox.ctsu.syngo.client;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.rmi.RemoteException;
import java.util.Calendar;
import java.util.Date;

import javax.activation.DataHandler;
import javax.activation.FileDataSource;
import javax.xml.stream.XMLStreamException;

import org.apache.axiom.om.impl.builder.StAXOMBuilder;
import org.apache.axis2.AxisFault;
import org.apache.axis2.Constants;
import org.apache.axis2.client.Options;
import org.apache.axis2.client.ServiceClient;
import org.apache.axis2.context.ConfigurationContext;
import org.apache.axis2.context.ConfigurationContextFactory;
import org.apache.axis2.description.AxisService;
import org.apache.axis2.transport.http.HTTPConstants;
import org.apache.neethi.Policy;
import org.apache.neethi.PolicyEngine;

import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.Attachment;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.ChangeDocumentEntityRequest;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.ChangeDocumentOrganizationalUnitRequest;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.ChangeDocumentPatientRequest;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.ChangeDocumentPatientResponse;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.CopyDocumentRequest;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.CopyDocumentResponse;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.DicomImportResult;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.DicomOrgUnitImportRequest;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.DocumentInfo;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.ExportDicomImageRequest;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.ExportFilesForReferencePointerRequest;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.GenericFileInfo;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.GenericImportResult;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.GenericOrgUnitImportRequest;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.PatientSearch;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.VisitSearch;
import com.icoserve.www.va20_documentmanipulationservice.WsFault;
import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub;
import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.AddKeywordsToDicomStudyRequest;
import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.DicomSeriesKeywords;
import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.DicomStudyKeywords;
import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.KeywordAddInformation;
import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.RetrieveKeywordsForDicomSeriesRequest;
import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.RetrieveKeywordsForDicomStudiesRequest;
import com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.RetrieveKeywordsForDicomStudiesResponse;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomDocumentFuzzySearch;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomDocumentFuzzySearchResult;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomImage;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomSeries;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomStudy;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DocumentFuzzySearch;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.FindDicomDocumentsRequest;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.FindGenericDocumentsRequest;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.FindPatientsRequest;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GenericDocumentFuzzySearch;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GenericDocumentFuzzySearchResult;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDicomImagesForDicomSeriesRequest;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDicomSeriesForDicomStudyRequest;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDicomStudyRequest;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDirectAccessUrlForPatientResponse;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.KeywordSearch;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.Patient;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.PatientFuzzySearch;



public class PACSComms
{
	/**
	 * The endpoint of the service that should be called. Can be retrieved in webadmin (Tools -> Webservices).
	 */

	private static final String SERVICE_EPR_QUERYSERICE = "https://syngo.ndph.ox.ac.uk/ws_api/services/BIOBANK/VA20/QueryService";
	private static final String SERVICE_EPR_DOCMANIPULATION = "https://syngo.ndph.ox.ac.uk/ws_api/services/BIOBANK/VA20/DocumentManipulationService";
	private static final String SERVICE_EPR_KEYWORDSERICE = "https://syngo.ndph.ox.ac.uk/ws_api/services/BIOBANK/VA20/KeywordService";

	/*	
	private static final String SERVICE_EPR_QUERYSERICE = "https://163.1.206.34/ws_api/services/BIOBANK/VA20/QueryService";
	private static final String SERVICE_EPR_DOCMANIPULATION = "https://163.1.206.34/ws_api/services/BIOBANK/VA20/DocumentManipulationService";
	private static final String SERVICE_EPR_KEYWORDSERICE = "https://163.1.206.34/ws_api/services/BIOBANK/VA20/KeywordService";
	*/
	// The login of the webservice user.
	private static String USERLOGIN = "webapi";
	private static String PASSWORD = "webapi_bio";

	// The path of the server certificate. Can be retrieved in webadmin (Tools
	// -> Webservices).
	private static final String KEYSTORE_PATH = "webservice_api.jks";

	// The password of the keystore containing the server certificate (Must be specified when downloading the keystore from webadmin).
	private static final String KEYSTORE_PASSWORD = "reallybadkeystorepassword";

	// Path where the eclipse project is contained. (The "modules" and "policy"
	// directories must be directly under the given directory).
//	private static final String WORKING_DIR = "C:/code/jonathanpr/eclipes/PACSClient";
	private static final String WORKING_DIR = ".";

	// Path where axis2 is allowed to cache receiving files.
	private static final String LOCALE_FILE_CACHING_DIR = "temp";
	private static final String FILE_OUTPUT_DIR = "temp";

	private static final int CLIENT_TIMEOUT_MILLIS = 600000; // 10 minutes

	VA20_QueryServiceStub m_queryService = null;
	VA20_DocumentManipulationServiceStub m_documentManipulationService = null;
	VA20_KeywordServiceStub m_keywordService = null;
	
	public boolean signin()
	{
		return signin(USERLOGIN,PASSWORD);
	}
		
	public boolean signin(String username, String password)
	{
		USERLOGIN = username;
		PASSWORD = password;
		try
		{
			m_queryService = createServiceStub_QueryService();
			m_documentManipulationService = createServiceStub_DocumentManipulationService();
			m_keywordService = createServiceStub_KeywordService();
		}
		catch (Exception e)
		{
			return false;
		}
		return true;
	}
	
	
	/*
	public DicomImportResult importDicomToPatient(String dicomfileToImport) throws Exception 
	{
		System.out.println("Now importing file");

		final DicomOrgUnitImportRequest request = new DicomOrgUnitImportRequest();
		request.setData(new DataHandler(new FileDataSource(dicomfileToImport)));
		request.setOrganizationalUnitName("BIOBANK");

		try
		{
			final DicomImportResult result = m_documentManipulationService.importDicomImageIntoOrgUnit(request).get_return();
			System.out.println("Successfully imported new dicom file. New Reference pointer is: " + result.getReferencePointer());
			return result;
		}
		catch (com.icoserve.www.va20_documentmanipulationservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
		catch (RemoteException e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
	}
	*/
	public GenericImportResult importFile(String fileToImport, String PID, String Desc, String ArchiveFileName, long userPID) throws Exception 
	{
		final PatientSearch patientSearch = new PatientSearch();
		patientSearch.setPatientId(PID);
		
		final DocumentInfo documentInfo = new DocumentInfo();
		documentInfo.setDescription(Desc);
		documentInfo.setCreationTimestamp(Calendar.getInstance());		

		final GenericFileInfo fileInfo = new GenericFileInfo();
		fileInfo.setDescription("BioBank Clinincal Information"); 
		fileInfo.setArchiveFileName(ArchiveFileName);
		fileInfo.setFileName(ArchiveFileName);		
		fileInfo.setExternalFileId(ArchiveFileName);
			
		final GenericOrgUnitImportRequest request = new GenericOrgUnitImportRequest();
		request.setPatientSearch(patientSearch);
		request.setDocumentInfo(documentInfo);
		request.setGenericFileInfo(fileInfo);
		request.setData(new DataHandler(new FileDataSource(fileToImport)));
		request.setOrganizationalUnitName("BIOBANK_PSI");
		
		//the documentation seams to say this throws if it fails!
		final GenericImportResult result = m_documentManipulationService.importGenericFileIntoOrgUnit(request).get_return();	
		ChangeDocumentPatientRequest changeDocumentPatientRequest = new ChangeDocumentPatientRequest();
		changeDocumentPatientRequest.setDocumentPk(result.getDocumentPk());
		changeDocumentPatientRequest.setPatientPk(userPID);
		ChangeDocumentPatientResponse cdr = m_documentManipulationService.changeDocumentPatient(changeDocumentPatientRequest);
		return result;
	}
	
	
	public DicomImage[] searchForDicomInSeries(long SeriesID) throws Exception
	{
//		final VA20_QueryServiceStub queryService = createServiceStub_QueryServic();
		GetDicomImagesForDicomSeriesRequest dicomSeriesID = new GetDicomImagesForDicomSeriesRequest();
		dicomSeriesID.setDicomSeriesPk(SeriesID);
		
		try
		{
			final DicomImage[] DicomResults = m_queryService.getDicomImagesForDicomSeries(dicomSeriesID).get_return();
			return DicomResults;
		}
		catch (com.icoserve.www.va20_queryservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
		catch (RemoteException e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
	}
	
	public DicomDocumentFuzzySearchResult[] searchForStudiesAfterInsertDate(Calendar insertdate,String OUName) throws Exception
	{
		final DicomDocumentFuzzySearch dicomFuzzySearch = new DicomDocumentFuzzySearch();
		final DocumentFuzzySearch docFuzzySearch = new DocumentFuzzySearch();
		
		docFuzzySearch.addOrganizationalUnitNames(OUName);	
		docFuzzySearch.setInsertionDateFrom(insertdate);
	
		dicomFuzzySearch.setDocumentFuzzySearch(docFuzzySearch);
				
		KeywordSearch keywordsearch = new KeywordSearch(); 
		dicomFuzzySearch.addKeywordSearches(keywordsearch);
//		dicomFuzzySearch.setMaximumResults(20); no restriction, as we don't know if it returns oldest first, randomly.. or what.
		FindDicomDocumentsRequest FDDR = new FindDicomDocumentsRequest();
		FDDR.setDicomDocumentFuzzySearch(dicomFuzzySearch);

		try
		{
			final DicomDocumentFuzzySearchResult[] DicomResults = m_queryService.findDicomDocuments(FDDR).get_return();
			return DicomResults;
		}
		catch (com.icoserve.www.va20_queryservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
	}
	
	public DicomDocumentFuzzySearchResult[] searchForPatientStudies(Calendar studydate,Calendar tonow,String []OUName) throws Exception
	{
		final DicomDocumentFuzzySearch dicomFuzzySearch = new DicomDocumentFuzzySearch();
		final DocumentFuzzySearch docFuzzySearch = new DocumentFuzzySearch();
		
		if (OUName != null)
			docFuzzySearch.setOrganizationalUnitNames(OUName);	
		
		//give some overlap to our searches.
		Calendar studydateback1hour = studydate;
		studydateback1hour.add(Calendar.HOUR, -24*14); //scan the last 2 weeks.

		docFuzzySearch.setInsertionDateFrom(studydateback1hour);
		docFuzzySearch.setInsertionDateTo(tonow);
	
		dicomFuzzySearch.setDocumentFuzzySearch(docFuzzySearch);
		dicomFuzzySearch.setMaximumResults(Long.MAX_VALUE);
		FindDicomDocumentsRequest FDDR = new FindDicomDocumentsRequest();
		FDDR.setDicomDocumentFuzzySearch(dicomFuzzySearch);

		try
		{
			final DicomDocumentFuzzySearchResult[] DicomResults = m_queryService.findDicomDocuments(FDDR).get_return();
			return DicomResults;
		}
		catch (com.icoserve.www.va20_queryservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
	}
	
	public DicomDocumentFuzzySearchResult[] searchForPatientStudies(String PatientID,String OUName) throws Exception
	{
//		final VA20_QueryServiceStub queryService = createServiceStub_QueryServic();
		final DicomDocumentFuzzySearch dicomFuzzySearch = new DicomDocumentFuzzySearch();
		final PatientFuzzySearch patientFuzzySearch = new PatientFuzzySearch();
		final DocumentFuzzySearch docFuzzySearch = new DocumentFuzzySearch();
		
		docFuzzySearch.addOrganizationalUnitNames(OUName);	
		patientFuzzySearch.setPatientId(PatientID);
	
		dicomFuzzySearch.setDocumentFuzzySearch(docFuzzySearch);
		
		dicomFuzzySearch.setPatientFuzzySearch(patientFuzzySearch);
		dicomFuzzySearch.setMaximumResults(20);
		FindDicomDocumentsRequest FDDR = new FindDicomDocumentsRequest();
		FDDR.setDicomDocumentFuzzySearch(dicomFuzzySearch);

		try
		{
			final DicomDocumentFuzzySearchResult[] DicomResults = m_queryService.findDicomDocuments(FDDR).get_return();
			return DicomResults;
		}
		catch (com.icoserve.www.va20_queryservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
	}
	
	public DicomStudyKeywords[] getForPatientStudyKeyWords(long studyPK) throws Exception
	{
		RetrieveKeywordsForDicomStudiesRequest request = new RetrieveKeywordsForDicomStudiesRequest();
		try
		{
			long keys[] = new long[1];
			keys[0] = studyPK;
			request.setDicomStudyPks(keys);
			DicomStudyKeywords[] keywordResults = m_keywordService.retrieveKeywordsForDicomStudies(request).get_return();
			return keywordResults;		
		}
		catch (com.icoserve.www.va20_keywordservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}		
	}
	
	public DicomSeriesKeywords[] getForPatientSeriesKeyWords(long seriesPK) throws Exception
	{
		RetrieveKeywordsForDicomSeriesRequest request = new RetrieveKeywordsForDicomSeriesRequest();
		try
		{
			long keys[] = new long[1];
			keys[0] = seriesPK;
			request.setDicomSeriesPks(keys);
			DicomSeriesKeywords[] keywordResults = m_keywordService.retrieveKeywordsForDicomSeries(request).get_return();
			return keywordResults;		
		}
		catch (com.icoserve.www.va20_keywordservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}		
	}
	
	public boolean addPatientStudyKeyWord(long studyPK,long pkKeyWord) throws Exception
	{
		AddKeywordsToDicomStudyRequest request = new AddKeywordsToDicomStudyRequest();
		try
		{
			KeywordAddInformation param = new KeywordAddInformation();
			param.setFieldTypeName("ctsu");
			param.setKeywordGroupName("ctsu");
			param.setKeywordPk(pkKeyWord);
			request.setDicomStudyPk(studyPK);
			request.addKeywords(param);
			return m_keywordService.addKeywordsToDicomStudy(request).get_return();
		}
		catch (com.icoserve.www.va20_keywordservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
	}
		
	public boolean changeOrganizationalUnit(long documentPk, String OUName) throws Exception 
	{
	//	final VA20_DocumentManipulationServiceStub documentManipulationService = createServiceStub_DocumentManipulationServic();		
		ChangeDocumentOrganizationalUnitRequest cr = new ChangeDocumentOrganizationalUnitRequest();
		cr.setDocumentPk(documentPk);
		cr.setOrganizationalUnitName(OUName);
		try
		{
			return m_documentManipulationService.changeDocumentOrganizationalUnit(cr).get_return();
		}
		catch (RemoteException e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}	
		catch (WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}	
	}
	
	public boolean copyToOrganizationalUnit(long documentPk, String OUName) throws Exception
	{
//		final VA20_DocumentManipulationServiceStub documentManipulationService = createServiceStub_DocumentManipulationServic();		
		CopyDocumentRequest cdr = new CopyDocumentRequest();
		cdr.setDocumentPk(documentPk);
		cdr.setOrganizationalUnitName(OUName);
		try
		{
			return m_documentManipulationService.copyDocument(cdr).get_return();
		}
		catch (RemoteException e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
		catch (WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
	}
		
	//Gets the PatientIDs from BIOBANK_PSI.
	//All scans in here, we know need data added, once added, they will be moved to BIOBANK_PSI/NEW
	//Study timestamps can be got some the returned data.
	public DicomDocumentFuzzySearchResult[] searchForScans(String org) throws Exception
	{
//		final VA20_QueryServiceStub queryService = createServiceStub_QueryServic();
		final DicomDocumentFuzzySearch dicomFuzzySearch = new DicomDocumentFuzzySearch();
		final DocumentFuzzySearch docFuzzySearch = new DocumentFuzzySearch();
		
		docFuzzySearch.addOrganizationalUnitNames(org);  		
		dicomFuzzySearch.setMaximumResults(50);		
		dicomFuzzySearch.setDocumentFuzzySearch(docFuzzySearch);
		FindDicomDocumentsRequest FDDR = new FindDicomDocumentsRequest();
		FDDR.setDicomDocumentFuzzySearch(dicomFuzzySearch);

		try
		{
			final DicomDocumentFuzzySearchResult[] DicomResults = m_queryService.findDicomDocuments(FDDR).get_return();
			return DicomResults;
		}
		catch (com.icoserve.www.va20_queryservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
	}
	
	public String getDirectAccessUrlForPatient(long patientPk) throws Exception
	{
//		final VA20_QueryServiceStub queryService = createServiceStub_QueryServic();	
		com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDirectAccessUrlForPatientRequest PR = new com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDirectAccessUrlForPatientRequest();
		PR.setPatientPk(patientPk);
		
		try
		{
			return m_queryService.getDirectAccessUrlForPatient(PR).get_return();
		}
		catch (com.icoserve.www.va20_queryservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
	}

	public DicomStudy getDicomStudy(long studyPK) throws Exception
	{
//		final VA20_QueryServiceStub queryService = createServiceStub_QueryServic();		
		GetDicomStudyRequest dicomStudyID = new GetDicomStudyRequest();
		dicomStudyID.setDicomStudyPk(studyPK);
		
		try
		{
			DicomStudy DS = m_queryService.getDicomStudy(dicomStudyID).get_return();
			return DS;
		}
		catch (com.icoserve.www.va20_queryservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage());
		}
	}
	
	public DicomSeries[] searchForPatientSeries(long StudyPK) throws Exception
	{
//		final VA20_QueryServiceStub queryService = createServiceStub_QueryServic();
		GetDicomSeriesForDicomStudyRequest dicomStudyID = new GetDicomSeriesForDicomStudyRequest();
		dicomStudyID.setDicomStudyPk(StudyPK);
		
		try
		{
			DicomSeries[] DS = m_queryService.getDicomSeriesForDicomStudy(dicomStudyID).get_return();
			return DS;
		}
		catch (com.icoserve.www.va20_queryservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
	}

	public Patient[] searchForPatients(String firstName, String LastName, String PatientID) throws Exception
	{
//		final VA20_QueryServiceStub queryService = createServiceStub_QueryServic();
		final PatientFuzzySearch patientFuzzySearch = new PatientFuzzySearch();
		patientFuzzySearch.setLastName(LastName);
		patientFuzzySearch.setFirstName(firstName);
		patientFuzzySearch.setPatientId(PatientID);
		final FindPatientsRequest request = new FindPatientsRequest();
		request.setPatientFuzzySearch(patientFuzzySearch);

		try
		{
			final Patient[] patients = m_queryService.findPatients(request).get_return();
			return patients;
		}
		catch (com.icoserve.www.va20_queryservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
	}
	

	public String ExportDicomImage(long dicomImagePK,String destination) throws Exception
	{
//		final VA20_DocumentManipulationServiceStub documentManipulationService = createServiceStub_DocumentManipulationServic();
		final ExportDicomImageRequest request = new ExportDicomImageRequest();
		request.setDicomImagePk(dicomImagePK);

		try
		{
			final Attachment attachment = m_documentManipulationService.exportDicomImage(request).get_return();
			return writeAttachmentToDisk(attachment,destination);
		}
		catch (com.icoserve.www.va20_documentmanipulationservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
	}
	
	public void ExportDicomImage(long dicomImagePK) throws Exception
	{
//		final VA20_DocumentManipulationServiceStub documentManipulationService = createServiceStub_DocumentManipulationServic();
		final ExportDicomImageRequest request = new ExportDicomImageRequest();
		request.setDicomImagePk(dicomImagePK);

		try
		{
			final Attachment attachment = m_documentManipulationService.exportDicomImage(request).get_return();
			writeAttachmentToDisk(attachment);
		}
		catch (com.icoserve.www.va20_documentmanipulationservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
	}

	public void ExportFile(String FileReferencePtr) throws Exception
	{
//		final VA20_DocumentManipulationServiceStub documentManipulationService = createServiceStub_DocumentManipulationServic();
		final ExportFilesForReferencePointerRequest request = new ExportFilesForReferencePointerRequest();
		request.setReferencePointer(FileReferencePtr);

		try
		{
			final Attachment[] attachments = m_documentManipulationService.exportFilesForReferencePointer(request).get_return();

			for (int i=0;i<attachments.length;i++)
				writeAttachmentToDisk(attachments[i]);
		}
		catch (final com.icoserve.www.va20_documentmanipulationservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}
	}

	private static void writeAttachmentToDisk(final Attachment attachment) throws IOException
	{
		//OUTPUT STUDY_ID/SERIES_ID_/FILENAME		
		System.out.println("Writing new file to disk. File name is: " + attachment.getFileName());
		OutputStream out = null;
		try
		{
			out = new FileOutputStream(FILE_OUTPUT_DIR  + attachment.getFileName());
			attachment.getData().writeTo(out);
		}
		finally
		{
			if (out != null)
				out.close();
		}
	}
	
	private static String writeAttachmentToDisk(final Attachment attachment,final String destination) throws Exception
	{
		OutputStream out = null;
		try
		{
			File dir = new File(destination);
			dir.mkdirs();
			out = new FileOutputStream(destination + attachment.getFileName());
			attachment.getData().writeTo(out);
			return destination + attachment.getFileName();
		}
		catch (FileNotFoundException e)
		{
			throw new Exception("Error saving:[FileNotFoundException] " + destination + attachment.getFileName()+ ":" + e.getMessage());	
		}
		catch (IOException e)
		{
			throw new Exception("Error saving:[IOException] " + destination + attachment.getFileName()+ ":" + e.getMessage());	
		}
		finally
		{
			if (out != null)
				out.close();
		}
	}


	public GenericDocumentFuzzySearchResult[] searchForDocument(String firstName, String LastName, String PatientID) throws Exception 
	{
//		final VA20_QueryServiceStub queryService = createServiceStub_QueryServic();

		final PatientFuzzySearch patientFuzzySearch = new PatientFuzzySearch();
		patientFuzzySearch.setPatientId(PatientID);
		patientFuzzySearch.setLastName(LastName);
		patientFuzzySearch.setFirstName(firstName);
		final GenericDocumentFuzzySearch genericDocumentFuzzySearch = new GenericDocumentFuzzySearch();
		genericDocumentFuzzySearch.setPatientFuzzySearch(patientFuzzySearch);
		genericDocumentFuzzySearch.setMaximumResults(50);

		final FindGenericDocumentsRequest request = new FindGenericDocumentsRequest();
		request.setGenericDocumentFuzzySearch(genericDocumentFuzzySearch);

		try
		{
			final GenericDocumentFuzzySearchResult[] result = m_queryService.findGenericDocuments(request).get_return();
			System.out.println("Found " + ((result == null) ? "0" : result.length) + " document(s)");
			return result;
		}
		catch (com.icoserve.www.va20_queryservice.WsFault e)
		{
			throw new Exception("Error returned from webservice_api: " + e.getMessage(),e);
		}		
	}

	private static VA20_DocumentManipulationServiceStub createServiceStub_DocumentManipulationService() throws Exception
	{
		try
		{
			final ConfigurationContext configContext = ConfigurationContextFactory.createConfigurationContextFromFileSystem(WORKING_DIR);
			final VA20_DocumentManipulationServiceStub queryService = new VA20_DocumentManipulationServiceStub(configContext, SERVICE_EPR_DOCMANIPULATION);
	
			// Setting up Rampart for WSS username token authentication
			final StAXOMBuilder builder = new StAXOMBuilder(WORKING_DIR + "/policy/UsernameTokenPolicy.xml");
			final Policy rampartPolicy = PolicyEngine.getPolicy(builder.getDocumentElement());
			final ServiceClient serviceClient = queryService._getServiceClient();
			setUpStub(rampartPolicy, serviceClient);
			return queryService;
		}
		catch (AxisFault e)
		{
			throw new Exception("Error setting up Document Manipulation Service stub: " + e.getMessage(),e);
		}
		catch (FileNotFoundException e)
		{
			throw new Exception("Error setting up Document Manipulation Service stub: " + e.getMessage(),e);
		}
		catch (XMLStreamException e)
		{
			throw new Exception("Error setting up Document Manipulation Service stub: " + e.getMessage(),e);
		}
	}

	private static VA20_QueryServiceStub createServiceStub_QueryService() throws Exception
	{
		try
		{
			final ConfigurationContext configContext = ConfigurationContextFactory.createConfigurationContextFromFileSystem(WORKING_DIR);
			final VA20_QueryServiceStub queryService = new VA20_QueryServiceStub(configContext, SERVICE_EPR_QUERYSERICE);
	
			// Setting up Rampart for WSS username token authentication
			final StAXOMBuilder builder = new StAXOMBuilder(WORKING_DIR + "/policy/UsernameTokenPolicy.xml");
			final Policy rampartPolicy = PolicyEngine.getPolicy(builder.getDocumentElement());
			final ServiceClient serviceClient = queryService._getServiceClient();
			setUpStub(rampartPolicy, serviceClient);
			return queryService;
		}
		catch (AxisFault e)
		{
			throw new Exception("Error setting up Query Service stub: " + e.getMessage(),e);
		}
		catch (FileNotFoundException e)
		{
			throw new Exception("Error setting up Query Service stub: " + e.getMessage(),e);
		}
		catch (XMLStreamException e)
		{
			throw new Exception("Error setting up Query Service stub: " + e.getMessage(),e);
		}
	}
	
	private static VA20_KeywordServiceStub createServiceStub_KeywordService() throws Exception
	{
		try
		{
			final ConfigurationContext configContext = ConfigurationContextFactory.createConfigurationContextFromFileSystem(WORKING_DIR);
			final VA20_KeywordServiceStub queryService = new VA20_KeywordServiceStub(configContext, SERVICE_EPR_KEYWORDSERICE);
	
			// Setting up Rampart for WSS username token authentication
			final StAXOMBuilder builder = new StAXOMBuilder(WORKING_DIR + "/policy/UsernameTokenPolicy.xml");
			final Policy rampartPolicy = PolicyEngine.getPolicy(builder.getDocumentElement());
			final ServiceClient serviceClient = queryService._getServiceClient();
			setUpStub(rampartPolicy, serviceClient);
			return queryService;
		}
		catch (AxisFault e)
		{
			throw new Exception("Error setting up Query Service stub: " + e.getMessage(),e);
		}
		catch (FileNotFoundException e)
		{
			throw new Exception("Error setting up Query Service stub: " + e.getMessage(),e);
		}
		catch (XMLStreamException e)
		{
			throw new Exception("Error setting up Query Service stub: " + e.getMessage(),e);
		}
	}

	private static void setUpStub(final Policy rampartPolicy, final ServiceClient serviceClient) throws AxisFault
	{
		final AxisService axisService = serviceClient.getAxisService();
		serviceClient.engageModule("rampart");
		axisService.applyPolicy(rampartPolicy);

		final Options options = serviceClient.getOptions();
		options.setUserName(USERLOGIN);
		options.setPassword(PASSWORD);

		// Setting up MTOM
		options.setProperty(Constants.Configuration.ENABLE_MTOM, Constants.VALUE_TRUE);

		// Setting up file caching for handling large files
		options.setProperty(Constants.Configuration.CACHE_ATTACHMENTS, Constants.VALUE_TRUE);
		options.setProperty(Constants.Configuration.ATTACHMENT_TEMP_DIR, LOCALE_FILE_CACHING_DIR);
		options.setProperty(Constants.Configuration.FILE_SIZE_THRESHOLD, "400000");

		// Setting client timeout
		options.setTimeOutInMilliSeconds(CLIENT_TIMEOUT_MILLIS);
		options.setProperty(HTTPConstants.SO_TIMEOUT, CLIENT_TIMEOUT_MILLIS);
		options.setProperty(HTTPConstants.CONNECTION_TIMEOUT, CLIENT_TIMEOUT_MILLIS);

		// Setting up SSL connection
		System.setProperty("javax.net.ssl.trustStore", KEYSTORE_PATH);
		System.setProperty("javax.net.ssl.trustStorePassword", KEYSTORE_PASSWORD);
	}
}
