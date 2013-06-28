//---------------------------------------------------------------------------

#ifndef FindSampleH
#define FindSampleH

//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "Rosetta.h"

class Box;

//---------------------------------------------------------------------------

class TfrmFind : public TForm
{
__published:	// IDE-managed Components
	TRadioGroup *rgTypes;
	TLabel *Label1;
	TLabel *Label2;
	TComboBox *cbType;
	TEdit *txtName;
	TComboBox *cbProject;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TLabel *Label3;
	void __fastcall cbTypeDropDown(TObject *Sender);
	void __fastcall cbProjectChange(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall cbTypeChange(TObject *Sender);
	void __fastcall rgTypesClick(TObject *Sender);

private:	// User declarations

	ROSETTA boxDetails;
	bool findBox();
	bool findCryovial();

public:		// User declarations
	__fastcall TfrmFind(TComponent* Owner);
	Box * getDetails() const;
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmFind *frmFind;
//---------------------------------------------------------------------------
#endif
