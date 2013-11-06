package com.icoserve.ws;

import java.io.FileNotFoundException;
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
import com.icoserve.ws.client.dms.WsFault;
import com.icoserve.ws.client.types.DocumentInfo;
import com.icoserve.ws.client.types.GenericFileInfo;
import com.icoserve.ws.client.types.GenericImportResult;
import com.icoserve.ws.client.types.GenericOrgUnitImportRequest;
import com.icoserve.ws.client.types.PatientSearch;

public class ImportFile {
	/**
	 * The endpoint of the service that should be called. Can be retrieved in webadmin (Tools -> Webservices).
	 */
//	private static final String SERVICE_EPR = "https://icodemo.ith-icoserve.com/ws_api/services/DEMO/VA15/DocumentManipulationService";
	private static final String SERVICE_EPR = "https://syngoshare.ith-icoserve.com/ws_api/services/DEMO/VA15/DocumentManipulationService";
	/**
	 * The login of the webservice user. 
	 */
	private static final String USERLOGIN = "Biobank";
	
	/**
	 * The password of the webservice user in clear text.
	 */
	private static final String PASSWORD = "Biobank";
	
	/**
	 * The path of the server certificate. Can be retrieved in webadmin (Tools -> Webservices). 
	 */
	private static final String KEYSTORE_PATH = "C:\\keys\\webservice_api.jks";
	
	/**
	 * The password of the keystore containing the server certificate (Must be specified when downloading the keystore from webadmin).
	 */
	private static final String KEYSTORE_PASSWORD = "biobank";
	
	/**
	 * Path where the eclipse project is contained. (The "modules" and "policy" directories must
	 * be directly under the given directory).
	 */
	private static final String WORKING_DIR = "C:\\jonathanpr\\eclipes\\WSClient";
	//private static final String WORKING_DIR = "C:\\Workspace\\WSClient";
	
	/**
	 * Path where axis2 is allowed to cache receiving files.
	 */
	private static final String LOCALE_FILE_CACHING_DIR = "C:\\temp";
	
	/**
	 * The reference pointer to export.
	 */
	private static final String FILE_TO_IMPORT =  "C:\\temp\\testupload.cvs";
	
	/**
	 * The clients time out in milliseconds.
	 */
	private static final int CLIENT_TIMEOUT_MILLIS = 600000; // 10 minutes  

	public static void main (final String[] args) throws Exception {
		
		final VA15_DocumentManipulationServiceStub documentManipulationService = createServiceStub();
		
		// Importing file
		
		System.out.println("Now importing file");
		
		final PatientSearch patientSearch = new PatientSearch();
		patientSearch.setPatientId("20500");
		patientSearch.setFirstName("Mangala");
		patientSearch.setLastName("Donald");
		patientSearch.setSex("F");
//		patientSearch.setBirthDate(Calendar.getInstance("1953.03.15"));
		
		final DocumentInfo documentInfo = new DocumentInfo();
		documentInfo.setDescription("Test CSV Import");
		documentInfo.setCreationTimestamp(Calendar.getInstance());
		
		final GenericFileInfo fileInfo = new GenericFileInfo();
		fileInfo.setDescription("Not my Import");
		fileInfo.setArchiveFileName("testupload.cvs");
		
		final GenericOrgUnitImportRequest request = new GenericOrgUnitImportRequest();
		request.setPatientSearch(patientSearch);
		request.setDocumentInfo(documentInfo);
		request.setGenericFileInfo(fileInfo);
		request.setData(new DataHandler(new FileDataSource(FILE_TO_IMPORT)));
		request.setOrganizationalUnitName("BIOBANK");
		
		
		try {
			final GenericImportResult result = documentManipulationService.importGenericFileIntoOrgUnit(request).get_return();
			System.out.println("Successfully imported new file. New Reference pointer is: " + result.getReferencePointer());
		} catch (final WsFault e) {
			System.out.println("Error returned from webservice_api: " + e.getFaultMessage().getWsFault().getMessage());
			e.printStackTrace();
		}
	}
//	RP:2:DEMO:GENERIC_CONTAINER:1.3.6.1.4.1.9784.10.1.8.3.27846
	private static VA15_DocumentManipulationServiceStub createServiceStub()
			throws AxisFault, XMLStreamException, FileNotFoundException {
		// Initialize service stub
		final ConfigurationContext configContext = ConfigurationContextFactory.createConfigurationContextFromFileSystem(WORKING_DIR);
		final VA15_DocumentManipulationServiceStub documentManipulationService = new VA15_DocumentManipulationServiceStub(configContext, SERVICE_EPR);
		
		// Setting up Rampart for WSS username token authentication
		final StAXOMBuilder builder = new StAXOMBuilder(WORKING_DIR + "/policy/UsernameTokenPolicy.xml");
		final Policy rampartPolicy = PolicyEngine.getPolicy(builder.getDocumentElement());
		final ServiceClient serviceClient = documentManipulationService._getServiceClient();
		final AxisService axisService = serviceClient.getAxisService();
		serviceClient.engageModule("rampart");
		axisService.applyPolicy(rampartPolicy);
		
		final Options options = serviceClient.getOptions();
		options.setUserName(USERLOGIN);
        options.setPassword(PASSWORD);

        // Setting up MTOM
        options.setProperty(Constants.Configuration.ENABLE_MTOM, Constants.VALUE_TRUE);

        // Setting up file caching for handling large files
        options.setProperty(Constants.Configuration.CACHE_ATTACHMENTS,Constants.VALUE_TRUE);
        options.setProperty(Constants.Configuration.ATTACHMENT_TEMP_DIR, LOCALE_FILE_CACHING_DIR);
        options.setProperty(Constants.Configuration.FILE_SIZE_THRESHOLD, "400000");
        
		// Setting client timeout
		options.setTimeOutInMilliSeconds(CLIENT_TIMEOUT_MILLIS);
		options.setProperty(HTTPConstants.SO_TIMEOUT, CLIENT_TIMEOUT_MILLIS);
		options.setProperty(HTTPConstants.CONNECTION_TIMEOUT, CLIENT_TIMEOUT_MILLIS);
        
		// Setting up SSL connection
		System.setProperty("javax.net.ssl.trustStore", KEYSTORE_PATH);
		System.setProperty("javax.net.ssl.trustStorePassword", KEYSTORE_PASSWORD);
		
		return documentManipulationService;
	}
}
