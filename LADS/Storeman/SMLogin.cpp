//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SMLogin.h"
#include "Storemain.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "TfrmLoginBase"
#pragma resource "*.dfm"

TfrmLogin *frmLogin;

//---------------------------------------------------------------------------

__fastcall TfrmLogin::TfrmLogin(TComponent* Owner) : TfrmLoginBase(Owner)
{}

//---------------------------------------------------------------------------

void TfrmLogin::runProgram( const std::string & title )
{
	frmStoremain -> init();
	frmStoremain -> Caption = title.c_str();
	frmStoremain -> ShowModal();
}

//---------------------------------------------------------------------------






