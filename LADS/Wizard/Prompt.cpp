//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Prompt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmPrompt *frmPrompt;
//---------------------------------------------------------------------------
__fastcall TfrmPrompt::TfrmPrompt(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrompt::FormActivate(TObject *Sender)
{
    lblPrompt->Caption = prompt;
    edtValue->Text = value;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrompt::btnCloseClick(TObject *Sender)
{
    value = edtValue->Text;
    Close();
}
//---------------------------------------------------------------------------
