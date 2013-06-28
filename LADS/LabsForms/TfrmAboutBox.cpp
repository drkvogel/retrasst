/*--------------------------------------------------------------------------
 *
 *	LIMS about box - displays text defined elsewhere in the program
 *
 *-------------------------------------------------------------------------*/

#include <vcl.h>
#include <StrUtils.hpp>
#include "TfrmAboutBox.h"
#include "LIMSParams.h"

#pragma hdrstop
#pragma resource "*.dfm"

//---------------------------------------------------------------------

TfrmAboutBox *frmAboutBox;

__fastcall TfrmAboutBox::TfrmAboutBox(TComponent* AOwner) : TForm(AOwner)
{}

//---------------------------------------------------------------------------
//  Show program details; wait for user to click on 'OK'
//---------------------------------------------------------------------------

void __fastcall TfrmAboutBox::FormShow(TObject *)
{
	ProductName -> Caption = Application -> Title;
	const LIMSParams & params = LIMSParams::instance();
	Version -> Caption = String("Version ") + params.getProgVersion().c_str();
	ProgramIcon -> Picture -> Icon = Application -> Icon;
	String mmm = AnsiLeftStr( compiled, 3 );
	String dd = AnsiMidStr( compiled, 5, 2 );
	String yyyy = AnsiRightStr( compiled, 4 );
	Copyright -> Caption = "Copyright © CTSU, " + dd + ' ' + mmm + ' ' + yyyy;
	Comments -> Caption = "Produced by " + String( author )
						+ ", Core Programming, " + String( extension );
}

//---------------------------------------------------------------------------




