//---------------------------------------------------------------------------

#ifndef AddTankMapping1H
#define AddTankMapping1H

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "LDbBoxType.h"
#include <Db.hpp>
#include <DBTables.hpp>

//---------------------------------------------------------------------------

class TAddTankMapping : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TButton *Save;
	TButton *btnClose;
	TLabel *Label8;
	TEdit *edtSerial;
	TLabel *Label9;
	TQuery *QueryCentral;
	TLabel *Label2;
	TEdit *edtRackCount;
	TLabel *Label3;
	TEdit *edtPosition;
	TLabel *Label1;
	TEdit *edtTemperature;
	TComboBox *cbRackType;
	TLabel *Label4;
	TComboBox *cbLocation;

	void __fastcall FormActivate(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);

private:	// User declarations

public:		// User declarations

	__fastcall TAddTankMapping(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAddTankMapping *AddTankMapping;
//---------------------------------------------------------------------------
#endif

