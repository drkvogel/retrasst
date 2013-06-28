//---------------------------------------------------------------------------
#ifndef TfrmAddNoteH
#define TfrmAddNoteH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>

//---------------------------------------------------------------------------
class TfrmEditNote : public TForm
{
__published:	// IDE-managed Components
	TListBox *lstExpected;
	TEdit *txtNote;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	void __fastcall lstExpectedClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);

private:	// User declarations

public:		// User declarations

   __fastcall TfrmEditNote(TComponent* Owner);
   void initialise( const char * options[] );

   String getNote() const { return txtNote -> Text; }
   void setNote( const String & note ) { txtNote -> Text = note; }
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEditNote *frmEditNote;
//---------------------------------------------------------------------------
#endif
