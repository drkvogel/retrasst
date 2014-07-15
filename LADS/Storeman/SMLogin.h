//---------------------------------------------------------------------------

#ifndef SMLoginH
#define SMLoginH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Vcl.Imaging.PngLang.hpp>
#include "TfrmLoginBase.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdSMTPBase.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>

//---------------------------------------------------------------------------

class TfrmSMLogin : public TfrmLoginBase
{
__published:	// IDE-managed Components

private:	// User declarations

	void runProgram( const std::string & title );

public:		// User declarations

	enum Pages { ANY, CONFIGURE, MOVE, DISCARD, REFERRED, RATIONALISE, RETRIEVE };	// permissions

	__fastcall TfrmSMLogin(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmSMLogin *frmSMLogin;
//---------------------------------------------------------------------------
#endif
