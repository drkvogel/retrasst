#ifndef DiscardSamplesH
#define DiscardSamplesH
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>

#include <set>
#include <map>
#include <vector>
#include <string>

#include "DiscardContext.h"

class TfrmDiscardSamples;
class GridStuff;

class TfrmDiscardSamples : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlResults;
	TPanel *pnlSearch;
	TStringGrid *grdResults;
	TButton *btnConfirm;
    TButton *btnClear;
    TButton *btnSearch;
    TPopupMenu *popCol;
    TMenuItem *miSortAsc;
    TMenuItem *miSortDesc;
    TButton *btnNote;
    TPanel *pnlStatus;
    TLabel *labStatus;
    TPopupMenu *popCell;
    TMenuItem *miDiscardCell;
    TMenuItem *miViewNote;
    TPopupMenu *popRow;
    TMenuItem *miDiscardRow;
    TMenuItem *miDiscardCol;
    TPopupMenu *popTab;
    TMenuItem *miDiscardAll;
    TMenuItem *miViewDnote;
    TBitBtn *bitDiscard;
    TBitBtn *bitNote;
    TLabel *labMode;
    TMenuItem *miSortAsc2;
    TMenuItem *miSortDesc2;
    TMenuItem *miNoteCol;
    TMenuItem *miNoteCell;
    TMenuItem *miNoteRow;
    TMenuItem *miNoteAll;
    TButton *btnAbort;
	void __fastcall grdResultsDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall grdResultsMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnConfirmClick(TObject *Sender);
	void __fastcall btnClearClick(TObject *Sender);
    void __fastcall btnSearchClick(TObject *Sender);
    void __fastcall grdResultsKeyPress(TObject *Sender, char &Key);
    void __fastcall miSortAscClick(TObject *Sender);
    void __fastcall miSortDescClick(TObject *Sender);
    void __fastcall grdResultsContextPopup(TObject *Sender, TPoint &MousePos,
          bool &Handled);
    void __fastcall btnNoteClick(TObject *Sender);
    void __fastcall cmbModeChange(TObject *Sender);
    void __fastcall grdResultsMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall miDiscardCellClick(TObject *Sender);
    void __fastcall miViewNoteClick(TObject *Sender);
    void __fastcall miDiscardAllClick(TObject *Sender);
    void __fastcall miDiscardColClick(TObject *Sender);
    void __fastcall miDiscardRowClick(TObject *Sender);
    void __fastcall miViewDnoteClick(TObject *Sender);
    void __fastcall bitDiscardClick(TObject *Sender);
    void __fastcall bitNoteClick(TObject *Sender);
    void __fastcall miSortAsc2Click(TObject *Sender);
    void __fastcall miSortDesc2Click(TObject *Sender);
    void __fastcall miNoteColClick(TObject *Sender);
    void __fastcall miNoteCellClick(TObject *Sender);
    void __fastcall miNoteRowClick(TObject *Sender);
    void __fastcall miNoteAllClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btnAbortClick(TObject *Sender);
private:	// User declarations
	void showSamples( );
    void resizeColumns( TStringGrid * grid );
    void doSelect( const bool isNoting );
    std::pair<TColor,TColor> getColours( const Discard::Cell & cell );
	void updateUI( );
    bool hasResults( ) const;
    void sortSamples( const int sortColno, const int direction );
    std::string addSamples( );
    std::string getDraftNote( const Discard::Cell & cell );
    bool isClickWholeRow( );
    bool isClickWholeCol( );
    bool isClickWholeTab( );
	Discard::SamplePile m_samples;
	Discard::Cells m_cells;
    Discard::Context * m_context;
    std::string m_barcode;
    int m_ncols;
    int m_nrows;
    int m_clickedColno;
    int m_clickedRowno;
    int m_firstSampleColno;
    int m_firstSampleRowno;
public:		// User declarations
	__fastcall TfrmDiscardSamples(TComponent* Owner);
	static const TColor BACKGROUND_OOPS = TColor( 0xff00ff );
	static const TColor BACKGROUND_CONFIRMED = TColor( 0x88ff88 );
	static const TColor BACKGROUND_REMOVED = TColor( 0x88ffff );
	static const TColor BACKGROUND_DESTROYED = TColor( 0x8888ff );
	static const TColor BACKGROUND_CANTSELECT = TColor( 0xffdddd );
	static const TColor BACKGROUND_HEADER = TColor( 0xdddddd );
	static const TColor FOREGROUND_OOPS = TColor( 0xff77ff );
	static const TColor FOREGROUND_NORMAL = TColor( 0x000000 );
	static const TColor FOREGROUND_ASCENDING = TColor( 0x00cc00 );
	static const TColor FOREGROUND_DESCENDING = TColor( 0x0000cc );
	void init( Discard::Context * context );
	void reset( );
};

extern PACKAGE TfrmDiscardSamples *frmDiscardSamples;

#endif
