#include <vcl.h>
#pragma hdrstop
#include "board.h"
#include "card.h"
#include "result.h"
#include "rand.h"
#include "xtime.h"
#include "utils.h"
#include "mess.h"
#include "tmessage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfm_board *fm_board;
//---------------------------------------------------------------------------
#define	POPUP_COLOUR			(clMaroon)
//---------------------------------------------------------------------------
__fastcall Tfm_board::Tfm_board(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

static void showMatchFunction( bool is_match )
{
	fm_result->display( is_match );
}
//---------------------------------------------------------------------------
void __fastcall Tfm_board::FormCreate(TObject *Sender)
{
	int	i;
	abandon = false;
	is_valid = true;
	String	image;
	try
		{
		game = new PAIR_GAME( image_play, PLAY_BACKGROUND,
			150, 120, "rear", showMatchFunction );
		for ( i = 0; i < 25; i++ )
			{
			image = String("f") + String(i);
			game->addCard( image );
			}
		}
	catch ( Exception &e )
		{MESS::error( "Failed to initialise cards", MB_OK );
		is_valid = false;
		}
	Top = 0;
	Left = 0;
	Height = Screen->Height;
	Width = Screen->Width;
	margin = 10;
	in_game = false;
	time_t	now;
	time( &now );
	RANDOM::randSetSeed( (long) now );
	init = new XTIME();
}
//---------------------------------------------------------------------------
void __fastcall Tfm_board::FormDestroy(TObject *Sender)
{
	delete game;
	delete init;
}
//---------------------------------------------------------------------------
void Tfm_board::resetBoard( void )
{
	int	h = 57;				// HEIGHT OF CONTROL PANELS
	int	board_width = Width - ( 2 * margin );
							// MAIN PANEL
	panel_play->Left = margin;
	panel_play->Top  = margin;
	panel_play->Width  = board_width;
	panel_play->Height = Height - ( 3 * margin ) - h;
	panel_play->Color = PLAY_BACKGROUND;
							// IMAGE PLAYING AREA
	image_play->Top = 4;
	image_play->Left = 4;
	image_play->Height = panel_play->Height - 12;
	image_play->Width = panel_play->Width - 12;
	fm_board->Color = clBlack;
	image_play->Canvas->Brush->Color = PLAY_BACKGROUND;
	image_play->Canvas->FillRect( image_play->ClientRect );
							// ABANDON BUTTON
	panel_abandon->Visible = false;
	panel_abandon->Color = this->Color;
	panel_abandon->Height = 41;
	panel_abandon->Top = Height - panel_abandon->Height - margin;
	panel_abandon->Left = margin;



}
//---------------------------------------------------------------------------
bool Tfm_board::playGame( TForm *caller, LAYOUT *ly )
{
	current_layout = ly;
	current_layout->match = 0;
	current_layout->miss  = 0;
	current_layout->duration = 0;
	resetBoard();
	int	npairs = ( current_layout->rows * current_layout->cols ) / 2;
	if ( npairs > (int) game->pack.size() )
		{MESS::bug( "Insufficient cards in playGame pack for next level");
		abandon = true;
		return( false );
		}
	if ( ! game->initPairs( npairs, current_layout ) )
		{MESS::bug( "Error in playGame random allocation" );
		return( false );
		}
	Show();
	caller->Hide();
	game->run( timer_preview, ( npairs < 5 ) ? 3000 : 5000 );
	while ( game->isRunning() )	// WAIT FOR END SIGNAL
		{Application->ProcessMessages();
		}
	current_layout->duration = game->getRunTime();
	caller->Show();
 	Hide();
	return( true );
}
//---------------------------------------------------------------------------
void __fastcall Tfm_board::timer_previewTimer(TObject *Sender)
{
	game->runStart();
	panel_abandon->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall Tfm_board::image_playMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	game->handleClick( X, Y );
}
//---------------------------------------------------------------------------
void Tfm_board::abandonGame( void )
{
	if ( mrYes != fm_tmessage->msgYesNo(
		"You have not yet matched all the pairs."
		"\n\nAre you certain you wish to abandon this game?"
		"\n\nYou will not be able to attempt it again.",
			"Please check!", POPUP_COLOUR ) )
		{return;
		}
	game->abandon();
}
//---------------------------------------------------------------------------
void __fastcall Tfm_board::panel_abandonClick(TObject *Sender)
{
	abandonGame();
}
//---------------------------------------------------------------------------

