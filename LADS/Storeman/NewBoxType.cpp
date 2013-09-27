//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewBoxType.h"
#include "LCDbObject.h"
#include "LDbBoxSize.h"
#include "LDbRange.h"
#include "LCDbProject.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmNewBoxType *frmNewBoxType;

//---------------------------------------------------------------------------
__fastcall TfrmNewBoxType::TfrmNewBoxType(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewBoxType::aliquotDropDown(TObject *Sender)
{
	TComboBox * target = dynamic_cast< TComboBox * >( Sender );
	if( target != NULL ) {
		target->Clear();
		for( Range< LCDbObject > at = LCDbObjects::records(); at.isValid(); ++ at ) {
			if( at->isActive() && at->getObjectType() == LCDbObject::ALIQUOT_TYPE ) {
				target->Items->Add( at -> getName().c_str() );
			}
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewBoxType::cbFormDropDown(TObject *Sender)
{
	cbForm->Clear();
	for( Range< LCDbBoxSize > bs = LCDbBoxSizes::records(); bs.isValid(); ++ bs ) {
		if( bs->isActive() ) {
			cbForm->Items->Add( bs -> getName().c_str() );
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewBoxType::txtNameExit(TObject *Sender)
{
	AnsiString desc = txtName->Text.Trim();
	const LCDbProject * proj = LCDbProjects::records().findByID( LCDbProjects::getCurrentID() );
	if( proj != NULL ) {
		AnsiString pName = proj->getName().c_str();
		desc = pName + ' ' + desc;
	}
	if( txtFull->Text.IsEmpty() ) {
		txtFull->Text = desc + ' ';
	}
	if( !desc.IsEmpty() ) {
		txtExample->Text = desc + " 14279";
	}
}

//---------------------------------------------------------------------------

LPDbBoxType TfrmNewBoxType::getDetails() {
	LPDbBoxType content;
	content.setName( AnsiString(txtName->Text).c_str() );
	content.setDescription( AnsiString(txtFull->Text).c_str() );
	std::vector< int > aliquotIDs;
	addAliquot( aliquotIDs, cbAT1 );
	addAliquot( aliquotIDs, cbAT2 );
	addAliquot( aliquotIDs, cbAT3 );
	content.setAliquots( aliquotIDs );
	AnsiString formation = cbForm->Text.Trim();
	const LCDbBoxSize * size = LCDbBoxSizes::records().find( formation.c_str() );
	if( size != NULL ) {
		content.setSizeID( size->getID() );
	}
}

//---------------------------------------------------------------------------

String TfrmNewBoxType::checkDetails() {
	const LPDbBoxTypes & existing = LPDbBoxTypes::records();
	AnsiString name = txtName->Text, desc = txtFull->Text;
	if( name.IsEmpty() || existing.find( name.c_str() ) != NULL
	 || desc.IsEmpty() || existing.find( desc.c_str() ) != NULL ) {
		return "Please specify unique name and description";
	}
	std::vector< int > aliquotIDs;
	if( (!cbAT1->Text.IsEmpty() && !addAliquot( aliquotIDs, cbAT1 ))
	 || (!cbAT2->Text.IsEmpty() && !addAliquot( aliquotIDs, cbAT2 ))
	 || (!cbAT1->Text.IsEmpty() && !addAliquot( aliquotIDs, cbAT1 ))
	 || aliquotIDs.empty() ) {
		return "Please select one or more aliquot types";
	}
	AnsiString formation = cbForm->Text.Trim();
	if( formation.IsEmpty() || LCDbBoxSizes::records().find( formation.c_str() ) == NULL ) {
		return "Please select appropriate formation type";
	}
	return "";
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

void __fastcall TfrmNewBoxType::BitBtn1Click(TObject *Sender)
{
	String message = checkDetails();
	if( message.IsEmpty() ) {
		ModalResult = mrOk;
	} else {
		Application->MessageBox( message.c_str(), NULL, MB_OK );
	}
}

//---------------------------------------------------------------------------

