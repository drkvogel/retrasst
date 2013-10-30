//---------------------------------------------------------------------------

#ifndef tdbSelectionH
#define tdbSelectionH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TDbSelectionDialog : public TForm
{
__published:	// IDE-managed Components
	TComboBox *dbCombobox;
	TButton *Button1;
	TButton *Button2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	String m_dbSelection;
	bool m_didUserQuit;

public:		// User declarations
	String getDBSelection() {return m_dbSelection;}
	bool didUserQuit() {return m_didUserQuit;}

	__fastcall TDbSelectionDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDbSelectionDialog *DbSelectionDialog;
//---------------------------------------------------------------------------
#endif
