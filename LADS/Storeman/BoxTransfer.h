//---------------------------------------------------------------------------

#ifndef BoxTransferH
#define BoxTransferH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include "LPDbBoxes.h"
#include "BoxSummary.h"
#include "LDbBoxType.h"

class BoxType;
class BoxName;

//---------------------------------------------------------------------------

class TfrmBoxList : public TForm
{
__published:	// IDE-managed Components

	TComboBox *cbProject;
	TBitBtn *btnPrint;
	TBitBtn *btnExport;
	TSaveDialog *saveDialogue;
	TPrintDialog *printDialogue;
	TEdit *ebBoxNum;
	TLabel *Label2;
	TBitBtn *btnAddBox;
	TStringGrid *sgBoxNames;
	TTimer *timer;
	TStringGrid *sgBoxTypes;
	TLabel *Label1;
	TLabel *Label3;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall cbProjectChange(TObject *Sender);
	void __fastcall btnExportClick(TObject *Sender);
	void __fastcall btnPrintClick(TObject *Sender);
	void __fastcall btnAddBoxClick(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall timerTimer(TObject *Sender);
	void __fastcall sgBoxNamesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
		  TGridDrawState State);
	void __fastcall sgBoxTypesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
		  TGridDrawState State);
	void __fastcall sgBoxTypesSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall sgBoxTypesFixedCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall sgBoxNamesFixedCellClick(TObject *Sender, int ACol, int ARow);

private:	// User declarations

	enum Cols { TYPE, COUNT, PLUS, MINUS, NUM_COLS };

	std::vector< BoxType > types;
	bool (*typeOrder)( const BoxType &, const BoxType & );
	enum ColourState { CLEAR, RED, AMBER, GREEN };
	bool (*boxOrder)( const BoxName &, const BoxName & );

	LPDbBoxNames boxes;
	BoxSummary analyses;

	void setExpected( int row, int expected );
	void listBoxTypes();
	void listBoxNames();
	void addBox( const LPDbBoxName & box );
	String getPercentage( const String & boxName );
	TColor getColour( const String & boxName );
	int checkProgress( const String & boxName );

	void setPrintParams();
	void printHeader();
	void printText( const String & text );
	void printFooter();

	// printer parameters
	TPrinter * printer;
	int leftMargin, lineHeight, pages;
	int firstLine, lastLine, footer;
	int line, page;

public:		// User declarations

	__fastcall TfrmBoxList(TComponent* Owner);
	void init( const LCDbProject * proj );
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmBoxList *frmBoxList;
//---------------------------------------------------------------------------

#endif
