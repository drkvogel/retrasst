//---------------------------------------------------------------------------

#ifndef tFormMainH
#define tFormMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ButtonGroup.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <vector>
#include <string>
#include "xdb.h"
#include "xquery.h"
#include "projectAccount.h"
#include "correspondence.h"
//---------------------------------------------------------------------------

class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TEdit *AccountName_Edit;
	TButton *AddWinAccountButton;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *NoWinAccounts_Edit;
	TEdit *NoLinuxAccounts_Edit;
	TButton *AddLinuxAccountButton;
	TButton *Add_Correspondence_Button;
	TStringGrid *Users_StringGrid;
	TGridPanel *Page1;
	TLabel *Label3;
	TEdit *ApplicationID_Edit;
	TLabel *Label4;
	TMemo *PI_Memo;
	TMemo *Project_Notes_Memo;
	TGridPanel *Page21;
	TLabel *Label5;
	TButtonGroup *AccoutAttribs_ButtonGroup;
	TMemo *Correspondence_Memo;
	TPanel *correspondence_pannel;
	TTabControl *MainTabControl;
	TGroupBox *footer_GroupBox;
	TButton *Update_Button;
	TButton *Save_Exit_Button;
	TPanel *Panel1;
	TLabel *Summary_Label;
	TEdit *Summary_Edit;
	TPanel *SummaryPanel;
	TPanel *CorrispondenceHistory_Panel;
	TPanel *AddUser_TopPanel;
	TPopupMenu *correspondence_PopupMenu;
	TMenuItem *DeleteCorrespondence;
	TMenuItem *EditCorrespondence;
	TBalloonHint *BalloonHint;
	TButton *Update_Correspondence_Button;
	TButton *Cancel_Correspondence_Button;
	TLabel *projectTitle_Label;
	TButton *NewProject_Button;
	TComboBox *Projects_ComboBox;
	TPanel *introPannel;
	TButton *OpenProject_Button;
	TButton *Cancel_Button;
	TPopupMenu *userAccount_PopupMenu;
	TMenuItem *UserAccount_Edit;
	TButton *updateUser_Button;
	TPanel *UserLogin_Panel;
	TEdit *password_Edit;
	TEdit *username_Edit;
	TLabel *Label6;
	TButton *SignIn_Button;
	TLabel *Label8;
	TPanel *Page2;
	TLabel *Label7;
	TButton *DeleteProject_Button;
	TButton *cancelUser_Button;
	TEdit *project_summary_Edit;
	TLabel *Label9;
	TEdit *PI_Email_Edit;
	TLabel *Label10;
	TEdit *project_summery_infomation_Edit;
	TButton *AddGPUAccountButton;
	TLabel *Label11;
	TEdit *NoGPUAccounts_Edit;
	TCheckBox *showArchivedProjects_CheckBox;
	TComboBox *projectStatus_ComboBox;
	TLabel *Label12;
	TComboBox *userStatus_ComboBox;
	TPopupMenu *changeUSerStatus_PopupMenu;
	TMenuItem *Changestatusto1;
	TMenuItem *statusChage_New;
	TMenuItem *statusChage_Active;
	TMenuItem *statusChage_Deleted;
	TMenuItem *Changestatusto2;
	TMenuItem *New1;
	TMenuItem *Active1;
	TMenuItem *Delete1;
	TEdit *userValue_Edit;
	TLabel *Label13;
	TLabel *Status_labal;
	TButton *Export_Button;
	TSaveTextFileDialog *SaveTextFileDialog;
	TButton *Signout_Button;
	TCheckBox *AccountNameLock_CheckBox;
	void __fastcall PI_MemoChange(TObject *Sender);
	void __fastcall Project_Notes_MemoChange(TObject *Sender);
	void __fastcall AccoutAttribs_ButtonGroupButtonClicked(TObject *Sender, int Index);
	void __fastcall MainTabControlChange(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall AccountName_EditChange(TObject *Sender);
	void __fastcall AddLinuxAccountButtonClick(TObject *Sender);
	void __fastcall AddWinAccountButtonClick(TObject *Sender);
	void __fastcall AccoutAttribs_ButtonGroupDrawButton(TObject *Sender, int Index, TCanvas *Canvas, TRect &Rect, TButtonDrawState State);
	void __fastcall Add_Correspondence_ButtonClick(TObject *Sender);
	void __fastcall Update_ButtonClick(TObject *Sender);
	void __fastcall Save_Exit_ButtonClick(TObject *Sender);
	void __fastcall correspondence_pannelClick(TObject *Sender);
	void __fastcall labalClick(TObject *Sender);
	void __fastcall correspondenceMenuClick(TObject *Sender);
	void __fastcall EditCorrespondenceClick(TObject *Sender);
	void __fastcall DeleteCorrespondenceClick(TObject *Sender);
	void __fastcall Cancel_Correspondence_ButtonClick(TObject *Sender);
	void __fastcall Update_Correspondence_ButtonClick(TObject *Sender);
	void __fastcall NewProject_ButtonClick(TObject *Sender);
	void __fastcall Projects_ComboBoxChange(TObject *Sender);
	void __fastcall OpenProject_ButtonClick(TObject *Sender);
	void __fastcall Cancel_ButtonClick(TObject *Sender);
	void __fastcall UserAccount_EditClick(TObject *Sender);
	void __fastcall Users_StringGridMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall updateUser_ButtonClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall SignIn_ButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ApplicationID_EditChange(TObject *Sender);
	void __fastcall DeleteProject_ButtonClick(TObject *Sender);
	void __fastcall cancelUser_ButtonClick(TObject *Sender);
	void __fastcall AddGPUAccountButtonClick(TObject *Sender);
	void __fastcall showArchivedProjects_CheckBoxClick(TObject *Sender);
	void __fastcall statusChage_NewClick(TObject *Sender);
	void __fastcall statusChage_ActiveClick(TObject *Sender);
	void __fastcall statusChage_DeletedClick(TObject *Sender);
	void __fastcall userValue_EditChange(TObject *Sender);
	void __fastcall Export_ButtonClick(TObject *Sender);
	void __fastcall Signout_ButtonClick(TObject *Sender);

private:	// User declarations
	std::string getPaddedString(int value,unsigned int minLength = 2);
	std::string paddedString(unsigned int whitepadding,std::string value,unsigned int minLength = 25);
	void setAccountNameLockState(bool newAccount);
	void initalise(void);
	void initaliseDB(void);
    void showMainGUI(void);
	bool isValidCTSUUserName(const std::string &username);
	bool isUserNameUnquie(const std::string &name, int type);
	void AddNoWindowsAccount(int type);
	void DeleteCorrespondenceGUI(String &ID);
	void DeleteCorrespondenceGUI();
	void UpdateCorrespondenceGUI(String &ID);
	void renderCorrispondance();
	bool hasCorrespondence(String &&id);
	projectAccount m_projectAccount;
	void reFreshUserTable();
	bool saveProject();
	void loadProject(int UID);
	void openCorrespondencePannel(std::string &controllerID);
	String m_selectedCorrespondenceID;
	void fillProjectCombo();
	correspondence m_correspondence;
	std::unique_ptr<XDB> m_db;
	long m_Application_UID;
	int m_currentWindowsUserIDNumber;
	bool askAreYouSure();
	bool isUserAccountValid(const std::string &userName,int type);
	void showErrorReport(const std::string &title,const std::string &report,const std::string &details);
	bool isDirty();
	void updateUserStatus(int newStatus);
	bool isCorrespondenceValid();
	bool m_isDirty;
	bool m_isInitalising;
	std::string m_UserAccountName;
public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
