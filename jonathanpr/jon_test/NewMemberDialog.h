//---------------------------------------------------------------------------

#ifndef NewMemberDialogH
#define NewMemberDialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TNewMemberDlg : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *nameEdit;
	TButton *SubmitButton;
	TButton *CancelButton;
	void __fastcall SubmitButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
private:	// User declarations
	String m_memberName;
	bool m_userQuit;
public:		// User declarations
	__fastcall TNewMemberDlg(TComponent* Owner);

	bool didUserQuit() {return m_userQuit;}
	String &getName() {return  m_memberName;}
};
//---------------------------------------------------------------------------
extern PACKAGE TNewMemberDlg *NewMemberDlg;
//---------------------------------------------------------------------------
#endif
