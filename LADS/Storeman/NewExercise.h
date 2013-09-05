//---------------------------------------------------------------------------

#ifndef NewExerciseH
#define NewExerciseH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <memory>
#include "Inventory.h"

class LCDbObject;

//---------------------------------------------------------------------------

class TfrmNewExercise : public TForm
{
__published:	// IDE-managed Components

	TButton *BtnOK;
	TEdit *TxtFull;
	TLabel *LblFull;
	TEdit *TxtName;
	TLabel *LblName;

	void __fastcall SaveClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations

public:		// User declarations

	__fastcall TfrmNewExercise(TComponent* Owner);
	const LCDbObject * createRecord();
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmNewExercise *frmNewExercise;
//---------------------------------------------------------------------------
#endif
