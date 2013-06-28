//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormCutNPaste.h"
#include "FormFront.h"
#include "FormUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"                  
TfrmClipboard *frmClipboard;

extern tdvecpEmpowerOutputRow vecpEmpowerOutputRows;

//---------------------------------------------------------------------------
__fastcall TfrmClipboard::TfrmClipboard(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmClipboard::Button1Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TfrmClipboard::btnCopyClick(TObject *Sender)
{
    Memo1->SelectAll();
    Memo1->CopyToClipboard();
    btnCopy->Enabled = false;
    Memo1->Clear();
}
//---------------------------------------------------------------------------

