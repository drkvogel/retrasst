#ifndef RetrAsstAddNoteH
#define RetrAsstAddNoteH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <sstream>
//#include <string>

class TfrmRetrAsstAddNote : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TMemo *memoNote;
    TBitBtn *btnAddNote;
    TBitBtn *btnCancel;
    void __fastcall btnAddNoteClick(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmRetrAsstAddNote(TComponent* Owner);
    void setText(std::string text);
};

extern PACKAGE TfrmRetrAsstAddNote *frmRetrAsstAddNote;
#endif
