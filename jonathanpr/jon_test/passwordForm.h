//---------------------------------------------------------------------------

#ifndef passwordFormH
#define passwordFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include <string>
//---------------------------------------------------------------------------
class TPassword : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *passwordEdit;
	TButton *Button1;
	TButton *Button2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button1Exit(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
	String m_password;
	bool m_userQuit;
public:		// User declarations
	__fastcall TPassword(TComponent* Owner);

	bool didUserQuit() {return m_userQuit;}
	String &getPassword() {return  m_password;}
};
//---------------------------------------------------------------------------
extern PACKAGE TPassword *Password;
//---------------------------------------------------------------------------
#endif
