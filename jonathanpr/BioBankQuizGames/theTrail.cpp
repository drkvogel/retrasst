#include <vcl.h>
#pragma hdrstop
#include "theTrail.h"
#include "rand.h"
#include "TrailCircle.h"
//===========================================================================
 int 	TheTrail::m_next_type_id = 1;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TheTrail::TheTrail( TImage *board, const TColor background )
	: m_playing_board( board ),m_background_colour( background )
{
//	m_selectedHighlight = new Graphics::TBitmap();
//	m_selectedHighlight->LoadFromResourceName( (int) HInstance, selectedHighlight );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TheTrail::~TheTrail( void )
{
	std::map<String,BASE_TrailCircle *>::iterator vit = m_pack.begin();
	while ( vit != m_pack.end() )
	{
		delete vit->second;
		vit++;
	}
//	delete m_selectedHighlight;
}
//---------------------------------------------------------------------------
				/* RESCALE INTEGER BY REAL FACTOR */
int TheTrail::rescale( const int arg, const double factor )
{
	double	val = factor * (double) arg;
	return( (int) val );
}
//---------------------------------------------------------------------------

BASE_TrailCircle *TheTrail::createTrailCircle( const String face_image )
{
	return( new BASE_TrailCircle( this, face_image ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void TheTrail::addTrailItem( const String face_image )
{
	m_pack.insert(std::pair<String,BASE_TrailCircle *>(face_image,createTrailCircle( face_image )));
}

