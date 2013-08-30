//---------------------------------------------------------------------------

#include <vcl.h>

#include "SelectSamples.h"
#include "AddSpecimens.h"
#include "SMLogin.h"
#include "StoreUtil.h"
#include "MoveJobs.h"
#include "LDbBoxType.h"
#include "StoreDAO.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "dvSelector"
#pragma resource "*.dfm"

TfrmSelectBoxes *frmSelectBoxes;

//---------------------------------------------------------------------------

__fastcall TfrmSelectBoxes::TfrmSelectBoxes(TComponent* Owner) : TForm(Owner)
{}


//---------------------------------------------------------------------------

bool TfrmSelectBoxes::init()
{
	cbType -> Clear();
	lstBoxes -> Clear();
	txtName -> Clear();
	boxes.clear();
	const LCDbProject * pr = LCDbProjects::records().findByID( LCDbProjects::getCurrentID() );
	if( pr == NULL ) {
		return false;
	} else {
		txtProject -> Text = pr->getName().c_str();
		for( Range< LPDbBoxType > bt = LPDbBoxTypes::records(); bt.isValid(); ++ bt ) {
			if( bt -> isActive() ) {
				cbType->Items->Add( bt->getName().c_str() );
			}
		}
		return true;
	}
}

//---------------------------------------------------------------------------
//	Add a box to the list; display its name but don't read it yet
//---------------------------------------------------------------------------

void __fastcall TfrmSelectBoxes::btnAddBoxClick(TObject *Sender)
{
	if( addBox() ) {
		lstBoxes -> Clear();
		for( Range< LPDbBoxName > found = boxes; found.isValid(); ++ found ) {
			lstBoxes -> Items -> Add( found -> getName().c_str() );
		}
	} else {
		String message = "Cannot find box " + txtProject->Text + " " + cbType->Text + " " + txtName->Text;
		Application -> MessageBox( message.c_str(), NULL, MB_ICONWARNING );
	}
}

//---------------------------------------------------------------------------
//	search for box with given name or ID and check the box type matches
//---------------------------------------------------------------------------

bool TfrmSelectBoxes::addBox() {
	ROSETTA boxDetails;
	StoreDAO & dao = StoreDAO::records();
	int projID = LCDbProjects::getCurrentID();
	AnsiString type =  cbType->Text;
	int boxID = txtName->Text.ToIntDef( 0 );
	if( boxID != 0 ) {
		// user won't distinguish +ve and -ve IDs - try both
		dao.loadBoxDetails( boxID, projID, boxDetails );
		if( !boxDetails.isInt( "box_cid" ) ) {
			dao.loadBoxDetails( -boxID, projID, boxDetails );
		}
	}
	if( !boxDetails.isInt( "box_cid" ) ) {
		std::vector<ROSETTA> results;
		// name may not match ID - try looking in the name
		AnsiString name =  txtName->Text;
		dao.loadBoxes( name.c_str(), type.c_str(), projID, results );
		if( results.size() == 1 ) {
			boxDetails = results.front();
		}
	}
	int typeID = boxDetails.getIntDefault( "box_type_cid", 0 );
	if( typeID != 0 ) {
		const LPDbBoxType * boxType = LPDbBoxTypes::records().find( type.c_str() );
		if( boxType != NULL && boxType->getID() == typeID ) {
			LPDbBoxName box( boxDetails.getString( "external_name" ), typeID );
			box.setID( boxDetails.getInt( "box_cid" ) );
			boxes.insert( box );
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------

