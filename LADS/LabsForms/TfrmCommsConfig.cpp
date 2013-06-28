//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TfrmCommsConfig.h"
#include "LIMSParams.h"
#include "LCDbAnalyser.h"
#include "LCDbProject.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmCommsConfig *frmCommsConfig;

//---------------------------------------------------------------------------

__fastcall TfrmCommsConfig::TfrmCommsConfig(TComponent* Owner) : TForm(Owner)
{
#ifdef _DEBUG
	rgDatabase -> ItemIndex = DbFamily::MIRROR_SYSTEM;
#else
	rgDatabase -> ItemIndex = DbFamily::LIVE_DATA;
#endif
}

//---------------------------------------------------------------------------

void __fastcall TfrmCommsConfig::FormShow(TObject *)
{
	cbAnalyser -> Clear();
	for( Range< LCDbAnalyser > ml = LCDbAnalysers::records(); ml.isValid(); ++ ml )
		if( ml -> getID() == LCDbAnalysers::getCurrentID() )
			cbAnalyser -> Text = ml -> getName();

	rgWorklist -> ItemIndex = LIMSParams::instance().useLocalWorklist();
}

//---------------------------------------------------------------------------

void __fastcall TfrmCommsConfig::okButtonClick(TObject *)
{
	LIMSParams & settings = LIMSParams::instance();
	settings.setLocalWorklist( rgWorklist -> ItemIndex );

	const LCDbAnalysers & analysers = LCDbAnalysers::records();
	const LCDbAnalyser * buddy = analysers.findByName( cbAnalyser -> Text );
	settings.setBuddyID( buddy == NULL ? 0 : buddy -> getID() );
}

//---------------------------------------------------------------------------

void __fastcall TfrmCommsConfig::cbAnalyserDropDown(TObject *Sender)
{
	cbAnalyser -> Clear();
	DbFamily().setDbSystem( qCentral -> OpenDatabase(), rgDatabase -> ItemIndex );
	LCDbAnalysers & analysers = LCDbAnalysers::records();
	analysers.read( qCentral, false );
	for( Range< LCDbAnalyser > ml = analysers; ml.isValid(); ++ ml )
		cbAnalyser -> Items -> Add( ml -> getName() );
}

//---------------------------------------------------------------------------

