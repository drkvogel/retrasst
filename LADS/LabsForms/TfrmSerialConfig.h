//---------------------------------------------------------------------------

#ifndef TfrmSerialConfigH
#define TfrmSerialConfigH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TfrmCommsConfig.h"
#include <Buttons.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmSerialConfig : public TfrmCommsConfig
{
__published:	// IDE-managed Components
	TPanel *serialParams;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TEdit *edPort;
	TEdit *edBaud;
	TEdit *edParity;
	TEdit *edStop;
	TEdit *edData;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall edPortExit(TObject *Sender);
	void __fastcall edDataExit(TObject *Sender);
	void __fastcall edParityExit(TObject *Sender);
	void __fastcall edStopExit(TObject *Sender);
	void __fastcall okButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmSerialConfig(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSerialConfig *frmSerialConfig;
//---------------------------------------------------------------------------
#endif
