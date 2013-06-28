//---------------------------------------------------------------------------

#ifndef AddMachine1H
#define AddMachine1H

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ComCtrls.hpp>

#include "LCDbAnalyser.h"

//---------------------------------------------------------------------------

class TAddMachine : public TForm
{
__published:	// IDE-managed Components

	TLabel *Label1;
	TEdit *MachineName;
	TLabel *Label2;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label11;
	TEdit *Location;
	TLabel *Label12;
	TPanel *Panel1;
	TButton *Save;
	TButton *Cancel;
	TMemo *Description;
	TLabel *Label3;
	TComboBox *cbxCluster;
	TDateTimePicker *dtpValidFrom;
	TDateTimePicker *dtpValidTo;
	TRadioGroup *rgStatus;
	TQuery *QueryCentral;

	void __fastcall SaveClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);

private:	// User declarations

	enum { RGS_ACTIVE, RGS_OBSOLETE };		// status buttons 
	int analyserID;

	void FillClusterBox();

public:		// User declarations

	__fastcall TAddMachine(TComponent* Owner);
	void SetMachine( int buddyID ) { analyserID = buddyID; }
};

//---------------------------------------------------------------------------
extern PACKAGE TAddMachine *AddMachine;
//---------------------------------------------------------------------------
#endif
