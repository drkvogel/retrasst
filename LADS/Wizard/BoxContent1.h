//---------------------------------------------------------------------------

#ifndef BoxContent1H
#define BoxContent1H
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
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

//---------------------------------------------------------------------------

class TBoxContent : public TForm
{
__published:	// IDE-managed Components

	TPanel *Panel2;
	TLabel *Label1;
	TButton *Add;
	TButton *Change;
	TPanel *Panel1;
	TButton *btnClose;
	TQuery *QueryProject;
	TStringGrid *BoxContentGrid;

	void __fastcall AddClick(TObject *Sender);
	void __fastcall ChangeClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall BoxContentGridMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall BoxContentGridDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);

private:	// User declarations

	std::set< int > selected;
	void CopyFromClassToGrid();

	// columns in grid
	enum { BC_NAME, BC_DESCRIP, BC_SIZE, BC_ALIQUOT1, BC_ALIQUOT2, BC_ALIQUOT3,
		   BC_SET_LINK, BC_POSITION, BC_STATUS, BC_TYPE_ID, BC_NUM_COLS };

public:		// User declarations

	__fastcall TBoxContent(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TBoxContent *BoxContent;
//---------------------------------------------------------------------------
#endif

