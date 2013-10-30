#include <vcl.h>
#pragma hdrstop
#include "tmessage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfm_tmessage *fm_tmessage;
//---------------------------------------------------------------------------
int Tfm_tmessage::int_height_list[12] = { 32,28,26,24,22,20,18,16,14,12,10,8 };
//---------------------------------------------------------------------------
__fastcall Tfm_tmessage::Tfm_tmessage(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
#define	PAN_FORMAT(p)	p->Color = clNavy;				\
			p->BevelOuter = bvNone;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void __fastcall Tfm_tmessage::FormCreate(TObject *Sender)
{
	PAN_FORMAT( panel_top    );
	PAN_FORMAT( panel_bot );
	PAN_FORMAT( panel_left   );
	PAN_FORMAT( panel_right  );
//	Color = TOUCH_MAIN_COLOUR;
	Color = (TColor) 0xB0FFFF;
}
//---------------------------------------------------------------------------
void __fastcall Tfm_tmessage::FormShow(TObject *Sender)
{
	timer_delay->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall Tfm_tmessage::timer_delayTimer(TObject *Sender)
{
	timer_delay->Enabled = false;
}
//---------------------------------------------------------------------------
void Tfm_tmessage::panelInit( TPanel *p, char *txt )
{
	p->Visible  = false;
	p->Caption = txt;
	p->Cursor = crNone;//TOUCH_BASE::cursor;
}
//---------------------------------------------------------------------------
void Tfm_tmessage::msgInit( const std::string txt, const std::string title,
	TColor col )
{
	int	hmargin = 30, wmargin = 32;
	panelInit( panel_ok,  "Continue" );
	panelInit( panel_yes, "Yes" );
	panelInit( panel_no,  "No" );
	if ( (TColor) 0 == col )
		{col = clPurple;
		}
	panel_top->Color   = col;
	panel_bot->Color   = col;
	panel_left->Color  = col;
	panel_right->Color = col;
	bool	show_title = ( title != "" );
	lab_title->Visible = show_title;
	if ( show_title )
		{lab_title->Caption = title.c_str();
		}
	lab_text->Top = hmargin + ( show_title
		? ( lab_title->Top + lab_title->Height ) : 0 );
	String	s = txt.c_str();
	lab_text->Caption = s;
	lab_text->Left = wmargin;
	int	available_width = Width - ( 2 * wmargin )
			- panel_left->Width - panel_right->Width;
	int	available_height = panel_ok->Top - lab_text->Top - hmargin;
	int	i = 0;
	do
		{lab_text->Font->Size = int_height_list[i];
		}
		while ( ++i < 10 && ( lab_text->Width > available_width
			|| lab_text->Height > available_height ) );
	Cursor = crNone;
}
//---------------------------------------------------------------------------
int Tfm_tmessage::msgOk( const std::string txt, const std::string title,
	const TColor col )
{
	msgInit( txt, title, col );
	panel_ok->Visible = true;
	return( ShowModal() );
}
//---------------------------------------------------------------------------
int Tfm_tmessage::msgYesNo( const std::string txt, const std::string title,
	const TColor col )
{
	msgInit( txt, title, col );
	panel_yes->Visible = true;
	panel_no->Visible  = true;
	return( ShowModal() );
}
//---------------------------------------------------------------------------
void __fastcall Tfm_tmessage::panel_okClick(TObject *Sender)
{
	if ( ! timer_delay->Enabled )
		{ModalResult = mrOk;
		}
}
//---------------------------------------------------------------------------
void __fastcall Tfm_tmessage::panel_yesClick(TObject *Sender)
{
	if ( ! timer_delay->Enabled )
		{ModalResult = mrYes;
		}
}
//---------------------------------------------------------------------------
void __fastcall Tfm_tmessage::panel_noClick(TObject *Sender)
{
	if ( ! timer_delay->Enabled )
		{ModalResult = mrNo;
		}
}
//---------------------------------------------------------------------------
void __fastcall Tfm_tmessage::FormKeyPress(TObject *Sender, char &Key)
{
	if ( VK_RETURN == Key && panel_ok->Visible )
		{panel_okClick( NULL );
		}
	else if ( 'Y' == toupper(Key) && panel_yes->Visible )
		{panel_yesClick( NULL );
		}
	else if ( 'N' == toupper(Key) && panel_no->Visible )
		{panel_noClick( NULL );
		}
}
//---------------------------------------------------------------------------

