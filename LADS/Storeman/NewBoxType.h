//---------------------------------------------------------------------------

#ifndef NewBoxTypeH
#define NewBoxTypeH

//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <set>
#include <vector>

class LCDbBoxSize;
class LPDbBoxType;

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
	TComboBox *cbForms;
	TLabel *Label8;
	TComboBox *cbTypes;
	TBitBtn *btnOK;
	TBitBtn *btnClear;

	void __fastcall aliquotDropDown(TObject *Sender);
	void __fastcall cbFormsDropDown(TObject *Sender);
	void __fastcall btnClearClick(TObject *Sender);
	void __fastcall cbFormsChange(TObject *Sender);
	void __fastcall cbTypesDropDown(TObject *Sender);
	void __fastcall cbTypesChange(TObject *Sender);
	void __fastcall txtNameExit(TObject *Sender);
	void __fastcall cbATChange(TObject *Sender);

private:	// User declarations

	std::set< int > expectedAliquots;
	int expectedTubeType;
	const LCDbBoxSize * currentFormation;
	const LPDbBoxType * currentBoxType;

	void clearAll();
	void clearDetails();
	void showDetails();
	void showExampleBox();
	bool addAliquot( std::vector< int > & aliquotIDs, TComboBox * cbType );
	bool checkDetails();
	bool matchesExpected( const std::vector< int > & aliquotIDs );

public:		// User declarations
	__fastcall TfrmNewBoxType(TComponent* Owner);
	void init( std::set< int > & aliquotIDs, int tubeTypeID );
	LPDbBoxType getDetails();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmNewBoxType *frmNewBoxType;
//---------------------------------------------------------------------------
#endif
