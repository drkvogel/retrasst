#include <vcl.h>
#pragma hdrstop
#include "card.h"
#include "board.h"
#include "rand.h"
//===========================================================================
int 	DECK::next_type_id = 1;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
String	DECK::graphics_location = "D:\\BB\\PAIRS\\";
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
DECK::DECK( TImage *board, const TColor background, const int ht, const int wd,
	String rear_image )
	:
	playing_board( board ),
	background_colour( background ),
	height( ht ),
	width( wd )
{
	rear = new Graphics::TBitmap();
//	rear->LoadFromFile( graphics_location + rear_image + ".bmp" );
	rear->LoadFromResourceName( (int) HInstance, rear_image );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
DECK::~DECK( void )
{
	std::vector<BASE_CARD *>::iterator	vit = pack.begin();
	while ( vit != pack.end() )
		{delete *vit;
		vit++;
		}
	delete rear;
}
//---------------------------------------------------------------------------
				/* RESCALE INTEGER BY REAL FACTOR */
int DECK::rescale( const int arg, const double factor )
{
	double	val = factor * (double) arg;
	return( (int) val );
}
//---------------------------------------------------------------------------
BASE_CARD *DECK::createBASE_CARD( const String face_image )
{
	return( new BASE_CARD( this, face_image ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool DECK::addCard( const String face_image )
{
	pack.push_back( createBASE_CARD( face_image ) );
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int DECK::getNextId( void )
{
	return( next_type_id++ );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void DECK::setInPlay( const bool inp )
{
	std::vector<BASE_CARD *>::iterator	vit = pack.begin();
	while ( vit != pack.end() )
		{(*vit)->in_play = inp;
		vit++;
		}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool DECK::selectNFaces( const int nf )
{
	int	i, j, n, indx;
	BASE_CARD	*base;
	setInPlay( false );
	for ( i = 0; i < nf; i++ )
		{
		n = RANDOM::getRand0n( pack.size() );
		base = NULL;		// SELECT CARD TO USE FOR NEXT PAIR
		for ( j = 0; j < (int) pack.size(); j++ )
			{indx = ( n + j ) % pack.size();
			if ( ! pack[indx]->in_play )
				{base = pack[indx];
				break;
				}
			}
		if ( NULL == base )
			{
			return( false );
			}
		base->in_play = true;
		}
	return( true );
}
//===========================================================================
BASE_CARD::BASE_CARD( DECK *parent, const String face_image )
	:
	in_play( false ),
	blank_card( parent )
{
	type_id = blank_card->getNextId(); 	// UNIQUE PER OBJECT
	face = new Graphics::TBitmap();			// IMAGE SHOWING CARD FACE
//	face->LoadFromFile( DECK::graphics_location + face_image + ".bmp" );
	face->LoadFromResourceName( (int) HInstance, face_image );

//	fm_board->image_play->Canvas->Draw( 0, 0, face );
//	AMSG( "PIC", "Info", MB_OK );

}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
BASE_CARD::~BASE_CARD( void )
{
	delete face;
}
//---------------------------------------------------------------------------
int BASE_CARD::getTypeId( void )
{
	return( type_id );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CARD *BASE_CARD::createCARD( const int tp, const int lf, const double scale )
{
	return( new CARD( this, tp, lf, scale ) );
}
//===========================================================================

CARD::CARD( const BASE_CARD *parent, const int tp, const int lf,
	const double scale )
	:
	base_card( parent ),
	top( tp ),
	left( lf ),
	is_active( true ),
	face_up( false ),
	rescale( false )
{
	width = parent->blank_card->width;
	height = parent->blank_card->height;
	rescale = ( scale < 0.99 );
	if ( rescale )
		{width = (int) ( scale * (double) width  );
		height = (int) ( scale * (double) height );
		}
	rect.left = left;
	rect.top = top;
	rect.right = left + width;
	rect.bottom = top + height;
	card_face = new Graphics::TBitmap();
	card_face->Assign( base_card->face );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CARD::~CARD( void )
{
	delete card_face;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CARD::acceptsClick( const int xpos, const int ypos )
{
	if ( is_active && ypos >= top && ypos <= (top+height)
		&& xpos >= left && xpos <= ( left+width) )
		{return( true );
		}
	return( false );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CARD::turnFaceUp( void )
{
	if ( rescale )
		{base_card->blank_card->playing_board->Canvas->StretchDraw(
			rect, card_face );
		}
	else
		{base_card->blank_card->playing_board->Canvas->Draw(
			left, top, card_face );
		}
	face_up = true;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CARD::turnFaceDown( void )
{
	if ( rescale )
		{base_card->blank_card->playing_board->Canvas->StretchDraw(
			rect, base_card->blank_card->rear );
		}
	else
		{base_card->blank_card->playing_board->Canvas->Draw(
			left, top, base_card->blank_card->rear );
		}
	face_up = false;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CARD::hide( void )
{
	setActive( false );
	base_card->blank_card->playing_board->Canvas->Brush->Color = PLAY_BACKGROUND_DONE;
	base_card->blank_card->playing_board->Canvas->FillRect( rect );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CARD::isActive( void )
{
	return( is_active );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CARD::setActive( const bool act )
{
	is_active = act;
}
//===========================================================================

