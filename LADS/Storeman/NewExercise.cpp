//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewExercise.h"
#include "StoreUtil.h"
#include "LCDbObject.h"
#include "TfrmConfirm.h"
#include "LCDbJob.h"
#include "SMLogin.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmNewExercise *frmNewExercise;

//---------------------------------------------------------------------------

__fastcall TfrmNewExercise::TfrmNewExercise(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TfrmNewExercise::SaveClick(TObject *Sender)
{
	if( Util::validateText( TxtName, LblName ) && Util::validateText( TxtFull, LblFull ) ) {
		ModalResult = mrOk;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewExercise::FormShow(TObject *Sender)
{
	TxtName->Text = "";
	TxtFull->Text = "";
	ActiveControl = TxtName;
}

//---------------------------------------------------------------------------

const LCDbObject * TfrmNewExercise::createRecord() {
	LCDbObject type( LCDbObject::STORAGE_EXERCISE );
	type.setName( AnsiString( TxtName->Text ).c_str() );
	type.setDescription( AnsiString( TxtFull->Text ).c_str() );
	if( type.saveRecord( LIMSDatabase::getCentralDb() ) ) {
		return LCDbObjects::records().findByID( type.getID() );
	}
	return NULL;
}

//---------------------------------------------------------------------------

