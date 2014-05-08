//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RatTanksCreateRetrievalJob.h"
#include "System.DateUtils.hpp"
#include <sstream>
#include <map>
#include <string>
#include "RatTanksUtil_Holder.h"
#include "RatTanksCreateExercise.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmRatTankCreateRetrievalJob *frmRatTankCreateRetrievalJob;
//---------------------------------------------------------------------------
__fastcall TfrmRatTankCreateRetrievalJob::TfrmRatTankCreateRetrievalJob(TComponent* Owner)
	: TForm(Owner)
{
	m_result = "";
}
//---------------------------------------------------------------------------

void TfrmRatTankCreateRetrievalJob::fillExercideList(const std::string &default_cid)
{
	clearComboBox(RetrivelExerciseComboBox);

	m_pRationaliseTanks->getExerciseList(m_exerciseList);
	std::map<std::string, std::map<std::string,std::string> >::iterator it = m_exerciseList.begin();

	int count =0;
	int selected = -1;
	for (; it!=m_exerciseList.end(); ++it)
	{
		std::map<std::string,std::string> &proj = it->second;

		util_stdStringmapHolder *ValueHolder = new util_stdStringmapHolder(proj);
		String s = String(proj["external_name"].c_str()) + " -  " + String(proj["external_full"].c_str());

		RetrivelExerciseComboBox->AddItem(s,ValueHolder);
		if (default_cid == proj["object_cid"])
			selected = count;
		count++;
	}
	if (selected>-1)
		RetrivelExerciseComboBox->ItemIndex = selected;
}
//---------------------------------------------------------------------------

void TfrmRatTankCreateRetrievalJob::clearComboBox(TComboBox *cleanme)
{
	for (int i = 0; i < cleanme->Items->Count; i++)
		 delete cleanme->Items->Objects[i];
	cleanme->Clear();
}

//---------------------------------------------------------------------------
void TfrmRatTankCreateRetrievalJob::setUp(ratTanksCtrl *rationaliseTanks,const std::string &selectedBoxType_cid)
{
	m_selectedBoxType_cid = selectedBoxType_cid;
	m_pRationaliseTanks = rationaliseTanks;

	std::stringstream ss;
	ss << DateTimeToUnix(Now());
	external_nameEdit->Text = L"defrag_" + String(ss.str().c_str());

	fillExercideList("");
}

//---------------------------------------------------------------------------
void __fastcall TfrmRatTankCreateRetrievalJob::ButtonOKClick(TObject *Sender)
{
	std::string externalName = std::string(AnsiString(external_nameEdit->Text.c_str()).c_str());
	std::string description = std::string(AnsiString(descriptionMemo->Text.c_str()).c_str());
	std::string reason = std::string(AnsiString(reasonEdit->Text.c_str()).c_str());

	std::string Exercise_cid = "0";

	int index = RetrivelExerciseComboBox->ItemIndex;
	if (index != -1)
	{
		util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(RetrivelExerciseComboBox->Items->Objects[index]);
		Exercise_cid = mapHolder->m_map["object_cid"];
	}

	if ((!RetrivelExerciseComboBox->Text.IsEmpty()) && (Exercise_cid == "0"))
	{
		BalloonHint1->Description = "You have entered an invalid exercise task name,\n please set it to one of the available ones in\n the dropdown or clear it for being part of no exercise";

		TPoint R( RetrivelExerciseComboBox->Left + ((RetrivelExerciseComboBox->Width)/2), RetrivelExerciseComboBox->Top + RetrivelExerciseComboBox->Height );
		R = ClientToScreen(R);
		BalloonHint1->ShowHint(R);
		return;
	}

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
		BalloonHint1->Description = "You must enter a summery for what is needed to\n be done in this task";
		TPoint R( descriptionMemo->Left+ ((descriptionMemo->Width)/2), descriptionMemo->Top + descriptionMemo->Height );
		R = ClientToScreen(R);
		BalloonHint1->ShowHint(R);
		return;
	}

	std::string currentProject = m_pRationaliseTanks->getSelectedProject();
	std::string currentAliquot = m_pRationaliseTanks->getSelectedAliquot();

	try
	{
		external_nameEdit->Enabled = false;
		descriptionMemo->Enabled = false;
		ButtonCancel->Enabled = false;
		ButtonOK->Enabled = false;

		m_result = "OK";
		m_pRationaliseTanks->createRetrievalList(externalName,description,currentProject,reason,currentAliquot,m_selectedBoxType_cid,ProgressBar,Exercise_cid);
		String msg = L"Reterval list saved.";
		Application->MessageBox(msg.c_str(), L"Success", MB_OK);
		this->Close();
	}
	catch(String e)
	{
		String err = L"There was a problem generating the Reterval list." + e;
		Application->MessageBox(err.c_str(), L"Error", MB_OK);
		this->Close();
		m_result = "Failed";
	}
	catch(...)
	{
		String err = L"There was a problem generating the Reterval list.";
		Application->MessageBox(err.c_str(), L"Error", MB_OK);
		this->Close();
		m_result = "Failed";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmRatTankCreateRetrievalJob::ButtonCancelClick(TObject *Sender)
{
	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTankCreateRetrievalJob::New_Retrival_exercise_ButtonClick(TObject *Sender)
{
	std::unique_ptr<TfrmRatTanksRetrivalExercise> retrivalExerciseDialog = std::unique_ptr<TfrmRatTanksRetrivalExercise>( new TfrmRatTanksRetrivalExercise(this) );

	retrivalExerciseDialog->setUp(m_pRationaliseTanks,m_exerciseList);
	retrivalExerciseDialog->ShowModal();

	if (retrivalExerciseDialog->didUserQuit())
		return;

	std::string object_cid = retrivalExerciseDialog->getObject_cid();
	fillExercideList(object_cid);
}
//---------------------------------------------------------------------------

