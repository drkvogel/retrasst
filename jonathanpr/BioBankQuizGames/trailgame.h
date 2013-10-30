#ifndef trailgame_h
#define trailgame_h
//---------------------------------------------------------------------------
#include "windef.h"
#include <System.Math.hpp>
#include <vcl.h>
#include <vector>
#include <string>
#include <time.h>

#include "TheTrail.h"
#include "TrailCircle.h"


//---------------------------------------------------------------------------
class TRAIL_GAME : public TheTrail
{
	class GAME_CLICKABLE_POSITIONS
	{
	public:
		GAME_CLICKABLE_POSITIONS(int x,int y, String name)
		{
			m_x = x;
			m_y = y;
			m_name = name;
		}
		static float m_scale;
        String m_name;
		int getX() {return Floor((float)m_x*m_scale);}
		int getY() {return Floor((float)m_y*m_scale);}
	private:
		int m_x;int m_y;
	};

private:
	bool	is_running;
	bool	ignore_click;
	int	left_start;
	int	left_gap;
	int	top_gap;
	int	top_start;
	bool	abandoned;
	void 	(* flashBGFunction)();
	void    (* resetBGFunction)();
	void drawText(std::vector<String> &info, bool showButton, int Size = 12);
	//so we know the bounds of what we want to show. so we can centre it.
	int m_PosMinX,m_PosMaxX,m_PosMinY,m_PosMaxY;
	System::TDateTime m_gameTime;
	System::TDateTime m_trailClickTime;
	void wipeTrail();
public:
	TRAIL_GAME( TImage *board, const TColor background,void (* flashBGFunction)(), void (* resetBGFunction)(),TButton *NextButton);
	~TRAIL_GAME( void );
	void initGame(); //layout game
	void setupGame(int gameID);
	void initaliseTrail(); //load in graphics..etc ready
	void drawTrail(bool drawText);
	void NextClicked();
    TRect getGameRec();
	clock_t	start_level, end_level;
	std::vector<TrailCircle *> m_TrailCircles;

	bool	handleClick( const int x, const int y );
	bool	run( );
	bool	isRunning( void );
	void 	abandon( void );
	int 	getRunTime( void );
	void 	runStart( void );
	bool 	wasAbandoned( void ) const;
	int 	m_nextSelectedID;
private:
	std::vector<String> m_praca_inst;
	std::vector<String> m_parta_inst;
	std::vector<String> m_pracb_inst;
	std::vector<String> m_partb_inst;
	std::vector<String> m_intoText;
	TButton *m_NextButton;
	TTimer *m_bgFlashTimer;
    void NewSaveRow();
	void initGameData();
	std::vector<GAME_CLICKABLE_POSITIONS *> m_gameData[4];
    TSize sizeText(std::vector<String> &info, int Size = 12);
	String getNextClickString(int nextSelectedID);
	int m_GameID; //type of game we r playing
public:
	struct returnDataHeader
	{
		String partid;// value="456" />
		String sessionid;// value="789" />
		String sectionid;// value="hm06" />

		String dateStarted;// value="2013-08-30T10:16:30:000" />

		int abandon;// value="0" />
		int introelapsed;// value="1265" />
		int totelapsed;// value="54148" />

		int practiceaelapsed;// value="5152" />
		int practiceaerrors;// value="1" />
		int practicebelapsed;// value="4839" />
		int practiceberrors;// value="1" />

		int partaelapsed;// value="16327" />
		int partaerrors;// value="2" />
		int partbelapsed;// value="26565" />
		int partberrors;// value="3" />
	};

	struct returnDataRow
	{
		int trial_number;//=level                   ; m_GameID
		int trial_component;// = (nextCircle+1)     ; 1->Num of circles. Current one
		int attempt;//"'+attempt+'"                 ; 1-> count. this attemt count of the current one
//data
		Char part;// = ((level<=2)?"A":"B")			; ((level<=2)?"A":"B")
		Char trialtype;// =trialTypes[level]         ; P = LEvel = 1 or 3, L = 2 or 4
		String expected;// =circlesList[level][nextCircle]   ;the num or letter we r looking for
		String selected;// =circlesList[level][x]             ; what was selected
		long elapsed;// =(now0-attStart)
		int match;// =mark    						; match = 1; failed = 0
	};
	returnDataHeader m_returnDataHeader;
	std::vector<returnDataRow *> m_returnDataRow;
    returnDataRow *m_CurrentRow;
};



float  TRAIL_GAME::GAME_CLICKABLE_POSITIONS::m_scale = 1.0f;

//---------------------------------------------------------------------------
#endif

