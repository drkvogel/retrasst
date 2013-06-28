//---------------------------------------------------------------------------
#ifndef AddOperator1H
#define AddOperator1H
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ComCtrls.hpp>
#include "LCDbOperator.h"

//---------------------------------------------------------------------------

class TAddOperator : public TForm
{
__published:	// IDE-managed Components

	TPanel *Panel1;
	TButton *Save;
	TButton *Cancel;
	TLabel *Label1;
	TEdit *OperatorName;
	TLabel *Label2;
	TMemo *Description;
	TLabel *Label4;
	TLabel *Label12;
	TLabel *Label5;
	TLabel *Label11;
	TEdit *Password;
	TLabel *Label6;
	TCheckBox *cbVal1;
	TCheckBox *cbVal2;
	TCheckBox *cbPlotqc;
	TCheckBox *cbWizard;
	TQuery *QueryCentral;
	TCheckBox *cbxStatus;
	TDateTimePicker *dtpFrom;
	TDateTimePicker *dtpTo;

	void __fastcall FormActivate(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);

private:	// User declarations

	int userID;

public:		// User declarations

	__fastcall TAddOperator(TComponent* Owner);
	void setOperator( int oper ) { userID = oper; }
};

//---------------------------------------------------------------------------
extern PACKAGE TAddOperator *AddOperator;
//---------------------------------------------------------------------------
#endif

