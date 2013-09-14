//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewType.h"
#include "StoreUtil.h"
#include "LCDbObject.h"
#include "TfrmConfirm.h"
#include "LCDbJob.h"
#include "SMLogin.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmNewStoreType *frmNewStoreType;

//---------------------------------------------------------------------------

__fastcall TfrmNewStoreType::TfrmNewStoreType(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TfrmNewStoreType::SaveClick(TObject *Sender)
{
	if( Util::validateText( TxtName, LblName )
	 && Util::validateText( TxtFull, LblFull ) ) {
		ModalResult = mrOk;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewStoreType::FormShow(TObject *Sender)
{
	TxtName->Text = "";
	TxtFull->Text = "";
	ActiveControl = TxtName;
}

//---------------------------------------------------------------------------

const LCDbObject * TfrmNewStoreType::createRecord() {
	LCDbObject type( LCDbObject::STORAGE_TYPE );
	AnsiString name = TxtName->Text.Trim();
	type.setName( name.c_str() );
	AnsiString full = TxtFull->Text.Trim();
	type.setDescription( full.c_str() );
	std::string operation = "Create new type " + type.getName();
	std::set< int > central;
	central.insert( 0 );
	frmConfirm -> initialise( TfrmSMLogin::CONFIGURE, operation.c_str(), central );
	if( frmConfirm->ShowModal() == mrOk && type.saveRecord( LIMSDatabase::getCentralDb() ) ) {
		return LCDbObjects::records().findByID( type.getID() );
	}
	return NULL;
}

//---------------------------------------------------------------------------

