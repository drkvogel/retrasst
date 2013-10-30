#ifndef cardH
#define cardH
//---------------------------------------------------------------------------
#include "windef.h"
#include "vcl.h"
#include <vector>
#include <string>
//---------------------------------------------------------------------------
class CARD;
class BASE_CARD;
//---------------------------------------------------------------------------
class DECK
{
private:
	static	int	next_type_id;
public:
	DECK( TImage *board, const TColor background, const int ht,
		const int wd, const String rear_image );
	~DECK( void );
	TImage	*playing_board;
	TColor	background_colour;
	int	height;
	int	width;
	Graphics::TBitmap	*rear;
	int 	getNextId( void );
	std::vector<BASE_CARD *> pack;
	BASE_CARD	*createBASE_CARD( const String face_image );
	bool	addCard( String face_image );
	static	String	graphics_location;
	void 	setInPlay( const bool inp );
	bool 	selectNFaces( const int nf );
	static int rescale( const int arg, const double factor );
};
//---------------------------------------------------------------------------
class BASE_CARD
{
private:
	int	type_id;			// UNIQUE PER OBJECT
protected:
public:
	BASE_CARD( DECK *owner, const String face_image );
	~BASE_CARD( void );
	bool	in_play;
	int	getTypeId( void );
	Graphics::TBitmap	*face; 			// IMAGE SHOWING CARD FACE
	DECK *blank_card;
	CARD *createCARD( const int tp, const int lf, const double scale ); // CREATE NEW CARD
};
//---------------------------------------------------------------------------
class CARD
{
private:
	bool	is_active;
protected:
	const BASE_CARD	*base_card;
public:
	CARD( const BASE_CARD *owner, const int tp, const int lf,
		const double scale );
	~CARD( void );
	int	top, left, height, width;
	int	card_id;
	bool	face_up;
	bool	rescale;
	TRect	rect;
	Graphics::TBitmap	*card_face;
	bool	acceptsClick( const int xpos, const int ypos );
	void	turnFaceUp( void );			// SHOW VALUE OF CARD
	void	turnFaceDown( void );		// HIDE VALUE OF CARD
	void	hide( void );			// MAKE INVISIBLE
	bool	isActive( void );		// STILL IN PLAY ?
	void	setActive( const bool act );
};
//---------------------------------------------------------------------------
#endif

