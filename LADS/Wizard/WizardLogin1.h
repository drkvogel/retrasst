//---------------------------------------------------------------------------
#ifndef WizardLogin1H
#define WizardLogin1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

#include "LCDbAuditTrail.h"

//---------------------------------------------------------------------------

class TWizardLogin : public TForm
{
__published:	// IDE-managed Components

	TRadioGroup *SystemChoice;
	TLabel *Label1;
	TLabel *Label4;
	TComboBox *cbxOperator;
	TLabel *Label5;
	TEdit *edtPassword;
	TButton *btnOperatorOK;
	TButton *btnExit;
	TButton *AboutButton;
	TDatabase *cDatabase;
	TQuery *cQuery;

	void __fastcall btnOperatorOKClick(TObject *Sender);
	void __fastcall SystemChoiceClick(TObject *Sender);
	void __fastcall btnExitClick(TObject *Sender);
	void __fastcall AboutButtonClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall cbxOperatorDropDown(TObject *Sender);

private:	// User declarations

	enum { LIVE_SYSTEM, MIRROR_SYSTEM };

	LCDbAuditTrail trail;

	void __fastcall AppException(TObject *Sender, Exception *E);

public:		// User declarations

	__fastcall TWizardLogin(TComponent* Owner);

	bool isMirror() const { return SystemChoice -> ItemIndex != LIVE_SYSTEM; }

	String setCaption( const String & formName ) const
	{
		return (isMirror() ? "MIRROR " : "LIVE ") + formName;
	}
};

//---------------------------------------------------------------------------
extern PACKAGE TWizardLogin *WizardLogin;
//---------------------------------------------------------------------------
#endif
