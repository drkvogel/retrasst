//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SMLogin.h"
#include "Storemain.h"
#include "LIMSParams.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "TfrmLoginBase"
#pragma resource "*.dfm"

TfrmSMLogin *frmSMLogin;

//---------------------------------------------------------------------------

__fastcall TfrmSMLogin::TfrmSMLogin(TComponent* Owner) : TfrmLoginBase(Owner)
{
	std::string ver = LIMSParams::instance().getProgVersion();
	String app = Application -> Title + " " + ver.c_str();
	version -> Caption = app + " ** DEMO **";
	rgDatabase -> ItemIndex = LIMSDatabase::MIRROR_SYSTEM;
}

//---------------------------------------------------------------------------

void TfrmSMLogin::runProgram( const std::string & title )
{
    //frmLoginBase->rgDatabase -> ItemIndex = LIMSDatabase::MIRROR_SYSTEM; 	// db v2.7.3 - OK
	frmStoremain -> Caption = title.c_str();
	frmStoremain -> ShowModal();
}

//---------------------------------------------------------------------------






