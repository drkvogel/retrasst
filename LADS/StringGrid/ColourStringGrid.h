//---------------------------------------------------------------------------
#ifndef ColourStringGridH
#define ColourStringGridH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include "CellProperties.h"
//---------------------------------------------------------------------------

class PACKAGE TColourStringGrid : public TStringGrid
{
	void __fastcall GrowGrid(int oldRows, int newRows,int oldCols, int newCols);
	void __fastcall ShrinkGrid(int oldRows, int newRows,int oldCols, int newCols);

	Graphics::TBitmap* __fastcall IconFromName(AnsiString icon_file);

	int colColCount;
	int colRowCount;
	int CSG_multiplier;
	int CSG_max;
	int FColCount;
	int FRowCount;
	bool FRoundRect;
	int FRoundRectX;
	int FRoundRectY;
	int FFirstBitmapLeft;
	bool FOutlineCell;

	int** theColourGrid;

protected:

	virtual void __fastcall DrawCell
			(int ACol, int ARow, const TRect &ARect, TGridDrawState AState);

	void __fastcall CMKillFocus(TMessage &Message);

	//mfc style message map macro
	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(WM_KILLFOCUS, TMessage, CMKillFocus)
	END_MESSAGE_MAP(TControl)

public:

	__fastcall TColourStringGrid(TComponent* Owner);
	virtual __fastcall ~TColourStringGrid();

	void GetMax(){}

	void __fastcall SetCellColour(int ACol, int ARow, TColor tc);
	void __fastcall DelCellColour(int ACol, int ARow);
	BOOL __fastcall GetCellColour(int ACol, int ARow, TColor& tc);

	void __fastcall SetFontColour(int ACol, int ARow, TColor tc);
	void __fastcall DelFontColour(int ACol, int ARow);
	BOOL __fastcall GetFontColour(int ACol, int ARow, TColor& tc);

	void __fastcall SetFontUnderline(int ACol, int ARow );
	bool __fastcall GetFontUnderline(int ACol, int ARow );
	void __fastcall DelFontUnderline(int ACol, int ARow);

	void __fastcall SetColColour(int ACol, TColor tc);
	void __fastcall SetRowColour(int ARow, TColor tc);
	void __fastcall DelColColour(int ACol);
	void __fastcall DelRowColour(int ARow);
	BOOL __fastcall GetColColour(int ACol, TColor& tc);
	BOOL __fastcall GetRowColour(int ARow, TColor& tc);
	void __fastcall SetColCount(int value);
	void __fastcall SetRowCount(int value);
	void __fastcall SetRoundRect(bool isRoundRect);
	void __fastcall SetOutlineCell( bool oc );// {  FOutlineCell = oc; }
	void __fastcall SetRoundRectX(int RoundRectX);
	void __fastcall SetRoundRectY(int RoundRectY);
	void __fastcall SetFirstBitmapLeft(int fbl) { FFirstBitmapLeft = fbl; }

	void __fastcall InsertRow(int row);
	void __fastcall CopyRow(int source_row, int dest_row);
	void __fastcall DeleteRow(int row);

	void __fastcall AddIcon(int ACol, int ARow, AnsiString icon_file);
	void __fastcall RemoveIcon(int ACol, int ARow, AnsiString icon_file);
	bool __fastcall IsIconDisplayed(int ACol, int ARow, AnsiString icon_file);

	void ClearAll();
	void __fastcall RemoveIcons(int ACol, int ARow);

__published:

	__property int ColCount = {read=FColCount, write=SetColCount, default=5};
	__property int RowCount = {read=FRowCount, write=SetRowCount, default=5};
	__property bool RoundRect = {read=FRoundRect, write=SetRoundRect, default=false};
	__property bool OutlineCell = {read=FOutlineCell, write=SetOutlineCell, default = false};
	__property int RoundRectX = {read=FRoundRectX, write=SetRoundRectX, default=0};
	__property int RoundRectY = {read=FRoundRectY, write=SetRoundRectY, default=0};
	__property int FirstBitmapLeft = {read=FFirstBitmapLeft, write=SetFirstBitmapLeft, default=90};
};

//---------------------------------------------------------------------------
#endif
