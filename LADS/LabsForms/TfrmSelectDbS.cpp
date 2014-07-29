//---------------------------------------------------------------------

#include <vcl.h>

#include "TfrmSelectDbS.h"
#include "LCDbOperator.h"
#include "LCDbProject.h"
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
	LIMSDatabase::DbSystem item = LIMSDatabase::VLAB_LIVE;
	do {
		rgDatabase->Items->Add( LIMSDatabase::getDescription( item ).c_str() );
		item = LIMSDatabase::DbSystem( item + 1 );
	} while( item < LIMSDatabase::SYSTEM_COUNT );
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

