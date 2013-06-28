//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormEnterResults.h"
#include "FormCommon.h"
#include "FormUtils.h"
#include "FormFront.h"
#include "LCDbTest.h"
#include "LDbResult.h"
#include "LCDbAnalyser.h"
#include "LDbWorklist.h"
#include "LIMSResult.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmEnterResults *frmEnterResults;

extern TStringList* tslResults;
extern tdvecpEmpowerOutputRow  vecpEmpowerOutputRows;

//---------------------------------------------------------------------------
__fastcall TfrmEnterResults::TfrmEnterResults(TComponent* Owner)
    : TForm(Owner)
{
}

//---------------------------------------------------------------------------


int TfrmEnterResults::fnProcessResults(TStringList* tsl)
{
    frmFront->fnDeleteEmpowerOutput();

    if (tsl->Text.IsEmpty()) {
        APPMSG("File is empty", "File is empty", MB_OK);
        return false;
    }

    int count = 0;

	for( int idx = 1; idx < tsl->Count; idx++ )
	{
		String record = tsl->Strings[ idx ];
		if (!record.IsEmpty()
		 && record.Pos( "Component Results" ) == 0
		 && record.Pos( "'#','SampleName'" ) == 0 )
		{
            if (!fnParseLine(record))
                return -1;
            else
                count++;
        }
    }

    return count;
}

bool TfrmEnterResults::fnParseLine(String strLine)
{
    pEmpowerOutputRow TheEmpowerOutputRow;
    char *szTok, *szLine;

    // cheap test to see if input is valid:
    szLine = strLine.c_str();
    int count = 0;
    while (*szLine != '\0')
        if (*szLine++ == ',') count++;
    if (count < 5) return false;

    try {
        TheEmpowerOutputRow = new EmpowerOutputRow;

        TokenString tsLine(strLine, ",");
        /*if (IDYES == APPMSG("tsLine: "+String(tsLine), "", MB_YESNO)) {
            String temp;
            temp = "Barcode: "+tsLine.First();
            temp += "Test: "+tsLine.Next();
            temp += "Conc: "+tsLine.Next();
            temp += "Height: "+tsLine.Next();
            temp += "Date: "+tsLine.Next();
            temp += "Vial: "+tsLine.Next();
            APPMSG("Result:\n"+temp, "", MB_OK);
        } */

        String strTok;
        // # <row number>
        strTok = tsLine.First();
        if (strTok.IsEmpty()) throw Exception("Missing row number");
        TheEmpowerOutputRow->iRow = atoi(strTok.c_str());

        // SampleName <sample barcode>
        strTok = tsLine.Next();
        if (strTok.IsEmpty()) throw Exception("Missing barcode");
        TheEmpowerOutputRow->strBarcode = fnTrimQuotes(strTok);

        // Name <test ID>
        strTok = tsLine.Next();
        if (strTok.IsEmpty()) throw Exception("Missing test ID");
        TheEmpowerOutputRow->strTestID = fnTrimQuotes(strTok);

        // Concentration <result value>
        strTok = tsLine.Next();
        if (strTok.IsEmpty())
            TheEmpowerOutputRow->flResultValue = -1;
        else
            TheEmpowerOutputRow->flResultValue = atof(strTok.c_str());

        // Area/Height <peak area/height of component>
        strTok = tsLine.Next();
        if (strTok.IsEmpty())
            TheEmpowerOutputRow->flAreaHeight = -1;
        else
            TheEmpowerOutputRow->flAreaHeight = atof(strTok.c_str());

        // Date Acquired <date of analysis>
        strTok = tsLine.Next();
        if (strTok.IsEmpty()) throw Exception("Missing date");
        TheEmpowerOutputRow->tdtDateOfAnalysis =
            TDateTime::TDateTime(fnTrimQuotes(strTok), TDateTime::DateTime);

        // Vial <vial number when analysed>
        strTok = tsLine.Next();
        if (strTok.IsEmpty()) throw Exception("Missing vial number");
        TheEmpowerOutputRow->iVialNum = atoi(fnTrimQuotes(strTok).c_str());

        //APPMSG("Result: "+String(*TheEmpowerOutputRow), "", MB_OK);

		vecpEmpowerOutputRows.push_back(TheEmpowerOutputRow);
    } catch (Exception &e) {
        APPMSG(e.Message, "Error", MB_OK);
        delete TheEmpowerOutputRow;
        return false;
    }

    return true;
}

String TfrmEnterResults::fnTrimQuotes(String str)
{
    return str.SubString(2, str.Length()-2);
}

void TfrmEnterResults::fnGetResults(void)
{
    tdvecpEmpowerOutputRow::const_iterator it;
    pEmpowerOutputRow pResult;

    tslResults->LoadFromFile(OpenDialog4->FileName);
    int nresults = fnProcessResults(tslResults);
    if (-1 == nresults) {
        APPMSG("Problem with analyser data, contact Core Programming", "Error", MB_OK);
        return;
    } else if (0 == nresults) {
        APPMSG("No results returned", "No results", MB_OK);
        return;
    }

    frmFront->bGotWorklist = false;
    frmFront->ListBox1->Clear();
    frmFront->ListBox1->Items->Add("Results added:");
    frmFront->ListBox1->Items->Add("");
    frmFront->ListBox1->Items->Add("Vial   Barcode          Test          Result  Area or Height  Timestamp");

	bool problem;
	bool therewereerrors = false;
	for (it = vecpEmpowerOutputRows.begin(); it != vecpEmpowerOutputRows.end(); it++) {
		pResult = *it;
		problem = false;

		String strError;
		int testID = LCDbTests::records().findTestID( pResult->strTestID );
		if( testID == 0 ) {
			static bool testnotfound = false;
			if (!testnotfound) {
				APPMSG( "Mapping for test '"+pResult->strTestID+"' not found, contact Core Programming"
						, "Error", MB_OK);
			}
			strError = "Test '"+pResult->strTestID+"' not found: ";
			testnotfound = true;
			problem = true;
			therewereerrors = true;
		}

		if (pResult->strBarcode.IsEmpty()) {
			static bool emptybarcode = false;
			if (!emptybarcode) {
				APPMSG( "Barcode not found, contact Core Programming"
						, "Error", MB_OK);
			}
			strError += "Barcode empty: ";
			emptybarcode = true;
			problem = true;
			therewereerrors = true;
		}

		String strOutput;
		if (problem) {
			strOutput = "Problem: "+ strError;
		} else {
			try { 	LIMSResult result( pResult->strBarcode,
										pResult->tdtDateOfAnalysis );
					result.setTestResult( testID,
										pResult->tdtDateOfAnalysis,
										pResult->flResultValue );
				result.updateWorklist( frmCommon->qryBuddyDB,
									   frmCommon->qryLabCentral );
				result.saveRecord( frmCommon->qryBuddyDB );

			} catch (Exception &e) {
				strOutput = "Problem: "+ e.Message;
			}
		}

        // "Vial Barcode          Test        Result    Area or height  Timestamp"
        String strTemp;
        strTemp.sprintf(
            "%4d   %-16s %-10s  %8.3f  %10.1f      %-20s",
            pResult->iVialNum,
            pResult->strBarcode,
            pResult->strTestID,
            pResult->flResultValue,
            pResult->flAreaHeight,
            pResult->tdtDateOfAnalysis );

        strOutput += strTemp;

        frmFront->ListBox1->Items->Add(strOutput);
	}
    frmFront->StatusBar1->SimpleText = String(nresults) + " results added";

    if (therewereerrors) {
        APPMSG( "There were errors - contact Core Programming"
                , "Error", MB_OK);
    }
}

void __fastcall TfrmEnterResults::FormActivate(TObject *Sender)
{
	edtFilename->Text = "";
	cbAnalysers->Items->Text = CBANALYSER_DEFAULT;
	cbAnalysers->ItemIndex = 0;

	const LCDbAnalysers & machines = LCDbAnalysers::records();
	std::set< int > cluster = machines.getAnalyserIDs();
	for( Range< LCDbAnalyser > ar = machines; ar.isValid(); ++ar )
		if( cluster.count( ar -> getID() ) != 0 )
			cbAnalysers->Items->Add( ar->getName() );
}

//---------------------------------------------------------------------------
void __fastcall TfrmEnterResults::btnBrowseClick(TObject *Sender)
{
    OpenDialog4->Execute();
    edtFilename->Text = OpenDialog4->FileName;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnterResults::btnOKClick(TObject *Sender)
{
    if (edtFilename->Text.IsEmpty()) {
        APPMSG( "Please choose an Empower(tm) output file"
                , "No output file selected", MB_OK);
        ModalResult = mrNone;
        return;
    }
	if (!FileExists(edtFilename->Text)) {
        APPMSG( "Invalid Empower(tm) output file specification"
                , "Invalid output file", MB_OK);
        ModalResult = mrNone;
        return;
    }

    if (cbAnalysers->Text.IsEmpty() || cbAnalysers->Text == CBANALYSER_DEFAULT
        && !MYDEBUG) {
        APPMSG( "Please choose a source analyser"
                , "No source analyser selected", MB_OK);
        ModalResult = mrNone;
        return;
    }

	LCDbAnalysers & analysers = LCDbAnalysers::records();
	const LCDbAnalyser * machine = analysers.findByName(cbAnalysers->Text);
	if (machine != NULL) {
		analysers.setCurrent( *machine );
		LCDbTests::records().read(frmCommon->qryLabCentral,false);
	} else {
		APPMSG( "Error - machine not recognised - contact Core Programming"
                , "Machine not recognised", MB_OK);
        ModalResult = mrNone;
        if (!MYDEBUG) return;
    }

    fnGetResults();     
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
