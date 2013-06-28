//---------------------------------------------------------------------------

#ifndef FormFrontH
#define FormFrontH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
//---------------------------------------------------------------------------
class TfrmFront : public TForm
{
__published:	// IDE-managed Components
    TButton *btnGetWorklist;
    TPanel *Panel1;
    TListBox *ListBox1;
    TButton *btnCreateSampleSelection;
    TBitBtn *BitBtnExit;
    TButton *btnEnterResults;
    TStatusBar *StatusBar1;
    TButton *btnCopyResults;
    void __fastcall btnGetWorklistClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btnCreateSampleSelectionClick(TObject *Sender);
    void __fastcall BitBtnExitClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall btnCopyResultsClick(TObject *Sender);
    void __fastcall btnEnterResultsClick(TObject *Sender);
private:
    void fnLoadWorklist(void);
    void fnDisplayWorklist(void);
    void fnDeleteWorklistTotal(void);
    void fnLoadWorklistSelection(void);
    void fnDeleteWorklistSelection(void);
    void fnDeleteWorklists(void);
public:		// User declarations
    __fastcall TfrmFront(TComponent* Owner);
    void fnDeleteEmpowerOutput(void);
    bool bGotWorklist;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmFront *frmFront;
//---------------------------------------------------------------------------
#endif
