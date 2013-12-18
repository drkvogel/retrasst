#ifndef TrailCircle_h
#define TrailCircle_h
//---------------------------------------------------------------------------

#include "windef.h"
#include "vcl.h"
#include <vector>
#include <string>

#include "TheTrail.h"

//---------------------------------------------------------------------------
class TrailCircle;

class BASE_TrailCircle
{
public:
	BASE_TrailCircle( TheTrail *parent, const String image );
	~BASE_TrailCircle();

	int getTypeId();
	TrailCircle *createTrailCircle( const int left, const int top, const double scale );

	bool m_in_play;
	TheTrail *m_parent;
	int m_type_id;
	String m_image;

	const int CIRCLE_SIZE;
};
//============================================================================

class TrailCircle
{
public:
	TrailCircle(BASE_TrailCircle *base,const int top,const int left,const double scale );
	~TrailCircle( void );

	bool	acceptsClick( const int xpos, const int ypos );
	void	draw();
	void    drawText(const std::string &text);
	void drawConnectingList(TrailCircle	*prevSelected);

	void setSelected(const bool sel) {m_selected = sel;}
	bool isSelected() {return m_selected;}
	int	m_card_id;
	String getName() {return m_base->m_image;}
private:

	bool m_selected;
	int	m_top, m_left;
	bool	m_is_active;
	BASE_TrailCircle *m_base;
};
//---------------------------------------------------------------------------
#endif

