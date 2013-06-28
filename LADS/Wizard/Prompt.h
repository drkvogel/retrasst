//---------------------------------------------------------------------------

#ifndef PromptH
#define PromptH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmPrompt : public TForm
{
__published:	// IDE-managed Components
    TLabel *lblPrompt;
    TEdit *edtValue;
    TPanel *Panel1;
    TButton *btnClose;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall btnCloseClick(TObject *Sender);
private:	// User declarations
    AnsiString prompt;
    AnsiString value;
public:		// User declarations
    __fastcall TfrmPrompt(TComponent* Owner);
    void setPrompt( AnsiString pr ) { prompt = pr; }
    void setValue( AnsiString val ) { value = val; }
    AnsiString getValue() { return value; }
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPrompt *frmPrompt;
//---------------------------------------------------------------------------
#endif
