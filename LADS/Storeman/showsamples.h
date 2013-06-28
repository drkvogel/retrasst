//---------------------------------------------------------------------------

#ifndef ShowSamplesH
#define ShowSamplesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include "inventory.h"
//---------------------------------------------------------------------------
class TfrmAliquotTypes : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *grdSamples;
	TLabel *LblCaption;
	TButton *BtnOK;
	TButton *BtnCancel;
	void __fastcall OKClicked(TObject *Sender);
private:	// User declarations
public:		// User declarations


	__fastcall TfrmAliquotTypes(TComponent* Owner);
	void init();
	void clearGrid();
	int selrow;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAliquotTypes *frmAliquotTypes;
//---------------------------------------------------------------------------
#endif
