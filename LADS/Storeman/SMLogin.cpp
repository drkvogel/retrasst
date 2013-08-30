//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SMLogin.h"
#include "Storemain.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "TfrmLoginBase"
#pragma resource "*.dfm"

TfrmSMLogin *frmSMLogin;

//---------------------------------------------------------------------------

__fastcall TfrmSMLogin::TfrmSMLogin(TComponent* Owner) : TfrmLoginBase(Owner)
{}

//---------------------------------------------------------------------------

void TfrmSMLogin::runProgram( const std::string & title )
{
	frmStoremain -> init();
	frmStoremain -> Caption = title.c_str();
	frmStoremain -> ShowModal();
}

//---------------------------------------------------------------------------






