//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RatTanksCreateExercise.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmRatTanksRetrivalExercise *frmRatTanksRetrivalExercise;
//---------------------------------------------------------------------------
__fastcall TfrmRatTanksRetrivalExercise::TfrmRatTanksRetrivalExercise(TComponent* Owner)
	: TForm(Owner)
{
	m_didUserQuit = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRatTanksRetrivalExercise::CancelButtonClick(TObject *Sender)
{
	m_didUserQuit = true;
	this->Close();
}

//---------------------------------------------------------------------------
void TfrmRatTanksRetrivalExercise::setUp(ratTanksCtrl *rationaliseTanks,std::map<std::string, std::map<std::string,std::string> > &m_exerciseList)
{
	m_pRationaliseTanks = rationaliseTanks;
	m_pExerciseList = &m_exerciseList;
}

//---------------------------------------------------------------------------
void __fastcall TfrmRatTanksRetrivalExercise::SaveButtonClick(TObject *Sender)
{
	if (!validate())
		return;

	m_external_name = AnsiString(NameEdit->Text.c_str()).c_str();
	m_discription = AnsiString(Description_Memo->Text.c_str()).c_str();

	m_object_cid = -1;

	try
	{
		m_object_cid = m_pRationaliseTanks->CreateRetrivalExercise(m_external_name,m_discription);
		m_didUserQuit = false;
		this->Close();
	}
	catch (String e)
	{
		String displayString = String("Failed to create retrival exercise- \n\n Reason :\n") + e.c_str();
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
	catch(...)
	{
		String displayString = String("Failed to create retrival exercise");
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

bool TfrmRatTanksRetrivalExercise::validate()
{
	std::string external_name = AnsiString(NameEdit->Text.c_str()).c_str();

	if (external_name.empty())
	{
		BalloonHint1->Description = "You need to name your Exercise";
		TPoint R( NameEdit->Left + ((NameEdit->Width)/2), NameEdit->Top + NameEdit->Height );
		R = ClientToScreen(R);
		BalloonHint1->ShowHint(R);
		return false;
	}

	std::map<std::string, std::map<std::string,std::string> >::iterator it = m_pExerciseList->begin();
	for (; it!=m_pExerciseList->end(); ++it)
	{
		std::map<std::string,std::string> &proj = it->second;
		std::string ext = proj["external_name"];

		if (ext == external_name)
		{
			BalloonHint1->Description = "The name needs to be unique";
			TPoint R( NameEdit->Left + ((NameEdit->Width)/2), NameEdit->Top + NameEdit->Height );
			R = ClientToScreen(R);
			BalloonHint1->ShowHint(R);
			return false;
		}
	}

	if (Description_Memo->Text.IsEmpty())
	{
		BalloonHint1->Description = "You need to give your exercise a discription";
		TPoint R( Description_Memo->Left + ((Description_Memo->Width)/2), Description_Memo->Top + Description_Memo->Height );
		R = ClientToScreen(R);
		BalloonHint1->ShowHint(R);
		return false;
	}
	return true;
}


