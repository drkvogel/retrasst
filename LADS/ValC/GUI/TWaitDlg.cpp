//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "TWaitDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TWaitDlg *WaitDlg;
//---------------------------------------------------------------------------
__fastcall TWaitDlg::TWaitDlg( TComponent* Owner )
	: TForm(Owner)
{
}

void __fastcall TWaitDlg::show( const std::string& msg )
{
	message->Text = msg.c_str();
	this->Show();
}
