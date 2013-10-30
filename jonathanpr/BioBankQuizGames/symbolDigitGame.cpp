#include <vcl.h>
#include <sstream>
#include "symbolDigitGame.h"
#include "rand.h"
#include "System.DateUtils.hpp"
#include "theTrail.h"
#define	PLAY_BACKGROUND	((TColor) 0xffffff)
#define	USERINPUT_DARKGRAY	((TColor) 0x7F7F7F)
#define	USERINPUT_WHITE	((TColor) 0xFFFFFF)
#define USERINPUT_NUMBERPAD_GRAY ((TColor) 0xCCCCCC)
#define	USERINPUT_BORDER	((TColor) 0x6F5F00)

#define	SYMBOLS_WIDE	8


//===========================================================================
SYMBOLDIGIT_GAME::SYMBOLDIGIT_GAME( TImage *board, const TColor background,void (* setTimeOut)(bool), void (* setFlashTimeOut) (bool,int), TButton *NextButton)
	:setTimeOutFunction(setTimeOut),setFlashTimeOutFunction(setFlashTimeOut), m_NextButton(NextButton),left_start( 15 ),left_gap( 30 ),top_gap( 30 ),top_start( 10 )
{
	m_playing_board = board;
	m_FlashCrossDigit = -1;
	m_FlashLineSetUpColumn = -1;
	m_CurrentRow= NULL;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
SYMBOLDIGIT_GAME::~SYMBOLDIGIT_GAME( VOID )
{
	if (m_CurrentRow)
	{
		delete m_CurrentRow;
		m_CurrentRow = NULL;
	}

	while (!m_returnDataRow.empty())
	{
		delete m_returnDataRow.back();
		m_returnDataRow.pop_back();
	}
}

//---------------------------------------------------------------------------

void SYMBOLDIGIT_GAME::NewSaveRow()
{
	if (m_CurrentRow)
	{
		m_returnDataRow.push_back(m_CurrentRow);
		m_CurrentRow = NULL;
	}
//fill out the basic details of this new row
	m_CurrentRow = new returnDataRow();
	m_CurrentRow->complete = false;

}

//---------------------------------------------------------------------------

void SYMBOLDIGIT_GAME::drawBlock(String Image, TColor PenCol,TColor BrushCol, int PosX, int PosY)
{
	const int Size = getGameSize();

	m_playing_board->Canvas->Pen->Width = 4.0;
	m_playing_board->Canvas->Pen->Color = PenCol;
	m_playing_board->Canvas->Brush->Color = BrushCol;
	m_playing_board->Canvas->Rectangle(PosX,PosY,PosX+Size,PosY+Size);
	m_playing_board->Canvas->Font->Size = 20;
	m_playing_board->Canvas->Font->Name = "Arial Black";

	if (Image.Length() > 0)
	{
		TSize textSize = m_playing_board->Canvas->TextExtent(Image);
		m_playing_board->Canvas->TextOutW(PosX+(Size/2)-(textSize.Width/2),PosY+(Size/2)-(textSize.Height/2),Image);
	}
	m_playing_board->Canvas->Font->Name = "Arial";
}

//---------------------------------------------------------------------------
void SYMBOLDIGIT_GAME::drawCrossDigit(int i)
{
	const int Size = getGameSize();
	const int BlocksWide = SYMBOLS_WIDE;
	int PosX = (m_playing_board->Width/2)-(Size*(BlocksWide/2));
	int PosY = (m_playing_board->Height/2)-(Size*6)+Size*3+Size/2;
	String Image = "X";

	PosX += Size*i;
	m_playing_board->Canvas->Pen->Width = 4.0;
	m_playing_board->Canvas->Pen->Color = USERINPUT_BORDER;
	m_playing_board->Canvas->Brush->Color = USERINPUT_WHITE;
	m_playing_board->Canvas->Rectangle(PosX,PosY,PosX+Size,PosY+Size);
	m_playing_board->Canvas->Font->Size = 20;

	TColor oldCol = m_playing_board->Canvas->Font->Color;
	m_playing_board->Canvas->Font->Color =(TColor) 0x0000FF;
	m_playing_board->Canvas->Font->Name = "Arial Black";

	TSize textSize = m_playing_board->Canvas->TextExtent(Image);
	m_playing_board->Canvas->TextOutW(PosX+(Size/2)-(textSize.Width/2),PosY+(Size/2)-(textSize.Height/2),Image);
	m_playing_board->Canvas->Font->Color = oldCol;
	m_playing_board->Canvas->Font->Name = "Arial";
}

//---------------------------------------------------------------------------
void SYMBOLDIGIT_GAME::drawUserDigit(int i, bool Blank,TColor BrushCol, String theImage)
{
	const int Size = getGameSize();
	const int BlocksWide = SYMBOLS_WIDE;
	int XPos = (m_playing_board->Width/2)-(Size*(BlocksWide/2));
	int YPos = (m_playing_board->Height/2)-(Size*6)+Size*3+Size/2;
	String Image = "";
	if (!Blank)
		Image = theImage;

	drawBlock(Image,USERINPUT_BORDER,BrushCol, XPos+Size*i, YPos);
}

//---------------------------------------------------------------------------
void SYMBOLDIGIT_GAME::drawGameText()
{
	m_playing_board->Canvas->Brush->Color = PLAY_BACKGROUND;
	m_playing_board->Canvas->Font->Size = 25;
	m_playing_board->Canvas->Font->Name = "Arial";

	if (m_practice)
	{
		TSize textSize = m_playing_board->Canvas->TextExtent(m_practiceText[0]);
		m_playing_board->Canvas->TextOutW((m_playing_board->Width/2)-(textSize.Width/2),40,m_practiceText[0]);

		m_playing_board->Canvas->Font->Size = 15;
		textSize = m_playing_board->Canvas->TextExtent(m_practiceText[1]);
		m_playing_board->Canvas->TextOutW((m_playing_board->Width/2)-(textSize.Width/2),100,m_practiceText[1]);

		if (!m_NextButton->Visible)
		{
			textSize = m_playing_board->Canvas->TextExtent(m_practiceText[2]);
			m_playing_board->Canvas->TextOutW((m_playing_board->Width/2)-(textSize.Width/2),450,m_practiceText[2]);
		}
		else
		{
			textSize = m_playing_board->Canvas->TextExtent(m_practiceText[3]);
			m_playing_board->Canvas->TextOutW((m_playing_board->Width/2)-(textSize.Width/2),450,m_practiceText[3]);
		}
	}
	else
	{
		TSize textSize = m_playing_board->Canvas->TextExtent(m_inGameText[0]);
		m_playing_board->Canvas->TextOutW((m_playing_board->Width/2)-(textSize.Width/2),40,m_inGameText[0]);

		m_playing_board->Canvas->Font->Size = 15;
		textSize = m_playing_board->Canvas->TextExtent(m_inGameText[1]);
		m_playing_board->Canvas->TextOutW((m_playing_board->Width/2)-(textSize.Width/2),450,m_inGameText[1]);
	}
}

//---------------------------------------------------------------------------

void SYMBOLDIGIT_GAME::drawBlankGamePads()
{
	const int Size = getGameSize();
	const int BlocksWide = SYMBOLS_WIDE;

	int XPos = (m_playing_board->Width/2)-(Size*(BlocksWide/2));
	int YPos = (m_playing_board->Height/2)-(Size*6);

	for (int i=0;i<BlocksWide;i++)
	{
		drawBlock("",USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*i, YPos);
		drawBlock("",USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*i, YPos+Size);
	}

	YPos += Size*2+Size/2;
	for (int i=0;i<BlocksWide;i++)
	{
		drawBlock("",USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*i, YPos);
		if (i==0)
			drawUserDigit(i,true,USERINPUT_WHITE,"");
		else
			drawUserDigit(i,true,USERINPUT_DARKGRAY,"");
	}
}

//---------------------------------------------------------------------------

void SYMBOLDIGIT_GAME::drawGamePadColums(int col, bool whiteOut)
{
	const int Size = getGameSize();
	const int BlocksWide = SYMBOLS_WIDE;

	int XPos = (m_playing_board->Width/2)-(Size*(BlocksWide/2));
	int YPos = (m_playing_board->Height/2)-(Size*6);

	if (whiteOut)
	{
		drawBlock("",USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*col, YPos);
		drawBlock("",USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*col, YPos+Size);

		YPos += Size*2+Size/2;
		drawBlock("",USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*col, YPos);
	}
	else
	{
		drawBlock(m_gameSymbols[m_codearray[col]],USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*col, YPos);
		drawBlock(col+1,USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*col, YPos+Size);

		YPos += Size*2+Size/2;
		drawBlock(m_gameSymbols[m_inputarray[col]],USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*col, YPos);
	}
}

//---------------------------------------------------------------------------

void SYMBOLDIGIT_GAME::drawGamePads()
{
	const int Size = getGameSize();
	const int BlocksWide = SYMBOLS_WIDE;

	int XPos = (m_playing_board->Width/2)-(Size*(BlocksWide/2));
	int YPos = (m_playing_board->Height/2)-(Size*6);

	for (int i=0;i<BlocksWide;i++)
	{
		drawBlock(m_gameSymbols[m_codearray[i]],USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*i, YPos);
		drawBlock(i+1,USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*i, YPos+Size);
	}

	YPos += Size*2+Size/2;
	for (int i=0;i<BlocksWide;i++)
	{
		drawBlock(m_gameSymbols[m_inputarray[i]],USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*i, YPos);
		drawUserDigit(i,false,USERINPUT_DARKGRAY,m_userkeyinput[i+1]); //string assumes a base index of 1.. dunny why
	}
}

//---------------------------------------------------------------------------

int SYMBOLDIGIT_GAME::getGamePadX()
{
	return (m_playing_board->Width/2)-(getGameSize()*2);
}
//---------------------------------------------------------------------------

int SYMBOLDIGIT_GAME::getGamePadY()
{
	return (m_playing_board->Height/2)+(getGameSize()*2);
}
//---------------------------------------------------------------------------

int SYMBOLDIGIT_GAME::getGameSize()
{
	return 50;
}
//---------------------------------------------------------------------------

void SYMBOLDIGIT_GAME::drawNumberPad()
{
	const int Size = getGameSize();

	int XPos = getGamePadX();
	int YPos = getGamePadY()-Size;
	drawBlock(String("1"),USERINPUT_BORDER,USERINPUT_NUMBERPAD_GRAY, XPos, YPos);
	drawBlock(String("5"),USERINPUT_BORDER,USERINPUT_NUMBERPAD_GRAY, XPos, YPos+Size);
	XPos+=Size;
	drawBlock(String("2"),USERINPUT_BORDER,USERINPUT_NUMBERPAD_GRAY, XPos, YPos);
	drawBlock(String("6"),USERINPUT_BORDER,USERINPUT_NUMBERPAD_GRAY, XPos, YPos+Size);
	XPos+=Size;
	drawBlock(String("3"),USERINPUT_BORDER,USERINPUT_NUMBERPAD_GRAY, XPos, YPos);
	drawBlock(String("7"),USERINPUT_BORDER,USERINPUT_NUMBERPAD_GRAY, XPos, YPos+Size);
	XPos+=Size;
	drawBlock(String("4"),USERINPUT_BORDER,USERINPUT_NUMBERPAD_GRAY, XPos, YPos);
	drawBlock(String("8"),USERINPUT_BORDER,USERINPUT_NUMBERPAD_GRAY, XPos, YPos+Size);
}

//---------------------------------------------------------------------------
void SYMBOLDIGIT_GAME::drawTrail()
{
	m_playing_board->Canvas->Brush->Color = PLAY_BACKGROUND;
	m_playing_board->Canvas->FillRect( m_playing_board->ClientRect );

	if (!((m_NextButton->Visible) && (m_practice)))
		drawNumberPad();
	if (m_FirstRun)
	{
		m_FirstRun = false;
		drawBlankGamePads();
	}
	else
	{
		drawGamePads();
	}
	drawGameText();
}
//---------------------------------------------------------------------------
void SYMBOLDIGIT_GAME::doIntoTextScreen()
{
	m_playing_board->Canvas->Brush->Color = PLAY_BACKGROUND;
	m_playing_board->Canvas->FillRect( m_playing_board->ClientRect );

	m_playing_board->Canvas->Brush->Color = PLAY_BACKGROUND;
	m_playing_board->Canvas->Font->Size = 25;
	m_playing_board->Canvas->Font->Name = "Arial";

	int LineY =  100;
	TSize textSize = m_playing_board->Canvas->TextExtent(m_introScreenGameText[0]);
	m_playing_board->Canvas->TextOutW((m_playing_board->Width/2)-(textSize.Width/2),LineY,m_introScreenGameText[0]);

	m_playing_board->Canvas->Font->Size = 15;
	textSize = m_playing_board->Canvas->TextExtent(m_introScreenGameText[1]);

	LineY += 60;
	m_playing_board->Canvas->TextOutW((m_playing_board->Width/2)-(textSize.Width/2),LineY,m_introScreenGameText[1]);
	LineY += 25;
	m_playing_board->Canvas->TextOutW((m_playing_board->Width/2)-(textSize.Width/2),LineY,m_introScreenGameText[2]);

 //---------------------------------------------------------------------------
	const int Size = getGameSize();
	const int BlocksWide = SYMBOLS_WIDE;

	int XPos = (m_playing_board->Width/2)-(Size*(BlocksWide/2));
	LineY += 60;

	for (int i=0;i<BlocksWide;i++)
	{
		drawBlock(m_gameSymbols[i],USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*i, LineY);
		drawBlock(i+1,USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*i, LineY+Size);
	}
 //---------------------------------------------------------------------------

	m_playing_board->Canvas->Font->Size = 15;
	LineY += 140;
	m_playing_board->Canvas->TextOutW((m_playing_board->Width/2)-(textSize.Width/2),LineY,m_introScreenGameText[3]);
	LineY += 25;
	m_playing_board->Canvas->TextOutW((m_playing_board->Width/2)-(textSize.Width/2),LineY,m_introScreenGameText[4]);
	LineY += 25;
	m_playing_board->Canvas->TextOutW((m_playing_board->Width/2)-(textSize.Width/2),LineY,m_introScreenGameText[5]);

 //---------------------------------------------------------------------------

	LineY += 60;
	for (int i=0;i<BlocksWide;i++)
	{
		drawBlock(m_gameSymbols[m_gameExampleData[i]],USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*i, LineY);
	}
	LineY += Size;
	drawBlock(m_gameExampleData[0]+1,USERINPUT_BORDER,USERINPUT_DARKGRAY, XPos, LineY);
	drawBlock(m_gameExampleData[1]+1,USERINPUT_BORDER,USERINPUT_DARKGRAY, XPos+Size, LineY);
	drawBlock("",USERINPUT_BORDER,USERINPUT_WHITE, XPos+Size*2, LineY);
	drawBlock("",USERINPUT_BORDER,USERINPUT_DARKGRAY, XPos+Size*3, LineY);
	drawBlock("",USERINPUT_BORDER,USERINPUT_DARKGRAY, XPos+Size*4, LineY);
	drawBlock("",USERINPUT_BORDER,USERINPUT_DARKGRAY, XPos+Size*5, LineY);
	drawBlock("",USERINPUT_BORDER,USERINPUT_DARKGRAY, XPos+Size*6, LineY);
	drawBlock("",USERINPUT_BORDER,USERINPUT_DARKGRAY, XPos+Size*7, LineY);

//---------------------------------------------------------------------------
 	m_playing_board->Canvas->Font->Size = 15;
 	m_playing_board->Canvas->Brush->Color = PLAY_BACKGROUND;

	LineY += 70;
	m_playing_board->Canvas->TextOutW((m_playing_board->Width/2)-(textSize.Width/2),LineY,m_introScreenGameText[6]);

	m_NextButton->Left =  (m_playing_board->Width/2)-m_NextButton->Width/2;
	LineY += 40;
	m_NextButton->Top = LineY;
	m_NextButton->Show();
}
//---------------------------------------------------------------------------

void SYMBOLDIGIT_GAME::timeout()
{
	m_timedOut = true;
}
//---------------------------------------------------------------------------

void SYMBOLDIGIT_GAME::flashTimeout()
{
	if (m_FlashCrossDigit > -1)
	{
		drawUserDigit(m_FlashCrossDigit,true,USERINPUT_WHITE,"");
		m_FlashCrossDigit = -1;
	}

	if (m_FlashLineSetUpColumn > -1)
	{
		if (m_FlashLineSetUpColumn < 8)
		{ //on way up.. make them white
			drawGamePadColums(m_FlashLineSetUpColumn, true);
			drawUserDigit(m_FlashLineSetUpColumn, true,USERINPUT_WHITE,"");
		}
		else if (m_FlashLineSetUpColumn < 16)
		{ //on way back, now fill them in
			drawGamePadColums(15-m_FlashLineSetUpColumn, false);

			if (m_FlashLineSetUpColumn == 15)
				drawUserDigit(0, true,USERINPUT_WHITE,"");
			else
				drawUserDigit(15-m_FlashLineSetUpColumn, true,USERINPUT_DARKGRAY,"");
		}
		else
		{ //done
			m_FlashLineSetUpColumn = -1;
			m_levelStart = Now();
			m_digTime=m_levelStart;
			return;
		}
		m_FlashLineSetUpColumn++;
		setFlashTimeOutFunction(true,50);
	}
}
//===========================================================================

void SYMBOLDIGIT_GAME::initalise()
{
	if (m_CurrentRow)
	{
		delete m_CurrentRow;
		m_CurrentRow = NULL;
	}

	while (!m_returnDataRow.empty())
	{
		delete m_returnDataRow.back();
		m_returnDataRow.pop_back();
	}

	Randomize();
	m_uservalueinput = "";
	m_userkeyinput = ""; //clear user input history
	m_practice = true;
	m_timedOut = false;
	m_lastDigit = 0;
	m_lastScore = 0;
	m_starttime = Now();
	NewSaveRow();
}
//---------------------------------------------------------------------------
bool SYMBOLDIGIT_GAME::initGame(const int gameid )
{
	m_attempt = 1;
	if (gameid == -2)
	{
		initalise();
		m_FirstRun = true;
		m_GameID = gameid;
		doIntoTextScreen();
		return true;
	}
	if (gameid == -1)
	{
		int startrow= (int)Floor(Random()*32);
		m_thisrow=startrow;      //index of live code row
		//contents point to symbol array and are codes for 1-8 respectively
		m_thatrow=(m_thisrow+1)%32;      //index of current live test symbol row

		//intro is finished..
		m_returnDataHeader.instrelapsed = MilliSecondsBetween(Now(),m_starttime);
		m_returnDataHeader.startrow = m_thatrow;
	}
	else if (gameid == 0)
	{//practice finished
		m_returnDataHeader.pracelapsed = MilliSecondsBetween(Now(),m_changeStart);
	}
	else
	{
		//thisrow stays constant
		m_thatrow++;
		m_thatrow=(m_thatrow)%32;
		if(m_thatrow==m_thisrow)
		{
			m_thatrow++;
			m_thatrow=(m_thatrow)%32;
		}
	}
	m_NextButton->Hide();
	m_FlashCrossDigit = -1;
	m_CurrentDigitPosition = 0;
	if (gameid == 0)
	{
		m_GameID = gameid;
		setTimeOutFunction(true); //set timeout 120 for the real thing
	}
	if (gameid == 0)
		m_practice = false;

	drawTrail(); //redraw old trail, updating text.
	m_uservalueinput = "";
	m_userkeyinput = ""; //clear user input history

	m_GameID = gameid;

	if (m_practice)
	{
		for(int i =0;i<8;i++)
		{
			m_codearray[i] = m_gameExampleData[i];
			m_symToDig[i] = m_gamePracticeSymbolDigit[i];
			m_inputarray[i] = m_gamePracticeData[i];
		}
	}
	else if (m_GameID >= 0)
	{
		for(int i =0;i<8;i++)
		{
			m_codearray[i] = m_gameData[m_thisrow][i];
			m_symToDig[m_codearray[i]]=i;
			m_inputarray[i] = m_gameData[m_thatrow][i];
		}
	}

	System::TDateTime timeNow = Now();
	if (m_GameID == 0)
		m_liveStartTime = timeNow;
	m_changeStart = timeNow;

	m_FlashLineSetUpColumn=0;
	setFlashTimeOutFunction(true,50);
	return true;
}

void SYMBOLDIGIT_GAME::NextClicked()
{
	initGame(m_GameID+1);
}

//---------------------------------------------------------------------------
bool SYMBOLDIGIT_GAME::handleClick( const int x, const int y )
{
	bool Complete = false;
	if ( ignore_click )	// PREVENT OVER-LAPPING CALLS
		return( false );
	if (m_FlashLineSetUpColumn >= 0)
		return (false);
	if (m_NextButton->Visible)
		return false;

	ignore_click = true;

	if (m_CurrentDigitPosition >= SYMBOLS_WIDE)
	{ //we are complete. Waiting on Next button
		ignore_click = false;
		return( false );
	}

	System::TDateTime timeNow = Now();

	const int Size = getGameSize();
	int XPos = getGamePadX();
	int YPos = getGamePadY()-Size;
	int Selected = -1;

	if ((x >= XPos) && (y >= YPos))
	{
		if ((x < XPos+Size*4) && (y < YPos+Size*2))
		{
			XPos = (x - XPos)/Size;
			YPos = (y - YPos)/Size;

			if (YPos == 0)
				Selected = XPos+1;
			else
				Selected = XPos+1+4;
		}
	}

	if (Selected == -1)
	{
		ignore_click = false;
		return( false );
	}

	int Expected = m_symToDig[m_inputarray[m_CurrentDigitPosition]]+1;

//	digSelects[dig]++;
//	levelSelects[level]++;

	if ((Selected != Expected) && (m_practice))
	{
		m_FlashCrossDigit = m_CurrentDigitPosition;
		drawCrossDigit(m_FlashCrossDigit);
		setFlashTimeOutFunction(true,250);

		m_CurrentRow->trial_number = m_GameID+1; //0 Practice, 1 Live Row 0, 2 Live Row 1 ..etc
		m_CurrentRow->trial_component = m_CurrentDigitPosition+1; //position in row
		m_CurrentRow->attempt = m_attempt;         //attempt

		if (m_GameID < 0)
			m_CurrentRow->trialtype = 'P';
		else
			m_CurrentRow->trialtype = 'L';

		m_CurrentRow->symbol = m_inputarray[m_CurrentDigitPosition];
		m_CurrentRow->expected = Expected;
		m_CurrentRow->selected = Selected;
		m_CurrentRow->elapsed =  MilliSecondsBetween(timeNow,m_digTime);
		m_CurrentRow->valid = (m_timedOut?0:1);

		m_CurrentRow->rsi = 0;
		if ((m_attempt == 1) && (m_CurrentDigitPosition==0))
				m_CurrentRow->rsi = MilliSecondsBetween(m_levelStart,m_changeStart);
		m_CurrentRow->complete = true;

		m_attempt++;
		m_digTime=timeNow;

		NewSaveRow();
	}
	else
	{
		if ((m_GameID>=0) && (!m_timedOut))
		{
			if (Expected == Selected)
				m_lastScore++;
			m_lastDigit++;
		}

		m_FlashCrossDigit = -1;
		drawUserDigit(m_CurrentDigitPosition,false,USERINPUT_DARKGRAY,Selected);
		m_uservalueinput = m_uservalueinput + m_gameSymbols[Selected-1];
		m_userkeyinput = m_userkeyinput + Selected;

		m_CurrentRow->trial_number = m_GameID+1; //0 Practice, 1 Live Row 0, 2 Live Row 1 ..etc
		m_CurrentRow->trial_component = m_CurrentDigitPosition+1; //position in row
		m_CurrentRow->attempt = m_attempt;         //attempt

		if (m_GameID < 0)
			m_CurrentRow->trialtype = 'P';
		else
			m_CurrentRow->trialtype = 'L';

		m_CurrentRow->symbol = m_inputarray[m_CurrentDigitPosition];
		m_CurrentRow->expected = Expected;
		m_CurrentRow->selected = Selected;
		m_CurrentRow->elapsed =  MilliSecondsBetween(timeNow,m_digTime);
		m_CurrentRow->valid = (m_timedOut?0:1);

		m_CurrentRow->rsi = 0;
		if ((m_CurrentDigitPosition==0) && (m_attempt == 1))
			m_CurrentRow->rsi = MilliSecondsBetween(m_levelStart,m_changeStart);
		m_CurrentRow->complete = true;

		m_digTime=timeNow;
		NewSaveRow();

		if (m_CurrentDigitPosition >= SYMBOLS_WIDE-1)
		{ //end of level
			if(m_practice)
			{
				//if practice level, show Next button
				m_NextButton->Left =  (m_playing_board->Width/2)-m_NextButton->Width/2;
				m_NextButton->Top = 570;

				m_NextButton->Show();
				drawTrail(); //redraw so we get the updated text.
			}
			else if(m_timedOut==false)
			{//next button after practice otherwise automatic
				initGame(m_GameID+1);
			}
			else if (m_timedOut)
			{  //last level completed
				//show please wait and date data:
				setTimeOutFunction(false);
				Complete = true;
				m_returnDataHeader.liveelapsed = MilliSecondsBetween(timeNow,m_liveStartTime);
				m_returnDataHeader.totelapsed= MilliSecondsBetween(timeNow,m_starttime);
				m_returnDataHeader.levels= m_GameID+1;
				m_returnDataHeader.digits= m_lastDigit;
				m_returnDataHeader.score = m_lastScore;
				m_returnDataHeader.abandon = false;
				m_returnDataHeader.sectionid = "hm06";
				String startDate = m_starttime.FormatString("yyyy-mm-dd") + "T" + m_starttime.FormatString("hh:mm:ss");
				m_returnDataHeader.dateStarted=startDate; //L"2013-08-30T10:16:30:000";
			}
		}
		else
		{ // more to get, make the next one white to show the active symbol
			m_CurrentDigitPosition++;
			drawUserDigit(m_CurrentDigitPosition,true,USERINPUT_WHITE,"");
		}

		m_attempt = 1;
	}
	if (Complete)
	{
		end_level = clock();
		is_running = false;
		return( true );
	}
	ignore_click = false;
	return( false );
}

//---------------------------------------------------------------------------
bool SYMBOLDIGIT_GAME::isRunning( void )
{
	return( is_running );
}
//---------------------------------------------------------------------------
void SYMBOLDIGIT_GAME::abandon( void )
{
	m_returnDataHeader.abandon = true;
	System::TDateTime timeNow = Now();
	m_returnDataHeader.liveelapsed = MilliSecondsBetween(timeNow,m_liveStartTime);
	m_returnDataHeader.totelapsed= MilliSecondsBetween(timeNow,m_starttime);
	m_returnDataHeader.levels= m_GameID+1;
	m_returnDataHeader.digits= m_lastDigit;
	m_returnDataHeader.score = m_lastScore;
	m_returnDataHeader.sectionid = "hm06";
	String startDate = m_starttime.FormatString("yyyy-mm-dd") + "T" + m_starttime.FormatString("hh:mm:ss");
	m_returnDataHeader.dateStarted=startDate; //L"2013-08-30T10:16:30:000";

	is_running = false;
	abandoned = true;
}
//---------------------------------------------------------------------------
bool SYMBOLDIGIT_GAME::wasAbandoned( void ) const
{
	return( abandoned );
}
//---------------------------------------------------------------------------
void SYMBOLDIGIT_GAME::runStart( void )
{
	is_running = true;
	m_practice = true;
}
//---------------------------------------------------------------------------
bool SYMBOLDIGIT_GAME::run()
{
	ignore_click = true;
	is_running = true;
	abandoned = false;

	runStart();

	ignore_click = false;
	start_level = clock();
	end_level = start_level;
	Application->ProcessMessages();
	return( true );
}
//---------------------------------------------------------------------------
				/* RETURN RUN-TIME IN DECI-SECONDS */
int SYMBOLDIGIT_GAME::getRunTime( void )
{
	return( (int)( ( 10 * ( end_level - start_level ) ) / CLK_TCK ) );
}
//---------------------------------------------------------------------------

const _TCHAR  SYMBOLDIGIT_GAME::m_gameSymbols[8] =  {L'⌠',L'⌡',L'╡',L'╢',L'╣',L'╪',L'╫',L'╬'};
const int  SYMBOLDIGIT_GAME::m_gameExampleData[8] = {2,4,6,1,3,5,7,0};
const int  SYMBOLDIGIT_GAME::m_gamePracticeData[8] = {7,1,5,4,6,0,3,2};
const int SYMBOLDIGIT_GAME::m_gamePracticeSymbolDigit[8] = {7,3,0,4,1,5,2,6};
const int  SYMBOLDIGIT_GAME::m_gameStartData[8] = {2,4,6,1,3,5,7,0};
const int SYMBOLDIGIT_GAME::m_gameData[33][8] =
		{{5,0,3,6,1,2,4,7},
		 {1,3,5,7,4,6,0,2},
		 {4,5,6,7,3,0,2,1},
		 {1,4,6,7,2,3,0,5},
		 {4,5,1,3,7,2,6,0},
		 {3,5,2,0,4,6,1,7},
		 {6,3,7,2,4,5,0,1},
		 {1,4,5,6,0,7,2,3},
		 {3,4,2,5,6,0,7,1},
		 {2,5,3,0,7,1,6,4},
		 {3,6,4,1,0,5,7,2},
		 {5,3,2,0,4,1,7,6},
		 {2,3,4,1,7,5,0,6},
		 {4,3,1,5,2,0,6,7},
		 {3,5,0,1,4,7,2,6},
		 {4,2,1,0,6,7,5,3},
		 {0,2,3,7,1,4,6,5},
		 {7,1,2,4,0,6,5,3},
		 {5,1,7,3,2,4,0,6},
		 {7,6,4,5,0,2,3,1},
		 {2,6,1,4,5,7,0,3},
		 {4,0,6,1,3,7,5,2},
		 {5,6,0,7,3,2,1,4},
		 {6,5,1,3,0,4,7,2},
		 {4,1,7,2,5,6,3,0},
		 {5,2,0,6,7,4,1,3},
		 {3,0,5,6,7,2,4,1},
		 {3,2,5,4,7,0,6,1},
		 {0,6,7,5,1,2,3,4},
		 {4,3,1,0,6,5,2,7},
		 {5,4,6,7,1,0,2,3},
		 {0,4,7,2,3,1,5,6},
		 {2,0,1,5,6,7,4,3}};

const String SYMBOLDIGIT_GAME::m_practiceText[4] = {
{"Symbol Digit Test - Practice"},
{"Please practise with this code grid (do not memorise it as the code will change for the test):"},
{"To select the number for each symbol, click on the number pad appearing on screen below:"},
{"Click \"Next\" to take the test."}
};

const String SYMBOLDIGIT_GAME::m_inGameText[2] = {
{"Symbol Digit Test"},
{"To select the number for each symbol, click on the number pad appearing on screen below:"}
};

const String SYMBOLDIGIT_GAME::m_introScreenGameText[7] = {
{"Symbol Digit Test - Introduction"},
{"This is a code-breaking game. A code is given in a bar near the top of the page linking a symbol to a number. In this"},
{"test you will have to match symbols and numbers."},
{"In the lower bar, place the correct number in the box under each symbol according to the code. Working from left to"},
{"right select the correct number using the number pad on the screen. Please work as quickly and accurately as you can."},
{"You will have two minutes to do as many as you can."},
{"Click \"Next\" to practise before taking the test."}
};

//===========================================================================
#pragma package(smart_init)

