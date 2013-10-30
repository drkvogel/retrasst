//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "signIn.h"
#include <regex>
#include "database.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TtSignIn *tSignIn;
//---------------------------------------------------------------------------
__fastcall TtSignIn::TtSignIn(TComponent* Owner)
	: TForm(Owner)
{
	m_didUserQuit = true;
}
//---------------------------------------------------------------------------

bool TtSignIn::isValidData()
{
	if (userNameTEdit->Text.IsEmpty())
	{
		Application->MessageBox(L"You need to enter a username",L"User error");
		return false;
	}
/****** FIND OUT WHAT THE FORMAT OF THE USER NAME AND PASSWORD IS *********
	if (!std::regex_match(m_UserName, std::regex("[A-Za-z0-9]+")))
	{
		MessageBox(L"Please check your username",L"User error");
		return false;
	}
***************************************************************************/

	if (passwordTEdit->Text.IsEmpty())
	{
		Application->MessageBox(L"You need to enter a password",L"User error");
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------

std::string TtSignIn::getUserName()
{
	return std::string(AnsiString(userNameTEdit->Text.c_str()).c_str());
}
//---------------------------------------------------------------------------
std::string TtSignIn::getPassword()
{
//	make sql safe..change ' to " and no spaces. ? ? ?
//WHERE name = 'anytext OR 1 eq 1  /*'
	return std::string(AnsiString(passwordTEdit->Text.c_str()).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TtSignIn::CancelButtonClick(TObject *Sender)
{
	m_didUserQuit = true;
	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TtSignIn::SignInButtonClick(TObject *Sender)
{
	if (isValidData())
	{
		m_didUserQuit = false;
		this->Close();
	}
}
//---------------------------------------------------------------------------

