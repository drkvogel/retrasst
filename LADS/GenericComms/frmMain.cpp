//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmMain.h"
#include "LCDbAuditTrail.h"
#include "LCDbProject.h"
#include "TfrmAboutBox.h"
#include "lease.h"
#include "LeaseIDs.h"
#include "TfrmLoginBase.h"
#include "TfrmLoginA.h"
#include "LCDbTest.h"
#include "LCDbObject.h"
#include "LCDbAnalyser.h"
#include "LIMSParams.h"
#include "frmClipboard.h"
#include "frmEnterResults.h"
#include "frmManage.h"
#include "LDbWorklist.h"
#include "LIMSAnalysis.h"
#include "BuddyLogin.h"
#include "LQuery.h"
#include "LCDbOperator.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformMain *formMain;
TForm * TfrmLoginBase::mainForm = NULL;
//---------------------------------------------------------------------------
const char * TfrmAboutBox::author = "Chris Bird";
const char * TfrmAboutBox::compiled = __DATE__;
const char * TfrmAboutBox::extension = "x3511";
const char * LCDbAuditTrail::default_email = "chris.bird@ctsu.ox.ac.uk";
const int LCDbProjects::leaseID = LEASEE_GENCOMMS;
//---------------------------------------------------------------------------
//tmd->getMachineID(), tmd->getCode(), tmd->getProtocol(), tmd->getTestOrder(), tmd->getSampleType());

char *strWLColumnHeaders[] = {
    "Barcode",
	"Machine ID",
	"Code",
	"Protocol",
	"Test",
	"Sample Type"
};
const int numStrWLColumnHeaders = sizeof(strWLColumnHeaders)/sizeof(strWLColumnHeaders[0]);
//---------------------------------------------------------------------------
__fastcall TformMain::TformMain(TComponent* Owner) : TForm(Owner)
{
	TfrmLoginBase::mainForm = this;
}

void __fastcall TformMain::FormCreate(TObject *Sender)
{
	if (!MYDEBUG) {
		//LCDbProjects::changeDbName( dbCentral, "ldbc" );
		//frmEnterResults->OpenDialog4->InitialDir = "K:\\LABS\\HPLC\\HPLC exports";
		Caption = Caption + "  **live**";
	} else {
		//LCDbProjects::changeDbName( dbCentral, "t_ldbc" );
		Caption = Caption + "  **test**";
		btnPopulateWorklist->Visible = true;
	}

	// worklist column headers
	sgWLColumnHeaders->ColCount = numStrWLColumnHeaders;
	for (int i=0; i < numStrWLColumnHeaders; i++) {
		sgWLColumnHeaders->Cells[i][0] = strWLColumnHeaders[i];
	}
}

void __fastcall TformMain::FormShow(TObject *Sender) {
	try {
		// init LIMS objects
		LCDbProjects::records().read(frmBuddyLogin->qCentral,false);
		comboProjects->Items->AddObject("(all projects)", static_cast< TObject * >(0));
		for (Range< LCDbProject > p = LCDbProjects::records(); p.isValid(); ++p) {
			comboProjects->Items->AddObject(p->getName(), (TObject *)p->getID()); // do need to cast here
		}
		comboProjects->ItemIndex = 0;
		LCDbTests::records().read(frmBuddyLogin->qCentral,false);
		LCDbObjects & names = LCDbObjects::records();
		names.read(frmBuddyLogin->qCentral, false);
		LCDbAnalysers & analysers = LCDbAnalysers::records();
		analysers.read(frmBuddyLogin->qCentral, false );
		LIMSParams & params = LIMSParams::instance();
		if (params.checkUnique() && params.checkMachine() && params.checkDiskSpace())
		{
			const LCDbAnalyser * analyser = analysers.findByID(LCDbAnalysers::getCurrentID());
			if (analyser != NULL)
				Caption = Caption + ": " + analyser->getName();
		}

		// check manager permissions
		// should have been read in already, don't read again
		// operators.read(frmBuddyLogin->qCentral, false); // ie. don't do this
		// need to insert into c_permission:
		// permission_cid: next_cid
		// operator_cid for cbird = -31438
		// program_cid: 110 (defined where?)
		// project_cid: 0 [any project]
		// page_number: [program defined]
		// machine_cid for KVOGEL-PROBOOK (this machine): -2000000 (set to 0 for any machine);
		// machine_cid: LCDbAnalysers::getCurrentID() = -9010, in this case
		// insert into c_permission
		//     (permission_cid, operator_cid, program_cid, project_cid, page_number, machine_cid)
		//     values (-9348209, -31438, 110, 0, 1, 0)
		LCDbOperators & operators = LCDbOperators::records();
		const LCDbOperator * user = operators.findByID(LCDbOperators::getCurrentID());
		// LCDbOperator::canRun( int project, int buddy, short page )
		if (user != NULL && user->canRun(0, LCDbAnalysers::getCurrentID(), PAGE_MANAGE_FORMATS)) {
			groupboxManage->Enabled = true;
			btnManage->Enabled = true;
		} else {
			groupboxManage->Enabled = false;
			btnManage->Enabled = false;
		}
	} catch (...) {
		Application->MessageBoxA("Error initialising form", "Error", MB_OK);
	}

}
//---------------------------------------------------------------------------
void __fastcall TformMain::btnGetWorklistClick(TObject *Sender)
{
	fnLoadWorklist();
}

void TformMain::fnLoadWorklist() {
	listboxWorklist->Clear();
	int expectedCategory = frmBuddyLogin->getCategory();
	int projectID = (int)(comboProjects->Items->Objects[comboProjects->ItemIndex]);
	// find samples that have been swiped in for this cluster with the expected category ID
	std::vector< LIMSAnalysis > activeEntries = LIMSAnalysis::checkWorklist(qCentral, expectedCategory, projectID);
	for( std::vector< LIMSAnalysis >::iterator a = activeEntries.begin(); a != activeEntries.end(); ++a ) {
		// check local and/or central worklists as appropriate to find tests required
		unsigned count = a->getExpectedTests(qLocal, qCentral);
		if(count > 0) {
			String barcode = a->getBarcode();
			std::set< int > expected = a->getTestIDs();
			// get the corresponding test/machine definitions
			for(Range< LCDbTest > t = LCDbTests::records(); t.isValid(); ++t) {
				if(expected.count(t->getID()) > 0) {
					String testName = t->getName();
					const LCDbTest::MachineDef * tmd = t->findMachineDef(0);
					// now have all the information needed
					// protocol is from c_test_machine
					String strTmp;
					strTmp.sprintf("%16s         %10d           %10s          %d          %d            %s",
						a->getBarcode().c_str(),
						tmd->getMachineID(), tmd->getCode().c_str(), tmd->getProtocol(),
						tmd->getTestOrder(), tmd->getSampleType().c_str());
					WorklistItem * worklistItem = new WorklistItem(
						tmd->getMachineID(), 	// _machineID,
						a->getBarcode(), 		// _barcode,
						t->getName(), 			// _testName,
						t->getID(), 			// _testID,
						projectID, 				// ? _projectID,
						"profileName", 			// _profileName,
						a->getTimeStamp() 		// _timeStamp)
					);
					listboxWorklist->Items->AddObject(strTmp, (TObject *)worklistItem);
// WIP - should use LDbWorkEntry but not sure how to do this atm
//					LDbWorkEntry( 	 a->getBarcode(),
//									 central.readInt( "project_cid" ),
//									 central.readInt( "sample_id" ),
//									 a->getTimeStamp(), // central.readDateTime( "in_date" ),
//									 t->getID()),
					//listboxWorklist->Items->AddObject(strTmp, (TObject *)t); // add test instead?
				}
			}
		}
	}
}

void TformMain::fnDeleteWorklistTotal(void) // TODO redundant
{
	// genericComms::tdvecpWorklistItem::const_iterator it;
	// for (it = vecpWorklistTotal.begin(); it != vecpWorklistTotal.end(); it++) {
		// delete *it; // TODO is this necessary?
	// }
	// vecpWorklistTotal.clear();
}

void __fastcall TformMain::btnCreateSampleSelectionClick(TObject *Sender)
{
	if (0 == listboxWorklist->Items->Count) { // || !bGotWorklist) {
		statusBar->SimpleText = "No worklist entries have been loaded!";
		return;
	}
	if (0 >= listboxWorklist->SelCount) {
		statusBar->SimpleText = "No worklist entries have been selected!";
		return;
	}
	// tdvecpWorklistItem::const_iterator it;
	WorklistItem * worklistItem;
	//fnDeleteWorklistSelection();
	vecpWorklistSelection.clear();
	for (int idx=0; idx < listboxWorklist->Items->Count; idx++) {
		if (listboxWorklist->Selected[idx]) {
			worklistItem = static_cast< WorklistItem * >(listboxWorklist->Items->Objects[idx]);
			vecpWorklistSelection.push_back(worklistItem);
		}
	}

	formClipboard->setSelection(& vecpWorklistSelection);

	if (mrOk == formClipboard->ShowModal()) {
		statusBar->SimpleText = ""; //???#
	}
}

void __fastcall TformMain::btnEnterResultsClick(TObject *Sender)
{
    formEnterResults->ShowModal();
}

void __fastcall TformMain::btnManageClick(TObject *Sender)
{
    formManage->ShowModal();    
}

void __fastcall TformMain::btnPopulateWorklistClick(TObject *Sender) // debug fn to add fake records
{
// mch -9005, test -1097756, record_cid 0 (rfp fills in)

		LQuery q(frmBuddyLogin->qCentral);
		DateTimeToString(tmp, String("yyyymmdd'T'hhnnss"), Now());
		q.setSQL("INSERT INTO c_buddy_worklist"
				 " (machine_cid, barcode, test_cid, sample_id, project_cid)"
				 " VALUES (:mch, :bar, :tst, :smp, :prj)");
		q.setParam("mch", LCDbAnalysers::getCurrentID()); // this machine
		q.setParam("bar", tmp); 	// pseudo-random (datetime)
		q.setParam("tst", -1097756); // any valid

		// pull valid test from c_test
		// and insert into c_test_machine
		q.setParam("smp", 3403984); // any valid
		q.setParam("prj", (int)(comboProjects->Items->Objects[comboProjects->ItemIndex])); // currently selected
		q.execSQL();
		OutputDebugString(q.getSQL().c_str());
		String tmp2; tmp2.sprintf("mch: %d, bar: '%s'", LCDbAnalysers::getCurrentID(), tmp.c_str());
		OutputDebugString(tmp2.c_str());
}

void __fastcall TformMain::comboProjectsChange(TObject *Sender)
{
	fnLoadWorklist();
}

void __fastcall TformMain::BitBtn1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
//	// test: do objects need to descend from TObject and/or cast in some way to and from TObject *
//	// to use as Data element with AddObject?
//
//	//typedef struct MyRec {    // this example from the help files fails to compile
//								// because MyRec is not a descendant of TObject - fail!
//	typedef struct MyRec : public TObject { // this is the way to do it
//	  AnsiString FName, LName;
//	  int	id;
//	} TMyRec;
//	typedef TMyRec* PMyRec;
//	TStringList * myStringList = new TStringList;
//	PMyRec  MyRecPtr, MyOtherRecPtr;
//	MyRecPtr = new TMyRec;
//	MyRecPtr->FName = "Test1";
//	MyRecPtr->LName = "Test2";
//	MyRecPtr->id = 123;
//	int testInt = 123;
//	myStringList->AddObject("Test", MyRecPtr); // don't have to explicitly cast to TObject *, but does need to descend TObject
//	myStringList->AddObject("Test", static_cast< TObject * >(MyRecPtr)); // but might not hurt
//	MyOtherRecPtr = (TMyRec *)(myStringList->Objects[0]); // but do need to cast back
//	MyOtherRecPtr = static_cast< TMyRec * >(myStringList->Objects[0]); // this is probably better
//	myStringList->AddObject("Test", dynamic_cast< TObject * >(MyRecPtr)); // this also works
//	myStringList->AddObject("Test", 0); // don't need to cast int literal
//	myStringList->AddObject("Test", ( TObject * )(MyRecPtr->id)); // but do need to cast member variable?
//	myStringList->AddObject("Test", ( TObject * )(testInt)); // or variable in general
//	// myStringList->AddObject("Test", dynamic_cast< TObject * >(testInt)); // this doesn't work
//	// myStringList->AddObject("Test", static_cast< TObject * >(testInt)); // neither does this
