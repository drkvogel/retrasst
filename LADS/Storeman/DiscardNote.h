#ifndef DiscardNoteH
#define DiscardNoteH
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include <string>

#include "DiscardContext.h"

class TfrmDiscardNote : public TForm
{
__published:	// IDE-managed Components
	TMemo *memNote;
	TButton *btnOK;
	TButton *btnCancel;
    TComboBox *cmbCanned;
    TButton *btnPaste;
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall btnOKClick(TObject *Sender);
	void __fastcall rgNoteClick(TObject *Sender);
	void __fastcall memNoteChange(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
    void __fastcall btnPasteClick(TObject *Sender);
    void __fastcall cmbCannedChange(TObject *Sender);
private:	// User declarations
	void updateUI( );
    Discard::Context * m_context;
public:		// User declarations
	__fastcall TfrmDiscardNote(TComponent* Owner);
    void init( Discard::Context * context );
};

extern PACKAGE TfrmDiscardNote *frmDiscardNote;

#endif
