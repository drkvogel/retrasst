//---------------------------------------------------------------------------

#ifndef fmResolveH
#define fmResolveH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>

#include "esg.h"

//---------------------------------------------------------------------------
class TfrmResolve : public TForm
{
__published:	// IDE-managed Components
    TButton *btnExit;
    TStringGrid *sgErrors;
    TPanel *Panel1;
    TPanel *Panel2;
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    ESG* esgErrors;
public:		// User declarations
    __fastcall TfrmResolve(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmResolve *frmResolve;
//---------------------------------------------------------------------------
#endif
