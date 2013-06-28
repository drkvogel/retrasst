//---------------------------------------------------------------------------
#ifndef AddObjectName1H
#define AddObjectName1H
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

#include "LCDbObject.h"

//---------------------------------------------------------------------------

class TAddObjectName : public TForm
{
__published:	// IDE-managed Components
	TEdit *edtObjectName;
	TLabel *Label11;
	TEdit *edtFullName;
	TPanel *Panel1;
	TButton *Save;
	TButton *Cancel;
	TPanel *Panel2;
	TButton *Add;
	TButton *Delete;
	TPanel *Panel3;
	TButton *btnClose;
	TQuery *QueryCentral;
	TColourStringGrid *linkedGrid;
	TCheckBox *cbActive;
	TLabel *Label1;
	
	void __fastcall SaveClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall AddClick(TObject *Sender);
	void __fastcall DeleteClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall linkedGridMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall edtObjectNameExit(TObject *Sender);

private:	// User declarations

	enum { OT_LINKID, OT_LINKNAME, OT_NUMCOLS };

	int objectCID;
	LCDbObject::Category objectType;

	void copyFromClassToGrid();
	int copyCRTests();
	void addCRTests();
	int copyTanks();
	int copyMachines();

public:		// User declarations

	__fastcall TAddObjectName(TComponent* Owner);
	void setObjectType( int type ) { objectType = LCDbObject::findObjectType( type ); }
	void setObjectID( int objectID ) { objectCID = objectID; }
};

//---------------------------------------------------------------------------
extern PACKAGE TAddObjectName *AddObjectName;
//---------------------------------------------------------------------------
#endif
