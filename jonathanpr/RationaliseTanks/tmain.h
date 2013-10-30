//---------------------------------------------------------------------------

#ifndef tmainH
#define tmainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
//#include "operatorSignedIn.h"
#include "rationaliseTanks.h"
#include <Vcl.ComCtrls.hpp>

class operatorSignedIn;

using namespace RationaliseTanks;

class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPanel *Scan;
	TComboBox *Site_ComboBox;
	TEdit *ThreshHoldEdit;
	TLabel *Label1;
	TTrackBar *TrackBarThreshold;
	TLabel *Label2;
	TComboBox *BoxSelection_ComboBox;
	TEdit *BoxInfoEdit;
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
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Site_ComboBoxChange(TObject *Sender);
	void __fastcall Vessel_ComboBoxChange(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall TrackBarThresholdChange(TObject *Sender);

	void __fastcall TrackBarTimer(TObject *Sender);
	void __fastcall Project_ComboBoxChange(TObject *Sender);
	void __fastcall Aliquot_ComboBoxChange(TObject *Sender);
	void __fastcall BoxSelection_ComboBoxChange(TObject *Sender);
	void __fastcall ShowDebugButtonClick(TObject *Sender);
	void __fastcall DefragButtonClick(TObject *Sender);
private:	// User declarations
	void scan();
	void clearComboBox(TComboBox *cleanme);
	void fillSiteComboBox(void);
	void fillAlquotComboBox(void);
	void fillVesselComboBox(void);
	void fillBoxTypeComboBox(const std::string &default_boxType_cid);
	void fillBoxDetails(void);
	int getThreshHold(void);
	bool requestUserSignOn();

	RationaliseTanks::operatorSignedIn m_operatorSignedIn;
	rationaliseTanks m_rationaliseTanks;
	bool m_bInitliaseTimer;
	bool m_bShowingBoxSelectionDialog;
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
