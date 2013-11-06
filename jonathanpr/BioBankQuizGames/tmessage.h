#ifndef tmessageH
#define tmessageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <string>
//---------------------------------------------------------------------------
class Tfm_tmessage : public TForm
{
__published:	// IDE-managed Components
	TPanel *panel_top;
	TPanel *panel_bot;
	TPanel *panel_right;
	TPanel *panel_left;
	TLabel *lab_title;
	TLabel *lab_text;
	TPanel *panel_yes;
	TPanel *panel_ok;
	TPanel *panel_no;
	TTimer *timer_delay;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall panel_okClick(TObject *Sender);
	void __fastcall panel_yesClick(TObject *Sender);
	void __fastcall panel_noClick(TObject *Sender);
	void __fastcall timer_delayTimer(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
private:	// User declarations
	void 	panelInit( TPanel *p, char *txt );
	void 	msgInit( const std::string txt, const std::string title,
			TColor col );
	static	int 	int_height_list[];
public:		// User declarations
	__fastcall Tfm_tmessage(TComponent* Owner);
	int 	msgOk( const std::string txt, const std::string title,
			const TColor col );
	int 	msgYesNo( const std::string txt, const std::string title,
			const TColor col );
};
//---------------------------------------------------------------------------
extern PACKAGE Tfm_tmessage *fm_tmessage;
//---------------------------------------------------------------------------
#endif

