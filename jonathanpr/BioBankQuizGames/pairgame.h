#ifndef pairgameH
#define pairgameH
//---------------------------------------------------------------------------
#include "windef.h"
#include <vcl.h>
#include <vector>
#include <string>
#include <time.h>
#include "card.h"
//---------------------------------------------------------------------------
typedef	struct
	{
	int	cols;
	int	rows;
	int	score;
	int	match;
	int	miss;
	time_t	duration;
	int	victory;	// SCORE TO ADVANCE TO NEXT LEVEL
	}
	LAYOUT;
//---------------------------------------------------------------------------
class PAIR_GAME : public DECK
{
private:
	bool	is_running;
	bool	ignore_click;
	int	left_start;
	int	left_gap;
	int	top_gap;
	int	top_start;
	bool	abandoned;
	LAYOUT	*current_layout;
	CARD	*selected_first;
	void 	(* matchFn)(bool match);
public:
	PAIR_GAME( TImage *board, const TColor background, const int ht,
		const int wd, const String rear_image,
		void (* showMatchFn)(bool match) );
	~PAIR_GAME( void );
	clock_t	start_level, end_level;
	std::vector<CARD *> cards;
	void	wipeCards( void );
	bool 	initPairs( const int np, LAYOUT *lay );
	void 	showCardsUp( void );
	void 	showCardsDown( void );
	bool	handleClick( const int x, const int y );
	bool	run( TTimer *t, const int interval );
	bool	isRunning( void );
	void 	abandon( void );
	int 	getRunTime( void );
	void 	runStart( void );
	bool 	wasAbandoned( void ) const;
};
//---------------------------------------------------------------------------
#endif

