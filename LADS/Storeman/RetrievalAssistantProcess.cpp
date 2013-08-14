#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistantProcess.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmProcess *frmProcess;

__fastcall TfrmProcess::TfrmProcess(TComponent* Owner) : TForm(Owner) { }

void __fastcall TfrmProcess::FormCreate(TObject *Sender) {
//    sgRetrieval->ColCount = SGRETRIEVAL_NUMCOLS;
//    for (int i=0; i<SGRETRIEVAL_NUMCOLS; i++) {
//        sgRetrieval->Cells[i][0]    = sgRetrievalColName[i];
//        sgRetrieval->ColWidths[i]   = sgRetrievalColWidth[i];
//    }
}

void __fastcall TfrmProcess::FormShow(TObject *Sender) {
    //
}




/*
 * Work through list or sub-section by giving the storage location and sample ID of each sample on the list in the order saved above (REQ 8.3.8);
 * As each sample is retrieved its barcode should be scanned, if the scanned barcode matches that on the list
the destination location should be displayed and the next ID/location should be displayed (REQ 8.3.9)
 * if the ID’s do not match a warning should be displayed and re-entry of the barcode required (REQ 8.3.10).
 * When working through the list the previous five successfully entered ID’s should always be visible (REQ 8.3.11).
 * The option to exit the process saving progress should be offered, with an “are you sure?” message in case of accidental selection (REQ 8.3.12).
*/

void __fastcall TfrmProcess::Exit1Click(TObject *Sender) {
    if (IDYES == Application->MessageBox(L"Are you sure you want to exit?\n\nCurrent progress will be saved.", L"Question", MB_YESNO)) {
        // save stuff
        Close();
    }
}

