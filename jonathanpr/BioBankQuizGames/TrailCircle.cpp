#include <vcl.h>
#pragma hdrstop
#include "TrailCircle.h"
#include "tTrailBoard.h"
#include "rand.h"


//===========================================================================
BASE_TrailCircle::BASE_TrailCircle( TheTrail *parent, const String image )
	: m_in_play( false ), CIRCLE_SIZE(30),m_parent( parent )
{
 	m_type_id = m_parent->getNextId(); 	// UNIQUE PER OBJECT
 //	face = new Graphics::TBitmap();			// IMAGE SHOWING CARD FACE
 //	face->LoadFromResourceName( (int) HInstance, face_image );

//	fm_board->image_play->Canvas->Draw( 0, 0, face );
//	AMSG( "PIC", "Info", MB_OK );

	m_image = image;

}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
BASE_TrailCircle::~BASE_TrailCircle( void )
{
//	delete face;
}
//---------------------------------------------------------------------------
int BASE_TrailCircle::getTypeId( void )
{
	return( m_type_id );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TrailCircle *BASE_TrailCircle::createTrailCircle( const int left, const int top, const double scale )
{
	return( new TrailCircle( this, left - CIRCLE_SIZE/2, top - CIRCLE_SIZE/2, scale ) );
}


//===========================================================================


//===========================================================================

TrailCircle::TrailCircle(BASE_TrailCircle *base,const int left,const int top,const double scale )
  : m_selected(false),  m_base(base), m_top( top ), m_left( left ), m_is_active( true )
{

}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TrailCircle::~TrailCircle( void )
{
 //	delete m_image;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool TrailCircle::acceptsClick( const int xpos, const int ypos )
{
	if (m_selected)
		return false;
	if ( m_is_active && ypos >= m_top && ypos <= (m_top+m_base->CIRCLE_SIZE) && xpos >= m_left && xpos <= ( m_left+m_base->CIRCLE_SIZE) )
		return( true );
	return( false );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void TrailCircle::drawConnectingList(TrailCircle *prevSelected)
{
	m_base->m_parent->m_playing_board->Canvas->Pen->Width = 2.0;
	m_base->m_parent->m_playing_board->Canvas->Pen->Color = ((TColor) 0x2FD62F);
	m_base->m_parent->m_playing_board->Canvas->Brush->Color = ((TColor) 0x2FD62F);
	m_base->m_parent->m_playing_board->Canvas->MoveTo(m_left+m_base->CIRCLE_SIZE/2,m_top+m_base->CIRCLE_SIZE/2);
	m_base->m_parent->m_playing_board->Canvas->LineTo(prevSelected->m_left+m_base->CIRCLE_SIZE/2,prevSelected->m_top+m_base->CIRCLE_SIZE/2);
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void TrailCircle::draw()
{
	if (m_selected)
	{
		m_base->m_parent->m_playing_board->Canvas->Pen->Width = 1.0;
		m_base->m_parent->m_playing_board->Canvas->Pen->Color = ((TColor) 0x33FFFF);
		m_base->m_parent->m_playing_board->Canvas->Brush->Color = ((TColor) 0x33FFFF);
		m_base->m_parent->m_playing_board->Canvas->Ellipse(m_left-5,m_top-5,m_left+m_base->CIRCLE_SIZE+5,m_top+m_base->CIRCLE_SIZE+5);
	}

	m_base->m_parent->m_playing_board->Canvas->Pen->Width = 2.0;
	m_base->m_parent->m_playing_board->Canvas->Pen->Color = ((TColor) 0x0000000);
	m_base->m_parent->m_playing_board->Canvas->Brush->Color = ((TColor) 0xFFFFFF);
	m_base->m_parent->m_playing_board->Canvas->Ellipse(m_left,m_top,m_left+m_base->CIRCLE_SIZE,m_top+m_base->CIRCLE_SIZE);

	m_base->m_parent->m_playing_board->Canvas->Font->Size = 12;

	int width = m_base->m_parent->m_playing_board->Canvas->TextWidth(m_base->m_image);
	int height = m_base->m_parent->m_playing_board->Canvas->TextHeight(m_base->m_image);

	m_base->m_parent->m_playing_board->Canvas->TextOutW(m_left + ((m_base->CIRCLE_SIZE - width)/2),m_top + ((m_base->CIRCLE_SIZE - height)/2),m_base->m_image);

//	m_trail->m_playing_board->Canvas->Pen
/*
	if ( m_rescale )
		m_trail->m_playing_board->Canvas->StretchDraw(m_rect, m_image );
	else
		m_trail->m_playing_board->Canvas->Draw(m_left, m_top, m_image );
*/
}



//===========================================================================

