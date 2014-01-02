//---------------------------------------------------------------------------

#ifndef RatTanksdbSelectionH
#define RatTanksdbSelectionH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TfrmRatTankDbSelectionDialog : public TForm
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

	__fastcall TfrmRatTankDbSelectionDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRatTankDbSelectionDialog *frmRatTankDbSelectionDialog;
//---------------------------------------------------------------------------
#endif
