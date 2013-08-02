//---------------------------------------------------------------------------

#ifndef RetrievalAssistantSamplesH
#define RetrievalAssistantSamplesH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TfrmSamples : public TForm
{
__published:	// IDE-managed Components
    TSplitter *Splitter1;
    TGroupBox *groupList;
    TPanel *Panel2;
    TButton *btnAddChunk;
    TButton *btnDelChunk;
    TButton *btnSaveChunk;
    TButton *btnIncr;
    TButton *btnDecr;
    TStringGrid *sgChunks;
    TPanel *Panel1;
    TButton *btnSave;
    TButton *btnCancel;
    TCheckBox *cbLog;
    TRadioGroup *radgrpRows;
    TRadioButton *radbutAll;
    TRadioButton *radbutDefault;
    TRadioButton *radbutCustom;
    TEdit *editCustomRows;
    TGroupBox *GroupBox1;
    TStringGrid *sgChunk;
    TMemo *memoDebug;
    TTimer *timerCustomRows;
private:	// User declarations
public:		// User declarations
    __fastcall TfrmSamples(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSamples *frmSamples;
//---------------------------------------------------------------------------
#endif
