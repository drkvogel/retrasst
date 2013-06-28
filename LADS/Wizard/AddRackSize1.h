//---------------------------------------------------------------------------

#ifndef AddRackSize1H
#define AddRackSize1H

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "LDbBoxType.h"
#include <Db.hpp>
#include <DBTables.hpp>
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------

class TAddRackSize : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *edtDescription;
	TLabel *Label2;
	TEdit *edtCapacity;
	TLabel *Label6;
	TPanel *Panel1;
	TButton *Save;
	TButton *Cancel;
	TEdit *edtFirstRack;
	TQuery *QueryCentral;
	TLabel *Label3;
	TComboBox *cbBoxSize;
	TCheckBox *cbActive;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);

private:	// User declarations

	int rackSizeID;

public:		// User declarations

	__fastcall TAddRackSize( TComponent* Owner );
	void setRackSize( int sid ) { rackSizeID = sid; }
};

//---------------------------------------------------------------------------
extern PACKAGE TAddRackSize *AddRackSize;
//---------------------------------------------------------------------------
#endif
