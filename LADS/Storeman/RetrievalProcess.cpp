#include <vcl.h>
#pragma hdrstop
#include "RetrievalProcess.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmRetrievalProcess *frmRetrievalProcess;

__fastcall TfrmRetrievalProcess::TfrmRetrievalProcess(TComponent* Owner) : TForm(Owner) { }

void __fastcall TfrmRetrievalProcess::FormCreate(TObject *Sender) {
    //
}

void __fastcall TfrmRetrievalProcess::FormShow(TObject *Sender) {
    //
}

void __fastcall TfrmRetrievalProcess::Exit1Click(TObject *Sender) {
    if (IDYES == Application->MessageBox(L"Are you sure you want to exit?\n\nCurrent progress will be saved.", L"Question", MB_YESNO)) {
        // save stuff
        Close();
    }
}

