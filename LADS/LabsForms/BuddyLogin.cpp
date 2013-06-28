//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BuddyLogin.h"
#include "LCDbObject.h"
#include "LCDbAnalyser.h"
#include "LDbWorklist.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "TfrmLoginBase"
#pragma resource "*.dfm"

TfrmBuddyLogin *frmBuddyLogin;

//---------------------------------------------------------------------------

__fastcall TfrmBuddyLogin::TfrmBuddyLogin(TComponent* Owner)
 : TfrmLoginBase(Owner), categoryID( 0 )
{}

//---------------------------------------------------------------------------

void __fastcall TfrmBuddyLogin::initialise(TObject *Sender)
{
	TfrmLoginBase::initialise( Sender );

	machineName -> Clear();
	for( Range< LCDbObject > ml = LCDbObjects::records(); ml.isValid(); ++ ml ) {
		if( ml -> isActive() && ml -> getObjectType() == LCDbObject::CLUSTER ) {
			machineName -> Items -> Add( ml->getName() );
		}
	}
	for( Range< LCDbAnalyser > ml = LCDbAnalysers::records(); ml.isValid(); ++ ml ) {
		if( ml -> isActive() ) {
			machineName -> Items -> Add( ml -> getName() );
		}
		if( ml -> getID() == LCDbAnalysers::getCurrentID() ) {
			machineName -> Text = ml -> getName();
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmBuddyLogin::okButtonClick(TObject *Sender)
{
	const LCDbOperator * user = logUserIn();
	if( user != NULL && checkAnalyser() ) {
		if( !user -> canRun( 0, LCDbAnalysers::getCurrentID(), 0 ) ) {
			String message = "No permission for " + user -> getName() + " to use " + machineName -> Text;
			Application -> MessageBox( message.c_str(), NULL, MB_ICONWARNING );
		} else {
			checkCategory();
			startProgram( Sender );
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmBuddyLogin::cbCategoryDropDown(TObject *Sender)
{
	cbCategory -> Items -> Text = "(All)";
	if( checkAnalyser() ) {
		std::set< int > idList = LCDbWorkList().getCategoryIDs( qCentral );
		for( Range< LCDbObject > or = LCDbObjects::records(); or.isValid(); ++ or )	{
			if( idList.count( or -> getID() ) != 0 ) {
				cbCategory -> Items -> Add( or -> getName() );
			}
		}
	}
}

//---------------------------------------------------------------------------

bool TfrmBuddyLogin::checkCategory()
{
	const String selected = cbCategory -> Text;
	if( !selected.IsEmpty() ) {
		const LCDbObject * active = LCDbObjects::records().find( selected, LCDbObject::SAMPLE_CATEGORY );
		if( active != NULL ) {
			categoryID = active -> getID();
			return true;
		}
	}
	categoryID = 0;
	cbCategory -> Text = "(All)";
	return false;
}

//---------------------------------------------------------------------------

bool TfrmBuddyLogin::checkAnalyser()
{
	LCDbAnalysers::records().setCurrent( machineName -> Text );
	if( LCDbAnalysers::getCurrentID() != 0 ) {
		return true;
	}
	Application -> MessageBox( "Please select an analyser", NULL, MB_ICONWARNING );
	return false;
}

//---------------------------------------------------------------------------

