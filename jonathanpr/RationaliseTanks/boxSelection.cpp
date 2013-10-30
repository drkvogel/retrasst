//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "boxSelection.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#include "util_Holder.h"

TtBoxSelection *tBoxSelection;

const std::string TtBoxSelection::DEFINE_RESULT_YES = "YES";
const std::string TtBoxSelection::DEFINE_RESULT_NO = "NO";
const std::string TtBoxSelection::DEFINE_RESULT_CANCEL = "CANCEL";
const std::string TtBoxSelection::DEFINE_RESULT_NONE = "NONE";

//---------------------------------------------------------------------------
__fastcall TtBoxSelection::TtBoxSelection(TComponent* Owner)
	: TForm(Owner)
{
	m_result = DEFINE_RESULT_NONE;
}
//---------------------------------------------------------------------------
void __fastcall TtBoxSelection::YesButtonClick(TObject *Sender)
{
	m_result = DEFINE_RESULT_YES;
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TtBoxSelection::NoButtonClick(TObject *Sender)
{
	m_result = DEFINE_RESULT_NO;
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TtBoxSelection::CancelButtonClick(TObject *Sender)
{
	m_result = DEFINE_RESULT_CANCEL;
	this->Close();
}
//---------------------------------------------------------------------------
void TtBoxSelection::setUp(std::map<std::string,std::map<std::string,std::string> > &aliquotsBoxTypes, std::string &defaultBox)
{
	m_aliquotsBoxTypes = aliquotsBoxTypes;
	bool hasDefault = false;
	std::map<std::string,std::map<std::string,std::string> >::iterator itBoxTypes = aliquotsBoxTypes.begin();
	for (;itBoxTypes!=aliquotsBoxTypes.end();++itBoxTypes)
	{
		if (!hasDefault)
		{
			if (itBoxTypes->first == defaultBox)
				hasDefault = true;
		}

		util_stdStringmapHolder *ValueHolder = new util_stdStringmapHolder(itBoxTypes->second);
		String s = String(itBoxTypes->first.c_str()); //external_name
		boxTypeComboBox->AddItem(s,ValueHolder);
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

void __fastcall TtBoxSelection::boxTypeComboBoxChange(TObject *Sender)
{
	NoButton->Enabled = true;
}
//---------------------------------------------------------------------------

std::string TtBoxSelection::getBoxType()
{
	std::string external_name = AnsiString(boxTypeComboBox->Text.c_str()).c_str();

	std::map<std::string,std::map<std::string,std::string> >::iterator itBoxTypes = m_aliquotsBoxTypes.find(external_name);
	if (itBoxTypes == m_aliquotsBoxTypes.end())
		return std::string("");

	return itBoxTypes->second["box_type_cid"];
}
//---------------------------------------------------------------------------

std::string TtBoxSelection::getBoxSize()
{
	std::string external_name = AnsiString(boxTypeComboBox->Text.c_str()).c_str();

	std::map<std::string,std::map<std::string,std::string> >::iterator itBoxTypes = m_aliquotsBoxTypes.find(external_name);
	if (itBoxTypes == m_aliquotsBoxTypes.end())
		return std::string("");

	return itBoxTypes->second["box_size_cid"];
}
//---------------------------------------------------------------------------


