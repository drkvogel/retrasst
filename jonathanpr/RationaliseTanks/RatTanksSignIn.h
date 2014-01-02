//---------------------------------------------------------------------------

#ifndef RatTanksSignInH
#define RatTanksSignInH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include <string>
//---------------------------------------------------------------------------
class TfrmRatTankSignIn : public TForm
{
__published:	// IDE-managed Components
	TEdit *passwordTEdit;
	TButton *CancelButton;
	TButton *SignInButton;
	TEdit *userNameTEdit;
	TBalloonHint *BalloonHint1;
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall SignInButtonClick(TObject *Sender);
private:	// User declarations
	bool m_didUserQuit;
	bool isValidData();
	std::string m_UserName;
	std::string m_Password;
public:		// User declarations
	__fastcall TfrmRatTankSignIn(TComponent* Owner);

	std::string getUserName();
	std::string getPassword();

	bool didUserQuit() {return m_didUserQuit;}
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRatTankSignIn *frmRatTankSignIn;
//---------------------------------------------------------------------------
#endif
