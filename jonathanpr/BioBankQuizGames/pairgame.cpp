#include <vcl.h>
#include "pairgame.h"
#include "rand.h"
//===========================================================================
PAIR_GAME::PAIR_GAME( TImage *board, const TColor background, const int ht,
	const int wd, const String rear_image,
	void (* showMatchFn)(bool match) )
	:
	DECK( board, background, ht, wd, rear_image ),
	left_start( 15 ),
	left_gap( 30 ),
	top_gap( 30 ),
	top_start( 10 ),
	matchFn( showMatchFn )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
PAIR_GAME::~PAIR_GAME( VOID )
{
}
//===========================================================================
void PAIR_GAME::wipeCards( void )
{
	std::vector<CARD *>::iterator	vit = cards.begin();
	while ( vit != cards.end() )
		{delete *vit;
		vit++;
		}
	cards.clear();
}
//---------------------------------------------------------------------------
bool PAIR_GAME::initPairs( const int np, LAYOUT *lay )
{
	int	i, j, k, n, cid, pos;
	int	w = width;
	int	h = height;
	int	used_width  = ( lay->cols * w )  + ( (lay->cols + 1) * left_gap );
	int	used_height = ( lay->rows * h ) + ( (lay->rows + 1) * top_gap  );
	double	scale = 1.0;
	if ( used_width > playing_board->Width )
		{double	sw = (double) playing_board->Width / (double) used_width;
		if ( sw < scale )
			{scale = sw;
			}
		}
	if ( used_height > playing_board->Height )
		{double	sh = (double) playing_board->Height / (double) used_height;
		if ( sh < scale )
			{scale = sh;
			}
		}
	if ( scale < 0.1 )
		{scale = 0.1;
		}
	if ( scale < 0.99 )
		{
		w = rescale( w, scale );
		h = rescale( h, scale );
		left_gap = rescale( left_gap, scale );
		top_gap  = rescale( top_gap,  scale );
		}
	current_layout = lay;
	int	main_width = ( lay->cols * w )  + ( (lay->cols - 1) * left_gap );
	int	main_height = ( lay->rows * h ) + ( (lay->rows - 1) * top_gap  );
	left_start = ( playing_board->Width  - main_width  ) / 2;
	top_start  = ( playing_board->Height - main_height ) / 2;
	int	left_step = w + left_gap;
	int	top_step  = h + top_gap;
	int	top, left;
	wipeCards();
	if ( ! selectNFaces( np ) )
		{return( false );
		}
	int	nt2 = np * 2;
	for ( i = 0; i < nt2; i++ )
		{cards.push_back( NULL );
		}
	std::vector<BASE_CARD *>::iterator ci = pack.begin();
	while ( ci != pack.end() )
		{
		if ( ! (*ci)->in_play )
			{ci++;
			continue;
			}
		cid = getNextId();
		for ( k = 0; k < 2; k++ )	// FIND PLACE FOR 2 CARDS
			{
			n = RANDOM::getRand0n( nt2 );
			for ( j = 0; j < nt2; j++ )
				{pos = ( n + j ) % nt2;
				if ( NULL == cards[pos] )
					{
					top = top_start
					 + ( top_step * ( pos / lay->cols ) );
					left = left_start
					 + ( left_step * ( pos % lay->cols ) );
					cards[pos] = (*ci)->createCARD(
						top, left, scale );
					cards[pos]->card_id = cid;
					break;
					}
				}
			}
		ci++;
		}
	return( true );
}
//---------------------------------------------------------------------------
void PAIR_GAME::showCardsUp( void )
{
	std::vector<CARD *>::iterator vit = cards.begin();
	while ( vit != cards.end() )
		{
		(*vit)->turnFaceUp();
		vit++;
		}
}
//---------------------------------------------------------------------------
void PAIR_GAME::showCardsDown( void )
{
	std::vector<CARD *>::iterator vit = cards.begin();
	while ( vit != cards.end() )
		{
		(*vit)->turnFaceDown();
		vit++;
		}
}
//---------------------------------------------------------------------------
bool PAIR_GAME::handleClick( const int x, const int y )
{
	if ( ignore_click )	// PREVENT OVER-LAPPING CALLS
		{return( false );
		}
	ignore_click = true;
	CARD	*selected = NULL;
	std::vector<CARD *>::iterator vit = cards.begin();
	while ( vit != cards.end() )
		{
		if ( (*vit)->acceptsClick( x, y ) )
			{selected = *vit;
			break;
			}
		vit++;
		}
	if ( NULL != selected && ! selected->face_up )
		{selected->turnFaceUp();
		if ( NULL == selected_first )	// FIRST HALF OF PAIR
			{selected_first = selected;
			}
		else
			{if ( selected->card_id != selected_first->card_id )
				{matchFn( false );
				selected->turnFaceDown();
				selected_first->turnFaceDown();
				current_layout->miss++;
				}
			else
				{matchFn( true );
				selected->hide();
				selected_first->hide();
				current_layout->match++;
				}
			selected_first = NULL;
			}
		}
	bool	completed = true;
	vit = cards.begin();
	while ( vit != cards.end() )
		{if ( (*vit)->isActive() )
			{completed = false;
			break;
			}
		vit++;
		}
	if ( completed )				// SIGNAL END OF GAME
		{end_level = clock();
		is_running = false;
		return( true );
		}
	ignore_click = false;
	return( false );
}
//---------------------------------------------------------------------------
bool PAIR_GAME::isRunning( void )
{
	return( is_running );
}
//---------------------------------------------------------------------------
void PAIR_GAME::abandon( void )
{
	is_running = false;
	abandoned = true;
}
//---------------------------------------------------------------------------
bool PAIR_GAME::wasAbandoned( void ) const
{
	return( abandoned );
}
//---------------------------------------------------------------------------
void PAIR_GAME::runStart( void )
{
	is_running = true;
}
//---------------------------------------------------------------------------
bool PAIR_GAME::run( TTimer *t, const int interval )
{
	ignore_click = true;
	is_running = false;
	abandoned = false;
	selected_first = NULL;	// FIRST HALF OF PAIR
	if ( NULL != t )
		{t->Enabled = false;
		showCardsUp();
		t->Interval = interval;
		t->Enabled = true;	// MUST CALL RUNSTART IN TIMER EVENT
		while ( ! is_running )
			{Application->ProcessMessages();
			}
		t->Enabled = false;
		}
	else
		{runStart();
		}
	showCardsDown();
	ignore_click = false;
	start_level = clock();
	end_level = start_level;
	Application->ProcessMessages();
	return( true );
}
//---------------------------------------------------------------------------
				/* RETURN RUN-TIME IN DECI-SECONDS */
int PAIR_GAME::getRunTime( void )
{
	return( (int)( ( 10 * ( end_level - start_level ) ) / CLK_TCK ) );
}
//===========================================================================
#pragma package(smart_init)

