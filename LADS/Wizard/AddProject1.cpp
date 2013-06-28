//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AddProject1.h"
#include "EditProjectDb1.h"
#include "LCDbProject.h"
#include "Project1.h"
#include "CreateProjectDatabase1.h"
#include "WizardLogin1.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TAddProject *AddProject;

//---------------------------------------------------------------------------

__fastcall TAddProject::TAddProject(TComponent* Owner)
 : TForm( Owner ), projectID( LCDbProject::NONE_SELECTED )
{}

//---------------------------------------------------------------------------

void __fastcall TAddProject::FormActivate(TObject *Sender)
{
	btnCreateDatabase->Enabled = false;
	const LCDbProject * theProject = LCDbProjects::records().findByID( projectID );
	if( theProject == NULL )
	{
		Caption = "Add " + WizardLogin -> setCaption( "Project" );
		ProjectName->Text = "";
		ProjectName->Enabled = true;
		FullName->Text = "";

		dtpFrom -> DateTime = TDateTime::CurrentDate();
		dtpTo -> DateTime = Now() + 3652; 		// Add 10 years by default

		ProjectDatabase->Text = FindNextDatabaseName();
		ProjectDatabase->Enabled = true;
		memURL->Text = "";
		rgStatus->ItemIndex = RGS_LIVE;
		edtDbVersion->Text = "2.3";
	}
	else
	{	Caption = "Change " + WizardLogin -> setCaption( "Project" );

		ProjectName->Text = theProject -> getName();
		ProjectName->Enabled = false;
		FullName->Text = theProject -> getDescription();

		std::pair< TDate, TDate > dates = theProject -> getValidDates();
		dtpFrom->DateTime = dates.first;
		dtpTo->DateTime = dates.second;

		ProjectDatabase->Text = theProject -> getDbName();
		ProjectDatabase->Enabled = false;
		memURL->Text = theProject -> getAddress();
		edtDbVersion -> Text = theProject -> getVersionString();

		if( theProject -> isActive() )
			rgStatus->ItemIndex = theProject -> isTest() ? RGS_TEST : RGS_LIVE;
		else
			rgStatus->ItemIndex = RGS_INACTIVE;

		// enable CreateDatabase button only if
		//   the system is live, the project is live
		//   the project does not already exist
		//
		// for creating test projects,
		//   create the live project and clone it using the makemirror
		//   procedure on the lab machine ~lust/util/makemirror

		if( !WizardLogin -> isMirror() && !theProject -> isTest() )
		{
			try
			{	LCDbProject::changeDbName( testDatabase, ProjectDatabase->Text );
				testDatabase -> Open();
			}
			catch(Exception& e)
			{
				btnCreateDatabase->Enabled = true;
			}
		}
	}
}

//---------------------------------------------------------------------------

AnsiString TAddProject::FindNextDatabaseName()
{
	int maxNum = 1;
	for( LCDbProjects::Range pr = LCDbProjects::records(); pr.isValid(); ++ pr )
	{
		const String & dbName = pr -> getDbName();
		String strDbNum = dbName.SubString( dbName.Pos( "ldb" ) + 3, 2 );
		int dbNum = strDbNum.ToIntDef( -1 );
		if( dbNum > maxNum )
			maxNum = dbNum;
	}

	String strDbName = "ldb" + IntToStr( maxNum + 1 );
	return WizardLogin -> isMirror() ? "t_" + strDbName : strDbName;
}

//---------------------------------------------------------------------------

void __fastcall TAddProject::SaveClick(TObject *Sender)
{
// validate project info
	bool validProject = true;
	if( ProjectName->Text.IsEmpty() || FullName->Text.IsEmpty()
	  || ProjectDatabase->Text.IsEmpty() || memURL->Text.IsEmpty() )
	{
		Application->MessageBox( "Fields must not be blank", NULL, MB_OK );
		validProject = false;
	}
	else if( ProjectName->Text.Length() > 16 )
	{
		Application->MessageBox( "Name must have fewer than 16 characters", NULL, MB_OK );
		validProject = false;
	}

	const LCDbProject * other = LCDbProjects::records().find( ProjectName->Text );
	if( other != NULL && other -> getID() != projectID )
	{
		Application->MessageBox( "Project name already in use", NULL, MB_OK );
		validProject = false;
	}
	else
	{	other = LCDbProjects::records().find( ProjectDatabase->Text );
		if( other != NULL && other -> getID() != projectID )
		{
			Application->MessageBox( "Database already in use", NULL, MB_OK );
			validProject = false;
		}
	}

	double version;
	try
	{	version = edtDbVersion -> Text.ToDouble();
	}
	catch( ... )
	{
		version = -1;
	}

	if( version < 2 )
	{
		Application->MessageBox( "Database version must be at least 2.0", NULL, MB_OK );
		validProject = false;
	}

	std::pair< TDate, TDate > dates( dtpFrom -> Date, dtpTo -> Date );
	if( (rgStatus -> ItemIndex != RGS_INACTIVE)
	 && (dates.second < dates.first || dates.first > Now() || dates.second < Now()) )
	{
		Application->MessageBox( "Invalid date range for active project", NULL, MB_OK );
		validProject = false;
	}

	bool ActionCompleted;
	if( validProject )
	{
		LCDbProject theProject( projectID );
		theProject.setName( ProjectName->Text );
		theProject.setDescription( FullName->Text );
		theProject.setValidDates( dates );
		theProject.setDbName( ProjectDatabase->Text );
		theProject.setVersionString( version );
		theProject.setAddress( memURL->Text );

		switch( rgStatus->ItemIndex )
		{
			case RGS_TEST:
				theProject.setActive( true );
				theProject.setTest( true );
				break;

			case RGS_INACTIVE:
				theProject.setActive( false );
				break;

			default:
				theProject.setActive( true );
				theProject.setTest( false );
		}

		theProject.saveRecord( QueryCentral );
		Close();
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddProject::CancelClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TAddProject::btnCreateDatabaseClick(TObject *Sender)
{
	CreateProjectDatabase->SetDatabaseName( ProjectDatabase->Text );
	CreateProjectDatabase->SetDatabaseVersion( edtDbVersion->Text );
    CreateProjectDatabase->ShowModal();
}
//---------------------------------------------------------------------------

