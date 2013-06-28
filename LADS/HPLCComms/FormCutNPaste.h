//---------------------------------------------------------------------------

#ifndef FormCutNPasteH
#define FormCutNPasteH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
//---------------------------------------------------------------------------
class TfrmClipboard : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TMemo *Memo1;
    TButton *Button1;
    TButton *btnCopy;
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall btnCopyClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmClipboard(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmClipboard *frmClipboard;
//---------------------------------------------------------------------------
#endif
