//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "tFormMain.h"
#include "utils.h"
#include "Vcl.StdCtrls.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#include <sstream>
#include "TFrmErrorReport.h"
#include <System.RegularExpressions.hpp>
#include <System.Character.hpp>
#include "xexec.h"
#include "ldapuser.h"
TfrmMain *frmMain;

#define LINES_TO_SHOW 1

#define MEMONAME String("HistoryMemo_")
#define MEMOPANALTOPNAME String("HistoryPanelTop_")
//---------------------------------------------------------------------------

__fastcall TfrmMain::TfrmMain(TComponent* Owner) : TForm(Owner), m_Application_UID(-1), m_isDirty(false)
{
}
//---------------------------------------------------------------------------

void TfrmMain::fillProjectCombo()
{
	project_summery_infomation_Edit->Text = L"";
	Projects_ComboBox->Clear();

	std::string statusString = "status = 1";
	if (showArchivedProjects_CheckBox->Checked)
		statusString = "status = 1 OR status = 2";

	std::string sql = "SELECT app_id,summary,status FROM ru_application WHERE " + statusString;
	XQUERY query(m_db.get(),sql);
	query.setAcceptNull(true);
	if (!query.open())
		throw String(query.getLastError().c_str());

	while (query.fetch())    //any data?
	{
		std::string app_id = query.result.getString(0);
		std::string summary = query.result.getString(1);
		int status = query.result.getInt(2);

		std::map<std::string,std::string> myMap;
		myMap.insert(std::pair<std::string,std::string>("app_id",app_id));
		myMap.insert(std::pair<std::string,std::string>("summary",summary));
		util_stdStringmapHolder *ValueHolder = new util_stdStringmapHolder(myMap);
		if (status == 1)
			Projects_ComboBox->AddItem(app_id.c_str(),ValueHolder);
		else
			Projects_ComboBox->AddItem(String("[Archived] ") + app_id.c_str(),ValueHolder);
	}
	query.close();

	if (Projects_ComboBox->Items->Count == 0)
	{
		Projects_ComboBox->Text = L"No Projects found";
		OpenProject_Button->Enabled = false;
	}
	else
		Projects_ComboBox->Text = L"Select a project";
}

//---------------------------------------------------------------------------
void TfrmMain::reFreshUserTable()
{
	const std::vector< projectAccount > *usrAccs = m_projectAccount.getAccounts();
	Users_StringGrid->RowCount = usrAccs->size()+1;

	if (!AccoutAttribs_ButtonGroup->Visible)
		Users_StringGrid->ColCount = 4;

	int RowCount = 0;
	for (unsigned int i=0;i<usrAccs->size();i++)
	{
		if (usrAccs->at(i).getAccountType() != L"UNKNOWN")
		{
			Users_StringGrid->Cells[0][RowCount+1] = usrAccs->at(i).getuserID();
			Users_StringGrid->ColWidths[0] = 0;

			Users_StringGrid->Cells[1][RowCount+1] = usrAccs->at(i).getUserName();
			Users_StringGrid->Cells[2][RowCount+1] = usrAccs->at(i).getAccountType();
			Users_StringGrid->Cells[3][RowCount+1] = usrAccs->at(i).getStatusType();
			if (AccoutAttribs_ButtonGroup->Visible)
				Users_StringGrid->Cells[4][RowCount+1] = usrAccs->at(i).getAttribString();
			RowCount++;
		}
	}
	Users_StringGrid->RowCount = RowCount+1;

	if (Users_StringGrid->RowCount > 1)
		Users_StringGrid->FixedRows = 1;

	NoWinAccounts_Edit->Text = m_projectAccount.getNoWindowsAccounts();
	NoLinuxAccounts_Edit->Text = m_projectAccount.getNoLinuxAccounts();
	NoGPUAccounts_Edit->Text = m_projectAccount.getNoGPUAccounts();
	FormResize(0);
	AccoutAttribs_ButtonGroup->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::PI_MemoChange(TObject *Sender)
{
	m_isDirty = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Project_Notes_MemoChange(TObject *Sender)
{
	m_isDirty = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::AccoutAttribs_ButtonGroupButtonClicked(TObject *Sender, int Index)
{
	String Name = AccoutAttribs_ButtonGroup->Items->Items[Index]->Caption;
	m_projectAccount.getAttribute()->toggleAttrib(AnsiString(Name.c_str()).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::MainTabControlChange(TObject *Sender)
{
	BalloonHint->HideHint();

	footer_GroupBox->Visible = false;
	Page1->Visible = false;
	Page2->Visible = false;
	Users_StringGrid->Visible = false;
	correspondence_pannel->Visible = false;
	MainTabControl->Visible = false;

	if (MainTabControl->TabIndex == 0)
	{
		Page1->Visible = true;
	}
	else if (MainTabControl->TabIndex == 1)
	{
		Page2->Visible = true;
		Users_StringGrid->Visible = true;
		utils::reSizeGrid(this,Users_StringGrid);
	}
	else if (MainTabControl->TabIndex == 2)
	{
		renderCorrispondance();
		correspondence_pannel->Visible = true;
	}
	MainTabControl->Visible = true;
	footer_GroupBox->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormResize(TObject *Sender)
{
	utils::reSizeGrid(this,Users_StringGrid);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::AccountName_EditChange(TObject *Sender)
{
	if (!AccountNameLock_CheckBox->Visible)
		return;

	if (m_isInitalising)
	{   //on load...
		userStatus_ComboBox->Enabled = false;
		if (userStatus_ComboBox->Text.Length() == 0)
			userStatus_ComboBox->Text = "New";

		String regExp = "^[a-z][a-z0-9]{2,9}$";
		if (!TRegEx::IsMatch(AccountName_Edit->Text,regExp))
		{
			AddGPUAccountButton->Enabled = false;
			AddLinuxAccountButton->Enabled = false;
			AddWinAccountButton->Enabled = false;
		}
		else
		{
			AddGPUAccountButton->Enabled = (m_projectAccount.getNoGPUAccounts() == "0");
			AddLinuxAccountButton->Enabled = (m_projectAccount.getNoLinuxAccounts() == "0");
			userStatus_ComboBox->Enabled = true;
			AddWinAccountButton->Enabled = true;
		}
		return;
	}
	if (AccountNameLock_CheckBox->Checked)
	{
		AccountName_Edit->Text = m_UserAccountName.c_str();
		BalloonHint->Title = "Locked";
		BalloonHint->Description = "The account name is protected. To change the username uncheck this box.";
		BalloonHint->ShowHint(AccountNameLock_CheckBox);
		return;
	}

	m_UserAccountName = AnsiString(AccountName_Edit->Text.c_str()).c_str();

	if (AccountName_Edit->Text.IsEmpty())
	{
		AddLinuxAccountButton->Enabled = false;
		AddWinAccountButton->Enabled = false;
		AddGPUAccountButton->Enabled = false;
		userStatus_ComboBox->Text = "";
		userStatus_ComboBox->Enabled = false;
		AccoutAttribs_ButtonGroup->Refresh();
	}
	else
	{
		String regExp = "^[a-z][a-z0-9]{2,9}$";
		if (!TRegEx::IsMatch(AccountName_Edit->Text,regExp))
		{
			BalloonHint->Title = "Bad username";
			BalloonHint->Description = "Username restrictions: lower case, length between 3 and 10 letters and numbers, but it cannot start with a number. Please check and try again.";
			BalloonHint->ShowHint(AccountName_Edit);
		}
		else
		{
			BalloonHint->HideHint();
			AddGPUAccountButton->Enabled = (m_projectAccount.getNoGPUAccounts() == "0");
			AddLinuxAccountButton->Enabled = (m_projectAccount.getNoLinuxAccounts() == "0");
			if (userStatus_ComboBox->Text.Length() == 0)
				userStatus_ComboBox->Text = "New";
			userStatus_ComboBox->Enabled = true;
			AddWinAccountButton->Enabled = true;
		}
	}
}
//---------------------------------------------------------------------------

bool TfrmMain::isUserNameUnquie(const std::string &name, int type)
{
	//check in memory
	if (projectAccount::has(name, type))
		return false;

	//check all projects, username + account type
	return (!projectAccount::hasInDB(m_db.get(),name, type));
}
//---------------------------------------------------------------------------
bool TfrmMain::isUserAccountValid(const std::string &userName,int type)
{
	//3 to 9 in length
	//lower case, doesn't start with a number
	//can have latters and numbers in the other poisitions
	String regExp = "^[a-z][a-z0-9]{2,9}$";
	if (!TRegEx::IsMatch(AccountName_Edit->Text,regExp))
	{
		BalloonHint->Title = "Bad username";
		BalloonHint->Description = "Username length is be between 3 and 10 latters, it can have letters and numbers, but it cannot start with a number. Please check and try again.";
		BalloonHint->ShowHint(AccountName_Edit);
		return false;
	}

	try
	{
		if (!isUserNameUnquie(userName,type))
		{
			MessageDlg("Duplicate username detected, failed to add user account", mtConfirmation, TMsgDlgButtons() << mbOK,0);
			return false;
		}
	}
	catch(std::string e)
	{
		showErrorReport("Error report","Error detected. Please try again.\r\nIf this error persists, please contact support",e);
		return false;
	}
	return true;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::AddWinAccountButtonClick(TObject *Sender)
{
	std::string userName = AnsiString(AccountName_Edit->Text.c_str()).c_str();
	std::string attribs = m_projectAccount.getAttribute()->seriliseAttrib();
	int status = projectAccount::getStatusValue(userStatus_ComboBox->Text);

	std::stringstream userNum;
	m_currentWindowsUserIDNumber++;
	userNum << m_currentWindowsUserIDNumber;
	m_currentWindowsUserIDNumber--;

	if (!isUserAccountValid(userName + userNum.str(), projectAccount::DEFINE_WINDOWS))
		return;

	m_currentWindowsUserIDNumber++;
	m_projectAccount.getAttribute()->clearAttib();
	std::stringstream ss;
	ss << (m_currentWindowsUserIDNumber+1);

	m_isInitalising = true;
	userValue_Edit->Text = ss.str().c_str();
	projectAccount::addUserAccount(userName+userNum.str(),status,projectAccount::DEFINE_WINDOWS,attribs);
	reFreshUserTable();
	m_isDirty = true;
	AccountNameLock_CheckBox->Checked = true;
	m_isInitalising = false;
}
//---------------------------------------------------------------------------

void TfrmMain::AddNoWindowsAccount(int type)
{
	std::string userName = AnsiString(AccountName_Edit->Text.c_str()).c_str();
	std::string attribs = m_projectAccount.getAttribute()->seriliseAttrib();
	int status = projectAccount::getStatusValue(userStatus_ComboBox->Text);

	int uid = m_projectAccount.getAccountID(type);
	if (uid == -1)
	{   //nothing in memory
		uid = m_projectAccount.loadTypeInDB(m_db.get(),m_Application_UID,type);
		if (uid == -1)
		{   //nothing in the database.
			if (!isUserAccountValid(userName,type))
				return;
		}
	}
	if (uid == -1)
		projectAccount::addUserAccount(userName,status,type,attribs);
	else
		projectAccount::updateUserAccount(uid,status,userName,attribs);

	m_projectAccount.getAttribute()->clearAttib();
	reFreshUserTable();
	if (type == projectAccount::DEFINE_LINUX)
		AddLinuxAccountButton->Enabled = false;
	else
		AddGPUAccountButton->Enabled = false;

	m_isDirty = true;
	AccountNameLock_CheckBox->Checked = true;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::AddLinuxAccountButtonClick(TObject *Sender)
{
	AddNoWindowsAccount(projectAccount::DEFINE_LINUX);
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::AddGPUAccountButtonClick(TObject *Sender)
{
	AddNoWindowsAccount(projectAccount::DEFINE_GPU);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::AccoutAttribs_ButtonGroupDrawButton(TObject *Sender, int Index, TCanvas *Canvas, TRect &Rect, TButtonDrawState State)
{
	String Name = AccoutAttribs_ButtonGroup->Items->Items[Index]->Caption;

	if (m_projectAccount.getAttribute()->getAttrib(AnsiString(Name.c_str()).c_str()))
	{
		AccoutAttribs_ButtonGroup->Canvas->Pen->Color = clHighlight ; //boarder of rect
		AccoutAttribs_ButtonGroup->Canvas->Brush->Color = clHighlight ; //rect
	}
	else
	{
		AccoutAttribs_ButtonGroup->Canvas->Pen->Color = clBtnFace ; //boarder of rect
		AccoutAttribs_ButtonGroup->Canvas->Brush->Color = clBtnFace ; //rect
	}
	AccoutAttribs_ButtonGroup->Canvas->Rectangle(Rect.Left+1,Rect.Top+1,Rect.Right-1, Rect.Bottom-1);

	int TWidth = AccoutAttribs_ButtonGroup->Canvas->TextWidth(Name);
	int THeight = AccoutAttribs_ButtonGroup->Canvas->TextHeight(Name);

	int XOff = (Rect.Width() - TWidth) / 2;
	int YOff = (Rect.Height() - THeight) / 2;
	AccoutAttribs_ButtonGroup->Canvas->TextOutW(Rect.Left+XOff,Rect.Top+YOff,Name);
}
//---------------------------------------------------------------------------
bool TfrmMain::isCorrespondenceValid()
{
	BalloonHint->HideHint();

	if ((Summary_Edit->Text.IsEmpty()) || (Summary_Edit->Text.Length() <= 3))
	{
		BalloonHint->Title = "Missing information";
		if (Summary_Edit->Text.IsEmpty())
			BalloonHint->Description = "Please enter an summary";
		else
			BalloonHint->Description = "The summary must be greater then 3 characters in length";
		BalloonHint->ShowHint(Summary_Edit);
		return false;
	}
	if (Correspondence_Memo->Text.IsEmpty())
	{
		BalloonHint->Title = "Missing information";
		BalloonHint->Description = "Please enter some information which is to be stored";
		BalloonHint->ShowHint(Correspondence_Memo);
		return false;
	}
	return true;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::Add_Correspondence_ButtonClick(TObject *Sender)
{
	if (!isCorrespondenceValid())
		return;

	m_correspondence.addCorrespondence(Summary_Edit->Text,Correspondence_Memo->Text);
	Summary_Edit->Text = L"";
	Correspondence_Memo->Text = L"";
	renderCorrispondance();
	m_isDirty = true;
}
//---------------------------------------------------------------------------

bool TfrmMain::hasCorrespondence(String &&id)
{
	for (int i=correspondence_pannel->ControlCount-1;i>=0;i--)
	{
		TControl *Temp = correspondence_pannel->Controls[i];
		if (dynamic_cast<TPanel*>(Temp))
		{
			TPanel *p = dynamic_cast<TPanel*>(Temp);
			for (int i=p->ControlCount-1;i>=0;i--)
			{
				if (p->Controls[i]->Name == id)
					return true;
			}
		}
	}
	return false;
}
//---------------------------------------------------------------------------

void TfrmMain::renderCorrispondance()
{
	correspondence_pannel->Visible = true;
 //	int Total = m_correspondence.int correspondence::getFirst()
	std::string insertedControlID = "-1";

	for (int i=m_correspondence.getFirst();i!=-1;i=m_correspondence.getNext(i))
	{
		std::string CorrespondenceID = m_correspondence.getCorrespondenceID(i);
		if (CorrespondenceID.empty())
			continue;

		String StringID = String(CorrespondenceID.c_str());
		if (hasCorrespondence(MEMONAME + String((CorrespondenceID).c_str())))
			continue;

		insertedControlID = CorrespondenceID;

		TMemo *pTM = new TMemo(correspondence_pannel);
		pTM->Text = String(m_correspondence.getCorrespondenceText(i).c_str()).TrimRight();
		pTM->Width = correspondence_pannel->Width;
		pTM->Visible = true;
		pTM->Align = alTop;
		pTM->TabStop = false;
//		pTM->Enabled = false;
		pTM->ReadOnly = true;
		pTM->Color = clBtnHighlight;
		std::wstringstream ss;
		ss << MEMONAME.c_str() << StringID.c_str();// << i;
		pTM->Name = String(ss.str().c_str());
		pTM->TextHint = StringID;
		pTM->ScrollBars = ssNone;
		CorrispondenceHistory_Panel->InsertControl(pTM);
		pTM->Height = ((-pTM->Font->Height)+2) * (LINES_TO_SHOW);
		pTM->OnClick = correspondence_pannelClick;

		TPanel *pPanel = new TPanel(correspondence_pannel);
		pPanel->Height = 20;
		pPanel->Width = correspondence_pannel->Width;
		pPanel->Visible = true;
		pPanel->Align = alTop;
		pPanel->ParentColor = false;
		pPanel->ParentBackground = false;
		pPanel->Color = clSkyBlue;
		pPanel->OnClick = correspondence_pannelClick;

		std::wstringstream ss3;
		ss3 << MEMOPANALTOPNAME.c_str() << StringID.c_str();//i;
		pPanel->Name = String(ss3.str().c_str());
		pPanel->Caption = "";
		CorrispondenceHistory_Panel->InsertControl(pPanel);

		TLabel *Subject_Label = new TLabel(pPanel);
		Subject_Label->Align = TAlign::alLeft;
		Subject_Label->SetTextBuf((String("     ") + String(m_correspondence.getCorrespondenceSummary(i).c_str())).c_str());
		Subject_Label->OnClick =labalClick;
		Subject_Label->Name = "summary";
		pPanel->InsertControl(Subject_Label);

		TButton *Button = new TButton(pPanel);
		Button->Align = TAlign::alRight;
		Button->SetTextBuf(L"...");
		Button->Width = 20;
		Button->TabStop = false;
		Button->OnClick = correspondenceMenuClick;
		pPanel->InsertControl(Button);

		TLabel *Date_Label = new TLabel(pPanel);
		Date_Label->Align = TAlign::alRight;
		Date_Label->SetTextBuf(String(m_correspondence.getCorrespondenceDate(i).c_str() + String("     ")).c_str());
		Date_Label->OnClick =labalClick;
		pPanel->InsertControl(Date_Label);
	}
	if (insertedControlID != "-1")
		openCorrespondencePannel(insertedControlID);
	correspondence_pannel->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Update_ButtonClick(TObject *Sender)
{
	if (updateUser_Button->Visible)
		cancelUser_ButtonClick(NULL);
	if (saveProject())
		Application->MessageBox(L"Project saved", L"Success", MB_OK);
}
//---------------------------------------------------------------------------
//THIS IS NOW CANCEL CHANGES BUTTON. BUT I'M SCARE TO REFACTOR
void __fastcall TfrmMain::Save_Exit_ButtonClick(TObject *Sender)
{
	if (updateUser_Button->Visible)
		cancelUser_ButtonClick(NULL);
	if (m_Application_UID == -1)
	{
		m_correspondence.clear();
		m_projectAccount.clear();
		ApplicationID_Edit->Text = "";
		PI_Memo->Text = "";
		Project_Notes_Memo->Text = "";
		reFreshUserTable();
		NewProject_ButtonClick(NULL);
	}
	else
		OpenProject_ButtonClick(NULL);
//	Cancel_ButtonClick(NULL);
}
//---------------------------------------------------------------------------

bool TfrmMain::saveProject()
{
	String regExp = "^[0-9]{3,30}$";
	if (!TRegEx::IsMatch(ApplicationID_Edit->Text,regExp))
	{
		MainTabControl->TabIndex = 0;
		MainTabControlChange(NULL);
		BalloonHint->Title = "Missing information.";
		if (ApplicationID_Edit->Text.IsEmpty())
			BalloonHint->Description = "Please enter an application id";
		else
			BalloonHint->Description = "Application id is a string of between 3 and 30 numbers. Please check and try again.";
		BalloonHint->ShowHint(ApplicationID_Edit);
		return false;
	}

	if ((project_summary_Edit->Text.IsEmpty()) || (project_summary_Edit->Text.Length() <= 3))
	{
		MainTabControl->TabIndex = 0;
		MainTabControlChange(NULL);
		BalloonHint->Title = "Missing information.";
		if (project_summary_Edit->Text.IsEmpty())
			BalloonHint->Description = "Please enter a project summary";
		else
			BalloonHint->Description = "Project summary must be greater then 3 characters in length";
		BalloonHint->ShowHint(project_summary_Edit);
		return false;
	}
	if (!PI_Email_Edit->Text.IsEmpty())
	{
		MainTabControl->TabIndex = 0;
		MainTabControlChange(NULL);
		String regExp = "[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?";
		String toCheck = PI_Email_Edit->Text.LowerCase();
		if (!TRegEx::IsMatch(toCheck,regExp))
		{
			MainTabControl->TabIndex = 0;
			MainTabControlChange(NULL);
			BalloonHint->Title = "Missing information.";
			BalloonHint->Description = "Please check the PI email address is correct.";
			BalloonHint->ShowHint(PI_Email_Edit);
			return false;
		}
	}
	if ((PI_Memo->Text.IsEmpty()) || (PI_Memo->Text.Length() <= 3))
	{
		MainTabControl->TabIndex = 0;
		MainTabControlChange(NULL);
		BalloonHint->Title = "Missing information.";
		if (PI_Memo->Text.IsEmpty())
			BalloonHint->Description = "Please enter the P.I.'s details";
		else
			BalloonHint->Description = "The P.I. details must be greater then 3 characters in length";
		BalloonHint->ShowHint(PI_Memo);
		return false;
	}

	String ApplicationID = ApplicationID_Edit->Text;
	String PrincipalInvestigator = PI_Memo->Text;
	String ProjectInfo = Project_Notes_Memo->Text;
	std::string PIEmail = AnsiString(PI_Email_Edit->Text.c_str()).c_str();
	std::string Summary = AnsiString(project_summary_Edit->Text.c_str()).c_str();
	int stdApplicationID = atoi(AnsiString(ApplicationID.c_str()).c_str());
	std::string userAccountPattern = AnsiString(AccountName_Edit->Text).c_str();

	int status = 1;
	if (projectStatus_ComboBox->Text == L"Archived")
		status = 2;

	//turn off auto commit...
	m_db->setAutoCommit(false);
	//save to db
	try
	{
		try
		{
//if we have a UID then it's an update
			if (m_Application_UID != -1)
			{
				XTIME D;
				D.setNow();

				ROSETTA R;
				R.setDate("tupdate",D);
				R.setString("piemail",PIEmail);
				R.setString("useraccountpattern",userAccountPattern);
				R.setString("summary",Summary);
				R.setInt("currentusernum",m_currentWindowsUserIDNumber);
				R.setInt("status",status);
				R.setString("pi",AnsiString(PrincipalInvestigator.c_str()).c_str());
				if (!ProjectInfo.IsEmpty())
					R.setString("info",AnsiString(ProjectInfo.c_str()).c_str());
				XEXEC exec(m_db.get());

				ROSETTA RWhere;
				RWhere.setInt("app_id",stdApplicationID);
				if (!exec.update("ru_application",&R,&RWhere))
					throw exec.getLastError();

				m_projectAccount.update(m_db.get(),stdApplicationID);
				m_correspondence.update(m_db.get(),stdApplicationID);
				return true;
			}
			//no application_uid, so it's never been saved before..
			XTIME D;
			D.setNow();

  //			m_Application_UID = utils::getNextValueFor(m_db.get(),"application_id_seq");
			ROSETTA R;
 //			R.setInt("id",m_Application_UID);
			R.setInt("status",status);
			R.setString("piemail",PIEmail);
			R.setString("summary",Summary);
			R.setDate("tinsert",D);
			R.setInt("currentusernum",m_currentWindowsUserIDNumber);
			R.setDate("tupdate",D);
			R.setInt("app_id",stdApplicationID);
			R.setString("pi",AnsiString(PrincipalInvestigator.c_str()).c_str());
			if (!ProjectInfo.IsEmpty())
				R.setString("info",AnsiString(ProjectInfo.c_str()).c_str());
			XEXEC exec(m_db.get());
			if (!exec.insert("ru_application",&R))
				throw exec.getLastError();
			m_projectAccount.save(m_db.get(),stdApplicationID);
			m_correspondence.save(m_db.get(),stdApplicationID);
		}
		catch(std::exception e)
		{
			m_db->rollback();
			showErrorReport("Error report","Error saving to database. Please try saving again.\r\nIf this error persists, please contact support",e.what());
			return false;
		}
		catch(std::string e)
		{
			m_db->rollback();
			showErrorReport("Error report","Error saving to database. Please try saving again.\r\nIf this error persists, please contact support",e);
			return false;
		}
		catch(...)
		{
			m_db->rollback();
			showErrorReport("Error report","Error saving to database. Please try saving again.\r\nIf this error persists, please contact support","Unknown");
			return false;
		}
	}
	__finally
	{
	//	submit transaction, if we have anything to submit
		m_db->commit();
		m_db->setAutoCommit(true);
		//as the user id's may now be different.. refresh the users
		reFreshUserTable();
		m_isDirty = false;
		ApplicationID_Edit->ReadOnly = true;
	}
	return true;
}
//---------------------------------------------------------------------------

void TfrmMain::showErrorReport(const std::string &title,const std::string &report,const std::string &details)
{
	std::unique_ptr<TFrmErrorReport> Report = std::unique_ptr<TFrmErrorReport>( new TFrmErrorReport(NULL) );
	Report->setTitle(title);
	Report->setReport(report);
	Report->setDetails(details);
	Report->ShowModal();
}

//---------------------------------------------------------------------------

//any mouse clicks on the lables, pass it to parent.
void __fastcall TfrmMain::labalClick(TObject *Sender)
{
	TLabel *l = dynamic_cast<TLabel*>(Sender);
	if (!l) return;
	TPanel *p = dynamic_cast<TPanel*>(l->Parent);
	if (!p) return;
	p->OnClick(p);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::correspondenceMenuClick(TObject *Sender)
{
	BalloonHint->HideHint();
	m_selectedCorrespondenceID = "";

	TButton *l = dynamic_cast<TButton*>(Sender);
	if (!l) return;
	TPanel *p = dynamic_cast<TPanel*>(l->Parent);
	if (!p) return;

	m_selectedCorrespondenceID = p->Name.SubString(MEMOPANALTOPNAME.Length()+1,p->Name.Length());
	correspondence_PopupMenu->Popup(Mouse->CursorPos.X,Mouse->CursorPos.Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::correspondence_pannelClick(TObject *Sender)
{
//hide hint if it's being displayed
	BalloonHint->HideHint();

	String caption = ((TButton *) Sender)->Caption;
	String name = ((TButton *) Sender)->Name;
	String text = name.SubString(0,MEMONAME.Length());

	if (name.SubString(0,MEMONAME.Length()) == MEMONAME)
		name = name.SubString(MEMONAME.Length()+1,name.Length());
	else
		name = name.SubString(MEMOPANALTOPNAME.Length()+1,name.Length());

	std::string controlID = AnsiString(name.c_str()).c_str();
	openCorrespondencePannel(controlID);
}
//---------------------------------------------------------------------------

void TfrmMain::openCorrespondencePannel(std::string &controlID)
{
	for (int i=correspondence_pannel->ControlCount-1;i>=0;i--)
	{
		TPanel *p = dynamic_cast<TPanel*>(correspondence_pannel->Controls[i]);
		if (p)
		{
			for (int i=p->ControlCount-1;i>=0;i--)
			{
				TMemo *Temp = dynamic_cast<TMemo*>(p->Controls[i]);
				if (!Temp)
					continue;

				if (Temp->Name.SubString(0,MEMONAME.Length()) != MEMONAME)
					continue;

				if (Temp->Name == MEMONAME + controlID.c_str())
				{   //it is the memo which has been clicked on
					//see if the memo contains the full text...
					String fulltext = String(m_correspondence.getCorrespondenceText(controlID).c_str());
					fulltext = fulltext.TrimRight();

					int minHeight = ((-Temp->Font->Height)+2) * (LINES_TO_SHOW);
					int currentHeight = Temp->Height;

					if ((minHeight == currentHeight) && (Temp->Lines->Count >= LINES_TO_SHOW)) //min
						Temp->Height = ((-Temp->Font->Height)+2) * (Temp->Lines->Count+1);
					else //maxed
						Temp->Height = ((-Temp->Font->Height)+2) * (LINES_TO_SHOW);
				}
				else
					Temp->Height = ((-Temp->Font->Height)+2) * (LINES_TO_SHOW);
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::EditCorrespondenceClick(TObject *Sender)
{
	if (m_selectedCorrespondenceID.Length() == 0)
		return;

	std::string id =  AnsiString(m_selectedCorrespondenceID.c_str()).c_str();
	String Subject = String(m_correspondence.getCorrespondenceSummary(id).c_str()).TrimRight();
	String Text = String(m_correspondence.getCorrespondenceText(id).c_str()).TrimRight();

	Summary_Edit->Text = Subject;
	Correspondence_Memo->Text = Text;

	Add_Correspondence_Button->Visible = false;
	Update_Correspondence_Button->Visible = true;
	Cancel_Correspondence_Button->Visible = true;
	m_isDirty = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::DeleteCorrespondenceClick(TObject *Sender)
{
	if (m_selectedCorrespondenceID.Length() == 0)
		return;

	if (!askAreYouSure())
		return;

	if (!m_correspondence.deleteCorrespondenceID(std::string(AnsiString(m_selectedCorrespondenceID.c_str()).c_str())))
	{
		Application->MessageBox(L"Error deleting correspondence. Please try again.", L"Error", MB_OK);
		return;
	}
	//now delete the GUI elements
	DeleteCorrespondenceGUI(m_selectedCorrespondenceID);
	m_isDirty = true;
}
//---------------------------------------------------------------------------

void TfrmMain::DeleteCorrespondenceGUI()
{
	//delete the GUI elements
	for (int i=correspondence_pannel->ControlCount-1;i>=0;i--)
	{
		TPanel *p = dynamic_cast<TPanel*>(correspondence_pannel->Controls[i]);
		if (p)
		{
			for (int i=p->ControlCount-1;i>=0;i--)
			{
				String thiscontrolID = "";
				TControl*Temp = p->Controls[i];

				if (Temp->Name.SubString(0,MEMONAME.Length()) == MEMONAME)
					thiscontrolID = Temp->Name.SubString(MEMONAME.Length()+1,Temp->Name.Length());
				else if (Temp->Name.SubString(0,MEMOPANALTOPNAME.Length()) == MEMOPANALTOPNAME)
					thiscontrolID = Temp->Name.SubString(MEMOPANALTOPNAME.Length()+1,Temp->Name.Length());
				else
					continue;
				delete Temp;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::DeleteCorrespondenceGUI(String &ID)
{
	//delete the GUI elements
	for (int i=correspondence_pannel->ControlCount-1;i>=0;i--)
	{
		TPanel *p = dynamic_cast<TPanel*>(correspondence_pannel->Controls[i]);
		if (p)
		{
			for (int i=p->ControlCount-1;i>=0;i--)
			{
				String thiscontrolID = "";
				TControl*Temp = p->Controls[i];

				if (Temp->Name.SubString(0,MEMONAME.Length()) == MEMONAME)
					thiscontrolID = Temp->Name.SubString(MEMONAME.Length()+1,Temp->Name.Length());
				else if (Temp->Name.SubString(0,MEMOPANALTOPNAME.Length()) == MEMOPANALTOPNAME)
					thiscontrolID = Temp->Name.SubString(MEMOPANALTOPNAME.Length()+1,Temp->Name.Length());
				else
					continue;

				if (thiscontrolID == ID)
					delete Temp;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::UpdateCorrespondenceGUI(String &ID)
{
	std::string stdID = std::string(AnsiString(ID.c_str()).c_str());

	//update the GUI elements
	for (int k=correspondence_pannel->ControlCount-1;k>=0;k--)
	{
		TPanel *p = dynamic_cast<TPanel*>(correspondence_pannel->Controls[k]);
		if (p)
		{
			for (int i=p->ControlCount-1;i>=0;i--)
			{
				TMemo *pTM = NULL;
				TLabel *pLb= NULL;

				String thiscontrolID = "";
				TControl*Temp = p->Controls[i];

				if (Temp->Name.SubString(0,MEMONAME.Length()) == MEMONAME)
				{
					thiscontrolID = Temp->Name.SubString(MEMONAME.Length()+1,Temp->Name.Length());
					if (ID == thiscontrolID)
						pTM = dynamic_cast<TMemo*>(Temp);
				}
				else if (Temp->Name.SubString(0,MEMOPANALTOPNAME.Length()) == MEMOPANALTOPNAME)
				{
					thiscontrolID = Temp->Name.SubString(MEMOPANALTOPNAME.Length()+1,Temp->Name.Length());
					if (ID != thiscontrolID)
						continue;
					TPanel *T = dynamic_cast<TPanel*>(Temp);
					if (T)
					{
						//find the GUI element which holds the summary
						for (int t=0;t<T->ControlCount;t++)
						{
							TLabel *l = dynamic_cast<TLabel*>(T->Controls[t]);
							if (l)
							{
								if (l->Name == "summary")
									pLb = l;
							}
						}
					}
				}
				else
					continue;
//update the text on any controls we have found.
				if (pTM)
					pTM->Text = String(m_correspondence.getCorrespondenceText(stdID).c_str()).TrimRight();

				if (pLb)
					pLb->SetTextBuf((String("     ") + String(m_correspondence.getCorrespondenceSummary(stdID).c_str())).c_str());
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Cancel_Correspondence_ButtonClick(TObject *Sender)
{
//cancelled the edit correspondence
	m_selectedCorrespondenceID = "";
	Summary_Edit->Text = "";
	Correspondence_Memo->Text = "";

	Add_Correspondence_Button->Visible = true;
	Update_Correspondence_Button->Visible = false;
	Cancel_Correspondence_Button->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Update_Correspondence_ButtonClick(TObject *Sender)
{
	if (!isCorrespondenceValid())
		return;
//update db
	if (!m_correspondence.updateCorrespondence(m_selectedCorrespondenceID,Summary_Edit->Text,Correspondence_Memo->Text))
	{
		Application->MessageBox(L"Failed to update correspondence. Please try again.", L"Error", MB_OK);
		return;
	}
	UpdateCorrespondenceGUI(m_selectedCorrespondenceID);

//reset the UI from edit mode
	Cancel_Correspondence_ButtonClick(NULL);
	m_isDirty = true;
}


//---------------------------------------------------------------------------
void __fastcall TfrmMain::NewProject_ButtonClick(TObject *Sender)
{
	m_isInitalising = true;
	m_currentWindowsUserIDNumber = 0;
	userValue_Edit->Text = "1";
	ApplicationID_Edit->Text = "";
	project_summary_Edit->Text = "";
	PI_Email_Edit->Text = "";
	PI_Memo->Text = "";
	Project_Notes_Memo->Text = "";
	AccountName_Edit->Text = "";
	projectStatus_ComboBox->Text = "Active";

	m_Application_UID = -1;
	showMainGUI();
	DeleteProject_Button->Enabled = false;
	m_isDirty = false;
	setAccountNameLockState(true);
	m_isInitalising = false;
	ApplicationID_Edit->ReadOnly = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Projects_ComboBoxChange(TObject *Sender)
{
	if (Projects_ComboBox->ItemIndex == -1)
		return;

	util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(Projects_ComboBox->Items->Objects[Projects_ComboBox->ItemIndex]);
	project_summery_infomation_Edit->Text = String(mapHolder->m_map["summary"].c_str());

	OpenProject_Button->Enabled = true;
	DeleteProject_Button->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::OpenProject_ButtonClick(TObject *Sender)
{
	if (Projects_ComboBox->ItemIndex == -1)
		return;

	m_isInitalising = true;
	util_stdStringmapHolder *mapHolder = static_cast<util_stdStringmapHolder * >(Projects_ComboBox->Items->Objects[Projects_ComboBox->ItemIndex]);
	m_Application_UID = atoi(mapHolder->m_map["app_id"].c_str());
	DeleteCorrespondenceGUI();

	try
	{
		loadProject(m_Application_UID);
	}
	catch(String e)
	{
		showErrorReport("Error report","Error loading from data. Please try running it again.\r\nIf this error persists, please contact support",AnsiString(e.c_str()).c_str());
		return;
	}
	catch(...)
	{
		showErrorReport("Error report","Error loading from data. Please try running it again.\r\nIf this error persists, please contact support","Unknown Error");
		return;
	}
//update GUI
	showMainGUI();
	ApplicationID_Edit->ReadOnly = true;

	AccountName_EditChange(NULL);
	reFreshUserTable();
	m_isDirty = false;
	DeleteProject_Button->Enabled = false;
	m_isInitalising = false;
}

//---------------------------------------------------------------------------
void TfrmMain::showMainGUI()
{
	introPannel->Visible = false;
	correspondence_pannel->Visible = false;

	footer_GroupBox->Visible = true;
	MainTabControl->Visible = true;
	Page1->Visible = true;

	DeleteCorrespondenceGUI();
	MainTabControl->TabIndex = 0;
	MainTabControlChange(NULL);
}

//---------------------------------------------------------------------------
void TfrmMain::loadProject(int UID)
{
	ROSETTA output;
	std::stringstream sql;
	sql << "SELECT * FROM ru_application WHERE app_id = '" << UID << "' AND (status = 1 OR status = 2)";
	XQUERY query(m_db.get(),sql.str());

	query.setAcceptNull(true);

	if (!query.open())
		throw String(query.getLastError().c_str());

	if (query.fetch(&output))
	{
		ApplicationID_Edit->Text = output.getString("app_id").c_str();
		PI_Memo->Text = output.getString("pi").c_str();
		Project_Notes_Memo->Text = output.getString("info").c_str();
		PI_Email_Edit->Text = output.getString("piemail").c_str();
		project_summary_Edit->Text = output.getString("summary").c_str();
		m_UserAccountName =  output.getString("useraccountpattern");
		AccountName_Edit->Text = m_UserAccountName.c_str();

		if (output.getInt("status") == 1)
			projectStatus_ComboBox->Text = "Active";
		else
			projectStatus_ComboBox->Text = "Archived";

		m_currentWindowsUserIDNumber =  output.getInt("currentusernum");
	}
	query.close();
	m_isDirty = false; //as this is set to true when the names are set.
	m_projectAccount.load(m_db.get(),UID);
	m_correspondence.load(m_db.get(),UID);

	std::stringstream ss;
	ss << (m_currentWindowsUserIDNumber+1);
	userValue_Edit->Text = ss.str().c_str();
	setAccountNameLockState(false);
}

//---------------------------------------------------------------------------
bool TfrmMain::isDirty()
{
	if (m_isDirty)
		return true;
	return false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Cancel_ButtonClick(TObject *Sender)
{
	if (isDirty())
	{
		if (MessageDlg("You have not saved...If you press OK you will loose any changes you have made.", mtConfirmation, TMsgDlgButtons() << mbOK << mbCancel,0) == mrCancel)
			return;
	}
	if (updateUser_Button->Visible)
		cancelUser_ButtonClick(NULL);

	m_isInitalising = true;
	project_summery_infomation_Edit->Text = L"";
	fillProjectCombo();
	introPannel->Visible = true;
	footer_GroupBox->Visible = false;
	Page1->Visible = false;
	Page2->Visible = false;
	correspondence_pannel->Visible = false;
	Users_StringGrid->Visible = false;
	MainTabControl->Visible = false;
	m_Application_UID = -1;
	ApplicationID_Edit->Text = "";
	PI_Memo->Text = "";
	Project_Notes_Memo->Text = "";
	m_correspondence.clear();
	m_projectAccount.clear();
	reFreshUserTable();
	OpenProject_Button->Enabled = false;
	m_isDirty = false;
	m_isInitalising = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Users_StringGridMouseDown(TObject *Sender, TMouseButton Button,TShiftState Shift, int X, int Y)
{
	if (Button != 1)
		return;

	int top = Users_StringGrid->Selection.Top;
	int bot = Users_StringGrid->Selection.Bottom;

	int ACol,ARow;
	Users_StringGrid->MouseToCell(X,Y,ACol,ARow);
	if (ARow <= 0)  //either header, or -1 for invalid
		return;
//if we right mouse clicked within the range, we have a range selection.
//if we right mouse clocked out side a selected range, then just select that line.

	if (top == bot || ARow < top || ARow > bot)
	{ //single row select
		Users_StringGrid->Row = ARow;
		userAccount_PopupMenu->Popup(Mouse->CursorPos.X,Mouse->CursorPos.Y);
	}
	else
	{ //multiple rows.. just change status option..
		changeUSerStatus_PopupMenu->Popup(Mouse->CursorPos.X,Mouse->CursorPos.Y);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::UserAccount_EditClick(TObject *Sender)
{
	AddWinAccountButton->Visible = false;
	AddLinuxAccountButton->Visible = false;
	AddGPUAccountButton->Visible = false;
	updateUser_Button->Visible = true;
	cancelUser_Button->Visible = true;
	Users_StringGrid->Enabled = false;

	int selected = Users_StringGrid->Row;
	String id = Users_StringGrid->Cells[0][selected];

	m_projectAccount.getAttribute()->clearAttib();

	if (!m_projectAccount.setCurrent(id.ToInt()))
		return;

	AccountNameLock_CheckBox->Visible = false;
	Label13->Visible = false;
	userValue_Edit->Visible = false;

	m_isInitalising = true;
	userStatus_ComboBox->Text = m_projectAccount.getStatusType();
	AccountName_Edit->Text = m_projectAccount.getUserName();
	m_isInitalising = false;
	AccoutAttribs_ButtonGroup->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::updateUser_ButtonClick(TObject *Sender)
{
	std::string userName = AnsiString(AccountName_Edit->Text.c_str()).c_str();
	std::string attribs = m_projectAccount.getAttribute()->seriliseAttrib();
	int status = projectAccount::getStatusValue(userStatus_ComboBox->Text);

//only validate the user name if it has been changed.
	if (AccountName_Edit->Text != m_projectAccount.getUserName())
	{
		if (!isUserAccountValid(userName,m_projectAccount.getOSType()))
			return;
	}

	AccountNameLock_CheckBox->Visible = true;
	Label13->Visible = true;
	userValue_Edit->Visible = true;

	projectAccount::updateUserAccount(m_projectAccount.getuserID(),status,userName,attribs);
	m_projectAccount.getAttribute()->clearAttib();

	m_isInitalising = true;

	AccountName_Edit->Text = m_UserAccountName.c_str();

	AddWinAccountButton->Visible = true;
	AddLinuxAccountButton->Visible = true;
	AddGPUAccountButton->Visible = true;
	updateUser_Button->Visible = false;
	cancelUser_Button->Visible = false;
	Users_StringGrid->Enabled = true;

	m_isInitalising = false;

	reFreshUserTable();
	m_isDirty = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if (isDirty())
	{
		if (MessageDlg("You have not saved...If you press OK you will loose any changes you have made.", mtConfirmation, TMsgDlgButtons() << mbOK << mbCancel,0) == mrCancel)
			CanClose = false;
	}
}
//---------------------------------------------------------------------------

void TfrmMain::initaliseDB()
{
	if (m_db)
		return;

	try
	{
 //		String dbName = "brat_cp::jonathan_test";
		String dbName = "bb5_extract::ace_trex_live";
		m_db = std::unique_ptr<XDB>( new XDB( AnsiString(dbName.c_str()).c_str() ) );
		if (!m_db->open())
			throw( std::string("Error connecting to database"));
	}
	catch( const std::runtime_error& e )
	{
		throw String(e.what());
	}
	catch(String &e)
	{
		showErrorReport("Error report","Error initalisaing. This program will now shutdown. Please try running it again.\r\nIf this error persists, please contact support",AnsiString(e.c_str()).c_str());
		this->Close();
	}
}
//---------------------------------------------------------------------------
void TfrmMain::initalise()
{
	try
	{
		try
		{
			attribute::initalise(m_db.get());
			fillProjectCombo();

			if (attribute::m_avalibleAttribs.size() == 0)
			{
				AccoutAttribs_ButtonGroup->Visible = false;
			}
			else
			{
				//fill out the possible options which each user account can have in the GUI
				std::map<std::string,int>::iterator it;
				for (it=attribute::m_avalibleAttribs.begin(); it!=attribute::m_avalibleAttribs.end(); ++it)
				{
					TGrpButtonItem *gbi = AccoutAttribs_ButtonGroup->Items->Add();
					gbi->Caption = it->first.c_str();
				}
			}

			Users_StringGrid->Cells[1][0] = "User Name";
			Users_StringGrid->Cells[2][0] = "Type";
			Users_StringGrid->Cells[3][0] = "Status";
			Users_StringGrid->Cells[4][0] = "Options";
			m_projectAccount.initaliseAccounts();
			reFreshUserTable();
		}
		catch( const std::runtime_error& e )
		{
			throw String( e.what() );
		}
	}
	catch(String &e)
	{
		showErrorReport("Error report","Error initalisaing. This program will now shutdown. Please try running it again.\r\nIf this error persists, please contact support",AnsiString(e.c_str()).c_str());
		this->Close();
	}
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::SignIn_ButtonClick(TObject *Sender)
{
	try{
//add format check, if there is one.. do same for password
	if (username_Edit->Text.IsEmpty())
	{
		BalloonHint->Title = "Missing information.";
		BalloonHint->Description = "Please enter a username";
		BalloonHint->ShowHint(username_Edit);
		return;
	}
	if (password_Edit->Text.IsEmpty())
	{
		BalloonHint->Title = "Missing information.";
		BalloonHint->Description = "Please enter a password";
		BalloonHint->ShowHint(password_Edit);
		return;
	}


	initaliseDB();

	std::string username = AnsiString(username_Edit->Text.c_str()).c_str();
	std::string password = AnsiString(password_Edit->Text.c_str()).c_str();

	if (!isValidCTSUUserName(username))
	{
		BalloonHint->Title = "Incorrect username.";
		BalloonHint->Description = "Username not found, please check and try again.";
		BalloonHint->ShowHint(username_Edit);
		return;
	}

    BalloonHint->HideHint();
	LdapUser ldap;
	std::string error = ldap.initialize("");
	if (error.length() > 0)
	{
		showErrorReport("Error report","Error communicating with LDAP server. Please try running it again.\r\n\r\nIf this error persists, please contact support",error);
		return;
	}
	error = ldap.authenticate(username,password);
	if (error.length() > 0)
	{
		showErrorReport("Error report","Unable to authenticate, please check your username and password.",error);
		return;
	}
//
	UserLogin_Panel->Visible = false;
	initalise();
	introPannel->Visible = true;
	}
	catch (std::string e)
	{
		showErrorReport("Error report","Unable to connect to database",e);
		return;
	}
}
//---------------------------------------------------------------------------

bool TfrmMain::isValidCTSUUserName(const std::string &username)
{
	int count = -1;
	std::string sql = "SELECT count(*) FROM user WHERE username = '" + username + "' AND status = 0";
	XQUERY query(m_db.get(),sql);
	if (!query.open())
		throw String(query.getLastError().c_str());

	if (query.fetch())    //any data?
		count = query.result.getLint(0);
	query.close();
	return (count > 0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormShow(TObject *Sender)
{
	MainTabControl->Visible = false;
	Page1->Visible = false;
	Page2->Visible = false;
	footer_GroupBox->Visible = false;
	correspondence_pannel->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ApplicationID_EditChange(TObject *Sender)
{
	if (m_isInitalising)
		return;

	String regExp = "^[0-9]{3,30}$";
	if (!TRegEx::IsMatch(ApplicationID_Edit->Text,regExp))
	{
		BalloonHint->Title = "Warning";
		BalloonHint->Description = "Application id is a string of between 3 and 30 numbers. Please check and try again.";
		BalloonHint->ShowHint(ApplicationID_Edit);
	}
	else
	{
		m_isDirty = true;
		BalloonHint->HideHint();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::DeleteProject_ButtonClick(TObject *Sender)
{
	if (Projects_ComboBox->ItemIndex == -1)
		return;

	if (!askAreYouSure())
		return;

	util_iHolder *iHolder = static_cast<util_iHolder * >(Projects_ComboBox->Items->Objects[Projects_ComboBox->ItemIndex]);
	m_Application_UID = iHolder->value;

	std::stringstream sql;
	sql << "UPDATE ru_application SET status = 0 WHERE id = '" << m_Application_UID << "'";
	XEXEC query(m_db.get(),sql.str());

	if (!query.exec())
	{
		showErrorReport("Error report","Error deleting the project. Please try running it again.\r\nIf this error persists, please contact support",query.getLastError());
		return;
	}

	DeleteProject_Button->Enabled = false;
	OpenProject_Button->Enabled = false;
	fillProjectCombo();
}
//---------------------------------------------------------------------------

bool TfrmMain::askAreYouSure()
{
	if (MessageDlg("Are you sure?", mtConfirmation, TMsgDlgButtons() << mbOK << mbCancel,0) == mrCancel)
		return false;
	return true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::cancelUser_ButtonClick(TObject *Sender)
{
	AccountNameLock_CheckBox->Visible = true;
	m_projectAccount.getAttribute()->clearAttib();
	AccountName_Edit->Text = m_UserAccountName.c_str();

	AddWinAccountButton->Visible = true;
	AddLinuxAccountButton->Visible = true;
	AddGPUAccountButton->Visible = true;
	updateUser_Button->Visible = false;
	cancelUser_Button->Visible = false;

	Users_StringGrid->Enabled = true;
	reFreshUserTable();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::showArchivedProjects_CheckBoxClick(TObject *Sender)
{
	fillProjectCombo();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::statusChage_NewClick(TObject *Sender)
{
	updateUserStatus(projectAccount::getStatusValue("New"));
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::statusChage_ActiveClick(TObject *Sender)
{
	updateUserStatus(projectAccount::getStatusValue("Active"));
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::statusChage_DeletedClick(TObject *Sender)
{
	updateUserStatus(projectAccount::getStatusValue("Deleted"));
}
//---------------------------------------------------------------------------

void TfrmMain::updateUserStatus(int newStatus)
{
	int top = Users_StringGrid->Selection.Top;
	int bot = Users_StringGrid->Selection.Bottom;

	for (int i=top;i<=bot;i++)
	{
		int id = atoi(AnsiString(Users_StringGrid->Cells[0][i].c_str()).c_str());
		projectAccount::updateUserAccount(id,newStatus);
	}
	reFreshUserTable();
	m_isDirty = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::userValue_EditChange(TObject *Sender)
{
	if (m_isInitalising)
		return;

	if (!AccountNameLock_CheckBox->Visible)
		return;

	if (AccountNameLock_CheckBox->Checked)
	{
		std::stringstream ss;
		ss << (m_currentWindowsUserIDNumber+1);
		userValue_Edit->Text = ss.str().c_str();
		BalloonHint->Title = "Locked";
		BalloonHint->Description = "The next append is protected. To change this you need to uncheck this box first.";
		BalloonHint->ShowHint(AccountNameLock_CheckBox);
		return;
	}

	String value = userValue_Edit->Text;
	try
	{
		int num = value.ToInt();

		if (num < 0)
			throw new EConvertError("Bad user");
		m_currentWindowsUserIDNumber = num-1;
	}
	catch(EConvertError &e)
	{
		//not an int
		BalloonHint->Title = "invalid";
		BalloonHint->Description = "Please enter a number greater or equal to 0";
		BalloonHint->ShowHint(userValue_Edit);

		std::stringstream ss;
		ss << (m_currentWindowsUserIDNumber+1);
		userValue_Edit->Text = ss.str().c_str();
	}
}
//---------------------------------------------------------------------------

std::string TfrmMain::getPaddedString(int value,unsigned int minLength)
{
	std::string result;
	std::stringstream ss;
	ss << value;

	unsigned int sslength = ss.str().length();
	while (sslength + result.length() < minLength)
		result += "0";
	return result + ss.str();
}
//---------------------------------------------------------------------------

std::string TfrmMain::paddedString(unsigned int whitepadding,std::string value,unsigned int minLength)
{
	std::stringstream ss;
	for (unsigned int i=0;i<whitepadding;i++)
		ss << " ";
	ss << " " << value;
	while (ss.str().length() < minLength)
		ss << " ";
	return ss.str() ;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::Export_ButtonClick(TObject *Sender)
{
	SaveTextFileDialog->Filter = "Text files (*.txt)|*.TXT|Any file (*.*)|*.*";

	if (SaveTextFileDialog->Execute())
	{
		UnicodeString fileName = SaveTextFileDialog->FileName;

		XTIME D;
		D.setNow();

		std::stringstream output;
		output << "Report generated: ";

		output << getPaddedString(D.getDay()) << "/" << getPaddedString(D.getMonth()) << "/" << D.getYear() << " ";
		output << getPaddedString(D.getHour()) << ":" << getPaddedString(D.getMinute()) << ":" << getPaddedString(D.getSecond());
		output << "\r\n\r\n";

		std::map<int,int> activeUserMap;
 /*		std::map<int,std::string> userIDToApplicationID;

		{
		//get the count of the active users in each account
		std::string sql = "SELECT app_id FROM ru_application WHERE status = 1";
		XQUERY query(m_db.get(),sql);
		if (!query.open())
			throw String(query.getLastError().c_str());

		while (query.fetch())    //any data?
		{
 //			std::string ss = query.result.serializeOut();
  //		 std::string appid = query.result.getString("applicationid");
			int id = query.result.getLint("app_id");
			userIDToApplicationID.insert(std::pair<int,int>(id,id));
		}
		query.close();
		}
 */
		//get the count of the active users in each account
		std::string sql = "SELECT app_id ,count(*) as others FROM ru_user WHERE status = 1 group by app_id";
		XQUERY query(m_db.get(),sql);
		if (!query.open())
			throw String(query.getLastError().c_str());

		while (query.fetch())    //any data?
		{
			int appid = query.result.getLint("app_id");
			int count = query.result.getInt("others");
			activeUserMap.insert(std::pair<int,int>(appid,count));
		}
		query.close();

		sql = "select name,type,app_id from ru_user where status = 0 AND app_id IN (select app_id from ru_application where status = 1) order by app_id,type";
		query.setSQL(sql);
		if (!query.open())
			throw String(query.getLastError().c_str());

		int lastType = -1;
		int lastApplicationID = -1;

		while (query.fetch())    //any data?
		{
			int type = query.result.getInt("type");
			int application_id = query.result.getLint("app_id");

			if (lastApplicationID != application_id)
			{
				lastApplicationID = application_id;
				output << "\r\n\r\n\r\n Application ID:" << application_id;

				if ((activeUserMap.find(application_id) != activeUserMap.end()) && (activeUserMap[application_id] > 0))
					output << "   [" << activeUserMap[application_id] << " user accounts already in this group]\r\n";
				else
					output << "  [New user group]\r\n";

				output << paddedString(0,"BioBank research account") << paddedString(2," ") << "OS\r\n";
				output << paddedString(0,"------------------------") << paddedString(2," ") << "--\r\n";
			}

			if (lastType == -1)
				lastType = type;
			else
			{
				if (lastType != type)
				{
					output << "\r\n";
					lastType = type;
				}
			}
			std::string name =  paddedString(2,query.result.getString("name"));
			output << name  << paddedString(0," ");

			if (type == projectAccount::DEFINE_WINDOWS)
				output << "Windows";
			else if (type == projectAccount::DEFINE_LINUX)
				output << "Linux";
			else if (type == projectAccount::DEFINE_GPU)
				output << "GPU";
			else
				output << "Unknown";

			output << "\r\n";
		}
		query.close();

		TMemo *M = new TMemo(this);
		M->Text = output.str().c_str();
		M->Lines->SaveToFile(fileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Signout_ButtonClick(TObject *Sender)
{
	username_Edit->Text = "";
	password_Edit->Text = "";
	UserLogin_Panel->Visible = true;
	introPannel->Visible = false;
}
//---------------------------------------------------------------------------

void TfrmMain::setAccountNameLockState(bool newAccount)
{
	if (newAccount)
		AccountNameLock_CheckBox->Checked = false;
	else
	{
		if (AccountName_Edit->Text.IsEmpty())
			AccountNameLock_CheckBox->Checked = false;
		else
			AccountNameLock_CheckBox->Checked = projectAccount::isLike(AnsiString(AccountName_Edit->Text.c_str()).c_str());
	}
}
//---------------------------------------------------------------------------



