//---------------------------------------------------------------------------

#ifndef TfrmLoginBaseH
#define TfrmLoginBaseH

#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include "LCDbOperator.h"
#include "LCDbProject.h"
#include "LCDbAuditTrail.h"
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdSMTPBase.hpp>

//---------------------------------------------------------------------------

class TfrmLoginBase : public TForm
{
__published:	// IDE-managed Components

    TBitBtn *exitButton;
    TLabel *Label2;
    TBitBtn *okButton;
    TComboBox *userList;
    TLabel *version;
    TTimer *timer;
	TLabel *Label3;
	TEdit *ebPassword;
	TLabel *lblSystem;
	TIdSMTP *IdSMTP1;

    void __fastcall exitButtonClick(TObject *Sender);
    void __fastcall okButtonClick(TObject *Sender);
    void __fastcall textChange(TObject *Sender);
    void __fastcall initialise(TObject *Sender);
	void __fastcall rgDatabaseClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall versionClick(TObject *Sender);
	void __fastcall hyperlinkEnter(TObject *Sender);
	void __fastcall hyperlinkLeave(TObject *Sender);
	void __fastcall FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight,
          bool &Resize);
	void __fastcall lblSystemClick(TObject *Sender);
	void __fastcall userListDropDown(TObject *Sender);

private:	// User declarations

	void __fastcall AppException(TObject *Sender, Exception *E);

protected:

	LCDbAuditTrail auditTrail;

	const LCDbOperator * logUserIn();
	void startProgram( TObject *Sender );

	virtual void runProgram( const std::string & title );

public:		// User declarations

	static TForm * mainForm;
	__fastcall TfrmLoginBase(TComponent* Owner);
};

//---------------------------------------------------------------------------

extern PACKAGE TfrmLoginBase *frmLoginBase;

//---------------------------------------------------------------------------
#endif
