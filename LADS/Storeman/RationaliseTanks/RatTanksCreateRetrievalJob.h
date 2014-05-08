//---------------------------------------------------------------------------

#ifndef RatTanksCreateRetrievalJobH
#define RatTanksCreateRetrievalJobH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <string>
#include "RatTanksCtrl.h"

using namespace RationaliseTanks;

//---------------------------------------------------------------------------
class TfrmRatTankCreateRetrievalJob : public TForm
{
__published:	// IDE-managed Components
	TEdit *external_nameEdit;
	TMemo *descriptionMemo;
	TButton *ButtonCancel;
	TButton *ButtonOK;
	TLabel *label;
	TLabel *Label1;
	TBalloonHint *BalloonHint1;
	TProgressBar *ProgressBar;
	TComboBox *RetrivelExerciseComboBox;
	TLabel *Label2;
	TButton *New_Retrival_exercise_Button;
	TLabel *Label3;
	TEdit *reasonEdit;
	TLabel *Label4;
	void __fastcall ButtonOKClick(TObject *Sender);
	void __fastcall ButtonCancelClick(TObject *Sender);
	void __fastcall New_Retrival_exercise_ButtonClick(TObject *Sender);
private:	// User declarations
	ratTanksCtrl *m_pRationaliseTanks;
	std::string m_selectedBoxType_cid;
	std::string m_result;
	std::map<std::string, std::map<std::string,std::string> > m_exerciseList;
	void fillExercideList(const std::string &default_cid);
    void clearComboBox(TComboBox *cleanme);
public:		// User declarations
	__fastcall TfrmRatTankCreateRetrievalJob(TComponent* Owner);
	void setUp(ratTanksCtrl *rationaliseTanks,const std::string &boxType_cid);
	std::string getResult() {return m_result;}
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRatTankCreateRetrievalJob *frmRatTankCreateRetrievalJob;
//---------------------------------------------------------------------------
#endif
