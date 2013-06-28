#ifndef DiscardReasonH
#define DiscardReasonH
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include <string>

#include "DiscardContext.h"

class TfrmDiscardReason : public TForm
{
__published:	// IDE-managed Components
    TComboBox *cmbReason;
    TButton *btnCancel;
    TPanel *pnlReason;
    TPanel *pnlNavigate;
    TButton *btnOK;
    TPanel *pnlDesc;
    TEdit *ediDesc;
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall cmbReasonChange(TObject *Sender);
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall ediDescChange(TObject *Sender);
private:	// User declarations
    void updateUI( );
    Discard::Context * m_context;
public:		// User declarations
    __fastcall TfrmDiscardReason(TComponent* Owner);
    void init( Discard::Context * context );
};

extern PACKAGE TfrmDiscardReason *frmDiscardReason;

#endif
