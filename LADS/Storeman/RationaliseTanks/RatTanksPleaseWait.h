//---------------------------------------------------------------------------

#ifndef RatTanksPleaseWaitH
#define RatTanksPleaseWaitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmRatTankPleaseWait : public TForm
{
__published:	// IDE-managed Components
	TProgressBar *ProgressBar1;
	TLabel *Label1;
	TTimer *Timer1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations

public:		// User declarations
	__fastcall TfrmRatTankPleaseWait(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRatTankPleaseWait *frmRatTankPleaseWait;
//---------------------------------------------------------------------------
#endif
