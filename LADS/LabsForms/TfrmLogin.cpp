//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TfrmLogin.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "TfrmLoginBase"
#pragma resource "*.dfm"

TfrmLogin *frmLogin;

//---------------------------------------------------------------------------

__fastcall TfrmLogin::TfrmLogin(TComponent* Owner)
	: TfrmLoginBase(Owner)
{}

//---------------------------------------------------------------------------
//	List the users (in base class) and projects to select from
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::initialise(TObject *Sender)
{
	TfrmLoginBase::initialise( Sender );

	//	list active projects in the current system, if available
	projName -> Clear();
	for( Range< LCDbProject > pl = LCDbProjects::records(); pl.isValid(); ++ pl )
		if( pl -> isActive() && !pl -> isCentral() )
			projName -> Items -> Add( pl -> getName() );
	projName -> Enabled = (projName -> Items -> Count != 0);
}

//---------------------------------------------------------------------------
//	Claim the selected project before loggin in; release it on exit
//---------------------------------------------------------------------------

static const char * WARNING_MESSAGE = "Any actions performed whilst logged on to this application will be signed"
		" with your electronic signature; by entering your user name and password you acknowedge that"
		" you have reviewed the data and consent to your electronic signature being applied to the data.";

void __fastcall TfrmLogin::okButtonClick(TObject *Sender)
{
	const LCDbProject * proj = LCDbProjects::records().findByName( projName -> Text );
	if( proj == NULL )
		Application -> MessageBox( "Please select a project", NULL, MB_ICONWARNING );

	else if( logUserIn() != NULL )
	{
		proj -> claim( centralDb, projectDb );
		if( Application -> MessageBox( WARNING_MESSAGE, "Warning", MB_OKCANCEL ) == IDOK )
			startProgram( Sender );
		proj -> release();
	}
}

//---------------------------------------------------------------------------

