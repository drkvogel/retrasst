//---------------------------------------------------------------------------

#ifndef RackSize1H
#define RackSize1H
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
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

class TRackSize : public TForm
{
__published:	// IDE-managed Components

	TPanel *Panel2;
	TLabel *Label1;
	TButton *Add;
	TButton *Change;
	TPanel *Panel1;
	TButton *btnClose;
	TQuery *QueryCentral;
	TStringGrid *RackSizeGrid;
	void __fastcall AddClick(TObject *Sender);
	void __fastcall ChangeClick(TObject *Sender);
	void __fastcall RackSizeGridMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall RackSizeGridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);

private:	// User declarations

	std::set< int > selected;
	void CopyFromClassToGrid();

	enum { RT_DESCRIPTION, RT_CID, RT_CAPACITY, RT_BOX_SIZE,
		   RT_CURRENT, RT_FIRST_RACK, RT_NUM_COLS };

public:		// User declarations

	__fastcall TRackSize(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRackSize *RackSize;
//---------------------------------------------------------------------------
#endif
