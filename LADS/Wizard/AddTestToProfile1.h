//---------------------------------------------------------------------------
#ifndef AddTestToProfile1H
#define AddTestToProfile1H

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "LCDbTest.h"
#include <Db.hpp>
#include <DBTables.hpp>

#include "LPDbProfile.h"

//---------------------------------------------------------------------------
class TAddTestToProfile : public TForm
{
__published:	// IDE-managed Components

	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TCheckBox *ckbIgnoreWarning;
	TEdit *editUpperLimit;
	TEdit *editLowerLimit;
	TPanel *Panel1;
	TButton *Cancel;
	TLabel *labelTestName;
	TLabel *Label4;
	TLabel *labelProfileName;
	TLabel *Label5;
	TEdit *editTriggerUpperLimit;
	TLabel *Label6;
	TEdit *editTriggerLowerLimit;
	TLabel *Label7;
	TLabel *Label8;
	TButton *btnSaveClose;
	TComboBox *cbxTriggerProfile;
	TLabel *Label9;
	TLabel *Label10;
	TComboBox *cbxMachines;
	TCheckBox *ckbPrivate;
	TCheckBox *ckbIgnoreLowerLimit;
	TLabel *lblHasCalc;
	TLabel *Label11;
	TEdit *edtUpperWarning;
	TQuery *QueryProject;
	TCheckBox *ckbIgnoreUpperLimit;
	TLabel *Label12;
	TEdit *edtLowerWarning;
	TLabel *Label13;
	void __fastcall btnSaveCloseClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations

	int profileID, testID;
	LPDbTestLimits theProfileTest;
	
	void setTriggerBox( int triggerID );
	void setMachineBox( int machineID );

public:		// User declarations

	__fastcall TAddTestToProfile(TComponent* Owner);
	void setProfile( int profile ) { profileID = profile; }
	void setTest( int test ) { testID = test; }
};
//---------------------------------------------------------------------------
extern PACKAGE TAddTestToProfile *AddTestToProfile;
//---------------------------------------------------------------------------
#endif
