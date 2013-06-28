//---------------------------------------------------------------------------

#ifndef AddDescriptor1H
#define AddDescriptor1H

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ColourStringGrid.h"
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include "LPDbDescriptor.h"

//---------------------------------------------------------------------------

class TAddDescriptor : public TForm
{
__published:	// IDE-managed Components

	TLabel *Label1;
	TEdit *edtName;
	TLabel *Label2;
	TEdit *edtDescription;
	TLabel *lblParseOrder;
	TEdit *edtParseOrder;
	TLabel *Label4;
	TLabel *Label10;
	TEdit *edtMinLength;
	TLabel *Label11;
	TEdit *edtMaxLength;
	TLabel *Label9;
	TLabel *Label6;
	TEdit *edtMapFieldName;
	TPanel *Panel1;
	TButton *Save;
	TButton *Cancel;
	TColourStringGrid *grdValue;
	TPanel *Panel2;
	TLabel *Label3;
	TButton *Add;
	TButton *Change;
	TPanel *Panel3;
	TButton *btnClose;
	TQuery *QueryProject;
	TComboBox *cbSource;
	TLabel *Label5;
	TEdit *edtSpecimenField;
	TLabel *Label7;
	TEdit *edtDelimiter;
	TComboBox *cbObjectType;
	TLabel *Label8;
	TCheckBox *cbRequired;
	TButton *Delete;

	void __fastcall FormActivate(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall AddClick(TObject *Sender);
	void __fastcall ChangeClick(TObject *Sender);
	void __fastcall grdValueMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall DeleteClick(TObject *Sender);

private:	// User declarations

	int descripID;
	String source;

	enum { VAL_NAME, VAL_MINVALUE, VAL_MAXVALUE, VAL_VALUEID, VAL_NUM_COLS };

	static bool inProfileMap( int valueID );

	void DefineValueGrid();
	bool checkMatch( const String & field1, const String & field2 );
	void CopyFromClassToGrid();

public:		// User declarations

	__fastcall TAddDescriptor(TComponent* Owner);
	void SetDescriptor( int descriptor ) { descripID = descriptor; }
	void SetSource( AnsiString src ) { source = src; }
};
//---------------------------------------------------------------------------
extern PACKAGE TAddDescriptor *AddDescriptor;
//---------------------------------------------------------------------------
#endif
