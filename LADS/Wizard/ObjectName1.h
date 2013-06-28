//---------------------------------------------------------------------------
#ifndef ObjectName1H
#define ObjectName1H

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ColourStringGrid.h"
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

#include "LCDbObject.h"

//---------------------------------------------------------------------------

class TObjectName : public TForm
{
__published:	// IDE-managed Components

	TPanel *Panel2;
	TLabel *Label1;
	TButton *Add;
	TButton *Change;
	TColourStringGrid *ObjectGrid;
	TPanel *Panel1;
	TButton *btnClose;
	TQuery *QueryCentral;

	void __fastcall AddClick(TObject *Sender);
	void __fastcall ChangeClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall ObjectGridMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);

private:	// User declarations

	LCDbObject::Category objectType;

	enum { OBJ_OBJECTID, OBJ_OBJECT, OBJ_FULL, OBJ_STATUS, OBJ_NUMCOLS };

	void CopyFromClassToGrid();

public:		// User declarations

	__fastcall TObjectName(TComponent* Owner);
	void setObjectType( int type ) { objectType = LCDbObject::findObjectType( type ); }
};

//---------------------------------------------------------------------------

extern PACKAGE TObjectName *ObjectName;

//---------------------------------------------------------------------------
#endif
