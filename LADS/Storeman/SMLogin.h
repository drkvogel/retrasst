//---------------------------------------------------------------------------

#ifndef SMLoginH
#define SMLoginH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include "TfrmLoginBase.h"

//---------------------------------------------------------------------------

class TfrmSMLogin : public TfrmLoginBase
{
__published:	// IDE-managed Components

private:	// User declarations

	void runProgram( const std::string & title );

public:		// User declarations

	enum Pages { ANY, CONFIGURE, MOVE, DISCARD, REFERRED, RETRIEVE };		// permissions

	__fastcall TfrmSMLogin(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmSMLogin *frmSMLogin;
//---------------------------------------------------------------------------
#endif
