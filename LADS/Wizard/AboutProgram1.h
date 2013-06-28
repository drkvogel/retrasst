//---------------------------------------------------------------------------
#ifndef AboutProgram1H
#define AboutProgram1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TAboutBox : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TButton *btnOK;
    TBevel *Bevel1;
    TLabel *lblVersion;
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TAboutBox(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutBox *AboutBox;
//---------------------------------------------------------------------------
#endif
