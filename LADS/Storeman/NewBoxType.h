//---------------------------------------------------------------------------

#ifndef NewBoxTypeH
#define NewBoxTypeH

//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include "LDbBoxType.h"

//---------------------------------------------------------------------------
class TfrmNewBoxType : public TForm
{
__published:	// IDE-managed Components

	TLabel *Label1;
	TEdit *txtName;
	TLabel *Label2;
	TEdit *txtFull;
	TLabel *Label3;
	TEdit *txtExample;
	TLabel *Label4;
	TComboBox *cbAT1;
	TLabel *Label5;
	TComboBox *cbAT2;
	TLabel *Label6;
	TComboBox *cbAT3;
	TLabel *Label7;
	TComboBox *cbForm;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;

	void __fastcall aliquotDropDown(TObject *Sender);
	void __fastcall cbFormDropDown(TObject *Sender);
	void __fastcall txtNameExit(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);

private:	// User declarations
	String TfrmNewBoxType::checkDetails();
	bool addAliquot( std::vector< int > & aliquotIDs, TComboBox * cbType );

public:		// User declarations
	__fastcall TfrmNewBoxType(TComponent* Owner);
	LPDbBoxType getDetails();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmNewBoxType *frmNewBoxType;
//---------------------------------------------------------------------------
#endif
