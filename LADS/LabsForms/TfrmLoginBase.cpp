/*---------------------------------------------------------------------------
 *
 *	LIMS Security and Auditting login screen - use as is or extend
 *
 *	18 Aug 2006: 	ignore (invisible) analyser when enabling OK button
 *  18 Jan 2008: 	add to audit trail on Close (avoids error in logging)
 *	3 Apr 08, NG: 	Allow user to select test, live or mirror system
 *  20 May 08, NG:	Split analyser drop-down into child: BuddyLogin
 *  30 May 08, NG:	Claim project before CFR confirmation message
 *  24 June 2008:	Identify machine from buddy ID and network name
 *  12 Aug 08, NG:  Release database lease as soon as main form closes
 *  7 Jan 2009      Also read test definitions when user log on
 *  7 Apr 09, NG:	Show user name, not "NULL", if they cannot log in!
 *  23 Mar 11, NG:	Warn user before switching between databases
 *
 *--------------------------------------------------------------------------*/

#include <vcl.h>
#include <inifiles.hpp>
#include <stdlib.h>

#include "TfrmLoginBase.h"
#include "LCDbOperator.h"
#include "LIMSParams.h"
#include "LCDbAnalyser.h"
#include "LCDbProject.h"
#include "LCDbObject.h"
#include "TfrmPassword.h"
#include "TfrmAboutBox.h"
#include "LIMSDatabase.h"

#pragma hdrstop

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmLoginBase *frmLoginBase;

//---------------------------------------------------------------------------
//  Warn about multiple instances; select live or mirror system
//---------------------------------------------------------------------------

__fastcall TfrmLoginBase::TfrmLoginBase(TComponent* Owner) : TForm(Owner)
{
	std::string ver = LIMSParams::instance().getProgVersion();
	String app = Application -> Title + " " + ver.c_str();
#ifdef _DEBUG
	rgDatabase -> ItemIndex = LIMSDatabase::MIRROR_SYSTEM; 	// db v2.7.3 - OK
	version -> Font -> Color = clRed;
	version -> Caption = app + " **DEBUG**";
#else
	//rgDatabase -> ItemIndex = LIMSDatabase::LIVE_DATA;		// db v2.7.1 - fail
    rgDatabase -> ItemIndex = LIMSDatabase::TEST_DATA;		// db v2.7.2 - OK
	version -> Font -> Color = clBlack;
	version -> Caption = app + " **FINAL**";
#endif
	//rgDatabase -> ItemIndex = LIMSDatabase::TEST_DATA;		// db v2.7.2 - OK
    //rgDatabase -> ItemIndex = LIMSDatabase::MIRROR_SYSTEM;
	Application -> OnException = AppException;
}

//---------------------------------------------------------------------------
//  Display exceptions and add them to the audit trail; re-initialise
//---------------------------------------------------------------------------

void __fastcall TfrmLoginBase::AppException(TObject *, Exception *E)
{
	try {
		AnsiString error = E -> Message;
		auditTrail.addRecord( error.c_str(), LCDbAuditTrail::MAJOR_ERROR );
	}
	catch( ... ) {
	}
	Screen -> Cursor = crDefault;
	String title = Application -> Title + " Error";
	Application -> MessageBox( E -> Message.c_str(), title.c_str(), MB_ICONWARNING );
	timer -> Enabled = true;
}

//---------------------------------------------------------------------------
//  Update shared data from the database and populate user list
//---------------------------------------------------------------------------

void __fastcall TfrmLoginBase::initialise(TObject *)
{
	timer -> Enabled = false;

	// select live, test or mirror databases
	LIMSDatabase::DbSystem system = LIMSDatabase::getCurrentSystem();
	LIMSDatabase::DbSystem selected = LIMSDatabase::DbSystem(rgDatabase -> ItemIndex);
	if( system != selected ) {
		if( system == LIMSDatabase::UNKNOWN || Application -> MessageBox(
			 L"Are you sure you want to switch database?", L"Warning", MB_YESNO ) == ID_YES ) {
			LIMSDatabase::setCurrentSystem( selected );
		}
		else rgDatabase -> ItemIndex = system;
	}

	// stop if the machine is not configured correctly
	LIMSParams & config = LIMSParams::instance();
	if( !config.checkUnique() || !config.checkMachine() ) {
		Application -> Terminate();
		return;
	}

	//	list active users; don't assume one has been set up
	userList -> Clear();
	for( Range< LCDbOperator > ul = LCDbOperators::records(); ul.isValid(); ++ ul ) {
		if( ul -> isActive() ) {
			userList -> Items -> Add( ul -> getDescription().c_str() );
		}
	}
	userList -> Enabled = (userList -> Items -> Count != 0);
	ebPassword -> Clear();
	ebPassword -> Enabled = userList -> Enabled;
	auditTrail.start();
    userList->ItemIndex = 4;
    ebPassword->Text = "albatross";
}

//---------------------------------------------------------------------------
//  Run program if user can log in (project selection can be overridden)
//---------------------------------------------------------------------------

void __fastcall TfrmLoginBase::okButtonClick(TObject *Sender)
{
	if( logUserIn() != NULL )
		startProgram( Sender );
}

//---------------------------------------------------------------------------
//  Start the audit trail and load the main program
//---------------------------------------------------------------------------

void TfrmLoginBase::startProgram( TObject * )
{
	AnsiString title = Application -> Title + ": ";

	int projID = LCDbProjects::getCurrentID();
	if( projID != LCDbProject::NONE_SELECTED )
	{
		const LCDbProject * proj = LCDbProjects::records().findByID( projID );
		if( proj != NULL )
			title = title + proj -> getName().c_str() + ", ";
	}

	int userID = LCDbOperators::getCurrentID();
	const LCDbOperator * user = LCDbOperators::records().findByID( userID );
	if( user != NULL )
		title = title + user -> getDescription().c_str();

	auditTrail.login();
	WindowState = wsMinimized;
	title = title + " (" + auditTrail.getProcessID() + ")";
	runProgram( title.c_str() );
	WindowState = wsNormal;
}

//---------------------------------------------------------------------------
//  Switch to the given user if valid; force new password if necessary
//---------------------------------------------------------------------------

const LCDbOperator * TfrmLoginBase::logUserIn()
{
	bool locked, matched;
	LCDbOperators & users = LCDbOperators::records();
	AnsiString userName = userList -> Text ;
	const LCDbOperator * user = users.findByName( userName.c_str( ) );
	if( user == NULL )
		matched = locked = false;
	else
	{	locked = user -> hasLockedAccount();
		AnsiString pwd = ebPassword -> Text ;
		matched = user -> matchPassword( pwd.c_str() );
	}
	ebPassword -> Clear();

	static short failCount = 0;
	if( user != NULL && !matched && !locked && ++ failCount >= 3 )
	{
		LCDbOperator record = *user;
		LQuery qCentral( LIMSDatabase::getCentralDb() );
		record.lockAccount();
		record.saveRecord( qCentral );
		locked = true;
	}

	if( locked )
	{
		auditTrail.sendEMail( user -> getDescription() + "'s account is locked" );
		Application -> MessageBox( L"Too many login attempts; account locked.", NULL, MB_ICONWARNING );
		return NULL;
	}

	if( !matched )
	{
		AnsiString email = userName + " failed to log in";
		auditTrail.sendEMail( email.c_str() );
		Application -> MessageBox( L"Please check name and password.", NULL, MB_ICONWARNING );
		return NULL;
	}

	users.setCurrent( *user );
	if( user -> needsPassword() && frmPassword -> ShowModal() != mrOk )
		return NULL;
	else
	{	failCount = 0;
		return user;
	}
}

//---------------------------------------------------------------------------
//  Allow the user to try logging in once they've filled the boxes
//---------------------------------------------------------------------------

void __fastcall TfrmLoginBase::textChange(TObject *)
{
	okButton -> Enabled = !userList -> Text.IsEmpty() && !ebPassword -> Text.IsEmpty();
}

//---------------------------------------------------------------------------
//	Reinitialise when the user switches between live, test and mirror
//---------------------------------------------------------------------------

void __fastcall TfrmLoginBase::rgDatabaseClick(TObject *)
{
	timer -> Enabled = true;
	okButton -> Enabled = false;
}

//---------------------------------------------------------------------------
//  Close program when the user requests it (not from the main form)
//---------------------------------------------------------------------------

void __fastcall TfrmLoginBase::exitButtonClick(TObject *)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TfrmLoginBase::FormClose(TObject *, TCloseAction &)
{
	timer -> Enabled = false;
	if( auditTrail.hasStarted() )
		auditTrail.stop();
//	LCDbProjects::records().releaseDatabases();
}

//---------------------------------------------------------------------------

void __fastcall TfrmLoginBase::FormCanResize(TObject *, int &, int &, bool &Resize)
{
	Resize = false;
}

//---------------------------------------------------------------------------
//	Version label is a hyperlink - show about box when it's clicked on
//---------------------------------------------------------------------------

void __fastcall TfrmLoginBase::versionMouseEnter(TObject *)
{
	Screen -> Cursor = crHandPoint;
}
//---------------------------------------------------------------------------

void __fastcall TfrmLoginBase::versionMouseLeave(TObject *)
{
	Screen -> Cursor = crDefault;
}

//---------------------------------------------------------------------------

void __fastcall TfrmLoginBase::versionClick(TObject *)
{
	frmAboutBox -> ShowModal();
}

//---------------------------------------------------------------------------
//	open the main form (if set): can be overridden by sub-classes
//---------------------------------------------------------------------------

void TfrmLoginBase::runProgram( const std::string & title )
{
	if( mainForm != NULL ) {
		mainForm -> Caption = title.c_str();
		mainForm -> ShowModal();
	}
}

//---------------------------------------------------------------------------



