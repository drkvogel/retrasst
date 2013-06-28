//---------------------------------------------------------------------------
#ifndef AddCentralTest1H
#define AddCentralTest1H
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include "LCDbTest.h"
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------

class TAddCentralTest : public TForm
{
__published:	// IDE-managed Components

	TLabel *Label1;
	TEdit *TestName;
	TLabel *Label2;
	TLabel *Label5;
	TLabel *Label12;
	TPanel *Panel1;
	TButton *Save;
	TButton *Cancel;
	TEdit *DefaultLowerLimit;
	TEdit *DefaultUpperLimit;
	TLabel *Label3;
	TLabel *Label13;
	TLabel *Label14;
	TMemo *Description;
	TLabel *Label15;
	TEdit *SampleType;
	TQuery *QueryCentral;
	TDateTimePicker *dtpFrom;
	TDateTimePicker *dtpTo;
	TCheckBox *cbActive;
	TPanel *Panel3;
	TButton *btnClose;
	TButton *btnAddMachines;
	TButton *btnCalculation;

	void __fastcall SaveClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall btnAddMachinesClick(TObject *Sender);
	void __fastcall btnCalculationClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);

private:	// User declarations

	int testID;

public:		// User declarations

	__fastcall TAddCentralTest(TComponent* Owner);
	void setTest( int test ) { testID = test; }
};
//---------------------------------------------------------------------------
extern PACKAGE TAddCentralTest *AddCentralTest;
//---------------------------------------------------------------------------
#endif

