//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "passwordForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPassword *Password;
//---------------------------------------------------------------------------
__fastcall TPassword::TPassword(TComponent* Owner)
	: TForm(Owner)
{
	m_userQuit = false;
}
//---------------------------------------------------------------------------
void __fastcall TPassword::Button1Click(TObject *Sender)
{
	if (passwordEdit->Text.IsEmpty())
	{
		Application->MessageBox(L"You need to enter a password", L"User Error", MB_OK);
	}
	m_password = AnsiString(passwordEdit->Text).c_str();
	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TPassword::Button2Click(TObject *Sender)
{
	m_userQuit = true;
	this->Close();
}
//---------------------------------------------------------------------------


void __fastcall TPassword::Button1Exit(TObject *Sender)
{
 		Application->MessageBox(L"!!!!", L"User Error", MB_OK);

}

