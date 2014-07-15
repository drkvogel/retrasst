//---------------------------------------------------------------------------

#ifndef TfrmLoginH
#define TfrmLoginH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
#include <Forms.hpp>
#include "TfrmLoginBase.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdSMTPBase.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------

class TfrmLogin : public TfrmLoginBase
{
__published:	// IDE-managed Components
	TComboBox *projName;
	TLabel *Label4;

	void __fastcall okButtonClick(TObject *Sender);
	void __fastcall projNameDropDown(TObject *Sender);

private:	// User declarations

	TLeaseManager * lm;

public:		// User declarations

	__fastcall TfrmLogin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLogin *frmLogin;
//---------------------------------------------------------------------------
#endif
