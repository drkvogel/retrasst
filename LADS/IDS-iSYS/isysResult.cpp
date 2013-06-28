//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#include <vcl.h>
#include "isysResult.h"
#include "isysMainForm.h"
#include "TfrmCommsMain.h"
// #include "DebugCommsFrm.h"
// #include "isysDebugComms.h"


#pragma hdrstop
#pragma package(smart_init)
//---------------------------------------------------------------------------

// extern TisysMainForm *isysMainForm;

IsysResult::IsysResult() : saved(0), notes(0)
{
	resultStage = WAITING;
	noteList = new TStringList;
}

IsysResult::~IsysResult()
{
	delete noteList;
}

static void report( const String & message, TmainForm::LogLevel level )
{
	isysMainForm -> Display_Message( message, level );
}

//extern TmainForm *mainForm;
//extern TisysMainForm *isysMainForm;

void IsysResult::setResultStage(enumResultStage stage) {
//    isysMainForm->statusBar->SimpleText = "test";
}

//---------------------------------------------------------------------------
//  Merge data from the analyser with existing data and/or store it
//---------------------------------------------------------------------------

bool IsysResult::add(const CommsMessage & received, LQuery data, LQuery worklist)
{
	const IsysMessage & msg =  dynamic_cast< const IsysMessage& >(received);
/*
#ifdef MYDEBUG
   if (IsysDebugComms::useDebugComms) {
		String strTemp; strTemp.sprintf("add: %s", msg.record);
		((IsysDebugComms *)(isysMainForm->input))->sendBytes(strTemp.c_str(), strTemp.Length());
		isysMainForm->Display_Message(strTemp, TmainForm::DEBUG);
   }
#endif
*/
	switch(msg.getRecordType())
	{
		case 'H':
			return parseMessageHeader(msg);

		case 'P':
			return parsePatientInformation(msg);

		case 'O':
			saveResult(data, worklist);
			updateSummary();
			return parseTestOrderRecord(msg);

		case 'R':
			saveResult(data, worklist);
			return parseResultRecord(msg);

		case 'Q':
			return parseInformationRequest(msg);

		case 'C':
			return parseCommentRecord(msg);

		case 'L':
			saveResult(data, worklist);
			updateSummary();
			return parseMessageTerminator(msg);

		default:
			report( "Ignoring unexpected record type " + msg.getRecordType(), TmainForm::DEBUG);
			return false;
	}
}

/* H - Header block

 1 Record Type				H 		Fixed value H to indicate a Header block.
 2 Delimiters Definition	|\^_ 	Definitions of the delimiters that will be used in the message. These are the 4 characters which follow the H, with the following being initialized:
										• The field delimiter ( I ) which separates the various fields.
										• The repeat delimiter ( \ ) which separates a repetition in a field.
										• The component delimiter ( ^ ) which separates the various components of the field.
										• The escape delimiter ( _ ) which indicates an escape sequence for dealing with special characters.
										N.B. The record delimiter is fixed and is always the character ASCII 13 (carriage return).
 3 Message Control ID 				Not used.
 4 Access Password 					Not used.
 5 Sender Name or ID 				Name or identifier of the issuer:
										• Not used in the direction LIS => analyser.
										• Fixed at IDS-iSYS + Serial number in the direction device to LIS.
 6 Sender Street Address 			Not used.
 7 Reserved Field
 8 Sender Telephone Number 			Not used.
 9 Characteristics of Sender 		Not used.
 10 Receiver ID 						Not used.
 11 Comment or Special Instruction 	Not used.
 12 Processing ID 			P 		Type of message fixed at P for Production.
 13 Version No. 			2 		Version of the protocol, always equal to 2.
 14 Date and Time of Message 	20030204142817  Date of message in the format YYYYMMDDHHmmss.

 e.g.  H|\^_||||||||||P|2|20050110162817  */

bool IsysResult::parseMessageHeader(const IsysMessage & msg)
{
	resultStage = HEADER;

	strHDelimiters 		= msg.getField(2);
	strHSenderName  	= msg.getField(5);
	strHProcessingID 	= msg.getField(12);
	strHVersionNo 		= msg.getField(13);
	dtmHDateTime  		= msg.getDate(14);

	String response;
	response.sprintf( "Header block: "
		"delims: %s, sender: %s, procID: %s, version: %s, datetime: %s",
		strHDelimiters, strHSenderName, strHProcessingID, strHVersionNo, dtmHDateTime.DateString());
	report( response, TmainForm::DEBUG);
	return true;
}

/* Patient block

# ASTM Field Name					Example Description
1 Record Type 						P 		Fixed value P to indicate a Patient block.
2 Sequence Number 					1 		Number of the Patient block in the message starts at 1 and is increased for each block in the message.
3 Practice Assigned Patient ID 				Not used.
4 Laboratory Assigned Patient ID	1236548563
											PID.
5 Patient ID # 								Not used.
6 Patient Name 						CESBRON^Samuel^^^^
											Field consisting of 5 components last name, first name, initials, suffix and title.
											Only the first 2 fields are used.
7 Mother’s Maiden Name 					  Not used.
8 Birth Date 						19731209
											Date of birth in format YYYYMMDD.
9 Patient Sex 						M 		Reference of patient values:
											• M for Man.
											• F for Woman.
											• E for Child.
											• U for others.
											In the direction LIS=>IDS-iSYS:
											• M for Man.
											• F for Woman.
											• U for Child.
											• U for others.
10 Patient Race 								Not used.
11 Patient Address 							Not used.
12 Reserved Field 							Not used.
13 Patient Telephone Number 				Not used.
14 Attending Physician ID 			PAUL^^^^^ Name of doctor associated with profile.
15 Special Field 				1 			Not used.
16 Special Field 				2 			Not used.
17 Patient Height 							Not used.
18 Patient Weight 							Not used.
19 Patient Known
   or Suspected Diagnosis 		Comment 	Comment associated with profile.
20 Patient Active Medication 				Not used.
21 Patient Diet 							Not used.
22 Practice Field 				1 			Not used.
23 Practice Field 				2 			Not used.
24 Admission and Discharge Dates 			Not used.
25 Admission Status 						Not used.
26 Location 								Not used.
27 Nature of Alternative Diagnostic Code 	Not used.
28 Alternative Diagnostic Code 				Not used.
29 Patient Religion 						Not used.
30 Marital Status 							Not used.
31 Isolation Status 						Not used.
32 Language 								Not used.
33 Hospital Service 			Radiology^^^^^
											Department associated with the profile.
34 Hospital Institution 					Not used.
35 Dosage Category 							Not used.

e.g. P|1||1236548563||CESBRON^Samuel^^^^||19731209|M||^^^^|||PAUL^^^^^|||||Comment||||||||||||Radiology^^^^^||
*/

bool IsysResult::parsePatientInformation(const IsysMessage & msg)
{
//    if (true) {
//        
//    }
    resultStage = PATIENT_DATA;

	intPSequenceNo 				= msg.getField(2).ToInt();
	strPLabPatientID 			= msg.getField(4);
	int patientID = strPLabPatientID.ToIntDef(0); // XXX SHOULD BE ALPHA
	if (patientID > 0) {
		buff = "Patient ID = " + strPLabPatientID;
	} else {
		buff = "Patient ID not given";
	}
	strPPatientFamilyName 		= msg.getPart(6, 1);
	strPPatientGivenName  		= msg.getPart(6, 2);
	dtmPPatientDOB  			= msg.getDate(8);
	strPPatientSex  			= msg.getField(9);  // M, F, E (child), U (others)
	strPDoctorName  			= msg.getField(14); // xxx multi part?
	strPPatientDiag 			= msg.getField(19);
	strPHospDept    			= msg.getField(33); // last field?

	String response;
	response.sprintf( "Patient block: "
		"seq: %d, patientID: %s, famname: %s, givenname: %s, dob: %s, sex: %s, "
		"doctorname: %s, diagnosis: %s, hospdept: %s",
		intPSequenceNo, strPLabPatientID,
		strPPatientFamilyName, strPPatientGivenName, dtmPPatientDOB.DateString(),
		strPPatientSex, strPDoctorName, strPPatientDiag, strPHospDept);
	report( response, TmainForm::DEBUG);
	return true;
}

/* Test Order Block

# ASTM Name 					Value 		Description and use in IDS-iSYS system
1 Record Type ID 				O 			Fixed value O for indicating an Order block.
2 Sequence Number 				1 			Number of the order within the Patient block starts at 1 and is increased for each Order block in the message.
3 Container and Specimen ID 	01^100      This field consists of 2 components:
											• The type of container used for the sample (2 digits).
											• 01 PT_11_4.5 mL.
											• 02 PT_13_6 mL.
											• 03 ST_13_5 mL H.
											• 05 Cup 500 ?L.
											• 06 Cup 2 mL.
											• 07 PT_16_10 ml.
											PT for Primary Tube; ST for Secondary Tube
											For correct functioning of the apparatus, it is important to allocate the correct type of container used for the sample.
											• The SID of the profile concerned by the request.
4 Instrument Specimen ID 					Not used.
5 Universal Test ID 			^^^123 		This ASTM field consists of 4 components:
											• Universal test ID.
											• Universal test ID Name.
											• Universal test ID Type.
											• Manufacturer’s Code.
											Only the last component is used and will be entered with the unique IDS-iSYS identifier of the analyte.
											In one single order it is possible to specify several tests by adding a repeat delimiter and by then adding one or several UTI thus obtaining a chain of the type ^^^123\^^^124.
6 Priority 						S 			This field may contain the values:
											• S = STAT.
											• A = ASAP.
											• R = Routine.
											• C = Callback.
											• P = Preoperative.
											For the device, only S has any influence, the passage of the associated profile into Urgent, and the other values are ignored.
7 Requested/Ordered	Date and Time			Not used.
8 Specimen Collection Date and Time			Not used.
9 Collection End Time 						Not used.
10 Collection Volume 						Not used.
11 Collector ID 							Not used.
12 Action Code 					A 			Action to be carried out on the block test or tests.
											This field may contain the values:
											• A for Add.
											• C for Cancel.
											• N for New.
											• P for Pending.
											• Q for QC.
											For the device, A and N are managed in the same way (new request or new rerun), C (cancellation of a request) can be managed if the test is not in progress. P and Q are not managed.
13 Danger Code 								Not used.
14 Relevant Clinical Information 			Not used.
15 Date and Time Specimen Received			Not used.
16 Specimen Descriptor 			S^ 			This field consists of two sections:
											• The sample type which can take for the IDS-iSYS the following values:
												• U = Urine.
												• S = Serum.
												• O = Other.
											• The sample source (place where the sample was taken) is not used.
17 Ordering Physician 						Doctor.
18 Physician Telephone Number 				Not used.
19 User Field 1 							Not used.
20 User Field 2 							Not used.
21 Laboratory Field 1 						Not used.
22 Laboratory Field 2 						Not used.
23 Date and Time Result Reported or Last Modified Not used.
24 Instrument Charge to Computer System   	Not used.
25 Instrument Section ID 					Not used.
26 Report Type 					O 			This field may take the values:
											• O = Order.
											• C = Correction.
											• P = Preliminary results.
											• F = Final results.
											• X = Cancel.
											• I = Instrument pending (in response to a request).
											• Y = No order on record for this test (in response to a request).
											• Z = No record for this patient (in response to a request).
											• Q = Response to a request.
											In the device C will not be managed.
27 Reserved Field 							Not used.
28 Location or Ward of Specimen Collection	Not used.
29 Nosocomial Infection Flag 				Not used.
30 Specimen Service 						Not used.
31 Specimen Institution 					Not used.

e.g. O|1|01^100||^^^123|S||||||||||S^||||||||||O|||||
*/

bool IsysResult::parseTestOrderRecord(const IsysMessage & msg)
{
	intOSequenceNo   		= msg.getField(2).ToInt();
	//intOContainerType 	= msg.getPart(3, 1).ToInt();
	//strOSpecimenID 		= msg.getPart(3, 2);
	strOSpecimenID 			= msg.getField(3);
	// in direction IDS->LIS, container type is not reported/known
	strOManufacturerCode    = msg.getPart(5, 4);
	strOPriority 			= msg.getField(6); // S, A, R, C, P
	strOActionCode 			= msg.getField(12); // A, C, N, P, Q
	strOSpecimenDescriptor 	= msg.getPart(16, 1); // U, S, O
	strOReportType 			= msg.getField(26); // O, C, P, F, X, I, Y, Z, Q

	resultStage = PATIENT_DATA;
	String response;
	response.sprintf("Test order: "
		"seq: %d, containertype: %d, specimenID: %s, manucode: %s, priority: %s, "
		"actioncode: %s, specimen: %s, reporttype: %s",
		intOSequenceNo, intOContainerType, strOSpecimenID, strOManufacturerCode,
		strOPriority, strOActionCode, strOSpecimenDescriptor, strOReportType);
	report( response, TmainForm::DEBUG);
	return true;
}

/*** Result block

1 Record Type ID 				R 			Fixed value R for indicating a Result block.
2 Sequence Number 				1 			The number of the result within the Patient block begins at 1 and is increased for each Result block in the message.
3 Universal Test ID 			^^^123 		This ASTM field consists of 4 components:
											• Universal test ID.
											• Universal test ID name.
											• Universal test ID type.
											• Manufacturer’s code.
											Only the last component is used and will be provided with information with the unique IDS-iSYS identifier of the analyte.
4 Data or Measurement Value 	12.5 		The value in the form of ASCII chain.
5 Units 						mg 			Blocks used
											WARNING: the abbreviations must be IS0 2955.
6 Reference Range 				10.5 to 13.5^Normal\ 9 to 15^Limit
											Normal reference values used for the patient sent through in the form of an ASCII chain.
7 Result abnormal Flags 		N 			This field may take the values:
											• L = Below low normal.
											• H = Above high normal.
											• LL = Below alarm low.
											• HH = Above alarm high.
											• < of absolute low (Field of measurement).
											• > of absolute high (Field of measurement).
											• N = Normal.
											• A = Abnormal.
											• U = Significant change Up.
											• D = Significant change Down.
											• B = Better.
											• W = Worse.
											Only the first 8 characters are managed by the IDS-iSYS.
8 Nature of Abnormality Testing	S 			This field forces S or A for the IDS-iSYS can take the values:
											• A = Age.
											• S = Sex.
											• R = Race.
											And is able to provide an indication about the bases on which the values of normality were determined.
9 Result Status 				F 			This field forces F, X or C (rerun) on the IDS-iSYS can take the following values:
											• C = Correction.
											• P = Preliminary results.
											• F = Final results.
											• X = Request not honored.
											• I = Results pending.
											• S = Partial results.
10 Date of change in Instrument Normative Values
											Not used.
11 Operator ID 								Not used.
12 Date and Time Test Started 	YYYYMMDDHHMMSS
											Date and time of start of test on the instrument.
13 Date and Time Test Completed YYYYMMDDHHMMSS
											Date and time of result of test on the GUI.
14 Instrument Identification 	IDS-ISYS01 	Name of device + Serial number

e.g. R|1|^^^123|12.5|mg|10.5a 13.5^Normal\ 9a 15^Limite|N|S|F|||18991230000000|18991230000000|
*/

bool IsysResult::parseResultRecord(const IsysMessage & msg)
{
	switch(resultStage) {
		case PATIENT_DATA:
			if (noteList->Count == 0)
				resultStage = RESULT_DATA;
			else
				throw Exception("Sample notes have not been saved");
			break;
		case RESULT_DATA:
        	break;
//			throw Exception("Unexpected Result record, already in RESULT_DATA"); //???
		default:
			throw Exception("Test result for unidentified sample");
	}
	intRSequenceNumber  		= msg.getField(2).ToInt();
	strRManufacturerCode		= msg.getPart(3, 4);
	strRDataValue				= msg.getField(4);
	strRUnits					= msg.getField(5);
	strRRange					= msg.getField(6);
	strRAbnormalFlags			= msg.getField(7);
	strRAbnormality			    = msg.getField(8);
	strRResultStatus		    = msg.getField(9);
	dtmRDateStarted				= msg.getDate(12);
	dtmRDateCompleted			= msg.getDate(13);
	strRInstrumentID			= msg.getField(14);

	//???testID = LCDbTests::records().findTestID(strRManufacturerCode, "");
	// test code from c_test, from c_test_machine, will be "25DRU" for VITD, isys manuf. code
	testID = LCDbTests::records().findTestID(LCDbAnalysers::getCurrentID(), strRManufacturerCode );

	String response;
	response.sprintf("Result block: "
			"seq: %d, manuf: %s, data: %s, units: %s, range: %s, "
			"abflags: %s, ab: %s, result: %s, started: %s, completed: %s, instrument: %s",
			intRSequenceNumber, strRManufacturerCode, strRDataValue, strRUnits, strRRange,
			strRAbnormalFlags, strRAbnormality, strRResultStatus, dtmRDateStarted.DateString(), dtmRDateCompleted.DateString(),
			strRInstrumentID);
	report( response, TmainForm::DEBUG);

    if (!strRAbnormalFlags.AnsiCompare("N")) {
        buff.sprintf("Abnormal flag '%s' for sample %s", strRAbnormalFlags, strOSpecimenID);
        noteList->Add(buff);
    }
	return true;
}

//	String testCode = ccm.getPart( 3, 4 );
//	testID = LCDbTests::records().findTestID( testCode, fluid );
//	if( testID == 0 )
//		noteList -> Add( "Test code: " + testCode );

//	if( !abnormal.IsEmpty() )
//	{
//		String flags = ccm.getPart( 4, 2 );
//		if( flags.IsEmpty() )
//			checkValue( result );
//		else
//			checkFlags( flags );
//
//		String range = ccm.getField( 7 );
//		if( !range.IsEmpty() )
//			noteList -> Add( "Range: " + range );
//	}

/* Q - Request Information Block
# Parse a request for information, origin from the iSYS only.

1 Record Type ID 				Q 			Fixed value Q to indicate a Request Information block.
2 Sequence Number 				1 			The request number starts at 1 and is increased for each request block in the message.
3 Starting Range ID Number 		11111^100^ 	This field consists of 3 elements; only the first 2 are used by the IDS-iSYS in order to specify the PID (optional) and the SID (obligatory) allowing the profile quoted to be identified.
4 Ending Range ID Number 					Not used.
5 Universal Test ID 			ALL 		This field can either indicate 1 particular test (in the identifier via its ID cf Test Order block) or request all the tests via the code ALL.
6 Nature of Request Time Limit 				Not used in the direction IDS-iSYS=>LIS.
											Useful if LIS => IDS-iSYS with the value R to say that the date to be taken into account in the event of filter is the date of the test results and the date of arrival of the profile (code S).
7 Beginning Request Results Date and Time 	Not used.
											Useful in the direction LIS=>IDS-iSYS.
8 Ending Request Results Date and Time		Not used.
											Useful in the direction LIS=>IDS-iSYS.
9 Requesting Physician Name 				Not used.
10 Requesting Physician Telephone Number	Not used.
11 User Field 1 							Not used.
12 User Field 2 							Not used.
13 Request Information Status Code	O 		Potential values:
											C P F X I S M R A N O D
											Only the values O and X are managed in the direction IDS-iSYS to LIS.
											In the direction LIS to IDS-iSYS F and I are managed.
e.g. # from example transfer, has 13 fields:
     Q|1|^999||ALL||||||||O

     # from record description, only has 12 fields
     Q|1|11111^100^|^^|ALL|||||||F */

bool IsysResult::parseInformationRequest(const IsysMessage & msg)
{
    if (resultStage != HEADER) {
        throw Exception("Information request when not HEADER");
    }
    resultStage = INFO_REQUESTED;

	intQSequenceNumber			 	= msg.getField(2).ToInt();
	//intQPID							= msg.getPart(3, 1).Length() > 0 ? msg.getPart(3, 1).ToInt() : -1;
	//intQSID							= msg.getPart(3, 2).ToInt();
	strQPID	= msg.getPart(3, 1); //msg.getPart(3, 1).Length() > 0 ? msg.getPart(3, 1) : "";
	strQSID                         = msg.getPart(3, 2);
	strQUniversalTestID				= msg.getField(5);
    //String strStatusCode            = msg.getField(13);
	chaQStatusCode					= msg.getField(13)[1]; // C P F X I S M R A N O D

	// ??? TODO xxx should initiate comms

//    if (!IsysDebugComms::disableReply)
	    isysMainForm->getWorklist().add(strQSID);

	String response;
	response.sprintf( "Request Information: "
		"seq: %d, PID: %s, SID: %s, UnivTestID: %s, statuscode: %c",
		intQSequenceNumber, strQPID, strQSID, strQUniversalTestID, chaQStatusCode);
	report( response, TmainForm::DEBUG);
	return true;
}

/* Comments block

1  Record Type ID 		C 	Fixed value C for indicating a Comment block.
2  Sequence Number 		1 	Number of the comment begins at 1 and is increased for each comment block in the message.
3  Comment Source 			I 	Issuer can take the following values:
							• P = Practice.
							• L = computer system in the direction LIS.
							• I = Instrument.
4  Comment Text 			NSU Text of comment.
5  Comment Type	 		N 	Type of comment. This field can take the values:
							• G = Generic comment (any permitted).
							• T = Test name comment.
							• P = Positive test comment.
							• N = Negative test comment.
							• I = Instrument flags comment.

e.g. C|1|I|NSU|N
*/

bool IsysResult::parseCommentRecord(const IsysMessage & msg)
{
	intCSequenceNumber		 		= msg.getField(2).ToInt();
	chaCCommentSource				= msg.getField(3)[1];
	strCCommentText 				= msg.getField(4);
	chaCCommentType					= msg.getField(5)[1];

	String response;
	response.sprintf( "Comment block: "
		"seq: %d, source: %c, text: %s, type: %c",
		intCSequenceNumber, chaCCommentSource, strCCommentText, chaCCommentType);
	report( response, TmainForm::DEBUG);

	noteList->Add(strCCommentText); //??
	return true;
}

/* Message terminator

1  Record Type ID 		L 	Fixed value L to indicate a Terminator block.
2  Sequence Number 		1 	Always at 1 as there is only one sequence per message.
3  Termination code 	N 	This code designates of end of the message with the following values, depending on the situation:
						• N = Normal.
						• T = Abandon issuer.
						• R = Abandon receiver.
						• E = Unknown Error.
						• Q = Error in last request.
						• I = No Information available for last query.
						• F = Last request for information processed.

e.g. L|1|N
*/

bool IsysResult::parseMessageTerminator(const IsysMessage & msg)
{
	chaLTerminationCode	= msg.getField(3)[1];
	String response;
	if (chaLTerminationCode != 'N') {
		response.sprintf("Abnormal termination code: %c");
		report( response, TmainForm::DETAIL );
		return false;
	} else {
		resultStage = WAITING;
		response = "Accepting message terminator";
		report( response, TmainForm::DEBUG );
		return true;
	}
}

//---------------------------------------------------------------------------
//	Save accumulated information in the relevant database tables
//---------------------------------------------------------------------------

// formerly keepResult
bool IsysResult::saveResult(LQuery & qData, LQuery & qWorklist)
{
	LIMSResult current(strOSpecimenID, dtmHDateTime); // iSys does not report time ordered
	const LBDbSource * header = current.findAnalysis( qData, intPAnalysisID );
	if( header == NULL ) {
		header = LBDbSources::records().readRecord( qData, strOSpecimenID, "", "" );
	}
	if( header != NULL ) {
		current = *header;
	}
	else if( !current.saveAnalysis( qData, "" ) ) {
		return false;
	}
	intPAnalysisID = current.getBuddySampleID();

	switch(resultStage)
	{
		case PATIENT_DATA:
            current.setSampleType(strOSpecimenDescriptor);
			return updateAnalysis(current, qData, qWorklist);

		case RESULT_DATA:
			current.setTestResult(testID, dtmRDateCompleted, strRDataValue);
			current.setUnitCode(strRUnits);
			if( !current.updateWorklist(qData, qWorklist)) {
				// if not in the worklist, result might be kept as a note
				String result = current.asSampleNote();
				if (!result.IsEmpty()) {
					noteList->Insert(0, result);
					return updateAnalysis(current, qData, qWorklist);
				}
			}
			return addResultData(current, qData, qWorklist);

		default:
			return false;
	}
}

bool IsysResult::updateAnalysis(LIMSResult & current, LQuery & qData, LQuery & qWorklist)
{
	String sampleNote, response;
	if (noteList->Count > 0)
		sampleNote = "iSys analysis: {" + noteList->CommaText + '}';

	if (current.saveAnalysis(qData, sampleNote)) {
		intPAnalysisID = current.getBuddySampleID();
	} else {
		response = "Error saving " + strOSpecimenID;
		report( response, TmainForm::NOTIFY );
		return false;
	}

	if (!sampleNote.IsEmpty()) {
		notes++;
		noteList->Clear();
	}

	response = "Analysis " + strOSpecimenID + " saved OK";
	report( response, TmainForm::DEBUG );
	return true;
}

bool IsysResult::addResultData(LIMSResult & current, LQuery & qData, LQuery & qWorklist)
{
	String resultNote, response;
	if (noteList->Count > 0)
		resultNote = "iSys result: {" + noteList->CommaText + '}';

	if (!current.saveTestResult(qData, resultNote)) {
		response = "Error saving result for " + strOSpecimenID;
		report( response, TmainForm::NOTIFY );
		return false;
	}

	if (!resultNote.IsEmpty()) {
		notes++;
		noteList->Clear();
	}

	saved++;
	response = "Result for " + strOSpecimenID + " saved OK";
	report( response, TmainForm::DEBUG );
	return true;
}

//---------------------------------------------------------------------------
//	End of results for current barcode; keep record of what happened
//---------------------------------------------------------------------------

void IsysResult::updateSummary()
{
	if (saved > 0 || notes > 0)
	{
		String 	message = strOSpecimenID + ": " + saved;
				message += (saved == 1) ? " result" : " results";
				message += " saved with " + String(notes);
				message += (notes == 1) ? " note" : " notes";
                message += "\n";
		saved = notes = 0;

		if (history.IsEmpty())
			history = message;
		else
			history = history + "; " + message;
	}
}

//---------------------------------------------------------------------------
//	Summarise what's happened recently; prepare for the next summary
//---------------------------------------------------------------------------

String IsysResult::summary()
{
	String summary;
	if (resultStage == WAITING)	{
		summary = history;
		history = "";
	}
	return summary;
}




