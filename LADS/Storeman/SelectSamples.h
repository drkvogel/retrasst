//---------------------------------------------------------------------------

#ifndef SelectSamplesH
#define SelectSamplesH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include "LPDbBoxes.h"
#include "LCDbJob.h"

//---------------------------------------------------------------------------

class TfrmSelectBoxes : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TLabel *Label1;
	TLabel *Label3;
	TComboBox *cbType;
	TEdit *txtName;
	TBitBtn *btnAddBox;
	TListBox *lstBoxes;
	TLabel *Label2;
	TEdit *txtProject;
	void __fastcall btnAddBoxClick(TObject *Sender);

private:	// User declarations

	LPDbBoxNames boxes;
	bool addBox();

public:		// User declarations

	__fastcall TfrmSelectBoxes(TComponent* Owner);
	bool init();
	const LPDbBoxNames & getSelected( ) const { return boxes; }
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmSelectBoxes *frmSelectBoxes;
//---------------------------------------------------------------------------
#endif
