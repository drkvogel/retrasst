//---------------------------------------------------------------------------

#ifndef TfrmLoginH
#define TfrmLoginH

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
//#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>

//---------------------------------------------------------------------------

class TfrmLogin : public TfrmLoginBase
{
__published:	// IDE-managed Components
	TComboBox *projName;
	TLabel *Label4;

	void __fastcall initialise(TObject *Sender);
	void __fastcall okButtonClick(TObject *Sender);

private:	// User declarations

	TLeaseManager * lm;

public:		// User declarations

	__fastcall TfrmLogin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLogin *frmLogin;
//---------------------------------------------------------------------------
#endif
