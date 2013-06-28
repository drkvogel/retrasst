//---------------------------------------------------------------------------

#ifndef ModifiedLoginH
#define ModifiedLoginH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TfrmLogin.h"
#include <Buttons.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmLogin1 : public TfrmLogin
{
__published:	// IDE-managed Components
	void __fastcall analysersDropDown(TObject *Sender);
	void __fastcall initialise(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmLogin1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLogin1 *frmLogin1;
//---------------------------------------------------------------------------
#endif
