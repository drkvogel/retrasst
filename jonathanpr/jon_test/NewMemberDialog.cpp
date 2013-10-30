//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewMemberDialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TNewMemberDlg *NewMemberDlg;
//---------------------------------------------------------------------------
__fastcall TNewMemberDlg::TNewMemberDlg(TComponent* Owner)
	: TForm(Owner)
{
	m_userQuit = false;
}
//---------------------------------------------------------------------------
void __fastcall TNewMemberDlg::SubmitButtonClick(TObject *Sender)
{
	if (nameEdit->Text.IsEmpty())
	{
		Application->MessageBox(L"You need to enter a name for the new member", L"User Error", MB_OK);
	}
	m_memberName = AnsiString(nameEdit->Text).c_str();
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TNewMemberDlg::CancelButtonClick(TObject *Sender)
{
	m_userQuit = true;
	this->Close();
}
//---------------------------------------------------------------------------
