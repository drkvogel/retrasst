#ifndef theTrail_h
#define theTrail_h
//---------------------------------------------------------------------------
#include "windef.h"
#include "vcl.h"
#include <vector>
#include <map>
#include <string>
//---------------------------------------------------------------------------
class BASE_TrailCircle;

class TheTrail
{
public:
	TheTrail( TImage *board, const TColor background );
	~TheTrail( void );
	void addTrailItem( String face_image );
	static int rescale( const int arg, const double factor );

	TImage	*m_playing_board;
	std::map<String,BASE_TrailCircle *> m_pack;
	int 	getNextId( void ){return( m_next_type_id++ );}
private:
	BASE_TrailCircle *createTrailCircle( const String face_image );
	static	int	m_next_type_id;

	TColor	m_background_colour;
};


//---------------------------------------------------------------------------
#endif

