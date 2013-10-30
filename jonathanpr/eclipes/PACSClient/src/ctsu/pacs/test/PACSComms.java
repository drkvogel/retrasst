package ctsu.pacs.test;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.rmi.RemoteException;
import java.util.Calendar;

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

import com.icoserve.ws.client.dms.VA15_DocumentManipulationServiceStub;
import com.icoserve.ws.client.qs.VA15_QueryServiceStub;
import com.icoserve.ws.client.types.Attachment;
import com.icoserve.ws.client.types.DicomDocumentFuzzySearch;
import com.icoserve.ws.client.types.DicomDocumentFuzzySearchResult;
import com.icoserve.ws.client.types.DicomImage;
import com.icoserve.ws.client.types.DicomImportResult;
import com.icoserve.ws.client.types.DicomOrgUnitImportRequest;
import com.icoserve.ws.client.types.DicomSeries;
import com.icoserve.ws.client.types.DocumentInfo;
import com.icoserve.ws.client.types.ExportDicomImageRequest;
import com.icoserve.ws.client.types.ExportFilesForReferencePointerRequest;
import com.icoserve.ws.client.types.FindDicomDocumentsRequest;
import com.icoserve.ws.client.types.FindGenericDocumentsRequest;
import com.icoserve.ws.client.types.FindPatientsRequest;
import com.icoserve.ws.client.types.GenericDocumentFuzzySearch;
import com.icoserve.ws.client.types.GenericDocumentFuzzySearchResult;
import com.icoserve.ws.client.types.GenericFileInfo;
import com.icoserve.ws.client.types.GenericImportResult;
import com.icoserve.ws.client.types.GenericOrgUnitImportRequest;
import com.icoserve.ws.client.types.GetDicomImagesForDicomSeriesRequest;
import com.icoserve.ws.client.types.GetDicomSeriesForDicomStudyRequest;
import com.icoserve.ws.client.types.Patient;
import com.icoserve.ws.client.types.PatientFuzzySearch;
import com.icoserve.ws.client.types.PatientSearch;

public class PACSComms
{
	/**
	 * The endpoint of the service that should be called. Can be retrieved in
	 * webadmin (Tools -> Webservices).
	 */
	private static final String SERVICE_EPR_QUERYSERICE = "https://syngoshare.ith-icoserve.com/ws_api/services/DEMO/VA15/QueryService";
	private static final String SERVICE_EPR_DOCMANIPULATION = "https://syngoshare.ith-icoserve.com/ws_api/services/DEMO/VA15/DocumentManipulationService";

	// The login of the webservice user.
	private static final String USERLOGIN = "Biobank";
	private static final String PASSWORD = "Biobank";

	// The path of the server certificate. Can be retrieved in webadmin (Tools
	// -> Webservices).
	private static final String KEYSTORE_PATH = "C:\\keys\\webservice_api.jks";

	// The password of the keystore containing the server certificate (Must be
	// specified when downloading the keystore from webadmin).
	private static final String KEYSTORE_PASSWORD = "biobank";

	// Path where the eclipse project is contained. (The "modules" and "policy"
	// directories must be directly under the given directory).
	private static final String WORKING_DIR = "C:/jonathanpr/eclipes/PACSClient";

	// Path where axis2 is allowed to cache receiving files.
	private static final String LOCALE_FILE_CACHING_DIR = "C:\\temp";
	private static final String FILE_OUTPUT_DIR = "C:\\temp\\export";

	private static final int CLIENT_TIMEOUT_MILLIS = 600000; // 10 minutes

	
	public String downLoadAll(String PatientID) throws XMLStreamException, IOException
	{
		final VA15_QueryServiceStub queryService = createServiceStub_QueryServic();
		final VA15_DocumentManipulationServiceStub documentManipulationService = createServiceStub_DocumentManipulationServic();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Searching for non-dicom
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//FOR NOW I SHALL ASSUME ALL DATA IN THE PACS IS DICOM FORMATTED
//SO EXPORT DICOM ISN'T A NECESSARY 		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
//Searching for dicom
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		System.out.println("Now searching for patient:" + PatientID + " studies");

		final DicomDocumentFuzzySearch dicomFuzzySearch = new DicomDocumentFuzzySearch();
		final PatientFuzzySearch patientFuzzySearch = new PatientFuzzySearch();
		patientFuzzySearch.setPatientId(PatientID);
		dicomFuzzySearch.setPatientFuzzySearch(patientFuzzySearch);
		dicomFuzzySearch.setMaximumResults(20);
		FindDicomDocumentsRequest FDDR = new FindDicomDocumentsRequest();
		FDDR.setDicomDocumentFuzzySearch(dicomFuzzySearch);

		try
		{
			final DicomDocumentFuzzySearchResult[] DicomStudyResults = queryService.findDicomDocuments(FDDR).get_return();
			if (DicomStudyResults == null)
				return "No Dicom found with PatientID " + PatientID;
			if (DicomStudyResults.length == 0)
				return "No Dicom found with PatientID " + PatientID;
			
			for (int StudyCount=0;StudyCount<DicomStudyResults.length;StudyCount++)
			{
				for (int i=0;i<DicomStudyResults[StudyCount].getDicomStudy().getNumSeries();i++)
				{
					System.out.println("Now searching for series: " + DicomStudyResults[StudyCount].getDicomStudy().getStudyInstanceUid());
	
					GetDicomSeriesForDicomStudyRequest dicomStudyID = new GetDicomSeriesForDicomStudyRequest();
					dicomStudyID.setDicomStudyPk(DicomStudyResults[StudyCount].getDicomStudy().getId());
					try
					{
						DicomSeries[] DicomStudy = queryService.getDicomSeriesForDicomStudy(dicomStudyID).get_return();
	
						System.out.println("Found " + ((DicomStudy == null) ? "0" : DicomStudy.length) + " Dicom(s)");
						for (int k = 0; k < DicomStudy.length; k++)
						{
							System.out.println("SeriesInstanceUID: " + DicomStudy[k].getSeriesInstanceUid());
							System.out.println("Num Images: " + DicomStudy[k].getNumImages());
							System.out.println("Now searching for dicom images");
						
							GetDicomImagesForDicomSeriesRequest dicomSeriesID = new GetDicomImagesForDicomSeriesRequest();
							dicomSeriesID.setDicomSeriesPk(DicomStudy[k].getId());
							
							try
							{
								final DicomImage[] ImageResults = queryService.getDicomImagesForDicomSeries(dicomSeriesID).get_return();
								System.out.println("Found " + ((ImageResults == null) ? "0" : ImageResults.length) + " Dicom(s)");
								for (int j = 0;j < ImageResults.length; j++)
								{
									// Exporting files for given reference pointer.
									System.out.println("Now exporting dicom");
	
									final ExportDicomImageRequest request = new ExportDicomImageRequest();
									request.setDicomImagePk(ImageResults[j].getId());
	
									try
									{
										final Attachment attachment = documentManipulationService.exportDicomImage(request).get_return();
										System.out.println("Successfully exported dicom image");
	
										OutputStream out = null;
										try
										{
											String FilePath = FILE_OUTPUT_DIR + "\\" + DicomStudyResults[i].getDicomStudy().getStudyInstanceUid() + "\\" + DicomStudy[k].getSeriesInstanceUid();
											
											File file = new File(FilePath + "\\" + attachment.getFileName());
											file.getParentFile().mkdirs();
	
											out = new FileOutputStream(file);
											attachment.getData().writeTo(out);
										}
										finally
										{
											if (out != null)
											{
												out.close();
											}
										}									
									}
									catch (final com.icoserve.ws.client.dms.WsFault e)
									{
										System.out.println("Error returned from webservice_api: " + e.getFaultMessage().getWsFault().getMessage());
										e.printStackTrace();
									}
								}	
							}
							catch (final com.icoserve.ws.client.qs.WsFault e)
							{
								System.out.println("Error returned from webservice_api: " + e.getFaultMessage().getWsFault().getMessage());
								e.printStackTrace();
							}
						}	
					}
					catch (final com.icoserve.ws.client.qs.WsFault e)
					{
						System.out.println("Error returned from webservice_api: " + e.getFaultMessage().getWsFault().getMessage());
						e.printStackTrace();
					}				
				}
			}
		}
		catch (final com.icoserve.ws.client.qs.WsFault e)
		{
			System.out.println("Error returned from webservice_api: " + e.getFaultMessage().getWsFault().getMessage());
			e.printStackTrace();
		}
		return null;
	}
	
	public DicomImportResult importDicomToPatient(String dicomfileToImport) throws FileNotFoundException, XMLStreamException, RemoteException
	{
		final VA15_DocumentManipulationServiceStub documentManipulationService = createServiceStub_DocumentManipulationServic();

		System.out.println("Now importing file");

		final DicomOrgUnitImportRequest request = new DicomOrgUnitImportRequest();
		request.setData(new DataHandler(new FileDataSource(dicomfileToImport)));
		request.setOrganizationalUnitName("BIOBANK");

		try
		{
			final DicomImportResult result = documentManipulationService.importDicomImageIntoOrgUnit(request).get_return();
			System.out.println("Successfully imported new dicom file. New Reference pointer is: " + result.getReferencePointer());
			return result;
		}
		catch (com.icoserve.ws.client.dms.WsFault e)
		{
			System.out.println("Error returned from webservice_api: " + e.getFaultMessage().getWsFault().getMessage());
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}
	
	// fileToImport = "C:\\temp\\testupload.cvs";
	public GenericImportResult importFile(String fileToImport, String PID, String FName, String LName, String Desc, String ArchiveFileName) throws FileNotFoundException, XMLStreamException, RemoteException
	{
		final VA15_DocumentManipulationServiceStub documentManipulationService = createServiceStub_DocumentManipulationServic();

		System.out.println("Now importing file");

		final PatientSearch patientSearch = new PatientSearch();
		patientSearch.setPatientId(PID);//"20500");
		patientSearch.setFirstName(FName);//"Mangala");
		patientSearch.setLastName(LName);//"Donald");
		//patientSearch.setSex("F");
		// patientSearch.setBirthDate(Calendar.getInstance("1953.03.15"));

		final DocumentInfo documentInfo = new DocumentInfo();
		documentInfo.setDescription(Desc);
		documentInfo.setCreationTimestamp(Calendar.getInstance());		

		final GenericFileInfo fileInfo = new GenericFileInfo();
		fileInfo.setDescription("What happens to this?"); 
		fileInfo.setArchiveFileName(ArchiveFileName);
				
		final GenericOrgUnitImportRequest request = new GenericOrgUnitImportRequest();
		request.setPatientSearch(patientSearch);
		request.setDocumentInfo(documentInfo);
		request.setGenericFileInfo(fileInfo);
		request.setData(new DataHandler(new FileDataSource(fileToImport)));
		request.setOrganizationalUnitName("BIOBANK");
		
		
		try
		{
			final GenericImportResult result = documentManipulationService.importGenericFileIntoOrgUnit(request).get_return();
			System.out.println("Successfully imported new file. New Reference pointer is: " + result.getReferencePointer());
			return result;
		}
		catch (com.icoserve.ws.client.dms.WsFault e)
		{
			System.out.println("Error returned from webservice_api: " + e.getFaultMessage().getWsFault().getMessage());
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}
	
	public DicomImage[] searchForDicomInSeries(long SeriesID) throws FileNotFoundException, XMLStreamException, RemoteException
	{
		final VA15_QueryServiceStub queryService = createServiceStub_QueryServic();

		// Searching for patients
		System.out.println("Now searching for dicom images");
	
		GetDicomImagesForDicomSeriesRequest dicomSeriesID = new GetDicomImagesForDicomSeriesRequest();
		dicomSeriesID.setDicomSeriesPk(SeriesID);
		
		try
		{
			final DicomImage[] DicomResults = queryService.getDicomImagesForDicomSeries(dicomSeriesID).get_return();
			System.out.println("Found " + ((DicomResults == null) ? "0" : DicomResults.length) + " Dicom(s)");
			for (int i = 0; i < DicomResults.length; i++)
			{
				System.out.println("DicomSopInstanceUID: " + DicomResults[i].getSopInstanceUid());
				System.out.println("Bits Stored: " + DicomResults[i].getBitsStored());
			}
			return DicomResults;
		}
		catch (final com.icoserve.ws.client.qs.WsFault e)
		{
			System.out.println("Error returned from webservice_api: " + e.getFaultMessage().getWsFault().getMessage());
			e.printStackTrace();
		}
		return null;
	}
	
	public DicomDocumentFuzzySearchResult[] searchForPatientStudies(String PatientID) throws FileNotFoundException, XMLStreamException, RemoteException
	{
		final VA15_QueryServiceStub queryService = createServiceStub_QueryServic();

		// Searching for patients
		System.out.println("Now searching for patients");

		final DicomDocumentFuzzySearch dicomFuzzySearch = new DicomDocumentFuzzySearch();
		final PatientFuzzySearch patientFuzzySearch = new PatientFuzzySearch();
		patientFuzzySearch.setPatientId(PatientID);
		dicomFuzzySearch.setPatientFuzzySearch(patientFuzzySearch);
		dicomFuzzySearch.setMaximumResults(20);
		FindDicomDocumentsRequest FDDR = new FindDicomDocumentsRequest();
		FDDR.setDicomDocumentFuzzySearch(dicomFuzzySearch);

		try
		{
			final DicomDocumentFuzzySearchResult[] DicomResults = queryService.findDicomDocuments(FDDR).get_return();
			System.out.println("Found " + ((DicomResults == null) ? "0" : DicomResults.length) + " Dicom(s)");
			for (int i = 0; i < DicomResults.length; i++)
			{
				System.out.println("StudyInstanceUID: " + DicomResults[i].getDicomStudy().getStudyInstanceUid());
				System.out.println("Study Modality: " + DicomResults[i].getDicomStudy().getAllModalities());
				System.out.println("Series in Study: " + DicomResults[i].getDicomStudy().getNumSeries());
			}
			return DicomResults;
		}
		catch (final com.icoserve.ws.client.qs.WsFault e)
		{
			System.out.println("Error returned from webservice_api: " + e.getFaultMessage().getWsFault().getMessage());
			e.printStackTrace();
		}
		return null;
	}

	public DicomSeries[] searchForPatientSeries(long SeriesPK) throws FileNotFoundException, XMLStreamException, RemoteException
	{
		final VA15_QueryServiceStub queryService = createServiceStub_QueryServic();

		// Searching for patients
		System.out.println("Now searching for patients");

		GetDicomSeriesForDicomStudyRequest dicomStudyID = new GetDicomSeriesForDicomStudyRequest();
		dicomStudyID.setDicomStudyPk(SeriesPK);
		try
		{
			DicomSeries[] DS = queryService.getDicomSeriesForDicomStudy(dicomStudyID).get_return();

			System.out.println("Found " + ((DS == null) ? "0" : DS.length) + " Dicom(s)");
			for (int i = 0; i < DS.length; i++)
			{
				System.out.println("SeriesInstanceUID: " + DS[i].getSeriesInstanceUid());
				System.out.println("Num Images: " + DS[i].getNumImages());
			}
			return DS;
		}
		catch (final com.icoserve.ws.client.qs.WsFault e)
		{
			System.out.println("Error returned from webservice_api: " + e.getFaultMessage().getWsFault().getMessage());
			e.printStackTrace();
		}
		return null;
	}

	public Patient[] searchForPatients(String firstName, String LastName, String PatientID) throws FileNotFoundException, XMLStreamException, RemoteException
	{
		final VA15_QueryServiceStub queryService = createServiceStub_QueryServic();

		// Searching for patients
		System.out.println("Now searching for patients");

		final PatientFuzzySearch patientFuzzySearch = new PatientFuzzySearch();
		patientFuzzySearch.setLastName(LastName);
		patientFuzzySearch.setFirstName(firstName);
		patientFuzzySearch.setPatientId(PatientID);
		final FindPatientsRequest request = new FindPatientsRequest();
		request.setPatientFuzzySearch(patientFuzzySearch);

		try
		{
			final Patient[] patients = queryService.findPatients(request).get_return();
			System.out.println("Found " + ((patients == null) ? "0" : patients.length) + " patient(s)");
			return patients;
		}
		catch (final com.icoserve.ws.client.qs.WsFault e)
		{
			System.out.println("Error returned from webservice_api: " + e.getFaultMessage().getWsFault().getMessage());
			e.printStackTrace();
		}
		return null;
	}
	
	public boolean ExportDicomImage(long dicomImagePK) throws XMLStreamException, IOException
	{
		final VA15_DocumentManipulationServiceStub documentManipulationService = createServiceStub_DocumentManipulationServic();

		// Exporting files for given reference pointer.
		System.out.println("Now exporting dicom");

		final ExportDicomImageRequest request = new ExportDicomImageRequest();
		request.setDicomImagePk(dicomImagePK);

		try
		{
			final Attachment attachment = documentManipulationService.exportDicomImage(request).get_return();
			System.out.println("Successfully exported dicom image");

			writeAttachmentToDisk(attachment);
			return true;
		}
		catch (final com.icoserve.ws.client.dms.WsFault e)
		{
			System.out.println("Error returned from webservice_api: " + e.getFaultMessage().getWsFault().getMessage());
			e.printStackTrace();
		}
		return false;
	}


	public boolean ExportFile(String FileReferencePtr) throws XMLStreamException, IOException
	{
		final VA15_DocumentManipulationServiceStub documentManipulationService = createServiceStub_DocumentManipulationServic();

		// Exporting files for given reference pointer.
		System.out.println("Now exporting files");

		final ExportFilesForReferencePointerRequest request = new ExportFilesForReferencePointerRequest();
		request.setReferencePointer(FileReferencePtr);

		try
		{
			final Attachment[] attachments = documentManipulationService.exportFilesForReferencePointer(request).get_return();
			System.out.println("Successfully exported file");

			for (int i=0;i<attachments.length;i++)
				writeAttachmentToDisk(attachments[i]);
			return true;
		}
		catch (final com.icoserve.ws.client.dms.WsFault e)
		{
			System.out.println("Error returned from webservice_api: " + e.getFaultMessage().getWsFault().getMessage());
			e.printStackTrace();
		}
		return false;
	}

	private static void writeAttachmentToDisk(final Attachment attachment) throws IOException
	{
		//OUTPUT STUDY_ID/SERIES_ID_/FILENAME
		
		System.out.println("Writing new file to disk. File name is: " + attachment.getFileName());
		OutputStream out = null;
		try
		{
			out = new FileOutputStream(FILE_OUTPUT_DIR + "\\" + attachment.getFileName());
			attachment.getData().writeTo(out);
		}
		finally
		{
			if (out != null)
			{
				out.close();
			}
		}
	}


	public GenericDocumentFuzzySearchResult[] searchForDocument(String firstName, String LastName, String PatientID) throws FileNotFoundException, XMLStreamException, RemoteException
	{
		final VA15_QueryServiceStub queryService = createServiceStub_QueryServic();

		System.out.println("Now searching for documents");

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
			final GenericDocumentFuzzySearchResult[] result = queryService.findGenericDocuments(request).get_return();
			System.out.println("Found " + ((result == null) ? "0" : result.length) + " document(s)");
			return result;
		}
		catch (final com.icoserve.ws.client.qs.WsFault e)
		{
			System.out.println("Error returned from webservice_api: " + e.getFaultMessage().getWsFault().getMessage());
			e.printStackTrace();
		}
		return null;
	}

	private static VA15_DocumentManipulationServiceStub createServiceStub_DocumentManipulationServic() throws AxisFault, XMLStreamException, FileNotFoundException
	{
		final ConfigurationContext configContext = ConfigurationContextFactory.createConfigurationContextFromFileSystem(WORKING_DIR);
		final VA15_DocumentManipulationServiceStub queryService = new VA15_DocumentManipulationServiceStub(configContext, SERVICE_EPR_DOCMANIPULATION);

		// Setting up Rampart for WSS username token authentication
		final StAXOMBuilder builder = new StAXOMBuilder(WORKING_DIR + "/policy/UsernameTokenPolicy.xml");
		final Policy rampartPolicy = PolicyEngine.getPolicy(builder.getDocumentElement());
		final ServiceClient serviceClient = queryService._getServiceClient();
		setUpStub(rampartPolicy, serviceClient);
		return queryService;
	}

	private static VA15_QueryServiceStub createServiceStub_QueryServic() throws AxisFault, XMLStreamException, FileNotFoundException
	{
		final ConfigurationContext configContext = ConfigurationContextFactory.createConfigurationContextFromFileSystem(WORKING_DIR);
		final VA15_QueryServiceStub queryService = new VA15_QueryServiceStub(configContext, SERVICE_EPR_QUERYSERICE);

		// Setting up Rampart for WSS username token authentication
		final StAXOMBuilder builder = new StAXOMBuilder(WORKING_DIR + "/policy/UsernameTokenPolicy.xml");
		final Policy rampartPolicy = PolicyEngine.getPolicy(builder.getDocumentElement());
		final ServiceClient serviceClient = queryService._getServiceClient();
		setUpStub(rampartPolicy, serviceClient);
		return queryService;
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
