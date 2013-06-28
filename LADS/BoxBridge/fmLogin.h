//---------------------------------------------------------------------------

#ifndef fmLoginH
#define fmLoginH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "IdExplicitTLSClientServerBase.hpp"
#include "IdSMTPBase.hpp"
#include "TfrmLogin.h"
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
#include "TfrmLoginBase.h"
//---------------------------------------------------------------------------

class TfrmLoginBoxBridge : public TfrmLoginBase
{
__published:	// IDE-managed Components

	void __fastcall initialise(TObject *Sender);
	void __fastcall okButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// User declarations

	virtual void runProgram( const String & title );

public:		// User declarations
	__fastcall TfrmLoginBoxBridge(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLoginBoxBridge *frmLoginBoxBridge;
//---------------------------------------------------------------------------
#endif
