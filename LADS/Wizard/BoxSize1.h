//---------------------------------------------------------------------------

#ifndef BoxSize1H
#define BoxSize1H

#include <Classes.hpp>
#include <Controls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
#include <set>

//---------------------------------------------------------------------------

class TBoxSize : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TLabel *Label1;
	TButton *Add;
	TButton *Change;
	TPanel *Panel1;
	TButton *btnClose;
	TQuery *QueryCentral;
	TStringGrid *BoxSizeGrid;
	void __fastcall AddClick(TObject *Sender);
	void __fastcall ChangeClick(TObject *Sender);
	void __fastcall BoxSizeGridMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall BoxSizeGridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);

private:	// User declarations

	std::set< int > selected;
	void CopyFromClassToGrid();

	enum { BS_DESCRIPTION, BS_CAPACITY, BS_EMPTY_POS, BS_TUBE_TYPE,	BS_SIZE_ID,
		   BS_VALIDFROM, BS_VALIDTO, BS_STATUS, BS_NUM_COLS };

public:		// User declarations
	__fastcall TBoxSize(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBoxSize *BoxSize;
//---------------------------------------------------------------------------
#endif
