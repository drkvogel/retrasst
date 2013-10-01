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

__fastcall TfrmSMLogin::TfrmSMLogin(TComponent* Owner) : TfrmLoginBase(Owner) {
	//frmLoginBase->rgDatabase -> ItemIndex = LIMSDatabase::MIRROR_SYSTEM; 	// db v2.7.3 - OK
	//frmLoginBase->version -> Font -> Color = clRed;
	//frmLoginBase->version -> Caption = app + " **DEBUG**";
}

//---------------------------------------------------------------------------

void TfrmSMLogin::runProgram( const std::string & title )
{
    //frmLoginBase->rgDatabase -> ItemIndex = LIMSDatabase::MIRROR_SYSTEM; 	// db v2.7.3 - OK
	frmStoremain -> Caption = title.c_str();
	frmStoremain -> ShowModal();
}

//---------------------------------------------------------------------------






