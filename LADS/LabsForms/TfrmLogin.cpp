// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TfrmLogin.h"
#include "lease.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "TfrmLoginBase"
#pragma resource "*.dfm"

TfrmLogin *frmLogin;

// ---------------------------------------------------------------------------

__fastcall TfrmLogin::TfrmLogin( TComponent* Owner ) : TfrmLoginBase( Owner ), lm( NULL ) {
}

// ---------------------------------------------------------------------------
// Claim the selected project before logging in; release it on exit
// ---------------------------------------------------------------------------

static const String WARNING_MESSAGE =
	"Any actions performed whilst logged on to this application will be" " signed with your electronic signature; by entering your user name and password you acknowedge that"
	" you have reviewed the data and consent to your electronic signature being applied to the data.";

void __fastcall TfrmLogin::okButtonClick( TObject *Sender ) {
	AnsiString name = projName->Text.Trim( );
	LCDbProjects & projList = LCDbProjects::records( );
	const LCDbProject * proj = projList.findByName( name.c_str( ) );
	if( proj == NULL ) {
		Application->MessageBox( L"Please select a project", NULL, MB_ICONWARNING );
	} else if( logUserIn( ) != NULL ) {
		if( lm == NULL ) {
			AnsiString program = Application->Title;
			lm = new TLeaseManager( program.c_str( ) );
		}
		std::string result;
		if( lm->Activate( proj->getID( ), LCDbProjects::leaseID, LEASE_TIME_DEFAULT, result ) != 0 )
		{
			if( result.empty( ) ) {
				throw new Exception( "Cannot claim lease on project database" );
			} else {
				std::string problem = "Waiting for " + result;
				lm->Deactivate( proj->getID( ), LCDbProjects::leaseID );
				throw new Exception( problem.c_str( ) );
			}
		}
		if( Application->MessageBox( WARNING_MESSAGE.c_str( ), L"Warning", MB_OKCANCEL ) == IDOK ) {
			projList.setCurrent( proj );
			startProgram( Sender );
			lm->Deactivate( proj->getID( ), LCDbProjects::leaseID );
		}
	}
}

// ---------------------------------------------------------------------------
// update project list in case the database system has changed
// ---------------------------------------------------------------------------

void __fastcall TfrmLogin::projNameDropDown( TObject *Sender ) {
	projName->Clear( );
	for( const LCDbProject & pl : LCDbProjects::records( ) ) {
		if( pl.isActive( ) && !pl.isCentral( ) ) {
			projName->Items->Add( pl.getName( ).c_str( ) );
		}
	}
	projName->Enabled = ( projName->Items->Count != 0 );
}

// ---------------------------------------------------------------------------
