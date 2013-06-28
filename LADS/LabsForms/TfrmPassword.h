//---------------------------------------------------------------------------

#ifndef TfrmPasswordH
#define TfrmPasswordH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <DB.hpp>

#include "LCDbOperator.h"

//---------------------------------------------------------------------------

class TfrmPassword : public TForm
{
__published:	// IDE-managed Components

	TLabel *Label3;
	TEdit *ebNewPass;
	TBitBtn *okButton;
	TBitBtn *exitButton;
	TLabel *Label1;
	TEdit *ebConfirmed;
	TLabel *version;
	TLabel *Label2;
	TEdit *ebUserName;
	TLabel *Label4;
	TEdit *ebOldPass;

	void __fastcall ebNewPassChange(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall okButtonClick(TObject *Sender);

private:	// User declarations

    LQuery qCentral;
	LCDbOperator user;

public:		// User declarations

	__fastcall TfrmPassword(TComponent* Owner);
};

//---------------------------------------------------------------------------

extern PACKAGE TfrmPassword *frmPassword;

//---------------------------------------------------------------------------

#endif
