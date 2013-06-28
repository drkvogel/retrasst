//---------------------------------------------------------------------------
#ifndef AddValue1H
#define AddValue1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "LPDbDescriptor.h"
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------

class TAddValue : public TForm
{
__published:	// IDE-managed Components

	TLabel *Label2;
	TLabel *Label3;
	TEdit *Minimum;
	TEdit *Maximum;
	TPanel *Panel1;
	TButton *btnSave;
	TButton *btnCancel;
	TEdit *ValueName;
	TLabel *Label7;
	TQuery *QueryProject;

	void __fastcall FormActivate(TObject *Sender);
	void __fastcall btnSaveClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall MinimumExit(TObject *Sender);

private:	// User declarations

	LPDbDescriptor owner;
	int valueID;

	bool CheckValues( String strMinimum, String strMaximum );
	bool CheckLength( String value );
	bool CheckObject( String value );

public:		// User declarations

	__fastcall TAddValue(TComponent* Owner);
	void setValue( int dvID );
	void setDescriptor( int descripID );
};

//---------------------------------------------------------------------------

extern PACKAGE TAddValue *AddValue;

//---------------------------------------------------------------------------

#endif
