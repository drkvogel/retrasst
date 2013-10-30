//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "tdbSelection.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDbSelectionDialog *DbSelectionDialog;
//---------------------------------------------------------------------------
__fastcall TDbSelectionDialog::TDbSelectionDialog(TComponent* Owner)
	: TForm(Owner)
{
	m_didUserQuit = true;
	m_dbSelection = "none";
}
//---------------------------------------------------------------------------
void __fastcall TDbSelectionDialog::Button1Click(TObject *Sender)
{
	m_didUserQuit = false;
	if (dbCombobox->ItemIndex == 0)
		 m_dbSelection = L"ldbc";
	else if (dbCombobox->ItemIndex == 1)
		 m_dbSelection =  L"test_ldbc";
	else
		m_dbSelection = L"t_ldbc";
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TDbSelectionDialog::Button2Click(TObject *Sender)
{
	m_didUserQuit = true;
	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TDbSelectionDialog::FormShow(TObject *Sender)
{
	dbCombobox->AddItem(String("Live database"),0);
	dbCombobox->AddItem(String("Test database"),0);
	dbCombobox->AddItem(String("Development database"),0);

	#ifdef _DEBUG
		dbCombobox->ItemIndex = 2;
	#else
		dbCombobox->ItemIndex = 0;
	#endif
}
//---------------------------------------------------------------------------
