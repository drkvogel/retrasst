#ifndef boardH
#define boardH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//#include <syncobjs.hpp>
//---------------------------------------------------------------------------
#include "pairgame.h"
#include "xtime.h"
//---------------------------------------------------------------------------
#define	PLAY_BACKGROUND	((TColor) 0x005000)
#define	PLAY_BACKGROUND_DONE	((TColor) 0x004000)
//#define	AMSG	Application->MessageBox
//---------------------------------------------------------------------------
class Tfm_board : public TForm
{
__published:	// IDE-managed Components
	TPanel *panel_play;
	TImage *image_play;
	TTimer *timer_preview;
	TPanel *panel_abandon;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall timer_previewTimer(TObject *Sender);
	void __fastcall image_playMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall panel_abandonClick(TObject *Sender);
private:	// User declarations
	bool	in_game;
	int	score;
	int	level;
	int	margin;
	PAIR_GAME	*game;
	LAYOUT	*current_layout;
	void 	resetBoard( void );
	void 	abandonGame( void );
public:		// User declarations
	bool	is_valid;
	__fastcall Tfm_board(TComponent* Owner);
	bool	abandon, bailout;
	XTIME	*init;
//	String	identity;
	bool 	playGame( TForm *caller, LAYOUT *ly  );
};
//---------------------------------------------------------------------------
extern PACKAGE Tfm_board *fm_board;
//---------------------------------------------------------------------------
#endif

