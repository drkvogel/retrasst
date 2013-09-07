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
	TButton *BtnAdd;
	TButton *BtnRetrieve;
	TComboBox *CmbAliquot1;
	TRadioGroup *RadIDType;
	TComboBox *CmbAliquot2;
	TLabel *LblAliquot2;
	TOpenDialog *OpenDialog1;
	TLabel *Label1;
	TComboBox *cbProject;
	TProgressBar *progress;
	TButton *BtnDest;
	TButton *Button1;
	void __fastcall AddClick(TObject *Sender);
	void __fastcall Retrieve(TObject *Sender);
	void __fastcall cbProjectChange(TObject *Sender);
	void __fastcall cbProjectDropDown(TObject *Sender);
	void __fastcall grdSamplesFixedCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall BtnDestClick(TObject *Sender);
	void __fastcall CmbAliquot1DropDown(TObject *Sender);
	void __fastcall CmbAliquot2DropDown(TObject *Sender);

private:	// User declarations

	enum Cols { SAMPLE, CRYOVIAL, ALIQUOT, OLD_BOX, OLD_POS, VESSEL, SHELF, STRUCTURE, NEW_BOX, NEW_POS, COL_COUNT };

	void populate( TComboBox * target, TComboBox * other );
	void drawGrid();
	void clearGrid();

public:		// User declarations

	struct GridEntry
	{
		std::string sid, cid;
		int aid;
		std::string adesc;
		std::string old_box, new_box;
		short old_pos, new_pos;
		std::string vessel, structure;
		short shelf;

		GridEntry( const ROSETTA & row );
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
