//---------------------------------------------------------------------------
#ifndef Descriptor1H
#define Descriptor1H

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "ColourStringGrid.h"
#include <Db.hpp>
#include <DBTables.hpp>
#include <Psock.hpp>
#include <DB.hpp>

//---------------------------------------------------------------------------

class TDescriptor : public TForm
{
__published:	// IDE-managed Components

	TPanel *Panel1;
	TColourStringGrid *DescriptorTable;
	TPanel *Panel2;
	TButton *Barcode;
	TButton *Byhand;
	TButton *Specimen;
	TLabel *Label1;
	TLabel *Label2;
	TButton *Change;
	TButton *btnClose;
	TQuery *QueryProject;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall BarcodeClick(TObject *Sender);
	void __fastcall ByhandClick(TObject *Sender);
	void __fastcall SpecimenClick(TObject *Sender);
	void __fastcall ChangeClick(TObject *Sender);

	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall DescriptorTableMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);

private:	// User declarations

	void AddDefaultDescriptors();
	void addDescriptor( AnsiString source );
	void CheckAtLeastOneValue();

	// columns in Grid
	enum { DL_DESCRIPTOR, DL_DESCRIPTION, DL_MINLENGTH, DL_MAXLENGTH,
		   DL_SOURCE, DL_PARSEORDER, DL_DELIMITER, DL_DESCRIPTORID,
		   DL_SPECIMEN_FIELD, DL_MAP_FIELD, DL_ACTIVE, DL_NUM_COLS };

public:		// User declarations

	__fastcall TDescriptor(TComponent* Owner);

	void CopyFromClassToGrid();
    AnsiString findNextSpecimenFieldName();
    AnsiString findNextMapFieldName();
};
//---------------------------------------------------------------------------
extern PACKAGE TDescriptor *Descriptor;
//---------------------------------------------------------------------------
#endif
