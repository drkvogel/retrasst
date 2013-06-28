//---------------------------------------------------------------------------
#ifndef CentralTest1H
#define CentralTest1H
#include <Classes.hpp>
#include <Controls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------

#include <set>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ColourStringGrid.h"
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

//---------------------------------------------------------------------------

class TCentralTest : public TForm
{
__published:	// IDE-managed Components

	TPanel *Panel2;
	TLabel *Label1;
	TButton *Add;
	TButton *Change;
	TPanel *Panel1;
	TButton *btnClose;
	TQuery *QueryCentral;
	TStringGrid *TestTable;

	void __fastcall AddClick(TObject *Sender);
	void __fastcall ChangeClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall TestTableMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall TestTableDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
		  
private:	// User declarations

	enum { T_TESTID, T_TEST, T_DESCRIPTION, T_VALIDFROM, T_VALIDTO, T_STATUS,
		   T_SAMPLETYPE, T_DEFAULTLOWERLIMIT, T_DEFAULTUPPERLIMIT, T_SOURCE,
		   T_DATATYPE, T_MINDATAPOINTS, T_MAXDATAPOINTS, T_TESTCOLS };

	std::set< int > selected;

public:		// User declarations

	__fastcall TCentralTest(TComponent* Owner);
	void CopyFromClassToGrid();
};

//---------------------------------------------------------------------------
extern PACKAGE TCentralTest *CentralTest;
//---------------------------------------------------------------------------
// columns in Grid
//---------------------------------------------------------------------------
#endif
