//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RatTanksBoxSelection.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#include "RatTanksUtil_Holder.h"

TfrmRatTanksBoxSelection *tBoxSelection;

const std::string TfrmRatTanksBoxSelection::DEFINE_RESULT_YES = "YES";
const std::string TfrmRatTanksBoxSelection::DEFINE_RESULT_NO = "NO";
const std::string TfrmRatTanksBoxSelection::DEFINE_RESULT_CANCEL = "CANCEL";
const std::string TfrmRatTanksBoxSelection::DEFINE_RESULT_NONE = "NONE";

//---------------------------------------------------------------------------
__fastcall TfrmRatTanksBoxSelection::TfrmRatTanksBoxSelection(TComponent* Owner)
	: TForm(Owner)
{
/*REMOVED SYSTEM MENU QUIT: IF THEY DON'T CHOOSE A BOX TYPE OR CHOOSE TO
  COMBINE IT, WE ONLY HAVE TO KEEP ASKING.. SO, FORCE THEM TO CHOOSE*/
	m_result = DEFINE_RESULT_CANCEL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRatTanksBoxSelection::YesButtonClick(TObject *Sender)
{
	m_result = DEFINE_RESULT_YES;
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmRatTanksBoxSelection::NoButtonClick(TObject *Sender)
{
	m_result = DEFINE_RESULT_NO;
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmRatTanksBoxSelection::CancelButtonClick(TObject *Sender)
{
	m_result = DEFINE_RESULT_CANCEL;
	this->Close();
}
//---------------------------------------------------------------------------
void TfrmRatTanksBoxSelection::setUp(std::map<std::string,std::map<std::string,std::string> > &aliquotsBoxTypes, std::string &defaultBox)
{
	m_aliquotsBoxTypes = aliquotsBoxTypes;
	bool hasDefault = false;

	{
		std::map<std::string,std::map<std::string,std::string> >::iterator itBoxTypes = aliquotsBoxTypes.begin();
		for (;itBoxTypes!=aliquotsBoxTypes.end();++itBoxTypes)
		{
			if (!hasDefault)
			{
				if (itBoxTypes->first == defaultBox)
					hasDefault = true;
			}

			util_stdStringmapHolder *ValueHolder = new util_stdStringmapHolder(itBoxTypes->second);
			String s = String(String(itBoxTypes->second["external_name"].c_str())); //external_name
			boxTypeComboBox->AddItem(s,ValueHolder);
		}
	}
	if (hasDefault)
	{
		boxTypeComboBox->Text = String(defaultBox.c_str());
		NoButton->Enabled = true;
	}
	else
		boxTypeComboBox->Text = "Please select";

}
//---------------------------------------------------------------------------

void TfrmRatTanksBoxSelection::clearComboBox(TComboBox *cleanme)
{
	for (int i = 0; i < cleanme->Items->Count; i++)
		 delete cleanme->Items->Objects[i];
	cleanme->Clear();
}

//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksBoxSelection::boxTypeComboBoxChange(TObject *Sender)
{
	NoButton->Enabled = true;
}
//---------------------------------------------------------------------------

std::string TfrmRatTanksBoxSelection::getBoxType()
{
	int pos = boxTypeComboBox->ItemIndex;
	util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(boxTypeComboBox->Items->Objects[boxTypeComboBox->ItemIndex]);
	return mapHolder->m_map["box_type_cid"];
}
//---------------------------------------------------------------------------

std::string TfrmRatTanksBoxSelection::getBoxSize()
{
	int pos = boxTypeComboBox->ItemIndex;
	util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(boxTypeComboBox->Items->Objects[boxTypeComboBox->ItemIndex]);
	return mapHolder->m_map["box_size_cid"];
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksBoxSelection::FormDestroy(TObject *Sender)
{
	clearComboBox(boxTypeComboBox);
}
//---------------------------------------------------------------------------

