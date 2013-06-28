//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fmReport.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmReport *frmReport;
//---------------------------------------------------------------------------
__fastcall TfrmReport::TfrmReport(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmReport::btnCloseClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------


void __fastcall TfrmReport::Copytoclipboard1Click(TObject *Sender)
{
    memoReport->SelectAll();
    memoReport->CopyToClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TfrmReport::Exit1Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TfrmReport::SaveAs1Click(TObject *Sender)
{
    if (SaveDialog1->Execute()) {
        memoReport->Lines->SaveToFile(SaveDialog1->FileName);                
    }
}
//---------------------------------------------------------------------------

