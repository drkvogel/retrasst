//---------------------------------------------------------------------------


#pragma hdrstop

#include "isysWorklist.h"
#include "LIMSParams.h"
#include "isysMainForm.h"
#include "TfrmCommsMain.h"
#include "DebugCommsFrm.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

// extern TisysMainForm *isysMainForm;

//---------------------------------------------------------------------------
//	Build next worklist record, header or trailer as part of message
//---------------------------------------------------------------------------

CommsMessage * IsysWorklist::nextRequest( LQuery qCluster, LQuery qCentral )
{
	if( worklistState == PARTICIPANT )
	{
		worklistState = SENDING;
		return msgORecord( qCluster, qCentral );		// 'O' follows 'P'
	}

	if( !sampleWaiting() ) {
		if( worklistState == WAITING )
			return NULL;                                // nothing to do
		else
		{	worklistState = WAITING;
			return IsysMessage::createTrailer();		// finish with 'L'
		}
	}

	if( worklistState == SENDING ) {
    	current = getNextSample();
		IsysMessage * response = msgPRecord( qCluster, qCentral );
		if( response != NULL )
		{
			worklistState = PARTICIPANT;
			patient ++;
		}
		return response;								// 'P' for sample
	}
	else
	{	worklistState = SENDING;
		patient = 1;
		return IsysMessage::createHeader();				// start with 'H'
	}
/*}

LIMSAnalysis IsysWorklist::next() {
	std::set< String >::iterator next = pending.begin();
	current = LIMSAnalysis(*next, Now(), getCategory());
	pending.erase(next);
	return current;
	*/
}

//---------------------------------------------------------------------------
//	check central and local worklist for barcodes that may be waiting
//---------------------------------------------------------------------------

void IsysWorklist::addPendingSamples(LQuery qCluster, LQuery qCentral)
{
	LCDbWorkList cwl; //  = LCDbWorkList::records();
	cwl.read( qCentral, false );
	std::set< int > machineIDs = LCDbAnalysers::records().getMachineIDs();

	// include older entries; new ones may not yet have all tests
	int category = isysMainForm->getSelectedCategoryID();
	static const TTime SCAN_INTERVAL = 0.5 / (24 * 60);
	TDateTime latest = Now() - SCAN_INTERVAL;
	for( Range< LCDbWorkEntry > wle = cwl; wle.isValid(); ++ wle ) {
		if( category == 0 || wle -> getCategoryID() == 0 || category == wle -> getCategoryID() ) {
			if( machineIDs.count( wle -> getTestLimits().getAnalyserID() ) != 0
			 && wle -> getStage() < LCDbWorkEntry::PROGRAMMED && wle -> getTimeStamp() < latest ) {
				add( wle -> getBarcode() );
             }
		}
	}

	// add triggers and re-runs from local worklist if necessary
	LBDbWorkList & bwl = LBDbWorkList::records();
	if( LIMSParams::instance().useLocalWorklist() && bwl.read( qCluster, false ) )
	{
		MapRange< String, LBDbWorkEntry > wle = bwl.getTests();
		do
		{	if( wle -> getStage() < LBDbWorkEntry::PROGRAMMED && wle -> getTimeStamp() < latest
			 && machineIDs.count( wle -> getTestLimits().getAnalyserID() ) != 0 )
			{
				add( wle -> getBarcode() );
			}
		} while( (++ wle).isValid() );
	}
}

//---------------------------------------------------------------------------
//	Check worklist for re-runs/tests to run; prepare header accordingly
//---------------------------------------------------------------------------

IsysMessage * IsysWorklist::msgPRecord(LQuery & qCluster, LQuery & qCentral)
{
	String msg;
//	std::set< String >::iterator next = pending.begin();
//	current = LIMSAnalysis(*next, Now(), getCategory());
//	pending.erase(next);

	try {
		if (!current.createHeader(qCluster, qCentral))
			throw Exception("Error saving " + current.getBarcode() + " header");
	} catch (Exception & e) {
		// report error + send empty P record
		msg.sprintf("P|1\r");
		return new IsysMessage(msg);
    }

// e.g. P|1||1236548563||CESBRON^Samuel^^^^||19731209|M||^^^^|||PAUL^^^^^|||||Comment||||||||||||Radiology^^^^^||
//        n  pid         fn      gn          YYYYMMDD           doc           comment            department
//	msg.sprintf("P|%d||%d||%s^%s^^^^||%s|%c||^^^^|||%s^^^^^|||||%s||||||||||||%s^^^^^||",
//                //                       sex
//	            intPSequenceNo, strPLabPatientID.c_str(), intPAnalysisID,
//                strPPatientFamilyName, strPPatientGivenName, strPPatientDOB,
//	            strPPatientSex, strPDoctorName, strPPatientDiag, strPHospDept);

	msg.sprintf( "P|%d||%d\r",	// "P|%d||%d||^^^^^|||||^^^^|||^^^^^|||||||||||||||||^^^^^||\r",
				patient, current.getID());
	return new IsysMessage(msg);
}

//---------------------------------------------------------------------------
//	Summarise what's happened recently; prepare for the next summary
//---------------------------------------------------------------------------

String IsysWorklist::summary()
{
	String summary;
	if (worklistState == WAITING) {
		unsigned p = programmed.size();
		unsigned c = cleared.size();
		if(p + c > 0) {
			if (p == 1)
				summary = "1 sample programmed";
			else
				summary = String(p) + " samples programmed";
			if (p > 0)
				programmed.clear();

			if (c > 0) {
				summary = summary + ", " + c + " cleared";
				cleared.clear();
			}
		}
	}
	return summary;
}

//---------------------------------------------------------------------------
//	Build test order record to follow patient header; update worklist
//---------------------------------------------------------------------------

IsysMessage * IsysWorklist::msgORecord( LQuery & qCluster, LQuery & qCentral )
{
	String sample  = current.getBarcode();
	String container = "01";   // or from user???
//    isysMainForm->Display_Message("Container type not known for sample "+sample, TmainForm::DEBUG);
	char chaOPriority   = 'S'; //???
	char chaOAction     = 'A'; //???
	char chaOSampleType = 'S'; //???
	char chaOReportType = 'O'; //???

	String panel;
	try {
		int buddyID = LCDbAnalysers::getCurrentID();
		const LCDbTests & tests = LCDbTests::records();
		current.getExpectedTests( qCluster,qCentral );
		std::set< int > testIDs=current.getTestIDs();
		for( std::set< int >::const_iterator ti = testIDs.begin(); ti != testIDs.end(); ++ ti ) {
			const LCDbTest::MachineDef * md = tests.get(*ti).findMachineDef( buddyID );
			if( md != NULL )
			{
				String chemCode = md->getCode();
				if( panel.IsEmpty() )
					panel = "^^^" + chemCode;
				else
					panel = panel + "\^^^" + chemCode;
			}
		}
		chaOSampleType = (current.getSampleType())[1]; //???
	} catch (Exception & e) {
		isysMainForm->Display_Message(e.Message, TmainForm::DEBUG);
//		isysMainForm->Display_Message("Couldn't get sample type for O record", TmainForm::DEBUG);
	}

	programmed.insert(sample);
	current.setStage( LDbWorkEntry::PROGRAMMED );
	current.confirmTests( qCluster, qCentral );
//	String msg;
//    msg.printf("O|1|%s^%s||^^^%s|S||||||%c||||%c^||||||||||%c|||||\r",
//        container, sample, manufacturerCode, chaOAction, chaOSampleType, chaOReportType);
	//return new IsysMessage(msg);
	return IsysMessage::createORecord(
		container,
		sample,
		panel,
		chaOPriority,
        chaOAction,
        chaOSampleType,
        chaOReportType);
}


