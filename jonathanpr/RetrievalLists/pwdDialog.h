//---------------------------------------------------------------------------

#ifndef pwdDialogH
#define pwdDialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TPasswordDialog : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *passwordEdit;
	TButton *SubmitButton;
	TButton *CancelButton;
	void __fastcall SubmitButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
private:	// User declarations
	String m_password;
	bool m_userQuit;
public:		// User declarations
	__fastcall TPasswordDialog(TComponent* Owner);

	bool didUserQuit() {return m_userQuit;}
	String &getPassword() {return  m_password;}
};
//---------------------------------------------------------------------------
extern PACKAGE TPasswordDialog *PasswordDialog;
//---------------------------------------------------------------------------
#endif
