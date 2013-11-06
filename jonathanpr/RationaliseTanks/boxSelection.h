//---------------------------------------------------------------------------

#ifndef boxSelectionH
#define boxSelectionH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include <string>
#include <map>

//---------------------------------------------------------------------------
class TtBoxSelection : public TForm
{
__published:	// IDE-managed Components
	TComboBox *boxTypeComboBox;
	TLabel *Label1;
	TButton *YesButton;
	TButton *CancelButton;
	TButton *NoButton;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TLabel *Label2;
	TLabel *Label3;
	void __fastcall YesButtonClick(TObject *Sender);
	void __fastcall NoButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall boxTypeComboBoxChange(TObject *Sender);
private:	// User declarations
	std::string m_result;   //"YES","NO","CANCEL"
	std::map<std::string,std::map<std::string,std::string> > m_aliquotsBoxTypes;
public:		// User declarations
	__fastcall TtBoxSelection(TComponent* Owner);

	void setUp(std::map<std::string,std::map<std::string,std::string> > &aliquotsBoxTypes, std::string &defaultBox);
	std::string getBoxType();
	std::string getBoxSize();
	std::string getResult() {return m_result;}

	static const std::string DEFINE_RESULT_YES;
	static const std::string DEFINE_RESULT_NO;
	static const std::string DEFINE_RESULT_CANCEL;
	static const std::string DEFINE_RESULT_NONE;
};
//---------------------------------------------------------------------------
extern PACKAGE TtBoxSelection *tBoxSelection;
//---------------------------------------------------------------------------
#endif
