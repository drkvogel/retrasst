//---------------------------------------------------------------------------
#ifndef Help1H
#define Help1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class THelp : public TForm
{
__published:	// IDE-managed Components
    TMemo *Memo1;
    TPanel *Panel1;
    TButton *btnClose;
    void __fastcall btnCloseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall THelp(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE THelp *Help;
//---------------------------------------------------------------------------
#endif
