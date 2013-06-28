/*---------------------------------------------------------------------------
 *
 *	Password change form - normally invoked from log-in screen
 *
 *--------------------------------------------------------------------------*/

#include <vcl.h>
#include "TfrmPassword.h"
#include "LQuery.h"
#include "StringUtil.h"
#include "LIMSDatabase.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmPassword *frmPassword;

//---------------------------------------------------------------------------

__fastcall TfrmPassword::TfrmPassword(TComponent* Owner)
 : TForm(Owner), qCentral( LIMSDatabase::getCentralDb() )
{}

//---------------------------------------------------------------------------
//	Find the current user, display their name and wait for password
//---------------------------------------------------------------------------

void __fastcall TfrmPassword::FormShow(TObject *)
{
	user = LCDbOperators::records().get( LCDbOperators::getCurrentID() );
	ebUserName -> Text = user.getDescription().c_str();
	ebOldPass -> Clear();
	ebNewPass -> Clear();
	ebConfirmed -> Clear();
	okButton -> Enabled = false;
}

//---------------------------------------------------------------------------
//	Only allow save button when two new, identical passwords entered
//---------------------------------------------------------------------------

void __fastcall TfrmPassword::ebNewPassChange(TObject *)
{
	std::string oldPass = bcsToStd(ebOldPass -> Text);
	std::string newPass = bcsToStd(ebNewPass -> Text);
	okButton -> Enabled = user.matchPassword( oldPass )
					&& (oldPass.compare( newPass ) != 0)
					&& (ebConfirmed -> Text == ebNewPass -> Text)
					&& (newPass.length() >= 6)
					&& !user.hasUsedPassword( qCentral, newPass );
}

//---------------------------------------------------------------------------
//	Change the password - should be valid for 6 months; close the form
//---------------------------------------------------------------------------

void __fastcall TfrmPassword::okButtonClick(TObject *)
{
	std::pair< TDateTime, TDateTime > dates = user.getValidDates();
	if( dates.first > Now() )
		dates.first = Now();
	dates.second = Now() + 190;
	user.setValidDates( dates );  
	user.setPassword( bcsToStd(ebNewPass -> Text) );
	user.setActive( true );
	user.saveRecord( qCentral );
}

//---------------------------------------------------------------------------


