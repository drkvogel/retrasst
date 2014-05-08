//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FindSample.h"
#include "LCDbProject.h"
#include "LDbBoxType.h"
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
		if( pr -> isInCurrentSystem() && !pr -> isCentral() ) {
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
	boxSearch = (rgTypes -> ItemIndex == 0);
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
		proj = pl.findByName( AnsiString( cbProject->Text ).c_str() );
	}
	if( proj == NULL ) {
		cbType->Enabled = false;
		txtName->Enabled = false;
	} else {
		pl.setCurrent( *proj );
		cbType->Enabled = true;
		txtName->Enabled = boxSearch;
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
	if( boxSearch ) {
		for( const LPDbBoxType & bt : LPDbBoxTypes::records() ) {
			if( bt.isActive() ) {
				if( bt.getProjectCID() == 0 || bt.getProjectCID() == LCDbProjects::getCurrentID() ) {
					cbType->Items->Add( bt.getName().c_str() );
				}
			}
		}
	} else {
		std::set< int > types = LPDbCryovials::getAliquotTypes( LIMSDatabase::getProjectDb() );
		for( const LCDbObject & at : LCDbObjects::records() ) {
			if( types.count( at.getID() ) != 0 ) {
				cbType->Items->Add( at.getName().c_str() );
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
	const char * missing = NULL;
	if( boxSearch ) {
		if( !findBox() ) {
			missing = "box";
		}
	} else {
		if( !findCryovial() ) {
			missing = "cryovial";
		}
	}
	if( missing == NULL ) {
		ModalResult = mrOk;
	} else {
		String message = "Cannot find " + cbProject->Text + ' ' + cbType->Text + ' ' + missing + ' ' + txtName->Text;
		Application -> MessageBox( message.c_str(), NULL, MB_ICONWARNING );
	}
}

//---------------------------------------------------------------------------
//	search for box with given name or ID and check the box type matches
//---------------------------------------------------------------------------

bool TfrmFind::findBox() {
	int projID = LCDbProjects::getCurrentID();
	std::string barcode = AnsiString( txtName->Text ).c_str();
	std::string type = AnsiString( cbType->Text ).c_str();
	StoreDAO().loadBoxDetails( barcode, type, projID, boxDetails );
	return boxDetails.isInt( "box_cid" );
}

//---------------------------------------------------------------------------
//	find box containing cryovial with given barcode and aliquot type
//---------------------------------------------------------------------------

bool TfrmFind::findCryovial() {
	AnsiString barcode = txtName->Text, type = cbType->Text;
	const LCDbObject * aliquot = LCDbObjects::records().find( type.c_str(), LCDbObject::ALIQUOT_TYPE );
	if( aliquot == NULL ) {
		return false;
	}
	LPDbCryovials db;
	LQuery pq( LIMSDatabase::getProjectDb() );
	const LPDbCryovial * cryovial = db.readRecord( pq, barcode.c_str(), aliquot->getID() );
	if( cryovial == NULL ) {
		return false;
	}
	return StoreDAO().loadBoxDetails( cryovial->getBoxID(), boxDetails );
}

//---------------------------------------------------------------------------

Box * TfrmFind::getDetails() const {
	return boxDetails.isEmpty() ? NULL : new Box( boxDetails );
}

//---------------------------------------------------------------------------

