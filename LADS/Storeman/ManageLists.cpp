#include <vcl.h>
#pragma hdrstop

#include "ManageLists.h"
#include "ManageListsShowList.h"

#pragma package(smart_init)
#pragma link "RetrievalListBase"
#pragma resource "*.dfm"
TfrmManageLists *frmManageLists;

__fastcall TfrmManageLists::TfrmManageLists(TComponent* Owner) : TfrmRetrievalJobList(Owner) { }

void __fastcall TfrmManageLists::sgJobsDblClick(TObject *Sender) {
    // override
    frmManListsShow->ShowModal();
}

