//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "tCreateRetrievalJob.h"
#include "System.DateUtils.hpp"
#include <sstream>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCreateRetrievalJobForm *CreateRetrievalJobForm;
//---------------------------------------------------------------------------
__fastcall TCreateRetrievalJobForm::TCreateRetrievalJobForm(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void TCreateRetrievalJobForm::setUp(rationaliseTanks *rationaliseTanks,const std::map<std::string,std::string> &selectBox)
{
	m_SelectBox = selectBox;
	m_pRationaliseTanks = rationaliseTanks;

	std::stringstream ss;
	ss << DateTimeToUnix(Now());

	external_nameEdit->Text = L"defrag_" + String(ss.str().c_str());
}

//---------------------------------------------------------------------------
void __fastcall TCreateRetrievalJobForm::ButtonOKClick(TObject *Sender)
{
	std::string externalName = std::string(AnsiString(external_nameEdit->Text.c_str()).c_str());
	std::string description = std::string(AnsiString(descriptionMemo->Text.c_str()).c_str());

	if (externalName.empty())
	{
		BalloonHint1->Description = "You must enter the name of this task";
		TPoint R( external_nameEdit->Left + ((external_nameEdit->Width)/2), external_nameEdit->Top + external_nameEdit->Height );
		R = ClientToScreen(R);
		BalloonHint1->ShowHint(R);
		return;
	}

	if (description.empty())
	{
		BalloonHint1->Description = "You must enter a summery for what is needed to be done in this task";
		TPoint R( descriptionMemo->Left+ ((descriptionMemo->Width)/2), descriptionMemo->Top + descriptionMemo->Height );
		R = ClientToScreen(R);
		BalloonHint1->ShowHint(R);
		return;
	}

	std::string currentProject = m_pRationaliseTanks->getSelectedProject();
	std::string currentAliquot = m_pRationaliseTanks->getSelectedAliquot();

	m_pRationaliseTanks->createRetrievalList(externalName,description,currentProject, currentAliquot,m_SelectBox );
}
//---------------------------------------------------------------------------
void __fastcall TCreateRetrievalJobForm::ButtonCancelClick(TObject *Sender)
{
	this->Close();
}
//---------------------------------------------------------------------------
