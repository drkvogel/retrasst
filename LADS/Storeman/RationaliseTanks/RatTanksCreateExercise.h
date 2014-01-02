//---------------------------------------------------------------------------

#ifndef RatTanksCreateExerciseH
#define RatTanksCreateExerciseH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <map>
#include "ratTanksCtrl.h"
//---------------------------------------------------------------------------

using namespace RationaliseTanks;

class TfrmRatTanksRetrivalExercise : public TForm
{
__published:	// IDE-managed Components
	TEdit *NameEdit;
	TMemo *Description_Memo;
	TLabel *Label1;
	TLabel *Label2;
	TButton *SaveButton;
	TButton *CancelButton;
	TBalloonHint *BalloonHint1;
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall SaveButtonClick(TObject *Sender);
private:	// User declarations
	bool m_didUserQuit;
    ratTanksCtrl *m_pRationaliseTanks;
	std::map<std::string, std::map<std::string,std::string> > *m_pExerciseList;
	bool validate();

	std::string m_external_name;
	std::string m_discription;
	std::string m_object_cid;

public:		// User declarations
	void setUp(ratTanksCtrl *rationaliseTanks,std::map<std::string, std::map<std::string,std::string> > &m_exerciseList);
	__fastcall TfrmRatTanksRetrivalExercise(TComponent* Owner);
	bool didUserQuit() {return m_didUserQuit;}
	std::string getExternalName(){return m_external_name;}
	std::string getDiscription(){return m_discription;}
	std::string getObject_cid() {return m_object_cid;}
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRatTanksRetrivalExercise *frmRatTanksRetrivalExercise;
//---------------------------------------------------------------------------
#endif
