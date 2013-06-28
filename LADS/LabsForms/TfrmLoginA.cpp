//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TfrmLoginA.h"
#include "LCDbObject.h"
#include "LCDbAnalyser.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "TfrmLoginBase"
#pragma resource "*.dfm"

TfrmAnalyserLogin *frmAnalyserLogin;

//---------------------------------------------------------------------------

__fastcall TfrmAnalyserLogin::TfrmAnalyserLogin(TComponent* Owner) : TfrmLoginBase(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TfrmAnalyserLogin::initialise(TObject *Sender)
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

void __fastcall TfrmAnalyserLogin::okButtonClick(TObject *Sender)
{
	const LCDbAnalyser * ap = LCDbAnalysers::records().findByName( machineName -> Text );
	if( ap == NULL ) {
		Application -> MessageBox( "Please select an analyser", NULL, MB_ICONWARNING );
	} else {
		const LCDbOperator * user = logUserIn();
		if( user != NULL ) {
			if( !user -> canRun( 0, ap -> getID(), 0 ) ) {
				String message = "No permission for " + user -> getName() + " to use " + ap -> getName();
				Application -> MessageBox( message.c_str(), NULL, MB_ICONWARNING );
			} else {
            	ap -> makeDefault();
				startProgram( Sender );
			}
		}
	}
}

//---------------------------------------------------------------------------

