//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FindSample.h"
#include "LCDbProject.h"
#include "LDbBoxType.h"
#include "StringUtil.h"
#include "StoreUtil.h"
#include "LPDbBoxes.h"
#include "BrowseSample.h"
#include "StoreDAO.h"
#include "LCDbObject.h"
#include "LPDbCryovial.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmFind *frmFind;

//---------------------------------------------------------------------------

__fastcall TfrmFind::TfrmFind(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------
//	update project drop-down; select current project if already set
//---------------------------------------------------------------------------

void __fastcall TfrmFind::FormShow(TObject *Sender)
{
	cbProject->Clear();
	std::string current;
	for( Range< LCDbProject > pr = LCDbProjects::records(); pr.isValid(); ++ pr ) {
		if( pr -> isValid() && !pr -> isCentral() ) {
			if( pr->getID() == LCDbProjects::getCurrentID() ) {
				current = pr->getName();
			}
			cbProject->Items->Add( pr->getName().c_str() );
		}
	}
	cbProject->Text = current.c_str();
	cbType->Clear();
	cbType->Enabled = !current.empty();
	txtName->Text = "";
	txtName->Enabled = false;
}

//---------------------------------------------------------------------------

void __fastcall TfrmFind::rgTypesClick(TObject *Sender)
{
	FormShow( Sender );
}

//---------------------------------------------------------------------------
//	switch to the selected project; user should select type before ID
//---------------------------------------------------------------------------

void __fastcall TfrmFind::cbProjectChange(TObject *Sender)
{
	LCDbProjects & pl = LCDbProjects::records();
	const LCDbProject * proj = NULL;
	if( !cbProject->Text.IsEmpty() ) {
		proj = pl.findByName( bcsToStd( cbProject->Text ) );
	}
	txtName->Enabled = false;
	if( proj == NULL ) {
		cbType->Enabled = false;
	} else {
		pl.setCurrent( *proj );
		cbType->Enabled = true;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmFind::cbTypeChange(TObject *Sender)
{
	txtName -> Enabled = (cbType -> ItemIndex >= 0);
}

//---------------------------------------------------------------------------
//	update box/aliquot type drop-down to match the selected search type
//---------------------------------------------------------------------------

void __fastcall TfrmFind::cbTypeDropDown(TObject *Sender)
{
	cbType->Clear();
	if( rgTypes -> ItemIndex == 1 ) {
		for( Range< LCDbObject > at = LCDbObjects::records(); at.isValid(); ++ at ) {
			if( at -> isActive() && at -> getObjectType() == LCDbObject::ALIQUOT_TYPE ) {
				cbType->Items->Add( at -> getName().c_str() );
			}
		}
	} else {
		for( Range< LPDbBoxType > bt = LPDbBoxTypes::records(); bt.isValid(); ++ bt ) {
			if( bt -> isActive() ) {
				cbType->Items->Add( bt->getName().c_str() );
			}
		}
	}
}

//---------------------------------------------------------------------------
//	search for selected item; return mrOk from ShowModal() if found
//---------------------------------------------------------------------------

void __fastcall TfrmFind::BitBtn1Click(TObject *Sender)
{
	boxDetails.clear();
	String message;
	if( rgTypes -> ItemIndex == 1 ) {
		if( !findCryovial() ) {
			message = "Cannot find " + cbProject->Text + " " + cbType->Text + " cryovial " + txtName->Text;
		}
	} else {
		if( !findBox() ) {
			message = "Cannot find box " + cbProject->Text + " " + cbType->Text + " " + txtName->Text;
		}
	}
	if( message.IsEmpty() ) {
		ModalResult = mrOk;
	} else {
		Application -> MessageBox( message.c_str(), NULL, MB_ICONWARNING );
	}
}

//---------------------------------------------------------------------------
//	search for box with given name or ID and check the box type matches
//---------------------------------------------------------------------------

bool TfrmFind::findBox() {
	StoreDAO & dao = StoreDAO::records();
	int projID = LCDbProjects::getCurrentID();
	std::string type = bcsToStd( cbType->Text );
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
		dao.loadBoxes( bcsToStd( txtName->Text ), type, projID, results );
		if( results.size() == 1 ) {
			boxDetails = results.front();
		}
	}

	int typeID = boxDetails.getIntDefault( "box_type_cid", 0 );
	if( typeID != 0 ) {
		const LPDbBoxType * boxType = LPDbBoxTypes::records().find( type );
		if( boxType != NULL && boxType->getID() == typeID ) {
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------
//	find box containing cryovial with given barcode and aliquot type
//---------------------------------------------------------------------------

bool TfrmFind::findCryovial() {
	std::string type = bcsToStd( cbType->Text );
	const LCDbObject * aliquot = LCDbObjects::records().find( type, LCDbObject::ALIQUOT_TYPE );
	if( aliquot == NULL ) {
		return false;
	}
	LPDbCryovials db;
	std::string barcode = bcsToStd( txtName->Text );
	const LPDbCryovial * cryovial = db.readRecord( Util::projectQuery(), barcode, aliquot->getID() );
	if( cryovial == NULL ) {
		return false;
	}
	StoreDAO & dao = StoreDAO::records();
	dao.loadBoxDetails( cryovial->getBoxID(), LCDbProjects::getCurrentID(), boxDetails );
	return boxDetails.isInt( "box_cid" );
}

//---------------------------------------------------------------------------

Box * TfrmFind::getDetails() const {
	if( boxDetails.isEmpty() ) {
		return NULL;
	}
	Box * box = new Box( boxDetails );
	if( rgTypes -> ItemIndex == 1 ) {
		box -> populate();
	}
	return box;
}

//---------------------------------------------------------------------------

