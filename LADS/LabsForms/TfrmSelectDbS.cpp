//---------------------------------------------------------------------

#include <vcl.h>

#include "TfrmSelectDbS.h"
#include "LCDbOperator.h"
#include "LCDbProject.h"
#include "LCDbAuditTrail.h"
#include "LIMSParams.h"

#pragma hdrstop

//---------------------------------------------------------------------

#pragma resource "*.dfm"

TfrmSelectDbSystem *frmSelectDbSystem;

//---------------------------------------------------------------------

__fastcall TfrmSelectDbSystem::TfrmSelectDbSystem( TComponent* AOwner ) : TForm( AOwner )
{}

//---------------------------------------------------------------------

void __fastcall TfrmSelectDbSystem::FormShow(TObject *Sender)
{
	rgDatabase->Items->Clear();
	for( LIMSDatabase::DbSystem item = LIMSDatabase::VLAB_LIVE; item < LIMSDatabase::SYSTEM_COUNT; ++ item ) {
		rgDatabase->Items->Add( LIMSDatabase::getDescription( item ).c_str() );
	}
	rgDatabase->ItemIndex = LIMSParams::instance().getDbSystem();
}

//---------------------------------------------------------------------------

void __fastcall TfrmSelectDbSystem::BitBtn1Click(TObject *Sender)
{
	LIMSDatabase::DbSystem system = LIMSDatabase::getCurrentSystem();
	LIMSDatabase::DbSystem selected = LIMSDatabase::DbSystem( rgDatabase -> ItemIndex );
	if( system != selected && selected != LIMSDatabase::UNKNOWN
	&& (system == LIMSDatabase::UNKNOWN || Application -> MessageBox(
			 L"Are you sure you want to switch database?", L"Warning", MB_YESNO ) == ID_YES) ) {
		LIMSDatabase::setCurrentSystem( selected );
	} else {
		rgDatabase -> ItemIndex = system;
	}
	LIMSParams::instance().setDbSystem( LIMSDatabase::getCurrentSystem() );
}

//---------------------------------------------------------------------------

