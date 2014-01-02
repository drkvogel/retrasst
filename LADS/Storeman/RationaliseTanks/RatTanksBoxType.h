//---------------------------------------------------------------------------

#ifndef RatTanksBoxTypeH
#define RatTanksBoxTypeH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "RatTanksCtrl.h"
#include <string>
#include <map>
//---------------------------------------------------------------------------


using namespace RationaliseTanks;

class TfrmRatTanksBoxType : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label6;
	TEdit *SimpleNameEdit;
	TEdit *DescriptionEdit;
	TEdit *ExampleBoxEdit;
	TComboBox *FirstAliquotComboBox;
	TComboBox *FormationComboBox;
	TComboBox *BoxType_ComboBox;
	TComboBox *ThirdAliquotComboBox;
	TComboBox *SecondAliquotComboBox;
	TButton *OK_Button;
	TButton *Cancel_Button;
	TButton *EditButton;
	TGroupBox *GroupBox1;
	TLabel *Label5;
	TLabel *Label7;
	TLabel *Label8;
	TBalloonHint *BalloonHint;
	void __fastcall Cancel_ButtonClick(TObject *Sender);
	void __fastcall OK_ButtonClick(TObject *Sender);
	void __fastcall BoxType_ComboBoxChange(TObject *Sender);
	void __fastcall SimpleNameEditChange(TObject *Sender);
	void __fastcall EditButtonClick(TObject *Sender);

private:	// User declarations
	void fillFormationComboBox(const std::string &box_size_cid);
	void fillAliquotComboBoxs(const std::string &aliquot_type1,const std::string &aliquot_type2,const std::string &aliquot_type3);
	bool areFormDetailsFilledIn();

	bool m_bDidUserQuit;
	void clearComboBox(TComboBox *cleanme);
	void fillDetails();
	std::string m_ProjectName;
	std::string m_default_box_type_cid;
	std::string m_default_box_size_cid;

	std::string m_chosen_box_type_cid;
	std::string m_chosen_box_size_cid;
	std::string m_chosen_box_name;
	std::string m_aliquot_cid;
	std::string m_aliquot_name;
	std::string m_projectcid;
	ratTanksCtrl *m_rationaliseTanks;
	std::map<std::string, std::map<std::string,std::string> > m_aliquotsInProject;
	std::map<std::string, std::map<std::string,std::string> > m_boxSizes;
public:		// User declarations
	__fastcall TfrmRatTanksBoxType(TComponent* Owner);
	void setUp(ratTanksCtrl &rationaliseTanks,const std::string &project_cid,const  std::string &aliquot_cid,const std::string &ProjectName, const std::string &aliquot_name);
	void clearDetails()
	{
		m_chosen_box_type_cid = "";
		m_chosen_box_size_cid= "";
		m_chosen_box_name = "";
	}
    bool didUserQuit() {return m_bDidUserQuit;}
	std::string getChosenBoxType() {return m_chosen_box_type_cid;}
	std::string getChosenBoxSize() {return m_chosen_box_size_cid;}
	std::string getChosenBoxName() {return m_chosen_box_name;}
};


//---------------------------------------------------------------------------
extern PACKAGE TfrmRatTanksBoxType *frmRatTanksBoxType;
//---------------------------------------------------------------------------
#endif
