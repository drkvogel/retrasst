//---------------------------------------------------------------------------

#ifndef NewBoxTypeH
#define NewBoxTypeH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmNewBoxType : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *Edit1;
	TLabel *Label2;
	TEdit *Edit2;
	TLabel *Label3;
	TEdit *Edit3;
	TLabel *Label4;
	TComboBox *ComboBox1;
	TLabel *Label5;
	TComboBox *ComboBox2;
	TLabel *Label6;
	TComboBox *ComboBox3;
	TLabel *Label7;
	TComboBox *ComboBox4;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
private:	// User declarations
public:		// User declarations
	__fastcall TfrmNewBoxType(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmNewBoxType *frmNewBoxType;
//---------------------------------------------------------------------------
#endif
