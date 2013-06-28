//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmClipboard.h"
#include "BuddyLogin.h"
#include "LCDbAnalyser.h"
#include "LQuery.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformClipboard *formClipboard;
using namespace genericComms;
//---------------------------------------------------------------------------
__fastcall TformClipboard::TformClipboard(TComponent* Owner)
    : TForm(Owner)
{
}

void __fastcall TformClipboard::FormShow(TObject *Sender)
{
	memoClipboard->Clear();
	// populate dropdown with output formats assigned to the current analyser
	comboFormat->Items->Clear();
	LQuery qc(frmBuddyLogin->qCentral);
	qc.setSQL("SELECT"
		" ft.filetype_cid,"
		" ft.name,"
		" ft.description,"
		" ft.type,"
		" ft.firstrow,"
		" ft.delim,"
		" ft.subdelim,"
		" ft.time_stamp"
		" FROM g_comms_filetype ft, g_comms_map map"
		" WHERE ft.id = map.id_filetype AND map.id_cluster = :mch "
		" AND ft.status = 0 AND map.status = 0"
		" AND ft.type = :typ");
	qc.setParam("mch", LCDbAnalysers::getCurrentID()); // this machine
	qc.setParam("typ", INPUT_FORMAT);
	qc.open();
	while (!qc.eof()) {
		FileFormat * ff = new FileFormat(
			qc.readInt("filetype_cid"), qc.readString("name"), qc.readString("description"),
			(FormatType)qc.readInt("type"), qc.readInt("firstrow"), qc.readString("delim"),
			qc.readString("subdelim"));
		comboFormat->Items->AddObject(ff->name, (TObject *)ff);
		qc.next();
	}
	if (comboFormat->Items->Count > 0) {
		comboFormat->ItemIndex = 0;
	}
}

void __fastcall TformClipboard::btnCloseClick(TObject *Sender)
{
	Close();
}

void __fastcall TformClipboard::btnCopyClick(TObject *Sender)
{
	memoClipboard->SelectAll();
	memoClipboard->CopyToClipboard();
	btnCopy->Enabled = false;
	memoClipboard->Clear();
}

void __fastcall TformClipboard::comboFormatChange(TObject *Sender)
{
	createFile();
}

void __fastcall TformClipboard::btnGenerateClick(TObject *Sender)
{
	createFile();
}
//---------------------------------------------------------------------------
void TformClipboard::createFile() {

	// create file from data based on selected format
	memoClipboard->Clear();
	tdvecpWorklistItem::const_iterator it;
	for (it = (*worklist).begin(); it != (*worklist).end(); it++) {
		WorklistItem * worklistItem = *it;
		tmp.sprintf(
			"WorklistItem: machineID: %d; barcode: '%s'; testName: '%s'; testID: '%d', projectID: '%d'; "
			"projectName: '%s'; profileName: '%s'; timeStamp: '%s'",
			worklistItem->machineID,
			worklistItem->barcode.c_str(),
			worklistItem->testName.c_str(),
			worklistItem->testID,
			worklistItem->projectID,
			worklistItem->projectName.c_str(),
			worklistItem->profileName.c_str(),
			worklistItem->timeStamp.DateTimeString().c_str());
		memoClipboard->Lines->Add(tmp);
	}
//	String strLine;
//	int idx;
//	frmClipboard->btnCopy->Enabled = true;
//	frmClipboard->Memo1->Clear();
//    DWORD   uiTabStops[] =    { 4 * 8,       //  "",
//                                4 * 16,      //  "Vial",
//                                4 * 30,      //  "SampleName",
//                                4 * 40,      //  "Level",
//                                4 * 50,      //  "Inj Vol",
//                                4 * 60,      //  "# of Injs",
//                                4 * 80,      //  "Function",
//                                4 * 110,     //  "Processing",
//                                4 * 140,     //  "Method Set / Report Method",
//                                4 * 150,     //  "Run Time",
//                                4 * 170,     //  "Next Inj Delay",
//                                4 * 190,     //  "SampleWeight",
//                                4 * 200,     //  "Dilution",
//                                4 * 220      //  "Auto Additions");
//                              };
//    UINT    lResult;
//    HWND    hWndControl = frmClipboard->Memo1->Handle;
//    WPARAM  wParam = 14;
//    LPARAM  lParam = (LPARAM)uiTabStops;
//    lResult = SendMessage(
//       hWndControl,      // handle to destination control
//       EM_SETTABSTOPS,   // message ID
//       wParam,
//       lParam
//    );
//    // or:
//    // Memo1->Perform(EM_SETTABSTOPS, 1, (int)TabStops);
//	strLine.sprintf(
//		"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s",
//			"",
//			"Vial",
//			"SampleName",
//			"Level",
//			"Inj Vol",
//			"# of Injs",
//			"Function",
//			"Processing",
//			"Method Set / Report Method",
//			"Run Time",
//			"Next Inj Delay",
//			"SampleWeight",
//			"Dilution",
//			"Auto Additions");
//	frmClipboard->Memo1->Lines->Add(strLine);
//	for (idx = 1, it = vecpWorklistSelection.begin(); it != vecpWorklistSelection.end(); it++, idx++) {
//		TheWorklistItem = *it;
//		strLine.sprintf(
//			//n  v   sam lev inj #in fun prc mth run del wgt dil add
//			"%d\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s",
//				idx,
//				idx,
//				TheWorklistItem->strBarcode,
//				"",
//				"10.00",
//				"1",
//				"Inject Samples",
//				"Don't Process or Report",
//				"",
//				"10.00",
//				"0.00",
//				"1.00",
//				"1.00",
//				"");
//		frmClipboard->Memo1->Lines->Add(strLine);

}

void TformClipboard::setSelection(tdvecpWorklistItem * selection) {
	// genericComms::tdvecpWorklistItem::const_iterator it;
	// for (it = vecpWorklistTotal.begin(); it != vecpWorklistTotal.end(); it++) {
		// delete *it; // TODO is this necessary?
	// }
	// vecpWorklistTotal.clear();
	worklist = selection;
}
//---------------------------------------------------------------------------

