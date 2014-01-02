#include <vcl.h>
#include <sstream>
#include "trailgame.h"
#include "rand.h"
#include "System.DateUtils.hpp"
#include "theTrail.h"


//===========================================================================
TRAIL_GAME::TRAIL_GAME( TImage *board, const TColor background,	void (* showMatchFn)(),	void (* resetBoardFn)(),TButton *NextButton )
	:flashBGFunction( showMatchFn ),resetBGFunction(resetBoardFn),left_start( 15 ),left_gap( 30 ),top_gap( 30 ),top_start( 10 ), TheTrail( board, background )
{
	TRAIL_GAME::GAME_CLICKABLE_POSITIONS::m_scale = 1.5f;//0.75f;
	m_NextButton = NextButton;
	m_NextButton->Hide();
	m_CurrentRow= NULL;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TRAIL_GAME::~TRAIL_GAME( VOID )
{
	wipeTrail();

//clear up the game data
	for (int i=0;i< 4;i++)
	{
		for (unsigned int j=0;j<m_gameData[i].size();j++)
			delete m_gameData[i][j];
	}

	for (unsigned int i=0;i<m_returnDataRow.size();i++)
		delete m_returnDataRow[i];

	if (m_CurrentRow)
		delete m_CurrentRow;
}
//===========================================================================

void TRAIL_GAME::initaliseTrail()
{
	//1->25
	int MAX_TRAIL_ITEMS = 25;
	for (int i=1;i<=MAX_TRAIL_ITEMS;i++)
	{
		std::stringstream ss;
		ss << i;
		addTrailItem(String(ss.str().c_str()));
	}
	//A->L
	addTrailItem(String("A"));
	addTrailItem(String("B"));
	addTrailItem(String("C"));
	addTrailItem(String("D"));
	addTrailItem(String("E"));
	addTrailItem(String("F"));
	addTrailItem(String("G"));
	addTrailItem(String("H"));
	addTrailItem(String("I"));
	addTrailItem(String("J"));
	addTrailItem(String("K"));
	addTrailItem(String("L"));

	initGameData();
}
//---------------------------------------------------------------------------

void TRAIL_GAME::wipeTrail()
{
	std::vector<TrailCircle *>::iterator	vit = m_TrailCircles.begin();
	while ( vit != m_TrailCircles.end() )
	{
		delete *vit;
		vit++;
	}
	m_TrailCircles.clear();
}
//---------------------------------------------------------------------------

void TRAIL_GAME::NextClicked()
{
	if (m_GameID == -1)
		m_returnDataHeader.introelapsed = MilliSecondsBetween(Now(),m_gameTime);  //milliseconds elapsed since 1 January 1970
	else if (m_GameID == 0)
		m_returnDataHeader.practiceaelapsed = MilliSecondsBetween(Now(),m_gameTime);  //milliseconds elapsed since 1 January 1970
	else if (m_GameID == 1)
		m_returnDataHeader.partaelapsed = MilliSecondsBetween(Now(),m_gameTime);  //milliseconds elapsed since 1 January 1970
	else if (m_GameID == 2)
		m_returnDataHeader.practicebelapsed = MilliSecondsBetween(Now(),m_gameTime);  //milliseconds elapsed since 1 January 1970
	else if (m_GameID == 3)
		m_returnDataHeader.partbelapsed = MilliSecondsBetween(Now(),m_gameTime);  //milliseconds elapsed since 1 January 1970

	m_gameTime = Now();
	m_NextButton->Hide();
	setupGame(++m_GameID);
}


//---------------------------------------------------------------------------
void TRAIL_GAME::drawTrail(bool bdrawText)
{
/*
//draw blue boarder
	m_playing_board->Canvas->Pen->Width = 2.0;
	m_playing_board->Canvas->Pen->Color = ((TColor) 0xEE0000);
	m_playing_board->Canvas->Brush->Color = ((TColor) 0xEE0000);

	m_playing_board->Canvas->MoveTo(m_PosMinX,m_PosMinY);
	m_playing_board->Canvas->LineTo(m_PosMinX,m_PosMaxY);
	m_playing_board->Canvas->LineTo(m_PosMaxX,m_PosMaxY);
	m_playing_board->Canvas->LineTo(m_PosMaxX,m_PosMinY);
	m_playing_board->Canvas->LineTo(m_PosMinX,m_PosMinY);
*/



	std::vector<TrailCircle *>::iterator vit = m_TrailCircles.begin();
	TrailCircle *prevSelected = NULL;
	while ( vit != m_TrailCircles.end() )
	{
		if ((prevSelected) && ((*vit)->isSelected()))
			(*vit)->drawConnectingList(prevSelected);
		prevSelected = *vit;
		vit++;
	}

	if ((m_GameID == 0) || (m_GameID == 2))
	{
		m_TrailCircles.at(0)->drawText("Begin");
		m_TrailCircles.at(m_TrailCircles.size()-1)->drawText("End");
	}

	vit = m_TrailCircles.begin();
	while ( vit != m_TrailCircles.end() )
	{
		(*vit)->draw();
		vit++;
	}

	if ((bdrawText) && (m_GameID==0))
		drawText(m_praca_inst,false);
	if ((bdrawText) && (m_GameID==2))
		drawText(m_pracb_inst,false);
}

//---------------------------------------------------------------------------

void TRAIL_GAME::drawText(std::vector<String> &info, bool showButton, int Size, System::Uitypes::TColor TextColour)
{
	m_playing_board->Canvas->Font->Size = Size;
	System::Uitypes::TColor  oldTextColour = m_playing_board->Canvas->Font->Color;

	m_playing_board->Canvas->Font->Color = TextColour;
	unsigned int i=0;
	for (;i<info.size();i++)
		m_playing_board->Canvas->TextOut(m_PosMinX, m_PosMaxY+20+20*i, info[i]);

	if (showButton)
	{
		m_NextButton->Left = m_PosMinX;
		m_NextButton->Top = m_PosMaxY+20+20+20*i;
		m_NextButton->Show();
	}
	else
		m_NextButton->Hide();
	m_playing_board->Canvas->Font->Color = oldTextColour;
}
//---------------------------------------------------------------------------

TSize TRAIL_GAME::sizeText(std::vector<String> &info, int Size)
{
	m_playing_board->Canvas->Font->Size = Size;

	TSize max;
	max.Width = 0;
	max.Height = 0;
	unsigned int i=0;
	for (;i<info.size();i++)
	{
		TSize size = m_playing_board->Canvas->TextExtent(info[i]);
		if (size.Width > max.Width)
			max.Width = size.Width;
		if (size.Height > max.Height)
			max.Height = size.Height;
	}
	return max;
}
//---------------------------------------------------------------------------
void TRAIL_GAME::initGame()
{
	while (!m_returnDataRow.empty())
	{
		delete m_returnDataRow.back();
		m_returnDataRow.pop_back();
	}
	if (m_CurrentRow)
	{
		delete m_CurrentRow;
		m_CurrentRow = NULL;
	}

	setupGame(-1);
}

//---------------------------------------------------------------------------
void TRAIL_GAME::setupGame(int gameID)
{
	m_GameID = gameID;
	m_gameTime = Now();
	if (m_GameID == -1)
	{
//position the text
		TSize textSize = sizeText(m_intoText);

		m_PosMinX = m_playing_board->Width/2 - textSize.Width/2;
		m_PosMaxY = m_playing_board->Height/2 -  - textSize.Height/2;
		drawText(m_intoText,true,16);
		return;
	}
	m_nextSelectedID = 0;
	wipeTrail();
	int cardID = 0;

//Initalise the boundy vars
	m_PosMinX = System::MaxInt;
	m_PosMinY = System::MaxInt;
	m_PosMaxY = -1;
	m_PosMaxX = -1;
//get the boundry

	std::vector<GAME_CLICKABLE_POSITIONS *>::iterator itrPos = m_gameData[m_GameID].begin();
	while ( itrPos != m_gameData[m_GameID].end() )
	{
		GAME_CLICKABLE_POSITIONS *pos = *itrPos;

		BASE_TrailCircle *BTC = m_pack[pos->m_name];
		if (BTC)
		{
			if (pos->getX() < m_PosMinX)
				m_PosMinX = pos->getX();
			if (pos->getX() > m_PosMaxX)
				m_PosMaxX = pos->getX();

			if (pos->getY() < m_PosMinY)
				m_PosMinY = pos->getY();
			if (pos->getY() > m_PosMaxY)
				m_PosMaxY = pos->getY();
		}
		else
		{
			throw Exception("AARRGGHHH");
		}
		itrPos++;
	}

	int trailWidth = m_PosMaxX - m_PosMinX;
	int trailHeight = m_PosMaxY - m_PosMinY;

	int Xgap = left_gap + left_start;
	int Ygap = top_gap + top_start;

	int ScreenCentreX = (m_playing_board->Width)/2;
	int ScreenCentreY = (m_playing_board->Height)/2;

	int TrailCentreX = m_PosMinX + trailWidth/2;
	int TrailCentreY = m_PosMinY + trailHeight/2;

	int Xoff = ScreenCentreX - TrailCentreX;
	int Yoff = ScreenCentreY - TrailCentreY;

 //reset min max for centralised position, for drawing blue box
	m_PosMinX = System::MaxInt;
	m_PosMinY = System::MaxInt;
	m_PosMaxY = -1;
	m_PosMaxX = -1;

//fill in the data + boundry information
	itrPos = m_gameData[m_GameID].begin();
	while ( itrPos != m_gameData[m_GameID].end() )
	{
		GAME_CLICKABLE_POSITIONS *pos = *itrPos;

		BASE_TrailCircle *BTC = m_pack[pos->m_name];
		if (BTC)
		{
			if (pos->getX()+Xoff < m_PosMinX)
				m_PosMinX = pos->getX()+Xoff;
			if (pos->getX()+Xoff > m_PosMaxX)
				m_PosMaxX = pos->getX()+Xoff;

			if (pos->getY()+Yoff < m_PosMinY)
				m_PosMinY = pos->getY()+Yoff;
			if (pos->getY()+Yoff > m_PosMaxY)
				m_PosMaxY = pos->getY()+Yoff;

			TrailCircle *TC	= BTC->createTrailCircle(pos->getX()+Xoff,pos->getY()+Yoff,1.0);
			TC->m_card_id = cardID;
			cardID++;
			m_TrailCircles.push_back( TC );
		}
		else
		{
			throw Exception("AARRGGHHH");
		}
		itrPos++;
	}

	m_PosMinX -= 75;
	m_PosMinY -= 75;
	m_PosMaxY += 75;
	m_PosMaxX += 75;
	resetBGFunction();
	m_trailClickTime = Now();
	NewSaveRow();
	drawTrail(true);
}

//---------------------------------------------------------------------------

void TRAIL_GAME::NewSaveRow()
{
	if (m_CurrentRow)
	{
		m_returnDataRow.push_back(m_CurrentRow);
	 	m_CurrentRow = NULL;
	}
	if (getNextClickString(m_nextSelectedID) == -1)
		return;

	m_CurrentRow = new returnDataRow();
	m_CurrentRow->attempt = 0;
	m_CurrentRow->trial_number = m_GameID;
	m_CurrentRow->trial_component = m_nextSelectedID;

	if (m_GameID <= 2)
		m_CurrentRow->part = 'A';
	else
		m_CurrentRow->part = 'B';

	if ((m_GameID == 0) || (m_GameID == 2))
		m_CurrentRow->trialtype = 'P';
	else
		m_CurrentRow->trialtype = 'L';

	m_CurrentRow->expected = getNextClickString(m_nextSelectedID);
	m_CurrentRow->elapsed = MilliSecondsBetween(Now(),m_trailClickTime);
}

//---------------------------------------------------------------------------

TRect TRAIL_GAME::getGameRec()
{
	return TRect(m_PosMinX,m_PosMaxY,m_PosMaxX,m_PosMinY);
}

//---------------------------------------------------------------------------

String TRAIL_GAME::getNextClickString(int nextSelectedID)
{
	if ((unsigned int)nextSelectedID >= m_gameData[m_GameID].size())
		return String("-1");

	GAME_CLICKABLE_POSITIONS *pos = m_gameData[m_GameID][nextSelectedID];
	return 	pos->m_name;
}

//---------------------------------------------------------------------------
bool TRAIL_GAME::handleClick( const int x, const int y )
{
	bool Complete = false;
	if ( ignore_click )	// PREVENT OVER-LAPPING CALLS
		return( false );

	ignore_click = true;

	TrailCircle	*selected = NULL;
	TrailCircle	*prevSelected = NULL;
	std::vector<TrailCircle *>::iterator vit = m_TrailCircles.begin();
	while ( vit != m_TrailCircles.end() )
	{
		if ( (*vit)->acceptsClick( x, y ) )
		{
			selected = *vit;
			break;
		}
		prevSelected = *vit;
		vit++;
	}

	if (selected)
	{
		String Next = getNextClickString(m_nextSelectedID);
		if (Next == "-1")
			Complete = true;

		m_CurrentRow->selected = selected->getName();
		m_CurrentRow->elapsed = MilliSecondsBetween(Now(),m_trailClickTime);
		m_trailClickTime = Now();
		if (Next == selected->getName())
		{
			m_CurrentRow->match = 1;
			m_nextSelectedID = selected->m_card_id+1;
			selected->setSelected(true);
			drawTrail(true);

			Next = getNextClickString(m_nextSelectedID);
			if (Next == "-1")
				Complete = true;
		}
		else
		{
			m_CurrentRow->match = 0;
			if (m_GameID == 0)
				m_returnDataHeader.practiceaerrors++;
			else if (m_GameID == 1)
				m_returnDataHeader.partaerrors++;
			else if (m_GameID == 2)
				m_returnDataHeader.practiceberrors++;
			else if (m_GameID == 3)
				m_returnDataHeader.partberrors++;

			flashBGFunction();
		}
		NewSaveRow();
	}
	if (Complete)
	{
		if (m_GameID >= 3)
		{
			m_returnDataHeader.partbelapsed = MilliSecondsBetween(Now(),m_gameTime);  //milliseconds elapsed since 1 January 1970
            NewSaveRow();
			end_level = clock();
			is_running = false;
			return( true );
		}
		if (m_GameID==0)
		{
			resetBGFunction();
			drawTrail(false);
			drawText(m_parta_inst,true);
		}
		else if (m_GameID==2)
		{
			resetBGFunction();
			drawTrail(false);
			drawText(m_partb_inst,true);
		}
		else
		{
			if (m_GameID == 1)
				m_returnDataHeader.partaelapsed = MilliSecondsBetween(Now(),m_gameTime);  //milliseconds elapsed since 1 January 1970
			setupGame(++m_GameID);
		}
	}

	ignore_click = false;
	return( false );
}
//---------------------------------------------------------------------------
bool TRAIL_GAME::isRunning( void )
{
	return( is_running );
}
//---------------------------------------------------------------------------
void TRAIL_GAME::abandon( void )
{
	is_running = false;
	abandoned = true;
}
//---------------------------------------------------------------------------
bool TRAIL_GAME::wasAbandoned( void ) const
{
	return( abandoned );
}
//---------------------------------------------------------------------------
void TRAIL_GAME::runStart( void )
{
	is_running = true;
}
//---------------------------------------------------------------------------
bool TRAIL_GAME::run()
{
	ignore_click = true;
	is_running = true;
	abandoned = false;

	ignore_click = false;
//	drawTrail(true);
	runStart();

	ignore_click = false;
	start_level = clock();
	end_level = start_level;
	Application->ProcessMessages();
	return( true );
}
//---------------------------------------------------------------------------
				/* RETURN RUN-TIME IN DECI-SECONDS */
int TRAIL_GAME::getRunTime( void )
{
	return( (int)( ( 10 * ( end_level - start_level ) ) / CLK_TCK ) );
}

void TRAIL_GAME::initGameData()
{
//Practice A
	m_gameData[0].push_back(new GAME_CLICKABLE_POSITIONS(245,167,"1"));
	m_gameData[0].push_back(new GAME_CLICKABLE_POSITIONS(285,66 ,"2"));
	m_gameData[0].push_back(new GAME_CLICKABLE_POSITIONS(384,127,"3"));
	m_gameData[0].push_back(new GAME_CLICKABLE_POSITIONS(325,156,"4"));
	m_gameData[0].push_back(new GAME_CLICKABLE_POSITIONS(353,236,"5"));
	m_gameData[0].push_back(new GAME_CLICKABLE_POSITIONS(97 ,217,"6"));
	m_gameData[0].push_back(new GAME_CLICKABLE_POSITIONS(48 ,117,"7"));
	m_gameData[0].push_back(new GAME_CLICKABLE_POSITIONS(176,111,"8"));

//Live A
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(277,390,"1"));    //live a coordinates
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(213,452,"2"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(319,452,"3"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(327,208,"4"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(206,267,"5"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(263,321,"6"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(185,356,"7"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS( 86,440,"8"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(107,490,"9"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(156,445,"10"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(206,511,"11"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS( 35,516,"12"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS( 65,287,"13"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS( 29,336,"14"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS( 29, 69,"15"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS( 72,159,"16"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(149, 36,"17"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(178,158,"18"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(284,117,"19"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(206,110,"20"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(236, 29,"21"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(406, 84,"22"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(419,493,"23"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(370,282,"24"));
	m_gameData[1].push_back(new GAME_CLICKABLE_POSITIONS(355,508,"25"));

//practice b coordinates
	m_gameData[2].push_back(new GAME_CLICKABLE_POSITIONS(245,167,"1"));
	m_gameData[2].push_back(new GAME_CLICKABLE_POSITIONS(285, 66,"A"));
	m_gameData[2].push_back(new GAME_CLICKABLE_POSITIONS(384,127,"2"));
	m_gameData[2].push_back(new GAME_CLICKABLE_POSITIONS(325,156,"B"));
	m_gameData[2].push_back(new GAME_CLICKABLE_POSITIONS(353,236,"3"));
	m_gameData[2].push_back(new GAME_CLICKABLE_POSITIONS( 97,217,"C"));
	m_gameData[2].push_back(new GAME_CLICKABLE_POSITIONS( 48,117,"4"));
	m_gameData[2].push_back(new GAME_CLICKABLE_POSITIONS(177,101,"D"));

//live b coordinates
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(228,232,"1"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(323,387,"A"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(175,420,"2"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(203,111,"B"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(234,169,"3"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(327,286,"C"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(277, 80,"4"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(386, 78,"D"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(380,246,"5"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(381,472,"E"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(233,457,"6"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(130,498,"F"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(155,225,"7"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS( 90,381,"G"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS( 65, 90,"8"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(108,268,"H"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(149, 79,"9"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(342, 29,"I"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(416, 29,"10"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(408,409,"J"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS(422,520,"11"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS( 38,521,"K"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS( 23,337,"12"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS( 56,452,"L"));
	m_gameData[3].push_back(new GAME_CLICKABLE_POSITIONS( 32, 40,"13"));

	m_praca_inst.push_back(L"This is a practice. Beginning with number 1, join all the circles in numeric order by");
	m_praca_inst.push_back(L"clicking the next numbered circle. Please work as quickly and accurately as you can.");

	m_parta_inst.push_back(L"Well done. Click next for the test.");

	m_pracb_inst.push_back(L"This is a practice. Beginning with number 1 then letter A, join all the circles in");
	m_pracb_inst.push_back(L"alternating numeric and alphabetic order by clicking the next numbered then the next");
	m_pracb_inst.push_back(L"lettered circle. Please work as quickly and accurately as you can.");

	m_partb_inst.push_back(L"Well done. Click next for the test.");

	m_intoText.push_back(L"We now want to measure your coordination.");
}
//===========================================================================
#pragma package(smart_init)

