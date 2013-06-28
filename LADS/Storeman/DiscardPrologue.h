#ifndef DiscardPrologueH
#define DiscardPrologueH
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>

#include <string>

#include "DiscardContext.h"

class TfrmDiscardPrologue : public TForm
{
__published:	// IDE-managed Components
	TButton *btnOK;
	TButton *btnCancel;
	TRadioGroup *rgCryovialStatus;
	TComboBox *cmbProject;
	TLabel *Label1;
	void __fastcall btnOKClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall rgCryovialStatusClick(TObject *Sender);
	void __fastcall cmbProjectChange(TObject *Sender);
private:	// User declarations
    Discard::Context * m_context;
public:		// User declarations
	__fastcall TfrmDiscardPrologue(TComponent* Owner);
	void init( Discard::Context * context );
	void updateUI( );
};

extern PACKAGE TfrmDiscardPrologue *frmDiscardPrologue;

#endif
