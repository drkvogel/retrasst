//---------------------------------------------------------------------------

#ifndef RatTanksMainDialogH
#define RatTanksMainDialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
//#include "operatorSignedIn.h"
#include "ratTanksCtrl.h"
#include <Vcl.ComCtrls.hpp>
#include "RatTanksBoxType.h"
#include "RatTanksPleaseWait.h"

class operatorSignedIn;

using namespace RationaliseTanks;

class TfrmRatTanksMainDialog : public TForm
{
__published:	// IDE-managed Components
	TPanel *Scan;
	TComboBox *Site_ComboBox;
	TEdit *ThreshHoldEdit;
	TLabel *Label1;
	TTrackBar *TrackBarThreshold;
	TLabel *Label2;
	TButton *DefragButton;
	TMemo *Memo;
	TTimer *Timer1;
	TComboBox *Project_ComboBox;
	TComboBox *Aliquot_ComboBox;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TComboBox *Vessel_ComboBox;
	TButton *ShowDebugButton;
	TPanel *DebugPanel;
	TMemo *debugMemo;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TButton *BoxType_Button;
	TEdit *ChosenBoxType_Edit;
	TTimer *Timer_SlideAnim;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Site_ComboBoxChange(TObject *Sender);
	void __fastcall Vessel_ComboBoxChange(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall TrackBarThresholdChange(TObject *Sender);

	void __fastcall TrackBarTimer(TObject *Sender);
	void __fastcall Project_ComboBoxChange(TObject *Sender);
	void __fastcall Aliquot_ComboBoxChange(TObject *Sender);
//	void __fastcall BoxSelection_ComboBoxChange(TObject *Sender);
	void __fastcall ShowDebugButtonClick(TObject *Sender);
	void __fastcall DefragButtonClick(TObject *Sender);
	void __fastcall BoxType_ButtonClick(TObject *Sender);
	void __fastcall Timer_SlideAnimTimer(TObject *Sender);
private:	// User declarations
	void scan();
	void reSetAll();
	void compileResults();
	void clearComboBox(TComboBox *cleanme);
	void fillSiteComboBox(void);
	void fillAlquotComboBox(void);
	void fillVesselComboBox(void);
	void fillBoxTypeComboBox();
//	void fillBoxDetails(void);
	int getThreshHold(void);
	bool requestUserSignOn();

	std::unique_ptr<TfrmRatTanksBoxType> m_BoxTypeDialog;
	std::unique_ptr<TfrmRatTankPleaseWait> m_PleaseWaitDialog;

	RationaliseTanks::operatorSignedIn m_operatorSignedIn;
	ratTanksCtrl m_rationaliseTanks;
	bool m_bInitliaseTimer;
	bool m_bShowingBoxSelectionDialog;

	int m_AnimDebugHeight_Target_Orginal_Form_Height;
	int m_AnimDebugHeight_Target_Form_Debug;
	int m_AnimDebugHeight_Velocity;
	int m_AnimDebugHeight_Current;

public:		// User declarations
	__fastcall TfrmRatTanksMainDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRatTanksMainDialog *frmRatTanksMainDialog;
//---------------------------------------------------------------------------
#endif
