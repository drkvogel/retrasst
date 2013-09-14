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
	TButton *btnDestination;
	TListBox *lbSortCols;
	TLabel *Label2;
	TButton *btnAddRecords;
	TButton *btnSaveList;
	TButton *btnClrSort;
	TButton *btnNewContent;
	TLabel *Label3;
	TComboBox *cbBoxType;
	void __fastcall AddClick(TObject *Sender);
	void __fastcall cbProjectChange(TObject *Sender);
	void __fastcall cbProjectDropDown(TObject *Sender);
	void __fastcall grdSamplesFixedCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnDestinationClick(TObject *Sender);
	void __fastcall CmbAliquot1DropDown(TObject *Sender);
	void __fastcall CmbAliquot2DropDown(TObject *Sender);
	void __fastcall btnLocateClick(TObject *Sender);
	void __fastcall btnSaveListClick(TObject *Sender);
	void __fastcall btnClrSortClick(TObject *Sender);
	void __fastcall btnNewContentClick(TObject *Sender);

private:	// User declarations

	enum Cols { SAMPLE, CRYOVIAL, ALIQUOT, SITE, LOCATION, VESSEL, SHELF,
			STRUCTURE, SLOT_POS, OLD_BOX, OLD_POS, NEW_BOX, NEW_POS, COL_COUNT };

	void populate( TComboBox * target, TComboBox * other );
	static int getTypeID( TComboBox * cb );
	void drawGrid();
	void resizeGrid();
	void setValue( int col, int row, short value );
	void setValue( int col, int row, const std::string & value );
	void enableButtons();

public:		// User declarations

	struct GridEntry {
		unsigned record_number;
		int sid, cid, aid, oid, nid;
		std::string sample, cryovial, aliquot;
		std::string site, vessel, structure;
		short location, shelf, rack_pos, slot;
		std::string old_box, new_box;
		short old_pos, new_pos;
		GridEntry( const ROSETTA & row );
		void copyLocation( const GridEntry & other );
		void copyLocation( const ROSETTA & row );
	};
	std::vector<GridEntry> rows;

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
		bool operator() ( const GridEntry & a, const GridEntry & b ) const;
	};
	Sorter sortList;

	std::vector<IPart*> slist;

	__fastcall TfrmRetrieveMain(TComponent* Owner);
	void init();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRetrieveMain *frmRetrieveMain;
//---------------------------------------------------------------------------
#endif
