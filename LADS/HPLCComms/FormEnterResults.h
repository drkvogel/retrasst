//---------------------------------------------------------------------------

#ifndef FormEnterResultsH
#define FormEnterResultsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>

#define CBANALYSER_DEFAULT "select"
//---------------------------------------------------------------------------
class TfrmEnterResults : public TForm
{
__published:	// IDE-managed Components
    TOpenDialog *OpenDialog4;
    TEdit *edtFilename;
    TButton *btnBrowse;
    TButton *btnOK;
    TButton *Button2;
    TLabel *Label1;
    TComboBox *cbAnalysers;
    TLabel *Label2;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall btnBrowseClick(TObject *Sender);
    void __fastcall btnOKClick(TObject *Sender);
private:	// User declarations
    int fnProcessResults(TStringList* tsl);
    bool fnParseLine(String strLine);
    String fnTrimQuotes(String str);
    void fnGetResults(void);
public:		// User declarations
    __fastcall TfrmEnterResults(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEnterResults *frmEnterResults;
//---------------------------------------------------------------------------
#endif
