//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ModifiedLogin.h"
#include "DbAnalyser.h"

#pragma package(smart_init)
#pragma link "TfrmLogin"
#pragma link "TfrmB2ALogin"
#pragma resource "*.dfm"

TfrmLogin1 *frmLogin1;

//---------------------------------------------------------------------------

__fastcall TfrmLogin1::TfrmLogin1(TComponent* Owner) : TfrmLogin(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TfrmLogin1::analysersDropDown(TObject *Sender)
{
	DbAnalyser::Range machine = DbAnalyser::getAll();
	for( analysers -> Clear(); machine.isValid(); ++ machine )
		analysers -> Items -> Add( machine -> getName() );
}

//---------------------------------------------------------------------------

void __fastcall TfrmLogin1::initialise(TObject *Sender)
{
	TfrmLogin::initialise( Sender );
	Label4 -> Visible = true;
	analysers -> Enabled = true;
}

//---------------------------------------------------------------------------

