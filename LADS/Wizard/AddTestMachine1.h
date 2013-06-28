//---------------------------------------------------------------------------
#ifndef AddTestMachine1H
#define AddTestMachine1H
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "LCDbTest.h"
#include <Db.hpp>
#include <DBTables.hpp>

//---------------------------------------------------------------------------

class TAddTestMachine : public TForm
{
__published:	// IDE-managed Components

	TLabel *Label1;
	TLabel *lblMachineName;
	TLabel *Label4;
	TEdit *edtMachineTestName;
	TLabel *Label8;
	TLabel *lblTest;
	TPanel *Panel1;
	TButton *Save;
	TButton *Cancel;
	TEdit *edtProtocol;
	TEdit *edtTestOrder;
	TLabel *Label2;
	TLabel *Label9;
	TQuery *QueryCentral;

	void __fastcall SaveClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);

private:	// User declarations

	LCDbTest theTest;
	int machineCID;

public:		// User declarations

	__fastcall TAddTestMachine(TComponent* Owner);

	void setTest( int testID );
	void setMachine( int machineID ) { machineCID = machineID; }
};

//---------------------------------------------------------------------------
extern PACKAGE TAddTestMachine *AddTestMachine;
//---------------------------------------------------------------------------
#endif
