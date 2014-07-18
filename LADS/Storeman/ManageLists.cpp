#include <vcl.h>
#pragma hdrstop

#include "ManageLists.h"
#include "ManageListsShowList.h"

#pragma package(smart_init)
#pragma link "RetrievalListBase"
#pragma resource "*.dfm"
TfrmManageLists *frmManageLists;

__fastcall TfrmManageLists::TfrmManageLists(TComponent* Owner) : TfrmRetrievalJobList(Owner) { }

void __fastcall TfrmManageLists::FormCreate(TObject *Sender) {
//    cbNewJob->          Checked = false;
//    cbInProgress->      Checked = false;
//    cbDone->            Checked = true;
//    cbDeleted->         Checked = false;
//    cbRejected->        Checked = true;
//    cbBoxRetrieval->    Checked = true;
//    cbSampleRetrieval-> Checked = true;
//    cbBoxMove->         Checked = true;
//    cbBoxDiscard->      Checked = true;
//    cbSampleDiscard->   Checked = true;
}

void __fastcall TfrmManageLists::sgJobsDblClick(TObject *Sender) {
    // override
    frmManListsShow->ShowModal();
    loadJobs();
}



