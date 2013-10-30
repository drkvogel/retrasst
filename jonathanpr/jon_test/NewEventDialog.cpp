//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewEventDialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TeventDialog *eventDialog;
//---------------------------------------------------------------------------
__fastcall TeventDialog::TeventDialog(TComponent* Owner)
	: TForm(Owner)
{
	m_userQuit = false;
}
//---------------------------------------------------------------------------
void __fastcall TeventDialog::SubmitButtonClick(TObject *Sender)
{
	if (nameEdit->Text.IsEmpty())
	{
		Application->MessageBox(L"You need to enter an event name", L"User Error", MB_OK);
	}
	m_eventName = AnsiString(nameEdit->Text).c_str();
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TeventDialog::CancelButtonClick(TObject *Sender)
{
	m_userQuit = true;
	this->Close();
}
//---------------------------------------------------------------------------

