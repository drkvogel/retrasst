//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AboutProgram1.h"
#include "DbAuditTrail.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//---------------------------------------------------------------------------
__fastcall TAboutBox::TAboutBox(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::btnOKClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::FormActivate(TObject *Sender)
{
    AnsiString strDate = __DATE__;
    AnsiString strTime = __TIME__;
    AnsiString String = "Created:  " + strDate  + "  " + strTime;
    Label3->Caption = String;
    lblVersion->Caption = DbAuditTrail::getProgVersion();
}
//---------------------------------------------------------------------------
