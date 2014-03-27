//---------------------------------------------------------------------------

#ifndef tFormNewOrOpenH
#define tFormNewOrOpenH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TNewOrOpenForm : public TForm
{
__published:	// IDE-managed Components
	TComboBox *Projects_ComboBox;
	TButton *OpenProject_Button;
	TButton *NewProject_Button;
	TLabel *projectTitle_Label;
private:	// User declarations
public:		// User declarations
	__fastcall TNewOrOpenForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TNewOrOpenForm *NewOrOpenForm;
//---------------------------------------------------------------------------
#endif
