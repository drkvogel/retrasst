//---------------------------------------------------------------------------

#ifndef TfrmCommsConfigH
#define TfrmCommsConfigH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TfrmCommsConfig : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *okButton;
	TBitBtn *btnCancel;
	TComboBox *cbAnalyser;
	TLabel *Label6;
	TRadioGroup *rgWorklist;
	TRadioGroup *rgDatabase;
	TQuery *qCentral;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall okButtonClick(TObject *Sender);
	void __fastcall cbAnalyserDropDown(TObject *Sender);

private:	// User declarations

public:		// User declarations

	__fastcall TfrmCommsConfig(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmCommsConfig *frmCommsConfig;
//---------------------------------------------------------------------------
#endif
