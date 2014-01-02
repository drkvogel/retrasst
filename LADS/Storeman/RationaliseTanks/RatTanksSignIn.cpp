//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RatTanksSignIn.h"
// #include <regex>
#include "RatTanksdatabase.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmRatTankSignIn *frmRatTankSignIn;
//---------------------------------------------------------------------------
__fastcall TfrmRatTankSignIn::TfrmRatTankSignIn(TComponent* Owner)
	: TForm(Owner)
{
	m_didUserQuit = true;
}
//---------------------------------------------------------------------------

bool TfrmRatTankSignIn::isValidData()
{
	if (userNameTEdit->Text.IsEmpty())
	{
		BalloonHint1->Description = "You need to enter a username";
		TPoint R( userNameTEdit->Left + ((userNameTEdit->Width)/2), userNameTEdit->Top + userNameTEdit->Height );
		R = ClientToScreen(R);
		BalloonHint1->ShowHint(R);
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
		BalloonHint1->Description = "You need to enter a password";
		TPoint R( passwordTEdit->Left + ((passwordTEdit->Width)/2), passwordTEdit->Top + passwordTEdit->Height );
		R = ClientToScreen(R);
		BalloonHint1->ShowHint(R);
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------

std::string TfrmRatTankSignIn::getUserName()
{
	return std::string(AnsiString(userNameTEdit->Text.c_str()).c_str());
}
//---------------------------------------------------------------------------
std::string TfrmRatTankSignIn::getPassword()
{
//	make sql safe..change ' to " and no spaces. ? ? ?
//WHERE name = 'anytext OR 1 eq 1  /*'
	return std::string(AnsiString(passwordTEdit->Text.c_str()).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTankSignIn::CancelButtonClick(TObject *Sender)
{
	m_didUserQuit = true;
	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTankSignIn::SignInButtonClick(TObject *Sender)
{
	if (isValidData())
	{
		m_didUserQuit = false;
		this->Close();
	}
}
//---------------------------------------------------------------------------

