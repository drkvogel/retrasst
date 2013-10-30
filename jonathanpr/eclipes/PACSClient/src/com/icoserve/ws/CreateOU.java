package com.icoserve.ws;

import java.io.FileNotFoundException;

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

import com.icoserve.ws.client.oas.VA15_OrganizationalUnitAdministrationServiceStub;
import com.icoserve.ws.client.types.FindOrganizationalUnitsResponse;

public class CreateOU {
	/**
	 * The endpoint of the service that should be called. Can be retrieved in webadmin (Tools -> Webservices).
	 */
	private static final String SERVICE_EPR = "https://icodemo.ith-icoserve.com/ws_api/services/DEMO/VA15/OrganizationalUnitAdministrationService";
	
	/**
	 * The login of the webservice user. 
	 */
	private static final String USERLOGIN = "Biobank";
//	private static final String USERLOGIN = "mlang";
	
	/**
	 * The password of the webservice user in clear text.
	 */
	private static final String PASSWORD = "Biobank";
//	private static final String PASSWORD = "mlang";
	
	/**
	 * The path of the server certificate. Can be retrieved in webadmin (Tools -> Webservices). 
	 */
//	private static final String KEYSTORE_PATH = "C:\\keys\\webservice_api.jks";
	private static final String KEYSTORE_PATH = "C:\\jonathanpr\\eclipes\\WSClient\\webservice_api.jks";
	
	
	/**
	 * The password of the keystore containing the server certificate (Must be specified when downloading the keystore from webadmin).
	 */
	private static final String KEYSTORE_PASSWORD = "changeit";
	
	/**
	 * Path where the eclipse project is contained. (The "modules" and "policy" directories must
	 * be directly under the given directory).
	 */
	
	private static final String WORKING_DIR = "C:\\jonathanpr\\eclipes\\WSClient";
//	private static final String WORKING_DIR = "C:\\Workspace\\WSClient";
	
	/**
	 * Path where axis2 is allowed to cache receiving files.
	 */
	private static final String LOCALE_FILE_CACHING_DIR = "C:\\temp";
	
	/**
	 * The reference pointer to export.
	 */
//	private static final String REFERENCE_POINTER_TO_EXPORT = "RP:2:PUBLIC_VA15:GENERIC_CONTAINER:1.3.6.1.4.1.9784.10.1.11.3.98793";
	
	/**
	 * The clients time out in milliseconds.
	 */
	private static final int CLIENT_TIMEOUT_MILLIS = 600000; // 10 minutes  

	public static void main (final String[] args) throws Exception {
		
		final VA15_OrganizationalUnitAdministrationServiceStub organizationalUnitAdministrationService = createServiceStub();
		
		// Exporting files for given reference pointer.
		
		System.out.println("Now querying available Org Units");
		
//		final ExportFilesForReferencePointerRequest request = new ExportFilesForReferencePointerRequest();
//		request.setReferencePointer(REFERENCE_POINTER_TO_EXPORT);
		
		FindOrganizationalUnitsResponse orgunits = organizationalUnitAdministrationService.findOrganizationalUnits(null);
//			System.out.println("Successfully exported " + attachments.length + " files(s)");
			System.out.println(orgunits.toString());
	}

	private static VA15_OrganizationalUnitAdministrationServiceStub createServiceStub()
			throws AxisFault, XMLStreamException, FileNotFoundException {
		// Initialize service stub
		final ConfigurationContext configContext = ConfigurationContextFactory.createConfigurationContextFromFileSystem(WORKING_DIR);
		final VA15_OrganizationalUnitAdministrationServiceStub organizationalUnitAdministrationService = new VA15_OrganizationalUnitAdministrationServiceStub(configContext, SERVICE_EPR);
		
		// Setting up Rampart for WSS username token authentication
		final StAXOMBuilder builder = new StAXOMBuilder(WORKING_DIR + "/policy/UsernameTokenPolicy.xml");
		final Policy rampartPolicy = PolicyEngine.getPolicy(builder.getDocumentElement());
		final ServiceClient serviceClient = organizationalUnitAdministrationService._getServiceClient();
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
		
		return organizationalUnitAdministrationService;
	}
}
