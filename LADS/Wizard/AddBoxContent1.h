//---------------------------------------------------------------------------

#ifndef AddBoxContent1H
#define AddBoxContent1H

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "LDbBoxType.h"
#include <Db.hpp>
#include <DBTables.hpp>

//---------------------------------------------------------------------------

class TAddBoxContent : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *edtBoxName;
	TLabel *Label2;
	TPanel *Panel1;
	TButton *Save;
	TButton *btnClose;
	TComboBox *cbxAliquot1;
	TComboBox *cbxAliquot2;
	TComboBox *cbxAliquot3;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TComboBox *cbxBoxSize;
	TLabel *Label8;
	TEdit *edtBoxLabel;
	TEdit *edtBoxSet;
	TEdit *edtBoxOrder;
	TLabel *Label9;
	TLabel *Label10;
	TCheckBox *cbxStatus;
	TQuery *QueryProject;
	TLabel *Label6;
	TEdit *edtDescription;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);
	void __fastcall edtBoxNameExit(TObject *Sender);

private:	// User declarations

	int boxTypeID;

	void FillBoxSizeComboBox();
	void FillAliquotComboBoxes();
	void SetAliquotItemIndex( TComboBox* cbx, int aliquotTypeID );

public:		// User declarations

	__fastcall TAddBoxContent(TComponent* Owner);
	void setBoxContent( int btid ) { boxTypeID = btid; }
};
//---------------------------------------------------------------------------
extern PACKAGE TAddBoxContent *AddBoxContent;
//---------------------------------------------------------------------------
#endif

