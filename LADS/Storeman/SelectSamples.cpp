//---------------------------------------------------------------------------

#include <vcl.h>

#include "LCDbProject.h"
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
//	search for box with given barcode and check the box type matches
//---------------------------------------------------------------------------

bool TfrmSelectBoxes::addBox() {
	ROSETTA boxDetails;
	int projID = LCDbProjects::getCurrentID();
	AnsiString barcode = txtName->Text, type = cbType->Text;
	StoreDAO().loadBoxDetails( barcode.c_str(), type.c_str(), projID, boxDetails );
	int typeID = boxDetails.getIntDefault( "box_type_cid", 0 );
	if( typeID == 0 ) {
		return false;
	} else {
		LPDbBoxName box( boxDetails.getString( "external_name" ), typeID );
		box.setID( boxDetails.getInt( "box_cid" ) );
		boxes.insert( box );
		return true;
	}
}

//---------------------------------------------------------------------------

