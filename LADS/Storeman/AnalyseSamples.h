//---------------------------------------------------------------------------

#ifndef AnalyseSamplesH
#define AnalyseSamplesH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>

//---------------------------------------------------------------------------

class TfrmAnalyseSpecimens : public TForm
{
__published:	// IDE-managed Components

	TRadioGroup *rgSource;
	TComboBox *cbProject;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TOpenDialog *openDialogue;
	TLabel *Label1;

	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall cbProjectChange(TObject *Sender);

private:	// User declarations

	bool addFromBoxes();
	bool addFromJob();
	bool addFromFile();

public:		// User declarations

	__fastcall TfrmAnalyseSpecimens(TComponent* Owner);
	void init();
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmAnalyseSpecimens *frmAnalyseSpecimens;
//---------------------------------------------------------------------------
#endif
