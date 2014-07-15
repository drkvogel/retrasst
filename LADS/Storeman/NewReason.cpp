//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewReason.h"
#include "StoreUtil.h"
#include "LCDbObject.h"
#include "TfrmConfirm.h"
#include "LCDbJob.h"
#include "SMLogin.h"
#include "LCDbCanned.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmNewReason *frmNewReason;

//---------------------------------------------------------------------------

__fastcall TfrmNewReason::TfrmNewReason(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------
void __fastcall TfrmNewReason::SaveClick(TObject *Sender)
{
	if( Util::validateText( TxtName, LblName )
	 && Util::validateText( TxtFull, LblFull ) ) {
		ModalResult = mrOk;
	}
}

//---------------------------------------------------------------------------

bool TfrmNewReason::init( int groupID ) {
	const LCDbObject * group = LCDbObjects::records().findByID( groupID );
	if( group == NULL ) {
		textID = 0;
		return false;
	}
	textID = groupID;
	lblReasonGroup->Caption = group->getDescription().c_str();
	TxtName->Text = "";
	TxtFull->Text = "";
	ActiveControl = TxtName;
	return true;
}

//---------------------------------------------------------------------------

const LCDbCanned * TfrmNewReason::createRecord() {
	LCDbCanned reason( textID );
	reason.setName( AnsiString( TxtName->Text.Trim() ).c_str() );
	reason.setDescription( AnsiString( TxtFull->Text.Trim() ).c_str() );
	AnsiString operation = "Create new " + AnsiString( lblReasonGroup->Caption );
	std::set< int > central;
	central.insert( 0 );
	frmConfirm -> initialise( TfrmSMLogin::CONFIGURE, operation.c_str(), central );
	if( frmConfirm->ShowModal() == mrOk && reason.saveRecord( LIMSDatabase::getCentralDb() ) ) {
		return LCDbCanneds::records().findByID( reason.getID() );
	}
	return NULL;
}

//---------------------------------------------------------------------------

