//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RatTanksBoxType.h"
//---------------------------------------------------------------------------
#include <map>
#include "RatTanksUtil_Holder.h"

#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmRatTanksBoxType *frmRatTanksBoxType;


//---------------------------------------------------------------------------
__fastcall TfrmRatTanksBoxType::TfrmRatTanksBoxType(TComponent* Owner)
	: TForm(Owner)
{
	m_bDidUserQuit = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRatTanksBoxType::Cancel_ButtonClick(TObject *Sender)
{
/*
	m_chosen_box_type_cid = "";
	m_chosen_box_size_cid = "";
	m_chosen_box_name = "";
*/	m_bDidUserQuit = true;
	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksBoxType::OK_ButtonClick(TObject *Sender)
{
	m_bDidUserQuit = false;
	this->Close();
}
//---------------------------------------------------------------------------

void TfrmRatTanksBoxType::setUp(ratTanksCtrl &rationaliseTanks,const std::string &project_cid,const std::string &aliquot_cid,const std::string &ProjectName, const std::string &aliquot_name)
{
	SimpleNameEdit->Text = "";
	DescriptionEdit->Text = "";;
	ExampleBoxEdit->Text = "";;
	clearComboBox(FirstAliquotComboBox);
	clearComboBox(FormationComboBox);
	clearComboBox(BoxType_ComboBox);
	clearComboBox(ThirdAliquotComboBox);
	clearComboBox(SecondAliquotComboBox);
	BoxType_ComboBox->ItemIndex = -1;
	EditButton->Caption = "New";

	m_ProjectName = ProjectName;
	m_aliquot_name = aliquot_name;
	m_rationaliseTanks = &rationaliseTanks;
	m_projectcid = project_cid;
	m_aliquot_cid = aliquot_cid;

	m_rationaliseTanks->getProjectBoxSizeList(m_boxSizes);
	m_rationaliseTanks->getAllAliquotsFromCentral(m_aliquotsInProject);
	std::map<std::string,std::map<std::string,std::string> > BoxTypesInDefrag = m_rationaliseTanks->getBoxTypes(m_projectcid,m_aliquot_cid);

	m_default_box_type_cid = "";
	m_default_box_size_cid = "";

//if something is already selected, default to that,
	if (!m_chosen_box_type_cid.empty())
	{
		m_default_box_type_cid = m_chosen_box_type_cid;
		m_default_box_size_cid = m_chosen_box_size_cid;
	}
//other wise, if only 1 box type, then defualt for that.
	else if (BoxTypesInDefrag.size() == 1)
	{
		//only one box type, set this as defualt
		std::map<std::string, std::map<std::string,std::string> >::iterator it;
		it = BoxTypesInDefrag.begin();

		m_default_box_type_cid = it->second["box_type_cid"];
		m_default_box_size_cid = it->second["default_box_size_cid"];
		//						"external_name"
	}

//   std::map<std::string,std::string> m_DefualtBoxSizeInfo = m_rationaliseTanks->getBoxSize(std::string &box_size_cid)

	clearComboBox(BoxType_ComboBox);
	int count =0;
	int defaultIndex = -1;
	std::string defaultSampleName = "";
	std::map<std::string, std::map<std::string,std::string> > aliquotLists;
	m_rationaliseTanks->getAvalibleBoxesForReBox(project_cid,aliquot_cid,aliquotLists);

	std::map<std::string, std::map<std::string,std::string> >::iterator it;
	for (it = aliquotLists.begin(); it != aliquotLists.end(); ++it)
	{
		std::map<std::string,std::string>  &data = it->second;
		util_stdStringmapHolder *ValueHolder = new util_stdStringmapHolder(data);
		String s = String(data["external_name"].c_str());
		BoxType_ComboBox->AddItem(s,ValueHolder);
		if (!m_default_box_type_cid.empty())
		{
			if (data["box_type_cid"] == m_default_box_type_cid)
			{
				defaultIndex = count;
			}
		}
		count++;
	}
	if (/*(aliquotLists.size() > 1) &&*/ (!m_default_box_type_cid.empty()) && (defaultIndex != -1))
	{
		BoxType_ComboBox->ItemIndex = defaultIndex;
		OK_Button->Enabled = true;
		fillDetails();
	}
	else
	{
		OK_Button->Enabled = false;
		BoxType_ComboBox->ItemIndex = -1;
		BoxType_ComboBox->Text = "Please select box type";
	}
}
//---------------------------------------------------------------------------

void TfrmRatTanksBoxType::clearComboBox(TComboBox *cleanme)
{
	for (int i = 0; i < cleanme->Items->Count; i++)
		 delete cleanme->Items->Objects[i];
	cleanme->Clear();
}

//---------------------------------------------------------------------------
void TfrmRatTanksBoxType::fillDetails()
{
//if button not edit and combo selection index -1.. no nothing
	if ((BoxType_ComboBox->ItemIndex == -1) && (EditButton->Caption != "Save"))
		return;

	clearComboBox(FormationComboBox);
	clearComboBox(ThirdAliquotComboBox);
	clearComboBox(SecondAliquotComboBox);
	clearComboBox(FirstAliquotComboBox);

	if (EditButton->Caption == "Save")
	{
		ExampleBoxEdit->Text = String(m_ProjectName.c_str()) + L" " + SimpleNameEdit->Text + L" 65677"; //last one is a sample box_cid

		fillAliquotComboBoxs(m_aliquot_cid,"","");
		fillFormationComboBox("");
	}
	else
	{
		if (BoxType_ComboBox->ItemIndex == -1)
			return;
		util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(BoxType_ComboBox->Items->Objects[BoxType_ComboBox->ItemIndex]);

		std::string external_name = mapHolder->m_map["external_name"];
		std::string description = mapHolder->m_map["description"];
		std::string box_size_cid = mapHolder->m_map["box_size_cid"];
		std::string box_cid = mapHolder->m_map["box_type_cid"];
		std::string aliquot_type1 = mapHolder->m_map["aliquot_type1"];
		std::string aliquot_type2 = mapHolder->m_map["aliquot_type2"];
		std::string aliquot_type3 = mapHolder->m_map["aliquot_type3"];

		m_chosen_box_type_cid = box_cid;
		m_chosen_box_size_cid = box_size_cid;
		m_chosen_box_name = external_name;

		SimpleNameEdit->Text = String(mapHolder->m_map["external_name"].c_str());
		DescriptionEdit->Text = String(description.c_str());
		ExampleBoxEdit->Text = String(m_ProjectName.c_str()) + L" " + SimpleNameEdit->Text + L" 65677"; //last one is a sample box_cid

		fillAliquotComboBoxs(aliquot_type1,aliquot_type2,aliquot_type3);
		fillFormationComboBox(box_size_cid);
	}
}
//---------------------------------------------------------------------------

void TfrmRatTanksBoxType::fillAliquotComboBoxs(const std::string &aliquot_type1,const std::string &aliquot_type2,const std::string &aliquot_type3)
{

	int count=0;
	int aliquot1_index = -1;
	int aliquot2_index = -1;
	int aliquot3_index = -1;
	std::map<std::string, std::map<std::string,std::string> >::iterator it;
	for (it = m_aliquotsInProject.begin(); it != m_aliquotsInProject.end(); ++it)
	{
		std::map<std::string,std::string>  &data = it->second;
		util_stdStringmapHolder *ValueHolder = new util_stdStringmapHolder(data);
		String s = String(data["external_name"].c_str());
		FirstAliquotComboBox->AddItem(s,ValueHolder);
		ValueHolder = new util_stdStringmapHolder(data);
		SecondAliquotComboBox->AddItem(s,ValueHolder);
		ValueHolder = new util_stdStringmapHolder(data);
		ThirdAliquotComboBox->AddItem(s,ValueHolder);
	}
 //We have to do this in a second pass as it's a sorted list.
//so some reason, trying off sorting, selecting the one you want then trying it back
//on results in the wrong item being selected.
	for (int i=0;i<FirstAliquotComboBox->Items->Count;i++)
	{
		util_stdStringmapHolder *ValueHolder = static_cast<util_stdStringmapHolder * > (FirstAliquotComboBox->Items->Objects[i]);
		std::string cid = ValueHolder->m_map["object_cid"];
		if (cid == aliquot_type1)
			aliquot1_index = count;
		if (cid == aliquot_type2)
			aliquot2_index = count;
		if (cid == aliquot_type3)
			aliquot3_index = count;
		count++;
	}

	FirstAliquotComboBox->ItemIndex = aliquot1_index;
	if (aliquot1_index == -1)
		FirstAliquotComboBox->Text="";
	SecondAliquotComboBox->ItemIndex = aliquot2_index;
	if (aliquot2_index == -1)
		SecondAliquotComboBox->Text="";
	ThirdAliquotComboBox->ItemIndex = aliquot3_index;
	if (aliquot3_index == -1)
		ThirdAliquotComboBox->Text="";
}
//---------------------------------------------------------------------------

void TfrmRatTanksBoxType::fillFormationComboBox(const std::string &box_size_cid)
{
	int count=0;
	int defaultIndex = -1;
	std::map<std::string, std::map<std::string,std::string> >::iterator it2;
	for (it2 = m_boxSizes.begin(); it2 != m_boxSizes.end(); ++it2)
	{
		std::map<std::string,std::string>  &data = it2->second;
		util_stdStringmapHolder *ValueHolder = new util_stdStringmapHolder(data);
		String s = String(data["description"].c_str());
		FormationComboBox->AddItem(s,ValueHolder);

		if (!box_size_cid.empty())
		{
			if (box_size_cid == it2->first)
				defaultIndex = count;
		}
		count++;
	}

	FormationComboBox->ItemIndex = defaultIndex;
	if (defaultIndex == -1)
		FormationComboBox->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksBoxType::BoxType_ComboBoxChange(TObject *Sender)
{
	FirstAliquotComboBox->Enabled = false;
	SecondAliquotComboBox->Enabled = false;
	ThirdAliquotComboBox->Enabled = false;
	FormationComboBox->Enabled = false;
	ExampleBoxEdit->Enabled = false;
	DescriptionEdit->Enabled = false;
	SimpleNameEdit->Enabled = false;

	EditButton->Caption = "New";
	OK_Button->Enabled = true;
//	m_chosen_box_type_cid = "";
//	m_chosen_box_size_cid = "";
//	m_chosen_box_name = "";

	SimpleNameEdit->Text = "";
	fillDetails();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksBoxType::SimpleNameEditChange(TObject *Sender)
{
	if (EditButton->Caption == L"New")
		return;
	fillDetails();
}
//---------------------------------------------------------------------------

bool TfrmRatTanksBoxType::areFormDetailsFilledIn()
{
	BalloonHint->HideHint();
	bool failed = false;

	if (SimpleNameEdit->Text.IsEmpty())
		failed = true;
	else
	{
		for (int i=0;i<BoxType_ComboBox->GetCount();i++)
		{
			if (SimpleNameEdit->Text == BoxType_ComboBox->Items->ToStringArray()[i])
			{
				failed = true;
				break;
			}
		}
	}
	if (failed)
	{
		BalloonHint->Description = "Please specifty unique name";
		TPoint R( SimpleNameEdit->Left + ((SimpleNameEdit->Width)/2), SimpleNameEdit->Top + SimpleNameEdit->Height );
		R = GroupBox1->ClientToScreen(R);
		BalloonHint->ShowHint(R);
		SimpleNameEdit->SetFocus();
		return false;
	}

	if (DescriptionEdit->Text.IsEmpty())
		DescriptionEdit->Text = String(m_ProjectName.c_str()) + " " + SimpleNameEdit->Text;

	bool hasReBoxAliquot = false;

	if (FirstAliquotComboBox->ItemIndex == -1)
		failed = true;
	else
	{
		String Aliquot = FirstAliquotComboBox->Text;

		if (String(m_aliquot_name.c_str()) == Aliquot)
			hasReBoxAliquot = true;

		if (Aliquot.IsEmpty())
			failed = true;
		else
		{
			failed = true;
			for (int i=0;i<FirstAliquotComboBox->GetCount();i++)
			{
				if (FirstAliquotComboBox->Text == FirstAliquotComboBox->Items->ToStringArray()[i])
				{
					failed = false;
					break;
				}
			}
		}
	}
	if (failed)
	{
		BalloonHint->Description = "Please select one or more aliquot type";
		TPoint R( FirstAliquotComboBox->Left + ((FirstAliquotComboBox->Width)/2), FirstAliquotComboBox->Top + FirstAliquotComboBox->Height );
		R = GroupBox1->ClientToScreen(R);
		BalloonHint->ShowHint(R);
		FirstAliquotComboBox->SetFocus();
		return false;
	}

	String Aliquot = SecondAliquotComboBox->Text;
	if (!Aliquot.IsEmpty())
	{
 		if (String(m_aliquot_name.c_str()) == Aliquot)
			hasReBoxAliquot = true;
		failed = true;
		for (int i=0;i<SecondAliquotComboBox->GetCount();i++)
		{
			if (SecondAliquotComboBox->Text == SecondAliquotComboBox->Items->ToStringArray()[i])
			{
				failed = false;
				break;
			}
		}
	}

	if (failed)
	{
		BalloonHint->Description = "Aliquot type not valid";
		TPoint R( SecondAliquotComboBox->Left + ((SecondAliquotComboBox->Width)/2), SecondAliquotComboBox->Top + SecondAliquotComboBox->Height );
		R = GroupBox1->ClientToScreen(R);
		BalloonHint->ShowHint(R);
		SecondAliquotComboBox->SetFocus();
		return false;
	}

	Aliquot = ThirdAliquotComboBox->Text;
	if (!Aliquot.IsEmpty())
	{
 		if (String(m_aliquot_name.c_str()) == Aliquot)
			hasReBoxAliquot = true;
		failed = true;
		for (int i=0;i<ThirdAliquotComboBox->GetCount();i++)
		{
			if (ThirdAliquotComboBox->Text == ThirdAliquotComboBox->Items->ToStringArray()[i])
			{
				failed = false;
				break;
			}
		}
	}

	if (failed)
	{
		BalloonHint->Description = "Aliquot type not valid";
		TPoint R( ThirdAliquotComboBox->Left + ((ThirdAliquotComboBox->Width)/2), ThirdAliquotComboBox->Top + ThirdAliquotComboBox->Height );
		R = GroupBox1->ClientToScreen(R);
		BalloonHint->ShowHint(R);
		ThirdAliquotComboBox->SetFocus();
		return false;
	}

	if (!hasReBoxAliquot)
	{
		BalloonHint->Description = "You need to have one aliquot type of " + String(m_aliquot_name.c_str()) + " selected";
		TPoint R( FirstAliquotComboBox->Left + ((FirstAliquotComboBox->Width)/2), FirstAliquotComboBox->Top + FirstAliquotComboBox->Height );
		R = GroupBox1->ClientToScreen(R);
		BalloonHint->ShowHint(R);
		return false;
	}

	String Formation = FormationComboBox->Text;
	if (!Formation.IsEmpty())
	{
		failed = true;
		for (int i=0;i<FormationComboBox->GetCount();i++)
		{
			if (FormationComboBox->Text == FormationComboBox->Items->ToStringArray()[i])
			{
				failed = false;
				break;
			}
		}
	}
	else
		failed = true;
	if (failed)
	{
		BalloonHint->Description = "Please select appropiate formation type";
		TPoint R( FormationComboBox->Left + ((FormationComboBox->Width)/2), FormationComboBox->Top + FormationComboBox->Height );
		R = GroupBox1->ClientToScreen(R);
		BalloonHint->ShowHint(R);
		FormationComboBox->SetFocus();
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksBoxType::EditButtonClick(TObject *Sender)
{
	if (EditButton->Caption != "Save")
	{
// has user got permissions?
		if (!m_rationaliseTanks->getOperatorSignedIn()->hasPermissionToRationalise(m_projectcid))
		{
			String displayString = String("Your sign in account doesn't have permissions to create a new box type.");
			Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
			return;
		}

		FirstAliquotComboBox->Enabled = true;
		SecondAliquotComboBox->Enabled = true;
		ThirdAliquotComboBox->Enabled = true;
		FormationComboBox->Enabled = true;
		ExampleBoxEdit->Enabled = true;
		DescriptionEdit->Enabled = true;
		SimpleNameEdit->Enabled = true;
		OK_Button->Enabled = false;

		EditButton->Caption = "Save";

		SimpleNameEdit->Text = "";
		DescriptionEdit->Text = "";
		ExampleBoxEdit->Text = "";

		fillDetails();
	}
	else
	{
		if (!areFormDetailsFilledIn())
			return;

		String displayString = String("Press Yes if you would like to save this box type and use it to rebox with");
		int res = Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Save Box?", MB_YESNO);

		if (res == IDYES)
		{
			std::string box_size_cid = "0";
			std::string aliquottpye1 = "0";
			std::string aliquottpye2 = "0";
			std::string aliquottpye3 = "0";
			std::string extern_name = std::string(AnsiString(SimpleNameEdit->Text.c_str()).c_str());
			std::string description = std::string(AnsiString(DescriptionEdit->Text.c_str()).c_str());
			if (FirstAliquotComboBox->ItemIndex >= 0)
			{
				util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(FirstAliquotComboBox->Items->Objects[FirstAliquotComboBox->ItemIndex]);
				aliquottpye1 = mapHolder->m_map["object_cid"];
			}
			if (SecondAliquotComboBox->ItemIndex >= 0)
			{
				util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(SecondAliquotComboBox->Items->Objects[SecondAliquotComboBox->ItemIndex]);
				aliquottpye2 = mapHolder->m_map["object_cid"];
			}
			if (ThirdAliquotComboBox->ItemIndex >= 0)
			{
				util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(ThirdAliquotComboBox->Items->Objects[ThirdAliquotComboBox->ItemIndex]);
				aliquottpye3 = mapHolder->m_map["object_cid"];
			}
			if (FormationComboBox->ItemIndex >= 0)
			{
				util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(FormationComboBox->Items->Objects[FormationComboBox->ItemIndex]);
				box_size_cid = mapHolder->m_map["box_size_cid"];
			}
			m_chosen_box_type_cid = m_rationaliseTanks->addBoxContext(extern_name,description,box_size_cid,aliquottpye1,aliquottpye2,aliquottpye3);
			m_chosen_box_size_cid = box_size_cid;
			m_chosen_box_name = extern_name;
			m_bDidUserQuit = false;
			this->Close();
		}
		else
		{

		}
	}
}
//---------------------------------------------------------------------------


