//---------------------------------------------------------------------------

#ifndef frmClipboardH
#define frmClipboardH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "gcommsObjects.h"
//---------------------------------------------------------------------------
using namespace genericComms;
class TformClipboard : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TButton *btnCopy;
	TButton *btnClose;
	TMemo *memoClipboard;
	TPanel *Panel2;
	TGroupBox *GroupBox1;
	TComboBox *comboFormat;
	TButton *btnSave;
	TButton *btnGenerate;
    void __fastcall btnCloseClick(TObject *Sender);
    void __fastcall btnCopyClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall comboFormatChange(TObject *Sender);
	void __fastcall btnGenerateClick(TObject *Sender);
private:	// User declarations
	void createFile();
public:		// User declarations
	__fastcall TformClipboard(TComponent* Owner);
	void setSelection(tdvecpWorklistItem * selection);
	tdvecpWorklistItem * worklist;
};
//---------------------------------------------------------------------------
extern PACKAGE TformClipboard *formClipboard;
//---------------------------------------------------------------------------
#endif
