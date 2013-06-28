//---------------------------------------------------------------------------

#ifndef fmReportH
#define fmReportH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <vcl\Clipbrd.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TfrmReport : public TForm
{
__published:	// IDE-managed Components
    TMemo *memoReport;
    TStatusBar *StatusBar1;
    TMainMenu *MainMenu1;
    TMenuItem *Report1;
    TMenuItem *Exit1;
    TMenuItem *SaveAs1;
    TMenuItem *Copytoclipboard1;
    TSaveDialog *SaveDialog1;
    void __fastcall btnCloseClick(TObject *Sender);
    void __fastcall Copytoclipboard1Click(TObject *Sender);
    void __fastcall Exit1Click(TObject *Sender);
    void __fastcall SaveAs1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmReport(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmReport *frmReport;
//---------------------------------------------------------------------------
#endif
