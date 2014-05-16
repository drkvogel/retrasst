//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewBoxType.h"
#include "LCDbObject.h"
#include "LDbBoxSize.h"
#include "LDbRange.h"
#include "LCDbProject.h"
#include "LDbBoxType.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmNewBoxType *frmNewBoxType;

//---------------------------------------------------------------------------

__fastcall TfrmNewBoxType::TfrmNewBoxType(TComponent* Owner)
 : TForm( Owner ), expectedTubeType( 0 ), currentFormation( NULL ), currentBoxType( NULL )
{}

//---------------------------------------------------------------------------

void TfrmNewBoxType::init( std::set< int > & aliquotIDs, int tubeTypeID ) {
	expectedTubeType = tubeTypeID;
	currentFormation = NULL;
	currentBoxType = NULL;
	expectedAliquots = aliquotIDs;
	clearAll();
}

//---------------------------------------------------------------------------

void TfrmNewBoxType::clearAll() {
	cbTypes->Clear();
	cbForms->Clear();
	clearDetails();
}

//---------------------------------------------------------------------------

void TfrmNewBoxType::clearDetails() {
	cbTypes->Text = "(create new type)";
	currentBoxType = NULL;
	txtName->Text = "";
	txtName->Enabled = true;
	txtFull->Text = "";
	txtFull->Enabled = true;
	txtExample->Text = "";
	txtExample->Enabled = false;
	cbAT1->Clear();
	cbAT1->Enabled = true;
	cbAT2->Clear();
	cbAT2->Enabled = true;
	cbAT3->Clear();
	cbAT3->Enabled = true;
	btnOK->Enabled = false;
}

//---------------------------------------------------------------------------

void TfrmNewBoxType::showDetails() {
	txtName->Text = currentBoxType->getName().c_str();
	txtName->Enabled = false;
	txtFull->Text = currentBoxType->getDescription().c_str();
	txtFull->Enabled = false;
	showExampleBox();

	const std::vector< int > & aliquots = currentBoxType->getAliquots();
	const LCDbObjects & names = LCDbObjects::records();
	std::vector< int >::const_iterator al = aliquots.begin();
	if( al == aliquots.end() ) {
		cbAT1->Text = "";
	} else {
		cbAT1->Text = names.get( *al++ ).getName().c_str();
	}
	cbAT1->Enabled = false;

	if( al == aliquots.end() ) {
		cbAT2->Text = "";
	} else {
		cbAT2->Text = names.get( *al++ ).getName().c_str();
	}
	cbAT2->Enabled = false;

	if( al == aliquots.end() ) {
		cbAT3->Text = "";
	} else {
		cbAT3->Text = names.get( *al++ ).getName().c_str();
	}
	cbAT3->Enabled = false;
	btnOK->Enabled = true;
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewBoxType::aliquotDropDown(TObject *Sender) {
	TComboBox * target = dynamic_cast< TComboBox * >( Sender );
	if( target != NULL ) {
		target->Clear();
		for( const LCDbObject & at : LCDbObjects::records() ) {
			if( at.isActive() ) {
				if( at.getObjectType() == LCDbObject::ALIQUOT_TYPE ) {
					target->Items->Add( at.getName().c_str() );
				}
			}
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewBoxType::cbFormsDropDown(TObject *Sender) {
	cbForms->Clear();
	for( const LCDbBoxSize & bs : LCDbBoxSizes::records() ) {
		if( bs.isActive() ) {
			if( expectedTubeType == 0 || bs.getTubeType() == expectedTubeType ) {
				cbForms->Items->Add( bs.getName().c_str() );
			}
		}
	}
}

//---------------------------------------------------------------------------

void TfrmNewBoxType::showExampleBox() {
	AnsiString desc = txtName->Text.Trim();
	const LCDbProject * proj = LCDbProjects::records().findByID( LCDbProjects::getCurrentID() );
	if( proj != NULL ) {
		AnsiString pName = proj->getName().c_str();
		desc = pName + ' ' + desc;
	}
	if( txtFull->Text.IsEmpty() ) {
		txtFull->Text = desc + " retrieval";
	}
	if( !desc.IsEmpty() ) {
		txtExample->Text = desc + " 14279";
	}
}

//---------------------------------------------------------------------------

LPDbBoxType TfrmNewBoxType::getDetails() {
	LPDbBoxType content;
	if( currentBoxType != NULL ) {
		content = *currentBoxType;
	} else {
		content.setName( AnsiString(txtName->Text).c_str() );
		content.setDescription( AnsiString(txtFull->Text).c_str() );
		if( currentFormation != NULL ) {
			content.setSizeID( currentFormation->getID() );
		}
		std::vector< int > aliquotIDs;
		addAliquot( aliquotIDs, cbAT1 );
		addAliquot( aliquotIDs, cbAT2 );
		addAliquot( aliquotIDs, cbAT3 );
		content.setAliquots( aliquotIDs );
	}
	return content;
}

//---------------------------------------------------------------------------

bool TfrmNewBoxType::addAliquot( std::vector< int > & aliquotIDs, TComboBox * cbType ) {
	AnsiString name = cbType->Text.Trim();
	if( !name.IsEmpty() ) {
		const LCDbObjects & names = LCDbObjects::records();
		const LCDbObject * aliquot = names.find( name.c_str(), LCDbObject::ALIQUOT_TYPE );
		if( aliquot != NULL ) {
			aliquotIDs.push_back( aliquot -> getID() );
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewBoxType::btnClearClick(TObject *Sender)
{
	clearAll();
	ModalResult = mrNone;
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewBoxType::cbFormsChange(TObject *Sender) {
	if( cbForms->ItemIndex >= 0 ) {
		AnsiString formation = cbForms->Items->Strings[ cbForms->ItemIndex ];
		currentFormation = LCDbBoxSizes::records().find( formation.c_str() );
		if( currentFormation == NULL ) {
			clearAll();
		} else if( currentBoxType != NULL ) {
			clearDetails();
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewBoxType::cbTypesDropDown(TObject *Sender) {
	cbTypes->Clear();
	for( const LPDbBoxType & bt : LPDbBoxTypes::records() ) {
		if( bt.isActive() ) {
			if( currentFormation == NULL || bt.getSizeID() == currentFormation->getID() ) {
				if( matchesExpected( bt.getAliquots() ) ) {
					cbTypes->Items->Add( bt.getName().c_str() );
				}
			}
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewBoxType::cbTypesChange(TObject *Sender) {
	if( cbTypes->ItemIndex >= 0 ) {
		AnsiString boxType = cbTypes->Items->Strings[ cbTypes->ItemIndex ];
		currentBoxType = LPDbBoxTypes::records().find( boxType.c_str() );
		if( currentBoxType != NULL ) {
			showDetails();
		}
	}
}

//---------------------------------------------------------------------------

bool TfrmNewBoxType::checkDetails() {
	if( currentFormation == NULL || txtName->Text.IsEmpty() || txtFull->Text.IsEmpty() ) {
		return false;
	}
	std::vector< int > aliquotIDs;
	if( (!cbAT1->Text.IsEmpty() && !addAliquot( aliquotIDs, cbAT1 ))
	 || (!cbAT2->Text.IsEmpty() && !addAliquot( aliquotIDs, cbAT2 ))
	 || (!cbAT3->Text.IsEmpty() && !addAliquot( aliquotIDs, cbAT3 )) ) {
		return false;
	}
	return matchesExpected( aliquotIDs );
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewBoxType::txtNameExit(TObject *Sender)
{
	AnsiString name = ((TEdit*)Sender)->Text;
	bool ready = false;
	if( !name.IsEmpty() ) {
		const LPDbBoxType * existing = LPDbBoxTypes::records().find( name.c_str() );
		if( existing != NULL ) {
			Application->MessageBox(L"Please specify unique name and description", NULL);
		} else {
			showExampleBox();
			if( checkDetails() ) {
				ready = true;
			}
		}
	}
	btnOK->Enabled = ready;
}

//---------------------------------------------------------------------------

bool TfrmNewBoxType::matchesExpected( const std::vector< int > & aliquotIDs ) {
	std::set< int > required = expectedAliquots;
	for( int aliquot : aliquotIDs ) {
		required.erase( aliquot );
	}
	return required.empty();
}

//---------------------------------------------------------------------------

