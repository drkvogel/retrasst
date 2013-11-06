//---------------------------------------------------------------------------

#ifndef NewEventDialogH
#define NewEventDialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TeventDialog : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *nameEdit;
	TButton *SubmitButton;
	TButton *CancelButton;
	void __fastcall SubmitButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
private:	// User declarations
	bool m_userQuit;
	String m_eventName;

public:		// User declarations
	__fastcall TeventDialog(TComponent* Owner);

	bool didUserQuit() {return m_userQuit;}
	String &getEvenName() {return  m_eventName;}
};
//---------------------------------------------------------------------------
extern PACKAGE TeventDialog *eventDialog;
//---------------------------------------------------------------------------
#endif
