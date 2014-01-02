//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <memory>
#include <sstream>
#include "RatTanksMainDialog.h"
#include "RatTanksdbSelection.h"
#include "RatTanksUtil_Holder.h"
#include "RatTanksSignIn.h"
#include "RatTanksCreateRetrievalJob.h"
#include "LIMSDatabase.h"
#include "LCDbOperator.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmRatTanksMainDialog *frmRatTanksMainDialog;


//---------------------------------------------------------------------------
__fastcall TfrmRatTanksMainDialog::TfrmRatTanksMainDialog(TComponent* Owner)
	: TForm(Owner)
{
	#ifdef _DEBUG
	ShowDebugButton->Visible = true;
	#endif

	DebugPanel->Height = 0;
	frmRatTanksMainDialog->Height -= 190;
	m_bInitliaseTimer = true;
	TrackBarThreshold->Position = 5;
	ThreshHoldEdit->Text = L"5";
	m_bInitliaseTimer= false;

	reSetAll();

	m_BoxTypeDialog = std::unique_ptr<TfrmRatTanksBoxType>( new TfrmRatTanksBoxType(this) );
	m_PleaseWaitDialog = std::unique_ptr<TfrmRatTankPleaseWait>( new TfrmRatTankPleaseWait(this) );
}

//---------------------------------------------------------------------------

void TfrmRatTanksMainDialog::reSetAll()
{
	Site_ComboBox->Enabled = false;
	Vessel_ComboBox->Enabled = false;
	Project_ComboBox->Enabled = false;
	Aliquot_ComboBox->Enabled = false;
	TrackBarThreshold->Enabled = false;
	BoxType_Button->Enabled = false;
	ChosenBoxType_Edit->Text = "";
	DefragButton->Enabled = false;
	m_bShowingBoxSelectionDialog = false;
	Memo->Clear();

	clearComboBox(Site_ComboBox);
	clearComboBox(Vessel_ComboBox);
	clearComboBox(Project_ComboBox);
	clearComboBox(Aliquot_ComboBox);
}

//---------------------------------------------------------------------------

void TfrmRatTanksMainDialog::fillBoxTypeComboBox()
{
	int indexa = Aliquot_ComboBox->ItemIndex;
	if (indexa == -1)
		return;

	int indexp = Project_ComboBox->ItemIndex;
	if (indexp == -1)
		return;

	TrackBarThreshold->Enabled = true;

	util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(Project_ComboBox->Items->Objects[indexp]);
	std::string project_cid = mapHolder->m_map["project_cid"];

	mapHolder = static_cast<util_stdStringmapHolder * >(Aliquot_ComboBox->Items->Objects[indexa]);
	std::string aliquot_cid = mapHolder->m_map["object_cid"];
//fill comboBox with boxes which have this alquiot type
	ChosenBoxType_Edit->Text = "";
	m_BoxTypeDialog->clearDetails();

	Project_ComboBox->Enabled = true;
	Aliquot_ComboBox->Enabled = true;
	TrackBarThreshold->Enabled = true;

	BoxType_Button->Enabled = true;
	BoxType_Button->SetFocus();

	Memo->Clear();
	Memo->Lines->Add("Please select a destination box type");
	DefragButton->Enabled = false;
}
//---------------------------------------------------------------------------

void TfrmRatTanksMainDialog::compileResults()
{
	try
	{
		std::string boxType_cid = m_BoxTypeDialog->getChosenBoxType();
		std::string box_size_cid = m_BoxTypeDialog->getChosenBoxSize();
		std::string boxName = m_BoxTypeDialog->getChosenBoxName();

		if (boxType_cid.empty())
			return;

		Site_ComboBox->Enabled = true;
		Vessel_ComboBox->Enabled = true;
		Project_ComboBox->Enabled = true;
		Aliquot_ComboBox->Enabled = true;
		TrackBarThreshold->Enabled = true;
		BoxType_Button->Enabled = true;
		TrackBarThreshold->SetFocus();
		DefragButton->Enabled = true;
		Memo->Clear();
		debugMemo->Clear();

		bool canDefrag = m_rationaliseTanks.compileResults(box_size_cid,this);

		m_rationaliseTanks.analiseForDatabaseUpdates(debugMemo,m_rationaliseTanks.getSelectedProject(),this);

		DefragButton->Enabled =  canDefrag;
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
	catch( ... )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n");
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void TfrmRatTanksMainDialog::fillAlquotComboBox()
{
	int index = Project_ComboBox->ItemIndex;
	if (index == -1)
		return;

	ChosenBoxType_Edit->Text = "";
	m_BoxTypeDialog->clearDetails();

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
			BoxType_Button->Enabled = false;
            m_BoxTypeDialog->clearDetails();
			Aliquot_ComboBox->SetFocus();
			TrackBarThreshold->Enabled = false;
			DefragButton->Enabled = false;
			debugMemo->Clear();
		}
		if (aliquotLists.size() == 1)
		{
			mapHolder = static_cast<util_stdStringmapHolder * >(Project_ComboBox->Items->Objects[Project_ComboBox->ItemIndex]);
			std::string project_cid = mapHolder->m_map["project_cid"];

			mapHolder = static_cast<util_stdStringmapHolder * >(Aliquot_ComboBox->Items->Objects[Aliquot_ComboBox->ItemIndex]);
			std::string aliquot_cid = mapHolder->m_map["object_cid"];

			int threshHold = getThreshHold();

			m_rationaliseTanks.analyseVessel(Memo,project_cid,aliquot_cid,this);

			if (!m_rationaliseTanks.refineSearch(threshHold,this))
			{
				fillBoxTypeComboBox();
				Memo->Clear();
				Memo->Lines->Add("Nothing fits your criteria");
				return;
			}
			Aliquot_ComboBox->ItemIndex = 0;
			//THIS CULLS!!!!
			if (!m_rationaliseTanks.checkForCombiningBoxesInTheAliquot(false))
			{
				fillBoxTypeComboBox();
				Memo->Clear();
				Memo->Lines->Add("Cancelled");
				return;
			}
			TrackBarThreshold->Enabled = true;
			fillBoxTypeComboBox();
		}
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
int TfrmRatTanksMainDialog::getThreshHold()
{
	String threshHold = ThreshHoldEdit->Text;
	std::string stdThreshHold = std::string();
	int ithreshhold = atoi(AnsiString(threshHold.c_str()).c_str());
	if (ithreshhold == 0)
		ithreshhold = 5;
	return ithreshhold;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksMainDialog::FormShow(TObject *Sender)
{
//on startup
	try
	{
		if (!m_rationaliseTanks.hasSelectedDB())
		{
/*			std::unique_ptr<TDbSelectionDialog> dbForm = std::unique_ptr<TDbSelectionDialog>( new TDbSelectionDialog(this) );

			dbForm->Position = poOwnerFormCenter;
			dbForm->ShowModal();
			if (dbForm->didUserQuit())
			{
				this->Close();
				return;
			}
			String selectedDB = dbForm->getDBSelection();
*/
			String selectedDB = LIMSDatabase::getCentralDb().getDbName().c_str();


			m_rationaliseTanks.initalise(selectedDB,debugMemo);

	//keep asking for sign in, untill user gets it right or quits.
/*			bool userNotQuit = true;
			bool userSignedIn = false;
			while (true)
			{
				userNotQuit = requestUserSignOn();

				if (!userNotQuit)
					break;
				userSignedIn = m_operatorSignedIn.hasSignedIn();

				if (!userSignedIn)
				{
					if (m_operatorSignedIn.getOperator_cid().empty())
						Application->MessageBox(L"Failed to authenticate, bad username or password.", L"Information", MB_OK);
					else
						Application->MessageBox(L"You do not have the correct permissions to use this software.", L"Information", MB_OK);
				}
				else
					break;
			}
			if (!userSignedIn)
			{
				this->Close();
				return;
			}
  */


			int userID = LCDbOperators::getCurrentID();
			std::stringstream ss;
			ss << userID;
//			const LCDbOperator * user = LCDbOperators::records().findByID( userID );
			 m_rationaliseTanks.LIMSSignIn(ss.str(),m_operatorSignedIn);



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
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void TfrmRatTanksMainDialog::clearComboBox(TComboBox *cleanme)
{
	for (int i = 0; i < cleanme->Items->Count; i++)
		 delete cleanme->Items->Objects[i];
	cleanme->Clear();
}
//---------------------------------------------------------------------------

void TfrmRatTanksMainDialog::fillSiteComboBox()
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
			TrackBarThreshold->Enabled = false;
			DefragButton->Enabled = false;
			BoxType_Button->Enabled = false;
			ChosenBoxType_Edit->Text = "";
         	m_BoxTypeDialog->clearDetails();

			debugMemo->Clear();
			Memo->Clear();
			Memo->Lines->Add("Please select a site from the drop down list");
		}
		else
			Site_ComboBox->Text = "No sites found!";
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void TfrmRatTanksMainDialog::fillVesselComboBox()
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
			String s = /*data[String(RationaliseTanks::database::DEFINE_OBJECT_NAME_EXTERNAL_NAME.c_str())] + " -  " +*/ data[String(RationaliseTanks::database::DEFINE_OBJECT_NAME_EXTERNAL_FULL.c_str())];
			Vessel_ComboBox->AddItem(s,ValueHolder);
		}
		if (newVesselLists.size() > 0)
		{
			Vessel_ComboBox->ItemIndex = 0;
			Vessel_ComboBox->Text = "Please select a vessel";
			Memo->Clear();
			debugMemo->Clear();
			Memo->Lines->Add("Please select a vessel to scan from the drop down list");

			Site_ComboBox->Enabled = true;
			Vessel_ComboBox->Enabled = true;
			Vessel_ComboBox->SetFocus();
			Project_ComboBox->Enabled = false;
			Aliquot_ComboBox->Enabled = false;
			TrackBarThreshold->Enabled = false;
			DefragButton->Enabled = false;
			BoxType_Button->Enabled = false;
			ChosenBoxType_Edit->Text = "";
			m_BoxTypeDialog->clearDetails();

			Project_ComboBox->Clear();
			Aliquot_ComboBox->Clear();
		}
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
	Vessel_ComboBox->Enabled = true;
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksMainDialog::Site_ComboBoxChange(TObject *Sender)
{
	fillVesselComboBox();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksMainDialog::Vessel_ComboBoxChange(TObject *Sender)
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
		BoxType_Button->Enabled = false;
		ChosenBoxType_Edit->Text = "";
		m_BoxTypeDialog->clearDetails();

		Project_ComboBox->Clear();
		Aliquot_ComboBox->Clear();
		debugMemo->Clear();
		Memo->Clear();

		TrackBarThreshold->Enabled = false;
		scan();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksMainDialog::FormDestroy(TObject *Sender)
{
	clearComboBox(Site_ComboBox);
	clearComboBox(Vessel_ComboBox);
	clearComboBox(Project_ComboBox);
	clearComboBox(Aliquot_ComboBox);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksMainDialog::TrackBarThresholdChange(TObject *Sender)
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

void __fastcall TfrmRatTanksMainDialog::TrackBarTimer(TObject *Sender)
{
	Timer1->Enabled = false;

	m_bShowingBoxSelectionDialog = true;
	int threshHold = getThreshHold();

	try
	{
	//rebuild list with new threshold
		if (!m_rationaliseTanks.refineSearch(threshHold,this))
		{
			Memo->Clear();
			Memo->Lines->Add("Nothing fits your criteria");
			m_bShowingBoxSelectionDialog = false;
			return;
		}
		//THIS CULLS!!!!
		//IF user said yes to all last time, it will do that again
		//If user selected a box type
		if (!m_rationaliseTanks.checkForCombiningBoxesInTheAliquot(true))
		{
			m_bShowingBoxSelectionDialog = false;
			Memo->Clear();
			Memo->Lines->Add("Cancelled");
			return;
		}

		std::string box_size_cid = m_BoxTypeDialog->getChosenBoxSize();
		if (box_size_cid.empty())
		{
			m_bShowingBoxSelectionDialog = false;
			Memo->Clear();
			Memo->Lines->Add("Please select a destination box type");
			BoxType_Button->Enabled = true;
			BoxType_Button->SetFocus();
			return;
		}

		bool canDefrag = m_rationaliseTanks.compileResults(box_size_cid,this);
		m_rationaliseTanks.analiseForDatabaseUpdates(debugMemo,m_rationaliseTanks.getSelectedProject(),this);

		DefragButton->Enabled =  canDefrag;
		m_bShowingBoxSelectionDialog = false;
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksMainDialog::Project_ComboBoxChange(TObject *Sender)
{
	fillAlquotComboBox();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksMainDialog::Aliquot_ComboBoxChange(TObject *Sender)
{
	int index = Project_ComboBox->ItemIndex;
	if (index == -1)
		return;
	index = Aliquot_ComboBox->ItemIndex;
	if (index == -1)
		return;

	BoxType_Button->Enabled = true;
//	BoxType_Button->SetFocus();

	TrackBarThreshold->Enabled = true;

	util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(Project_ComboBox->Items->Objects[Project_ComboBox->ItemIndex]);
	std::string project_cid = mapHolder->m_map["project_cid"];

	mapHolder = static_cast<util_stdStringmapHolder * >(Aliquot_ComboBox->Items->Objects[Aliquot_ComboBox->ItemIndex]);
	std::string aliquot_cid = mapHolder->m_map["object_cid"];

	try
	{
		m_rationaliseTanks.analyseVessel(Memo,project_cid,aliquot_cid,this);

		int threshHold = getThreshHold();
		if (!m_rationaliseTanks.refineSearch(threshHold,this))
		{
			Memo->Clear();
			Memo->Lines->Add("Nothing fits your criteria");
		}
		else if (!m_rationaliseTanks.checkForCombiningBoxesInTheAliquot(false))
		{
			Memo->Clear();
			Memo->Lines->Add("Cancelled");
		}
		fillBoxTypeComboBox();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
	Screen->Cursor = crDefault;
}

//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksMainDialog::DefragButtonClick(TObject *Sender)
{
	try
	{
		std::string status = m_rationaliseTanks.getProjectActivityStatus(m_rationaliseTanks.getSelectedProject());

		if ((status != "5") && (status != "6"))
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

			#ifdef _DEBUG
			String displayString = String("You should not be rationalising that database, but as this is in debug mode, would you like to anyway?");
			int result = Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_YESNO);
			if (result == IDNO)
				return;
			#else
			return;
			#endif
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
		std::auto_ptr<TfrmRatTankCreateRetrievalJob> CreateRetrievalJobForm = std::auto_ptr<TfrmRatTankCreateRetrievalJob>( new TfrmRatTankCreateRetrievalJob(this) );
		std::string boxType_cid = m_BoxTypeDialog->getChosenBoxType();
		CreateRetrievalJobForm->Position = poOwnerFormCenter;
		CreateRetrievalJobForm->setUp(&m_rationaliseTanks,boxType_cid);
		CreateRetrievalJobForm->ShowModal();
		if (CreateRetrievalJobForm->getResult() == "OK")
		{
		//success, now lets reset everything, as the info we have is out of date.
			reSetAll();
			Vessel_ComboBox->Enabled = false;
			fillSiteComboBox();
		}
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
}

//---------------------------------------------------------------------------

bool TfrmRatTanksMainDialog::requestUserSignOn()
{
// ASK FOR USER NAME/PASSWORD, return false if user quit.
	std::auto_ptr<TfrmRatTankSignIn> dbForm = std::auto_ptr<TfrmRatTankSignIn>( new TfrmRatTankSignIn(this) );

	dbForm->Position = poOwnerFormCenter;

	dbForm->ShowModal();
	if (dbForm->didUserQuit())
		return false;

	std::string userName = dbForm->getUserName();
	std::string Password = dbForm->getPassword();

	m_rationaliseTanks.signIn(userName,Password,m_operatorSignedIn);
	return true;
}

//---------------------------------------------------------------------------

void TfrmRatTanksMainDialog::scan()
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
		m_PleaseWaitDialog->Position = poOwnerFormCenter;
		m_PleaseWaitDialog->Show();
		m_rationaliseTanks.scanVessel(mapHolder->m_map,Memo,this);
		m_PleaseWaitDialog->Hide();

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
			if (!((activityFlag== "6") || (activityFlag== "7") || (activityFlag== "0")))
				s = L"[Analyse only] " + s;
			Project_ComboBox->AddItem(s,ValueHolder);
		}
		if (projectsLists.size() > 1)
		{
			Project_ComboBox->ItemIndex = 0;
			Project_ComboBox->Text = "Please select a project";
			Memo->Clear();
			Memo->Lines->Add("Please select a project from the drop down list");

			m_BoxTypeDialog->clearDetails();
			BoxType_Button->Enabled = false;
			ChosenBoxType_Edit->Text = "";
	
			Site_ComboBox->Enabled = true;
			Vessel_ComboBox->Enabled = false;
			Project_ComboBox->Enabled = true;
			Project_ComboBox->SetFocus();
			debugMemo->Clear();
			Aliquot_ComboBox->Enabled = false;
			TrackBarThreshold->Enabled = false;

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
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmRatTanksMainDialog::BoxType_ButtonClick(TObject *Sender)
{
	if (Project_ComboBox->ItemIndex == -1)
		return;
	util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(Project_ComboBox->Items->Objects[Project_ComboBox->ItemIndex]);
	std::string project_cid = mapHolder->m_map["project_cid"];
	std::string project_name = mapHolder->m_map["external_name"];

	mapHolder = static_cast<util_stdStringmapHolder * >(Aliquot_ComboBox->Items->Objects[Aliquot_ComboBox->ItemIndex]);
	std::string aliquot_cid = mapHolder->m_map["object_cid"];
	std::string aliquot_name = mapHolder->m_map["external_name"];

	m_BoxTypeDialog->Position = poOwnerFormCenter;
	m_BoxTypeDialog->setUp(m_rationaliseTanks,project_cid,aliquot_cid,project_name,aliquot_name);
	m_BoxTypeDialog->ShowModal();

	if (m_BoxTypeDialog->didUserQuit())
		return;

 	std::string boxName = m_BoxTypeDialog->getChosenBoxName();
	TrackBarThreshold->Enabled = true;

	ChosenBoxType_Edit->Text = String(boxName.c_str());
	compileResults();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksMainDialog::ShowDebugButtonClick(TObject *Sender)
{
	if (Timer_SlideAnim->Enabled)
		return;

	if (DebugPanel->Height == 0)
	{
		m_AnimDebugHeight_Target_Orginal_Form_Height = frmRatTanksMainDialog->Height;
		m_AnimDebugHeight_Target_Form_Debug = 200;
		m_AnimDebugHeight_Velocity = 0;
		m_AnimDebugHeight_Current = 0;
		Timer_SlideAnim->Enabled = true;
	}
	else
	{
		m_AnimDebugHeight_Target_Orginal_Form_Height = frmRatTanksMainDialog->Height;
		m_AnimDebugHeight_Target_Form_Debug = -200;
		m_AnimDebugHeight_Velocity = 0;
		m_AnimDebugHeight_Current = 0;
		Timer_SlideAnim->Enabled = true;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmRatTanksMainDialog::Timer_SlideAnimTimer(TObject *Sender)
{
	if ((m_AnimDebugHeight_Current == m_AnimDebugHeight_Target_Form_Debug) && (m_AnimDebugHeight_Velocity == 0))
	{
		Timer_SlideAnim->Enabled = false;
		return;
	}

	if (m_AnimDebugHeight_Target_Form_Debug < 0)
	{
		m_AnimDebugHeight_Velocity -= 5;
		m_AnimDebugHeight_Current += m_AnimDebugHeight_Velocity;
		if (m_AnimDebugHeight_Current < m_AnimDebugHeight_Target_Form_Debug)
		{
			m_AnimDebugHeight_Current = m_AnimDebugHeight_Target_Form_Debug;
			m_AnimDebugHeight_Velocity = -(m_AnimDebugHeight_Velocity/2);
		}
	}
	else
	{
		m_AnimDebugHeight_Velocity += 5;
		m_AnimDebugHeight_Current += m_AnimDebugHeight_Velocity;
		if (m_AnimDebugHeight_Current > m_AnimDebugHeight_Target_Form_Debug)
		{
			m_AnimDebugHeight_Current = m_AnimDebugHeight_Target_Form_Debug;
			m_AnimDebugHeight_Velocity = -(m_AnimDebugHeight_Velocity/2);
		}
	}

	if ((abs(m_AnimDebugHeight_Velocity) <=1) && (m_AnimDebugHeight_Current == m_AnimDebugHeight_Target_Form_Debug))
	{
		Timer_SlideAnim->Enabled = false;
		if (m_AnimDebugHeight_Current > 0)
		{
			DebugPanel->Height = 200;
			frmRatTanksMainDialog->Height = m_AnimDebugHeight_Target_Orginal_Form_Height + 200;
		}
		else
		{
			DebugPanel->Height = 0;
			frmRatTanksMainDialog->Height = m_AnimDebugHeight_Target_Orginal_Form_Height - 200;
		}
		return;
	}

	frmRatTanksMainDialog->Height = m_AnimDebugHeight_Target_Orginal_Form_Height + m_AnimDebugHeight_Current;
	if (m_AnimDebugHeight_Current > 0)
		DebugPanel->Height = m_AnimDebugHeight_Current;
	else
		DebugPanel->Height = 200 - m_AnimDebugHeight_Current;

}
//---------------------------------------------------------------------------


