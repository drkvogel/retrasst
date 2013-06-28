//---------------------------------------------------------------------------

#include "Login1.h"
#include "EditCentralDb1.h"
#include "EditProjectDb1.h"
#include "LCDbProject.h"
#include "WizardLogin1.h"
#include "LDbBoxType.h"
#include "LPDbProfile.h"
#include "LPDbDescriptor.h"
#include "LPDbProfileMap.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TLogin *Login;

//---------------------------------------------------------------------------

__fastcall TLogin::TLogin(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TLogin::FormActivate(TObject *Sender)
{
	ProjectPanel -> Visible = false;
}

//---------------------------------------------------------------------------

void __fastcall TLogin::BtnCentralDbClick(TObject *Sender)
{
	EditCentralDb->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TLogin::BtnProjectDbClick(TObject *Sender)
{
	cbProjectName -> Clear();
	for( LCDbProjects::Range pl = LCDbProjects::records(); pl.isValid(); ++ pl )
		if( pl -> getID() != 0 && pl -> isActive() )
			cbProjectName -> Items -> Add( pl -> getName() );

	ProjectPanel -> Visible = true;
	cbProjectName -> SetFocus();
}

//---------------------------------------------------------------------------


String TLogin::setProjectCaption( const String & formName ) const
{
	const LCDbProject & proj = LCDbProjects::records().get( LCDbProjects::getCurrentID() );
	return formName + " for " + WizardLogin -> setCaption( proj.getName() );
}

//---------------------------------------------------------------------------

void __fastcall TLogin::Button1Click(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TLogin::cbProjectNameChange(TObject *Sender)
{
	const LCDbProject * proj = LCDbProjects::records().find( cbProjectName -> Text );
	if( proj != NULL )
	{
		proj -> claimDatabase( pDatabase );
		LCDbProject::changeDbName( dDatabase, proj -> getDbName() );

		LPDbBoxTypes::records().read( QueryProject );
		LPDbDescriptors::records().read( QueryProject );
		LPDbProfiles::records().read( QueryProject );
		LPDbProfileMaps::records().read( QueryProject );
		ProjectPanel -> Visible = false;
		EditProjectDb->ShowModal();
//		LCDbProjects::releaseDatabases();
	}
}

//---------------------------------------------------------------------------

