//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewType.h"
#include "StoreUtil.h"
#include "StringUtil.h"
#include "LCDbObject.h"
#include "TfrmConfirm.h"
#include "LCDbJob.h"
#include "SMLogin.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmNewType *frmNewType;

//---------------------------------------------------------------------------

__fastcall TfrmNewType::TfrmNewType(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TfrmNewType::SaveClick(TObject *Sender)
{
	if( Util::validateText( TxtName, LblName )
	 && Util::validateText( TxtFull, LblFull ) ) {
		ModalResult = mrOk;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewType::FormShow(TObject *Sender)
{
	TxtName->Text = "";
	TxtFull->Text = "";
	ActiveControl = TxtName;
}

//---------------------------------------------------------------------------

const LCDbObject * TfrmNewType::createRecord() {
	LCDbObject type( LCDbObject::STORAGE_TYPE );
	type.setName( bcsToStd(TxtName->Text.Trim()) );
	type.setDescription( bcsToStd(TxtFull->Text.Trim()) );
	std::string operation = "Create new type " + type.getName();
	std::set< int > central;
	central.insert( 0 );
	frmConfirm -> initialise( TfrmLogin::CONFIGURE, operation.c_str(), central );
	if( frmConfirm->ShowModal() == mrOk
	 && type.saveRecord( LIMSDatabase::getCentralDb() ) ) {
		return LCDbObjects::records().findByID( type.getID() );
	}
	return NULL;
}

//---------------------------------------------------------------------------

