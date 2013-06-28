//---------------------------------------------------------------------------

#ifndef Operator1H
#define Operator1H

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ColourStringGrid.h"
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <set>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

//---------------------------------------------------------------------------

class TOperator : public TForm
{
__published:	// IDE-managed Components

	TPanel *Panel2;
	TLabel *Label1;
	TButton *Add;
	TButton *Change;
	TPanel *Panel1;
	TButton *btnClose;
	TQuery *QueryCentral;
	TStringGrid *OperatorTable;

	void __fastcall AddClick(TObject *Sender);
	void __fastcall ChangeClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall OperatorTableMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall OperatorTableDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);

private:	// User declarations

	std::set< int > selected;
	enum { O_OPERATOR, O_DESCRIPTION, O_VALIDFROM, O_VALIDTO, O_STATUS,
			O_OPERATORID, O_OPERATORCOLS };

public:		// User declarations

	__fastcall TOperator(TComponent* Owner);
	void CopyFromClassToGrid();
};

//---------------------------------------------------------------------------
extern PACKAGE TOperator *Operator;
//---------------------------------------------------------------------------
#endif