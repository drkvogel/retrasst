//---------------------------------------------------------------------------

#ifndef AddBoxSize1H
#define AddBoxSize1H
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

class TAddBoxSize : public TForm
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
	TEdit *edtEmptyPos;
	TQuery *QueryCentral;
	TLabel *Label5;
	TLabel *Label12;
	TDateTimePicker *dtpFrom;
	TDateTimePicker *dtpTo;
	TCheckBox *cbActive;
	TCheckBox *cbVacutainer;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);

private:	// User declarations

	int boxSizeID;

public:		// User declarations

	__fastcall TAddBoxSize( TComponent* Owner );
	void setBoxSize( int bsid ) { boxSizeID = bsid; }
};

//---------------------------------------------------------------------------
extern PACKAGE TAddBoxSize *AddBoxSize;
//---------------------------------------------------------------------------
#endif
