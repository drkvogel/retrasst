#ifndef DiscardMethodH
#define DiscardMethodH
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include <string>

#include "DiscardContext.h"

class TfrmDiscardMethod : public TForm
{
__published:	// IDE-managed Components
    TComboBox *cmbMethod;
    TButton *btnCancel;
    TPanel *pnlMethod;
    TPanel *pnlNavigate;
    TButton *btnOK;
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall cmbMethodChange(TObject *Sender);
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall ediDescChange(TObject *Sender);
private:	// User declarations
    void updateUI( );
    Discard::Context * m_context;
public:		// User declarations
    __fastcall TfrmDiscardMethod(TComponent* Owner);
    void init( Discard::Context * context );
};

extern PACKAGE TfrmDiscardMethod *frmDiscardMethod;

#endif
