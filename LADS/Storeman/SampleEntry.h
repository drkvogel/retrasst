//---------------------------------------------------------------------------

#ifndef SampleEntryH
#define SampleEntryH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include "inventory.h"
#include <ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ComCtrls.hpp>

class LPDbBoxType;
class LCDbCryoJob;

//---------------------------------------------------------------------------

struct GridEntry {
	unsigned record_number;
	int sid, cid, aid, bid, pid;
	std::string sample, cryovial, aliquot;
	std::string site, vessel, structure;
	short location, shelf, rack_pos, slot;
	std::string old_box;
	short old_pos, new_pos;
	GridEntry( const ROSETTA & details, short position );
	void copyLocation( const GridEntry & other );
	void copyLocation( const ROSETTA & row );
};

//---------------------------------------------------------------------------

class TfrmRetrieveMain : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *grdSamples;
	TLabel *LblAliquot1;
	TButton *btnAddFile;
	TButton *btnLocate;
	TComboBox *CmbAliquot1;
	TRadioGroup *rgItemType;
	TComboBox *CmbAliquot2;
	TLabel *LblAliquot2;
	TOpenDialog *OpenDialog1;
	TLabel *Label1;
	TComboBox *cbProject;
	TProgressBar *progress;
	TListBox *lbSortCols;
	TLabel *Label2;
	TButton *btnAddRecords;
	TButton *btnSaveList;
	TButton *btnClrSort;
	TButton *btnClearList;

	void __fastcall AddClick(TObject *Sender);
	void __fastcall cbProjectChange(TObject *Sender);
	void __fastcall cbProjectDropDown(TObject *Sender);
	void __fastcall grdSamplesFixedCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall CmbAliquot1DropDown(TObject *Sender);
	void __fastcall CmbAliquot2DropDown(TObject *Sender);
	void __fastcall btnLocateClick(TObject *Sender);
	void __fastcall btnSaveListClick(TObject *Sender);
	void __fastcall btnClrSortClick(TObject *Sender);
	void __fastcall btnClearListClick(TObject *Sender);

private:	// User declarations

	enum Cols { SAMPLE, CRYOVIAL, ALIQUOT, SITE, LOCATION, VESSEL, SHELF,
			STRUCTURE, SLOT_POS, OLD_BOX, OLD_POS, NEW_POS, COL_COUNT };

	short nextPos;

	void populate( TComboBox * target, TComboBox * other );
	static int getAliquotTypeID( TComboBox * cb );
	void fillGrid();
	void resizeGrid();
	void checkBoxTypes();
	void setValue( int col, int row, short value );
	void setValue( int col, int row, const std::string & value );
	void addCryovials( const std::string & barcode );
	LPDbBoxType * getBoxType();
	LCDbCryoJob * createJob();
	void createBoxes( const LCDbCryoJob & job, const LPDbBoxType & boxType );
	void enableButtons();
	void updateDisplay();

	typedef int (*CompareFunction)( const GridEntry & a, const GridEntry & b );

	struct Column {
		CompareFunction fn;
		bool ascending;
		std::string label;
		Column( CompareFunction cf, bool asc, const std::string & lbl )
			: fn( cf ), ascending( asc ), label( lbl )
		{}
	};

	struct Sorter {
		std::vector< Column > columns;
		void addColumn( CompareFunction cf, const std::string & lbl );
		void clear() { columns.clear(); }
		bool empty() { return columns.empty(); }
		bool operator() ( const GridEntry & a, const GridEntry & b ) const;
	};
	Sorter sortList;

public:		// User declarations

	std::vector<IPart*> slist;
	std::vector<GridEntry> rows;

	__fastcall TfrmRetrieveMain(TComponent* Owner);
	void init();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRetrieveMain *frmRetrieveMain;
//---------------------------------------------------------------------------
#endif
