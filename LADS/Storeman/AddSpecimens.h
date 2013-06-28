//---------------------------------------------------------------------------

#ifndef AddSpecimensH
#define AddSpecimensH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include "dvSelector.h"
#include <ExtCtrls.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include "SampleList.h"
#include <Buttons.hpp>

//---------------------------------------------------------------------------

class TfrmAddSpecimens : public TForm
{
__published:	// IDE-managed Components

	TStringGrid *sgSpecimens;
	TselectorFrame *selector;
	TBitBtn *BitBtn2;
	TBitBtn *BitBtn1;
	TTimer *Timer1;
	TStringGrid *sgProfiles;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);

private:	// User declarations

	SampleList samples;
	int profile_column;
	void updateGrid();

public:		// User declarations

	__fastcall TfrmAddSpecimens(TComponent* Owner);
	bool init( TStrings * barcodes );
	unsigned addFromBox( int boxID );
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmAddSpecimens *frmAddSpecimens;
//---------------------------------------------------------------------------
#endif
