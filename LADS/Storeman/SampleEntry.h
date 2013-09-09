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
	TRadioGroup *RadIDType;
	TComboBox *CmbAliquot2;
	TLabel *LblAliquot2;
	TOpenDialog *OpenDialog1;
	TLabel *Label1;
	TComboBox *cbProject;
	TProgressBar *progress;
	TButton *btnDestination;
	TButton *btnAddRecords;
	TListBox *lbSortCols;
	TLabel *Label2;
	void __fastcall AddClick(TObject *Sender);
	void __fastcall cbProjectChange(TObject *Sender);
	void __fastcall cbProjectDropDown(TObject *Sender);
	void __fastcall grdSamplesFixedCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnDestinationClick(TObject *Sender);
	void __fastcall CmbAliquot1DropDown(TObject *Sender);
	void __fastcall CmbAliquot2DropDown(TObject *Sender);
	void __fastcall btnLocateClick(TObject *Sender);

private:	// User declarations

	enum Cols { SAMPLE, CRYOVIAL, ALIQUOT, VESSEL, SHELF, STRUCTURE, OLD_BOX, OLD_POS, NEW_BOX, NEW_POS, COL_COUNT };

	void populate( TComboBox * target, TComboBox * other );
	static int getTypeID( TComboBox * cb );
	void drawGrid();
	void resizeGrid();
	void setValue( int col, int row, short value );
	void setValue( int col, int row, const std::string & value );

public:		// User declarations

	struct GridEntry
	{
		int sid, cid, aid, bid;
		std::string sample, cryovial, aliquot;
		std::string old_box, new_box;
		short old_pos, new_pos;
		std::string vessel, structure;
		short rack_pos, shelf;

		GridEntry( const ROSETTA & row );
		void copyLocation( const GridEntry & other );
		void copyLocation( const ROSETTA & row );
	};

	std::vector<GridEntry> rows;
	std::vector<IPart*> slist;

	__fastcall TfrmRetrieveMain(TComponent* Owner);
	void init();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRetrieveMain *frmRetrieveMain;
//---------------------------------------------------------------------------
#endif
