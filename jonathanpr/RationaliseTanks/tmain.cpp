//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <memory>
#include "tmain.h"
#include "tdbSelection.h"
#include "util_Holder.h"
#include "signIn.h"
#include "tCreateRetrievalJob.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	#ifdef _DEBUG
	ShowDebugButton->Visible = true;
	#endif

	DebugPanel->Height = 0;
	Form1->Height -= 190;

	m_bInitliaseTimer = true;
	TrackBarThreshold->Position = 5;
	ThreshHoldEdit->Text = L"5";
	m_bInitliaseTimer= false;

	Site_ComboBox->Enabled = false;
	Vessel_ComboBox->Enabled = false;
	Project_ComboBox->Enabled = false;
	Aliquot_ComboBox->Enabled = false;
	BoxSelection_ComboBox->Enabled = false;
	TrackBarThreshold->Enabled = false;
	DefragButton->Enabled = false;
	m_bShowingBoxSelectionDialog = false;
	Memo->Clear();
}
//---------------------------------------------------------------------------

void TForm1::fillBoxTypeComboBox(const std::string &default_boxType_cid)
{
	int indexa = Aliquot_ComboBox->ItemIndex;
	if (indexa == -1)
		return;

	int indexp = Project_ComboBox->ItemIndex;
	if (indexp == -1)
		return;

	util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(Project_ComboBox->Items->Objects[indexp]);
	std::string project_cid = mapHolder->m_map["project_cid"];

	mapHolder = static_cast<util_stdStringmapHolder * >(Aliquot_ComboBox->Items->Objects[indexa]);
	std::string aliquot_cid = mapHolder->m_map["object_cid"];
//fill comboBox with boxes which have this alquiot type
	clearComboBox(BoxSelection_ComboBox);
	try
	{
		int defaultIndex = -1;
		int count = 0;
		std::map<std::string, std::map<std::string,std::string> > aliquotLists;
		m_rationaliseTanks.getAvalibleBoxesForReBox(project_cid,aliquot_cid,aliquotLists);

		std::map<std::string, std::map<std::string,std::string> >::iterator it;
		for (it = aliquotLists.begin(); it != aliquotLists.end(); ++it)
		{
			std::map<std::string,std::string>  &data = it->second;
			util_stdStringmapHolder *ValueHolder = new util_stdStringmapHolder(data);
			String s = String(data["external_name"].c_str());
			BoxSelection_ComboBox->AddItem(s,ValueHolder);
			if (!default_boxType_cid.empty())
			{
				if (data["box_type_cid"] == default_boxType_cid)
					defaultIndex = count;
			}
			count++;
		}
		if ((aliquotLists.size() > 1) && (!default_boxType_cid.empty()) && (defaultIndex != -1))
		{
			BoxSelection_ComboBox->ItemIndex = defaultIndex;
//			BoxSelection_ComboBox->Text = String(default_boxType_cid.c_str());
			fillBoxDetails();
		}
		else if (aliquotLists.size() > 1)
		{
			BoxSelection_ComboBox->ItemIndex = 0;
			BoxSelection_ComboBox->Text = "Please select a box type";
			BoxInfoEdit->Text = L"";

			Memo->Clear();
			Memo->Lines->Add("Please select a box type in which to rebox the vials into from the drop down list");

			Site_ComboBox->Enabled = true;
			Vessel_ComboBox->Enabled = true;
			Project_ComboBox->Enabled = true;
			Aliquot_ComboBox->Enabled = true;
			BoxSelection_ComboBox->Enabled = true;
			BoxSelection_ComboBox->SetFocus();
			TrackBarThreshold->Enabled = true;
			DefragButton->Enabled = false;
		}
		if (aliquotLists.size() == 1)
		{
			BoxSelection_ComboBox->ItemIndex = 0;
			fillBoxDetails();
		}
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void TForm1::fillBoxDetails()
{
	try
	{
		int indexb = BoxSelection_ComboBox->ItemIndex;
		if (indexb == -1)
			return;

		Site_ComboBox->Enabled = true;
		Vessel_ComboBox->Enabled = true;
		Project_ComboBox->Enabled = true;
		Aliquot_ComboBox->Enabled = true;
		BoxSelection_ComboBox->Enabled = true;
		TrackBarThreshold->Enabled = true;
		TrackBarThreshold->SetFocus();
		DefragButton->Enabled = true;
		Memo->Clear();

		util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(BoxSelection_ComboBox->Items->Objects[indexb]);
		std::string external_name = mapHolder->m_map["external_name"];
		std::string description = mapHolder->m_map["description"];
		std::string box_type_cid = mapHolder->m_map["box_type_cid"];

		BoxInfoEdit->Text = String(description.c_str());

		mapHolder = static_cast<util_stdStringmapHolder * >(BoxSelection_ComboBox->Items->Objects[BoxSelection_ComboBox->ItemIndex]);
		std::string box_size_cid = mapHolder->m_map["box_size_cid"];

		bool canDefrag = m_rationaliseTanks.compileResults(box_size_cid);
		DefragButton->Enabled =  canDefrag;
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}

}
//---------------------------------------------------------------------------

void TForm1::fillAlquotComboBox()
{
	int index = Project_ComboBox->ItemIndex;
	if (index == -1)
		return;

	util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(Project_ComboBox->Items->Objects[index]);
	std::string project_cid = mapHolder->m_map["project_cid"];
//fill comboBox with Projects in vessel
	clearComboBox(Aliquot_ComboBox);
	try
	{
		std::map<std::string, std::map<std::string,std::string> > aliquotLists;
		m_rationaliseTanks.getAliquotList(project_cid,aliquotLists);

		std::map<std::string, std::map<std::string,std::string> >::iterator it;
		for (it = aliquotLists.begin(); it != aliquotLists.end(); ++it)
		{
			std::map<std::string,std::string>  &data = it->second;
			util_stdStringmapHolder *ValueHolder = new util_stdStringmapHolder(data);
			String s = String(data["external_name"].c_str()) + " -  " + String(data["external_full"].c_str());
			Aliquot_ComboBox->AddItem(s,ValueHolder);
		}
		Aliquot_ComboBox->ItemIndex = 0;
		if (aliquotLists.size() > 1)
		{
			Aliquot_ComboBox->Text = "Please select a aliquot";
			Memo->Clear();
			Memo->Lines->Add("Please select a aliquot from the drop down list");

			Site_ComboBox->Enabled = true;
			Vessel_ComboBox->Enabled = true;
			Project_ComboBox->Enabled = true;
			Aliquot_ComboBox->Enabled = true;
			Aliquot_ComboBox->SetFocus();
			BoxSelection_ComboBox->Enabled = false;
			TrackBarThreshold->Enabled = true;
			DefragButton->Enabled = false;
		}
		if (aliquotLists.size() == 1)
		{
			mapHolder = static_cast<util_stdStringmapHolder * >(Project_ComboBox->Items->Objects[Project_ComboBox->ItemIndex]);
			std::string project_cid = mapHolder->m_map["project_cid"];

			mapHolder = static_cast<util_stdStringmapHolder * >(Aliquot_ComboBox->Items->Objects[Aliquot_ComboBox->ItemIndex]);
			std::string aliquot_cid = mapHolder->m_map["object_cid"];

			int threshHold = getThreshHold();

			m_rationaliseTanks.analyseVessel(Memo,project_cid,aliquot_cid);
			m_rationaliseTanks.refineSearch(threshHold);
			Aliquot_ComboBox->ItemIndex = 0;

			//THIS CULLS!!!!
			if (!m_rationaliseTanks.checkForCombiningBoxesInTheAliquot(false))
			{
				Memo->Lines->Add("Cancelled");
				return;
			}
			fillBoxTypeComboBox(m_rationaliseTanks.getDefaultBoxType());
		}
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
}
//---------------------------------------------------------------------------
int TForm1::getThreshHold()
{
	String threshHold = ThreshHoldEdit->Text;
	std::string stdThreshHold = std::string();
	int ithreshhold = atoi(AnsiString(threshHold.c_str()).c_str());
	if (ithreshhold == 0)
		ithreshhold = 5;
	return ithreshhold;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormShow(TObject *Sender)
{
//on startup
	try
	{
		if (!m_rationaliseTanks.hasSelectedDB())
		{
			std::unique_ptr<TDbSelectionDialog> dbForm = std::unique_ptr<TDbSelectionDialog>( new TDbSelectionDialog(this) );

			dbForm->ShowModal();
			if (dbForm->didUserQuit())
			{
				this->Close();
				return;
			}
			String selectedDB = dbForm->getDBSelection();
			m_rationaliseTanks.initalise(selectedDB);

	//keep asking for sign in, untill user gets it right or quits.
			bool userNotQuit = true;
			bool userSignedIn = false;
			while (true)
			{
				userNotQuit = requestUserSignOn();

				if (!userNotQuit)
					break;
				userSignedIn = m_operatorSignedIn.hasSignedIn();

				if (!userSignedIn)
				{
					Application->MessageBox(L"Failed to authenticate.", L"Information", MB_OK);
				}
				else
					break;
			}
			if (!userSignedIn)
			{
				this->Close();
				return;
			}

			Vessel_ComboBox->Enabled = false;
			//if we get to here, we have access to the database
			fillSiteComboBox();
			return;
		}
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
		this->Close();
		return;
	}
}
//---------------------------------------------------------------------------

void TForm1::clearComboBox(TComboBox *cleanme)
{
	for (int i = 0; i < cleanme->Items->Count; i++)
		 delete cleanme->Items->Objects[i];
	cleanme->Clear();
}
//---------------------------------------------------------------------------

void TForm1::fillSiteComboBox()
{
	clearComboBox(Site_ComboBox);
	try
	{
		std::map<int,std::map<String,String> > newSiteLists;
		m_rationaliseTanks.getSiteList(newSiteLists);

		std::map<int,std::map<String,String> >::iterator it;
		for (it = newSiteLists.begin(); it != newSiteLists.end(); ++it)
		{
			std::map<String,String>  &data = it->second;
			util_mapHolder *ValueHolder = new util_mapHolder(data);
			String s = data[String(RationaliseTanks::database::DEFINE_OBJECT_NAME_EXTERNAL_NAME.c_str())] + " -  " + data[String(RationaliseTanks::database::DEFINE_OBJECT_NAME_EXTERNAL_FULL.c_str())];
			Site_ComboBox->AddItem(s,ValueHolder);
		}
		if (newSiteLists.size() > 0)
		{
			Site_ComboBox->ItemIndex = 0;
			Site_ComboBox->Text = "Please select a site";
			Site_ComboBox->Enabled = true;
			Site_ComboBox->SetFocus();
			Vessel_ComboBox->Enabled = false;
			Project_ComboBox->Enabled = false;
			Aliquot_ComboBox->Enabled = false;
			BoxSelection_ComboBox->Enabled = false;
			TrackBarThreshold->Enabled = false;
			DefragButton->Enabled = false;
			Memo->Clear();
			Memo->Lines->Add("Please select a site from the drop down list");
		}
		else
		{
			Site_ComboBox->Text = "No sites found!";
		}
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void TForm1::fillVesselComboBox()
{
	int index = Site_ComboBox->ItemIndex;
	if (index == -1)
		return;

	util_mapHolder *mapHolder = static_cast<util_mapHolder * >(Site_ComboBox->Items->Objects[index]);
	if (mapHolder == 0)
		return;

	clearComboBox(Vessel_ComboBox);
	try
	{
		std::map<int,std::map<String,String> > newVesselLists;
		m_rationaliseTanks.getVesslList(newVesselLists ,mapHolder->m_map);

		std::map<int,std::map<String,String> >::iterator it;
		for (it = newVesselLists.begin(); it != newVesselLists.end(); ++it)
		{
			std::map<String,String>  &data = it->second;
			util_mapHolder *ValueHolder = new util_mapHolder(data);
			String s = ""+ data[String(RationaliseTanks::database::DEFINE_OBJECT_NAME_EXTERNAL_NAME.c_str())] + " -  " + data[String(RationaliseTanks::database::DEFINE_OBJECT_NAME_EXTERNAL_FULL.c_str())];
			Vessel_ComboBox->AddItem(s,ValueHolder);
		}
		if (newVesselLists.size() > 0)
		{
			Vessel_ComboBox->ItemIndex = 0;
			Vessel_ComboBox->Text = "Please select a vessel";
			Memo->Clear();
			Memo->Lines->Add("Please select a vessel to scan from the drop down list");

			Site_ComboBox->Enabled = true;
			Vessel_ComboBox->Enabled = true;
			Vessel_ComboBox->SetFocus();
			Project_ComboBox->Enabled = false;
			Aliquot_ComboBox->Enabled = false;
			BoxSelection_ComboBox->Enabled = false;
			TrackBarThreshold->Enabled = false;
			DefragButton->Enabled = false;
			Project_ComboBox->Clear();
			Aliquot_ComboBox->Clear();
			BoxSelection_ComboBox->Clear();
			BoxInfoEdit->Text = L"";
		}
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
	Vessel_ComboBox->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Site_ComboBoxChange(TObject *Sender)
{
	fillVesselComboBox();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Vessel_ComboBoxChange(TObject *Sender)
{
	try
	{
		int index = Vessel_ComboBox->ItemIndex;
		if (index == -1)
			return;

		util_mapHolder *mapHolder = static_cast<util_mapHolder * >(Vessel_ComboBox->Items->Objects[index]);
		if (mapHolder == 0)
			return;

		Site_ComboBox->Enabled = true;
		Vessel_ComboBox->Enabled = true;

		DefragButton->Enabled = false;

		Project_ComboBox->Enabled = false;
		Aliquot_ComboBox->Enabled = false;
		BoxSelection_ComboBox->Enabled = false;

		Project_ComboBox->Clear();
		Aliquot_ComboBox->Clear();
		BoxSelection_ComboBox->Clear();
		BoxInfoEdit->Clear();

		TrackBarThreshold->Enabled = false;
		Memo->Clear();

		scan();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
	clearComboBox(Site_ComboBox);
	clearComboBox(Vessel_ComboBox);
	clearComboBox(Project_ComboBox);
	clearComboBox(Aliquot_ComboBox);
	clearComboBox(BoxSelection_ComboBox);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TrackBarThresholdChange(TObject *Sender)
{
	Timer1->Enabled = false;

	if (!m_bShowingBoxSelectionDialog)
	{
		ThreshHoldEdit->Text = IntToStr(TrackBarThreshold->Position);
		Timer1->Interval = 500;

		if (!m_bInitliaseTimer)
			Timer1->Enabled = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TrackBarTimer(TObject *Sender)
{
	Timer1->Enabled = false;

	if (BoxSelection_ComboBox->ItemIndex == -1)
		return;

//if no box selected... do nothing.
	if (BoxSelection_ComboBox->Text == L"Please select a box type")
		return;

	m_bShowingBoxSelectionDialog = true;

	util_stdStringmapHolder *mapHolder;

	mapHolder = static_cast<util_stdStringmapHolder * >(BoxSelection_ComboBox->Items->Objects[BoxSelection_ComboBox->ItemIndex]);
	std::string box_size_cid = mapHolder->m_map["box_size_cid"];

	mapHolder = static_cast<util_stdStringmapHolder * >(BoxSelection_ComboBox->Items->Objects[BoxSelection_ComboBox->ItemIndex]);
	std::string box_type_cid = mapHolder->m_map["box_type_cid"];

	mapHolder = static_cast<util_stdStringmapHolder * >(Project_ComboBox->Items->Objects[Project_ComboBox->ItemIndex]);
	std::string project_cid = mapHolder->m_map["project_cid"];

	mapHolder = static_cast<util_stdStringmapHolder * >(Aliquot_ComboBox->Items->Objects[Aliquot_ComboBox->ItemIndex]);
	std::string aliquot_cid = mapHolder->m_map["object_cid"];

	int threshHold = getThreshHold();

	try
	{
	//rebuild list with new threshold
		m_rationaliseTanks.refineSearch(threshHold);

		//THIS CULLS!!!!
		//IF user said yes to all last time, it will do that again
		//If user selected a box type
		if (!m_rationaliseTanks.checkForCombiningBoxesInTheAliquot(true))
		{
			m_bShowingBoxSelectionDialog = false;
			Memo->Lines->Add("Cancelled");
			return;
		}

		bool canDefrag = m_rationaliseTanks.compileResults(box_size_cid);
		DefragButton->Enabled =  canDefrag;
		m_bShowingBoxSelectionDialog = false;
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Project_ComboBoxChange(TObject *Sender)
{
	fillAlquotComboBox();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Aliquot_ComboBoxChange(TObject *Sender)
{
	int index = Project_ComboBox->ItemIndex;
	if (index == -1)
		return;
	index = Aliquot_ComboBox->ItemIndex;
	if (index == -1)
		return;

	util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(Project_ComboBox->Items->Objects[Project_ComboBox->ItemIndex]);
	std::string project_cid = mapHolder->m_map["project_cid"];

	mapHolder = static_cast<util_stdStringmapHolder * >(Aliquot_ComboBox->Items->Objects[Aliquot_ComboBox->ItemIndex]);
	std::string aliquot_cid = mapHolder->m_map["object_cid"];

	try
	{
		m_rationaliseTanks.analyseVessel(Memo,project_cid,aliquot_cid);

		int threshHold = getThreshHold();

		m_rationaliseTanks.refineSearch(threshHold);

		//THIS CULLS!!!!
		if (!m_rationaliseTanks.checkForCombiningBoxesInTheAliquot(false))
		{
			Memo->Lines->Add("Cancelled");
			return;
		}
		fillBoxTypeComboBox(m_rationaliseTanks.getDefaultBoxType());
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BoxSelection_ComboBoxChange(TObject *Sender)
{
	int indexp = BoxSelection_ComboBox->ItemIndex;
	if (indexp == -1)
		return;

	fillBoxDetails();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ShowDebugButtonClick(TObject *Sender)
{
	if (DebugPanel->Height == 0)
	{
		Form1->Height += 200;
		DebugPanel->Height = 200;
	}
	else
	{
		DebugPanel->Height = 0;
		Form1->Height -= 200;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DefragButtonClick(TObject *Sender)
{
	try
	{
		std::string status = m_rationaliseTanks.getProjectActivityStatus(m_rationaliseTanks.getSelectedProject());

		if ((status != "5") || (status != "6"))
		{
			if (status == "0")
				Application->MessageBox(L"The project can't be rationalised as it contains historic data and may be incomplete", L"Information", MB_OK);
			else if (status == "1")
				Application->MessageBox(L"The project can't be rationalised as samples are arriving for analysis", L"Information", MB_OK);
			else if (status == "2")
				Application->MessageBox(L"The project can't be rationalised as samples are arriving for storage", L"Information", MB_OK);
			else if (status == "3")
				Application->MessageBox(L"The project can't be rationalised as samples are arriving for analysis and storage", L"Information", MB_OK);
			else if (status == "4")
				Application->MessageBox(L"The project can't be rationalised as boxes are arriving for storage", L"Information", MB_OK);
			else
				Application->MessageBox(L"The project can't be rationalised", L"Information", MB_OK);
			return;
		}

		if (!requestUserSignOn())
			return; //user cancelled

		if (!m_operatorSignedIn.hasSignedIn())
		{
			Application->MessageBox(L"Failed to signin.", L"Information", MB_OK);
			return;
		}

		if (!m_operatorSignedIn.hasPermissionToRationalise(m_rationaliseTanks.getSelectedProject()))
		{
			Application->MessageBox(L"You do not have permission to rationalise the tanks.", L"Information", MB_OK);
			return;
		}

	// Do the retreavl job now
		std::auto_ptr<TCreateRetrievalJobForm> CreateRetrievalJobForm = std::auto_ptr<TCreateRetrievalJobForm>( new TCreateRetrievalJobForm(this) );

		util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(BoxSelection_ComboBox->Items->Objects[BoxSelection_ComboBox->ItemIndex]);

		CreateRetrievalJobForm->setUp(&m_rationaliseTanks,mapHolder->m_map);
		CreateRetrievalJobForm->ShowModal();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
}

//---------------------------------------------------------------------------

bool TForm1::requestUserSignOn()
{
// ASK FOR USER NAME/PASSWORD, return false if user quit.
	std::auto_ptr<TtSignIn> dbForm = std::auto_ptr<TtSignIn>( new TtSignIn(this) );

	dbForm->ShowModal();
	if (dbForm->didUserQuit())
		return false;

	std::string userName = dbForm->getUserName();
	std::string Password = dbForm->getPassword();

	m_rationaliseTanks.signIn(userName,Password,m_operatorSignedIn);
	return true;
}

//---------------------------------------------------------------------------

void TForm1::scan()
{
	int index = Vessel_ComboBox->ItemIndex;
	if (index == -1)
		return;

	Memo->Clear();
	Memo->Lines->Add("Searching....");

	String threshHold = ThreshHoldEdit->Text;
	std::string stdThreshHold = std::string();
	int ithreshhold = atoi(AnsiString(threshHold.c_str()).c_str());
	if (ithreshhold == 0)
		ithreshhold = 5;
	util_mapHolder *mapHolder = static_cast<util_mapHolder * >(Vessel_ComboBox->Items->Objects[index]);
	try
	{
		m_rationaliseTanks.scanVessel(mapHolder->m_map,Memo);

	//fill comboBox with Projects in vessel
		clearComboBox(Project_ComboBox);

		std::map<std::string, std::map<std::string,std::string> > projectsLists;
		m_rationaliseTanks.getProjectsList(projectsLists);

		std::map<std::string, std::map<std::string,std::string> >::iterator it;
		for (it = projectsLists.begin(); it != projectsLists.end(); ++it)
		{
			std::map<std::string,std::string>  &data = it->second;
			util_stdStringmapHolder *ValueHolder = new util_stdStringmapHolder(data);
			String s = String(data["external_name"].c_str()) + " -  " + String(data["external_full"].c_str());

//Check the activity flag.. put a note on it if you can't rationalise.
			std::string activityFlag = data["activity_flags"];
			if (!((activityFlag== "5") || (activityFlag== "6")))
				s = L"[Analyse only] " + s;
			Project_ComboBox->AddItem(s,ValueHolder);
		}
		if (projectsLists.size() > 1)
		{
			Project_ComboBox->ItemIndex = 0;
			Project_ComboBox->Text = "Please select a project";
			Memo->Clear();
			Memo->Lines->Add("Please select a project from the drop down list");

			Site_ComboBox->Enabled = true;
			Vessel_ComboBox->Enabled = false;
			Project_ComboBox->Enabled = true;
			Project_ComboBox->SetFocus();
			Aliquot_ComboBox->Enabled = false;
			BoxSelection_ComboBox->Enabled = false;
			BoxInfoEdit->Text = L"";
			TrackBarThreshold->Enabled = true;
			DefragButton->Enabled = false;
		}
		else if (projectsLists.size() == 1)
		{
			Project_ComboBox->ItemIndex = 0;
			fillAlquotComboBox();
		}
		else // == 0
		{
			Memo->Clear();
			Memo->Lines->Add(L"Vessel is empty.");
		}
		m_rationaliseTanks.analiseForDatabaseUpdates(debugMemo);
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

