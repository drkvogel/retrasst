//---------------------------------------------------------------------------

#ifndef AddTestCalculation1H
#define AddTestCalculation1H

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include "LCDbTest.h"

//---------------------------------------------------------------------------

class CalculationElementClass;

class TAddTestCalculation : public TForm
{
__published:	// IDE-managed Components
	TComboBox *cbxTest1;
	TComboBox *cbxCalc;
	TComboBox *cbxTest2;
	TPanel *Panel1;
	TButton *btnCancel;
	TButton *btnSave;
	TEdit *edtTest1;
	TEdit *edtTest2;
	TLabel *Label1;
	TLabel *Label2;
	TQuery *QueryCentral;
	TButton *btnClear;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall btnSaveClick(TObject *Sender);
	void __fastcall btnClearClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);

private:	// User declarations

	int testID;

public:		// User declarations

	__fastcall TAddTestCalculation(TComponent* Owner);
	void setTest( int test ) { testID = test; }
};
//---------------------------------------------------------------------------
extern PACKAGE TAddTestCalculation *AddTestCalculation;
//---------------------------------------------------------------------------
#endif
