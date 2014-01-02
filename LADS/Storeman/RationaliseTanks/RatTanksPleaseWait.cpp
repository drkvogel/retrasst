//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RatTanksPleaseWait.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmRatTankPleaseWait *frmRatTankPleaseWait;
//---------------------------------------------------------------------------
__fastcall TfrmRatTankPleaseWait::TfrmRatTankPleaseWait(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmRatTankPleaseWait::FormShow(TObject *Sender)
{
	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTankPleaseWait::FormHide(TObject *Sender)
{
	Timer1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTankPleaseWait::Timer1Timer(TObject *Sender)
{
	this->SetFocus();
	return;
}
//---------------------------------------------------------------------------

