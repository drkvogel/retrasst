#ifndef symbolDigitGame_h
#define symbolDigitGame_h
//---------------------------------------------------------------------------
#include "windef.h"
#include <System.Math.hpp>
#include <vcl.h>
#include <vector>
#include <string>
#include <time.h>
//#include "digitalSymbol.h"


//---------------------------------------------------------------------------
class SYMBOLDIGIT_GAME
{
private:
	bool	is_running;
	bool	ignore_click;
	int	left_start;
	int	left_gap;
	int	top_gap;
	int	top_start;
	bool	abandoned;

public:
	SYMBOLDIGIT_GAME( TImage *board, const TColor background,void (* setTimeOut)(bool), void (* setFlashTimeOut) (bool,int),TButton *NextButton);
	~SYMBOLDIGIT_GAME( void );
	bool initGame(const int gameid = -2 ); //layout game
	void initalise(); //load in graphics..etc ready
	void drawTrail();
	void timeout();
	void flashTimeout();
	clock_t	start_level, end_level;

	bool	handleClick( const int x, const int y );
	bool	run( );
	bool	isRunning( void );
	void 	abandon( void );
	int 	getRunTime( void );
	void 	runStart( void );
	bool 	wasAbandoned( void ) const;
	int 	m_nextSelectedID;
	void 	NextClicked();

private:
	void NewSaveRow();
	void 	(* setTimeOutFunction)(bool);
	void 	(* setFlashTimeOutFunction) (bool, int);
	TButton *m_NextButton;
	void doIntoTextScreen();
	int getGameSize();
	int getGamePadX();
	int getGamePadY();
	void drawCrossDigit(int i);
	void drawGamePads();
	void drawGameText();
	void drawBlankGamePads();
	void drawGamePadColums(int col, bool whiteOut);
	void drawNumberPad();
	void drawBlock(String Image, TColor PenCol,TColor BrushCol, int PosX, int PosY);
	void drawUserDigit(int i,bool Blank,TColor BrushCol, String theImage);
 //	std::vector<digitalSymbol *> m_digitalSymbols;
    bool m_timedOut;
	TTimer *m_bgFlashTimer;
	int m_GameID; //type of game we r playing
    int m_CurrentDigitPosition;
//const static data for the game. due to limitations of C++ it needs to be static :(
	static const _TCHAR  m_gameSymbols[8];
	static const int m_gameData[33][8];
	int m_FlashCrossDigit;  //the position of the digit we are flashing the cross over
	int m_FlashLineSetUpColumn;  //the position in the Column we are waiting to fill
	TImage *m_playing_board;
	static const String m_practiceText[4];
	static const int m_gameExampleData[8];
	static const int m_gamePracticeData[8];
	static const int m_gameStartData[8];
	static const String m_inGameText[2];
	static const String m_introScreenGameText[7];
	static const int m_gamePracticeSymbolDigit[8];
	bool m_FirstRun;
	bool m_practice;
	String m_userkeyinput;
	String m_uservalueinput;
//to make it the same as the JAvascript
	int m_thisrow, m_thatrow; // keeps track of where we are in the data.
	int m_symToDig[8];
	int m_inputarray[8];
	int m_codearray[8];

	int m_attempt;
	int m_lastScore;  //keeps a count of all sucessful attempts in live
	int m_lastDigit; //keeps a count of all attempts in live
	System::TDateTime m_digTime;
	System::TDateTime m_starttime; //The time quiz part started
	System::TDateTime m_levelStart; //time when a digit is presented!
	System::TDateTime m_changeStart; //could be more sensibly called levelstart. The time the level has been running
	System::TDateTime m_liveStartTime; //The time the live game has been running
public:
	struct returnDataHeader
	{
		String partid;// value="456" />
		String sessionid;// value="789" />
		String sectionid;// value="hm06" />

		String dateStarted;// value="2013-08-30T10:16:30:000" />

		int abandon;// value="0" />
		int liveelapsed;//  = nowtime-changeStart[1];
		int totelapsed;// = nowtime-starttime;
		int instrelapsed;  //intro time
		int levels;// = level;
		int digits;// = lastDigit;
		int	 score;// = lastScore;

		int startrow; //start row
		int pracelapsed; //practice time took
	};

	struct returnDataRow
	{
		int trial_number;    //level
		int trial_component; //dig+1
		int attempt;         //attempt

		Char trialtype;      //levelType
		int symbol;               //inputarray[dig]
		String expected;     //expected
		String selected;     //selected
		long elapsed;         //nowtime-digTime
		int valid;                //((t)?1:0)
		int rsi;                  //((dig==0)?(levelStart[level]-changeStart[level]):0
		int complete; //is the data entry complete. will not be if aborted
	};
	returnDataRow *m_CurrentRow;

	returnDataHeader m_returnDataHeader;
	std::vector<returnDataRow *> m_returnDataRow;
};


//---------------------------------------------------------------------------
#endif

