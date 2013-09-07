//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewSite.h"
#include "StoreUtil.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmNewSite *frmNewSite;

//---------------------------------------------------------------------------

__fastcall TfrmNewSite::TfrmNewSite(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TfrmNewSite::SaveClick(TObject *Sender)
{
	if( Util::validateText( TxtName, LblName )
	 && Util::validateText( TxtFull, LblFull ) )
	{
		AnsiString name = TxtName->Text.Trim();
		site.setName( name.c_str() );
		AnsiString full = TxtFull->Text.Trim();
		site.setFullName( full.c_str() );
		ModalResult = mrOk;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewSite::FormShow(TObject *Sender)
{
	TxtName->Text = "";
	TxtFull->Text = "";
	ActiveControl = TxtName;
}

//---------------------------------------------------------------------------

