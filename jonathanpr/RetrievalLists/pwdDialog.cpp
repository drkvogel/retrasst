//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "pwdDialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPasswordDialog *PasswordDialog;
//---------------------------------------------------------------------------
__fastcall TPasswordDialog::TPasswordDialog(TComponent* Owner)
	: TForm(Owner), m_userQuit(false)
{

}
//---------------------------------------------------------------------------
void __fastcall TPasswordDialog::SubmitButtonClick(TObject *Sender)
{
	if (passwordEdit->Text.IsEmpty())
	{
		Application->MessageBox(L"You need to enter a password", L"User Error", MB_OK);
	}
	m_password = AnsiString(passwordEdit->Text).c_str();
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TPasswordDialog::CancelButtonClick(TObject *Sender)
{
	m_userQuit = true;
	this->Close();
}
//---------------------------------------------------------------------------
