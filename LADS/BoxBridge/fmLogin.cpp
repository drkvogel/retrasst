//---------------------------------------------------------------------------

#include <vcl.h>
#include <inifiles.hpp>
#include <stdlib.h>
#include "LCDbOperator.h"
#include "LIMSParams.h"
#include "LCDbAnalyser.h"
#include "LCDbProject.h"
#include "LCDbObject.h"
#include "TfrmPassword.h"
#include "TfrmAboutBox.h"
#include "frmCommon.h"

#pragma hdrstop

#include "fmLogin.h"
#include "fmMain.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "TfrmLoginBase"
#pragma link "TfrmLoginBase"
#pragma resource "*.dfm"

TfrmLoginBoxBridge *frmLoginBoxBridge;

//---------------------------------------------------------------------------

__fastcall TfrmLoginBoxBridge::TfrmLoginBoxBridge(TComponent* Owner)
	: TfrmLoginBase(Owner)
{}

//---------------------------------------------------------------------------
//  Update shared data from the database and populate lists
//---------------------------------------------------------------------------

void __fastcall TfrmLoginBoxBridge::initialise(TObject *Sender)
{
	TfrmLoginBase::initialise( Sender );		// added: NG, 24/03/09
	Caption = Application -> Title;
}

//---------------------------------------------------------------------------
//  Log operator into selected project; force new password if necessary
//---------------------------------------------------------------------------

void __fastcall TfrmLoginBoxBridge::okButtonClick(TObject *Sender)
{
	TfrmLoginBase::okButtonClick( Sender );		// added: NG, 24/03/09
	LCDbProjects::records().releaseDatabases();
/*
	const char * warning = "Any actions performed whilst logged on to this application will be signed"
		" with your electronic signature; by entering your user name and password you acknowedge that"
		" you have reviewed the data and consent to your electronic signature being applied to the data.";
    if (LCDbProjects::LIVE_DATA == TESTING_STATUS) {
	    if( Application->MessageBox( warning, "Warning", MB_OKCANCEL ) != IDOK )
		    return;
    }
*/
}

//---------------------------------------------------------------------------
//	open the main form: implements TFrmLoginBase::runProgram()
//---------------------------------------------------------------------------

void TfrmLoginBoxBridge::runProgram( const String & title )
{
	frmMain -> Caption = title;
	frmMain -> ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TfrmLoginBoxBridge::FormClose(TObject *Sender, TCloseAction &Action)
{
	timer -> Enabled = false;
	if( auditTrail.getID() != 0 )
		auditTrail.stop();
	//LCDbProjects::releaseDatabases();
}

//---------------------------------------------------------------------------

