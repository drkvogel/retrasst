//---------------------------------------------------------------------------

#ifndef BuddyLoginH
#define BuddyLoginH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "IdExplicitTLSClientServerBase.hpp"
#include "IdSMTPBase.hpp"
#include "TfrmLoginBase.h"
#include <Buttons.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>

//---------------------------------------------------------------------------

class TfrmBuddyLogin : public TfrmLoginBase
{
__published:	// IDE-managed Components

	TComboBox *machineName;
	TLabel *Label4;
	TLabel *Label1;
	TComboBox *cbCategory;

	void __fastcall initialise(TObject *Sender);
	void __fastcall okButtonClick(TObject *Sender);
	void __fastcall cbCategoryDropDown(TObject *Sender);

private:	// User declarations

	int categoryID;

	bool checkCategory();
	bool checkAnalyser();

public:		// User declarations

	__fastcall TfrmBuddyLogin(TComponent* Owner);
	int getCategory() const { return categoryID; }
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmBuddyLogin *frmBuddyLogin;
//---------------------------------------------------------------------------
#endif
