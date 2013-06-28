//---------------------------------------------------------------------------
//
//  	HPLC Analyser communications program
//
//		Version history:
//
//      	1.0.0, Chris B: 	Initial release
//          1.1.0, Nick G:		Incorporate updated shared classes
//          1.1.1, NG, 13/9/05	Bug fix: skip 'Component Results' line
//          1.1.2, NG, 16/3/06	Replace log file with XML log (always on C:)
//
//---------------------------------------------------------------------------

#include <vcl.h>
#include <algorithm>
#include <ClipBrd.hpp>

#pragma hdrstop

#include "FormFront.h"

#include "LCDbProject.h"
#include "LCDbAnalyser.h"
#include "LDbResult.h"
#include "LIMSParams.h"

#include "FormCommon.h"
#include "FormUtils.h"
#include "FormEnterResults.h"
#include "FormCutNPaste.h"
#include "LDbWorklist.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmFront *frmFront;

tdvecpWorklistItem      vecpWorklistTotal;
tdvecpWorklistItem      vecpWorklistSelection;
tdvecpEmpowerOutputRow  vecpEmpowerOutputRows;

TStringList* tslResults;

//---------------------------------------------------------------------------

__fastcall TfrmFront::TfrmFront(TComponent* Owner)
    : TForm(Owner)
{
}

void __fastcall TfrmFront::btnGetWorklistClick(TObject *Sender)
{
	fnLoadWorklist();

	ListBox1->Enabled = true;
}

void TfrmFront::fnLoadWorklist(void)
{
	LBDbWorkList::records().copyCentral( frmCommon->qryBuddyDB, frmCommon->qryLabCentral );

	std::set< int > cluster = LCDbAnalysers::records().getMachineIDs();
	std::set< int >::const_iterator ci = cluster.begin();
	String machines = *ci;
	while( ++ ci != cluster.end() )
		machines = machines + ',' + *ci;

	QTEXT(qryBuddyDB) =
			"SELECT machine_cid, barcode, test_name, test_id,"
			" project_cid, profile_name, time_stamp"
			" FROM buddy_worklist"
			" WHERE machine_cid IN (" + machines + ")"
			" AND STATUS = 'Q'"
			" ORDER BY Time_stamp, barcode";

	if (!QOPEN(qryBuddyDB)) return;

	fnDeleteWorklistTotal();

	pWorklistItem TheWorklistItem;
	String strCurBarcode;

	while(!QEOF(qryBuddyDB)) {

		TheWorklistItem = new WorklistItem
			(   QF(qryBuddyDB)[0]->AsInteger,
				strCurBarcode =	QF(qryBuddyDB)[1]->AsString,        // strBarcode
				QF(qryBuddyDB)[2]->AsString,            // strTestName
				QF(qryBuddyDB)[3]->AsInteger,           // iTestID
				QF(qryBuddyDB)[4]->AsInteger,           // iProjectID
				QF(qryBuddyDB)[5]->AsString,            // strProfileName
				QF(qryBuddyDB)[6]->AsDateTime);

		QNEXT(qryBuddyDB);
		while (!QEOF(qryBuddyDB) && QF(qryBuddyDB)[0]->AsString == strCurBarcode) {
			TheWorklistItem->strTestName += ", "+QF(qryBuddyDB)[1]->AsString;
			QNEXT(qryBuddyDB);
		}

		vecpWorklistTotal.push_back(TheWorklistItem);
	}
	QCLOSE(qryBuddyDB);

	std::sort(vecpWorklistTotal.begin(), vecpWorklistTotal.end(), foWorklistItemLessTDT());
	fnDisplayWorklist();
}

void TfrmFront::fnDisplayWorklist(void)
{
	tdvecpWorklistItem::const_iterator it;

	pWorklistItem TheWorklistItem;

    int iBarcodeMaxLen = 12;
    int iTestMaxLen = 20;
    int iProjectMaxLen = 15;
    int iProfileMaxLen = 15;

    for (it = vecpWorklistTotal.begin(); it != vecpWorklistTotal.end(); it++) {
        TheWorklistItem = *it;

		if (TheWorklistItem->strTestName.Length() > iTestMaxLen)
            iTestMaxLen = TheWorklistItem->strTestName.Length();

        if (TheWorklistItem->strBarcode.Length() > iBarcodeMaxLen)
            iBarcodeMaxLen = TheWorklistItem->strBarcode.Length();

        if (TheWorklistItem->strProjectName.Length() > iProjectMaxLen)
            iProjectMaxLen = TheWorklistItem->strProjectName.Length();

        if (TheWorklistItem->strProfileName.Length() > iProfileMaxLen)
            iProfileMaxLen = TheWorklistItem->strProfileName.Length();

    }

    ListBox1->Clear();

    /*String strLegend =
        "Barcode" + AnsiString::StringOfChar(' ', iBarcodeMaxLen-7)
        + "  Test" + AnsiString::StringOfChar(' ', iTestMaxLen-4)
        + "  Project              Profile               Timestamp"; */

    String strFormat, strLegend, strLine;

    strFormat.sprintf(
        "%%-%ds  %%-%ds  %%-%ds  %%-%ds  %%-20s",
        iBarcodeMaxLen,
        iTestMaxLen,
        iProjectMaxLen,
        iProfileMaxLen
        );

    strLegend.sprintf(
        strFormat.c_str(),
        "Barcode",
        "Test",
        "Project",
        "Profile",
        "Timestamp");

    ListBox1->Items->Add(strLegend);

    int count = 0;
    for (it = vecpWorklistTotal.begin(); it != vecpWorklistTotal.end(); it++) {
        TheWorklistItem = *it;

        strLine.sprintf(
                strFormat.c_str(),
                //TheWorklistItem->iMachineID,
                TheWorklistItem->strBarcode,
                TheWorklistItem->strTestName,
                //TheWorklistItem->iTestID,
                //DbProject::lookup(TheWorklistItem->iProjectID).getName(),
                TheWorklistItem->strProjectName,
                TheWorklistItem->strProfileName,
                TheWorklistItem->tdtTimeStamp.DateTimeString()
        );

        ListBox1->Items->Add(strLine);
        count++;
    }

    StatusBar1->SimpleText = String(count) + " barcodes retrieved";
    if (count >= 1) bGotWorklist = true;
}

void TfrmFront::fnDeleteWorklistTotal(void)
{
    tdvecpWorklistItem::const_iterator it;

    for (it = vecpWorklistTotal.begin(); it != vecpWorklistTotal.end(); it++) {
        delete *it;
    }

    vecpWorklistTotal.clear();
}

void TfrmFront::fnDeleteWorklistSelection(void)
{
    tdvecpWorklistItem::const_iterator it;

//    for (it = vecpWorklistSelection.begin(); it != vecpWorklistSelection.end(); it++) {
//        delete *it;
//    }

    vecpWorklistSelection.clear();
}

void TfrmFront::fnDeleteWorklists(void)
{
    fnDeleteWorklistTotal();
	fnDeleteWorklistSelection();
}

void __fastcall TfrmFront::FormDestroy(TObject *Sender)
{
	fnDeleteWorklists();
	fnDeleteEmpowerOutput();

	delete tslResults;
}
//---------------------------------------------------------------------------


void TfrmFront::fnLoadWorklistSelection(void)
{
}

void __fastcall TfrmFront::btnCreateSampleSelectionClick(TObject *Sender)
{
	if (0 == ListBox1->Items->Count || !bGotWorklist) {
		APPMSG("No worklist entries have been loaded!", "No entries loaded", MB_OK);
		return;
	}

	ListBox1->Selected[0] = false;

	if (0 >= ListBox1->SelCount) {
		// (when MultiSelect enabled)
		APPMSG("No worklist entries have been selected!", "No entries selected", MB_OK);
		return;
	}

	tdvecpWorklistItem::const_iterator it;
	pWorklistItem TheWorklistItem;

	fnDeleteWorklistSelection();

	for (int idx=0; idx < ListBox1->Items->Count; idx++) {
		if (ListBox1->Selected[idx]) {
			TheWorklistItem = vecpWorklistTotal[idx-1];
			vecpWorklistSelection.push_back(TheWorklistItem);
		}
	}

	String strLine;
	int idx;

	frmClipboard->btnCopy->Enabled = true;
	frmClipboard->Memo1->Clear();

		DWORD   uiTabStops[] =    { 4 * 8,     //"",
									4 * 16,      //  "Vial",
									4 * 30,     //   "SampleName",
									4 * 40,      //  "Level",
									4 * 50,      //  "Inj Vol",
									4 * 60,     //   "# of Injs",
									4 * 80,      //  "Function",
									4 * 110,     //   "Processing",
									4 * 140,    // "Method Set / Report Method",
									4 * 150,      //  "Run Time",
									4 * 170,     //   "Next Inj Delay",
									4 * 190,     //   "SampleWeight",
									4 * 200,      //  "Dilution",
									4 * 220     //   "Auto Additions");
								  };
		UINT    lResult;
		HWND    hWndControl = frmClipboard->Memo1->Handle;
		WPARAM  wParam = 14;
		LPARAM  lParam = (LPARAM)uiTabStops;

		lResult = SendMessage(
		   hWndControl,      // handle to destination control
		   EM_SETTABSTOPS,   // message ID
		   wParam,
		   lParam
		);

		// or:
		// Memo1->Perform(EM_SETTABSTOPS, 1, (int)TabStops);

	strLine.sprintf(
		"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s",
			"",
			"Vial",
			"SampleName",
			"Level",
			"Inj Vol",
			"# of Injs",
			"Function",
			"Processing",
			"Method Set / Report Method",
			"Run Time",
			"Next Inj Delay",
			"SampleWeight",
			"Dilution",
			"Auto Additions");

	frmClipboard->Memo1->Lines->Add(strLine);

	for (idx = 1, it = vecpWorklistSelection.begin(); it != vecpWorklistSelection.end(); it++, idx++) {
		TheWorklistItem = *it;

		strLine.sprintf(
			//n  v   sam lev inj #in fun prc mth run del wgt dil add
			"%d\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s",
				idx,
				idx,
				TheWorklistItem->strBarcode,
				"",
				"10.00",
				"1",
				"Inject Samples",
				"Don't Process or Report",
				"",
				"10.00",
				"0.00",
				"1.00",
				"1.00",
				"");

		frmClipboard->Memo1->Lines->Add(strLine);
	}

	if (mrOk == frmClipboard->ShowModal())
		StatusBar1->SimpleText = "";
}

//---------------------------------------------------------------------------

void __fastcall TfrmFront::BitBtnExitClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void TfrmFront::fnDeleteEmpowerOutput(void)
{
    tdvecpEmpowerOutputRow::const_iterator it;

    for (it = vecpEmpowerOutputRows.begin(); it != vecpEmpowerOutputRows.end(); it++) {
        delete *it;
    }
    vecpEmpowerOutputRows.clear();
}


void __fastcall TfrmFront::FormCreate(TObject *Sender)
{
    tslResults = new TStringList;
}
//---------------------------------------------------------------------------

void __fastcall TfrmFront::btnCopyResultsClick(TObject *Sender)
{
    Clipboard()->SetTextBuf(ListBox1->Items->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmFront::btnEnterResultsClick(TObject *Sender)
{
    frmEnterResults->ShowModal();        
}
//---------------------------------------------------------------------------

