#include <vcl.h>
#pragma hdrstop
#include "RetrievalProcess.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmProcess *frmProcess;

__fastcall TfrmProcess::TfrmProcess(TComponent* Owner) : TForm(Owner) { }

void __fastcall TfrmProcess::FormCreate(TObject *Sender) {
    //
}

void __fastcall TfrmProcess::FormShow(TObject *Sender) {
    //
}

void __fastcall TfrmProcess::Exit1Click(TObject *Sender) {
    if (IDYES == Application->MessageBox(L"Are you sure you want to exit?\n\nCurrent progress will be saved.", L"Question", MB_YESNO)) {
        // save stuff
        Close();
    }
}

