//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "tTrailBoard.h"
#include "tmessage.h"
#include "mess.h"
#include "rand.h"
#include <sstream>

#include "System.DateUtils.hpp"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTrailBoard *TrailBoard;
ROSETTA	*TTrailBoard::results = NULL;
#define	POPUP_COLOUR			(clMaroon)
//---------------------------------------------------------------------------
__fastcall TTrailBoard::TTrailBoard(TComponent* Owner)
	: TForm(Owner)
{
}

std::string TTrailBoard::toSTD(String in)
{
	return std::string(AnsiString(in).c_str());
}

//---------------------------------------------------------------------------
void TTrailBoard::extractResults( )
{
	String sectionAppend = "_" + game->m_returnDataHeader.sectionid;
	results = new ROSETTA();

	ROSETTA *g = new ROSETTA();
	g->setString(std::string("partid"),toSTD(game->m_returnDataHeader.partid));
	g->setString("sessionid",toSTD(game->m_returnDataHeader.sessionid));
	g->setString("sectionid",toSTD(game->m_returnDataHeader.sectionid));

	System::TDateTime DT = Now();
	String completeDate = DT.FormatString("yyyy-mm-dd") + "T" + DT.FormatString("hh:mm:ss");
	g->setString("dateCompleted",std::string(toSTD(completeDate)));
	results->attachSubRosetta("header",g);

	g = new ROSETTA();
	g->setString(toSTD(L"dateStarted"+sectionAppend),toSTD(game->m_returnDataHeader.dateStarted));
	g->setInt(toSTD(L"abandon"+sectionAppend),game->m_returnDataHeader.abandon);
	g->setInt(toSTD(L"introelapsed"+sectionAppend),game->m_returnDataHeader.introelapsed);
	g->setInt(toSTD(L"totelapsed"+sectionAppend),game->m_returnDataHeader.totelapsed);
	g->setInt(toSTD(L"practiceaelapsed"+sectionAppend),game->m_returnDataHeader.practiceaelapsed);
	g->setInt(toSTD(L"practiceaerrors"+sectionAppend),game->m_returnDataHeader.practiceaerrors);
	g->setInt(toSTD(L"practicebelapsed"+sectionAppend),game->m_returnDataHeader.practicebelapsed);
	g->setInt(toSTD(L"practiceberrors"+sectionAppend),game->m_returnDataHeader.practiceberrors);
	g->setInt(toSTD(L"partaelapsed"+sectionAppend),game->m_returnDataHeader.partaelapsed);
	g->setInt(toSTD(L"partaerrors"+sectionAppend),game->m_returnDataHeader.partaerrors);
	g->setInt(toSTD(L"partbelapsed"+sectionAppend),game->m_returnDataHeader.partbelapsed);
	g->setInt(toSTD(L"partberrors"+sectionAppend),game->m_returnDataHeader.partberrors);

	for (unsigned int i=0;i<game->m_returnDataRow.size();i++)
	{
		TRAIL_GAME::returnDataRow *m_CurrentRow = game->m_returnDataRow[i];

		String trail_number = System::Sysutils::IntToStr(m_CurrentRow->trial_number);
		String trial_component = System::Sysutils::IntToStr(m_CurrentRow->trial_component+1); //components start at 1
		String attempt = System::Sysutils::IntToStr(m_CurrentRow->attempt+1); //attempts start from 1
		String Combined = trail_number+"_"+trial_component+"_"+attempt+sectionAppend;

		g->setInt(toSTD("elapsed_"+Combined),m_CurrentRow->elapsed);  //long
		g->setString(toSTD("expected_"+Combined),toSTD(m_CurrentRow->expected)); //string
		g->setString(toSTD("selected_"+Combined),toSTD(m_CurrentRow->selected));    //string
		g->setInt(toSTD("match_"+Combined),m_CurrentRow->match);      //int
		g->setString(toSTD("part_"+Combined),toSTD(m_CurrentRow->part));        //CHAR
		g->setString(toSTD("trialtype_"+Combined),toSTD(m_CurrentRow->trialtype));
	}
	results->attachSubRosetta("qidresp",g);
}
//---------------------------------------------------------------------------


static void flashBGFunction(  )
{
	TrailBoard->doFlashBackGround();
}

static void resetBGFunction(  )
{
	TrailBoard->resetBoard();
}

//---------------------------------------------------------------------------
void __fastcall TTrailBoard::image_playMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	game->handleClick( X, Y );
}

//---------------------------------------------------------------------------
void __fastcall TTrailBoard::FormCreate(TObject *Sender)
{
	int	i;
	abandon = false;
	is_valid = true;
	String	image;
	try
	{
		game = new TRAIL_GAME( image_play, PLAY_BACKGROUND,flashBGFunction,resetBGFunction,NextButton );
//load in the graphics ready
		game->initaliseTrail();
	}
	catch ( Exception &e )
	{
		MESS::error( "Failed to initialise cards", MB_OK );
		is_valid = false;
	}
	Top = 0;
	Left = 0;
	Height = Screen->Height;
	Width = Screen->Width;
	margin = 10;
	in_game = false;
	time_t	now;
	time( &now );
	RANDOM::randSetSeed( (long) now );
}
//---------------------------------------------------------------------------

void TTrailBoard::resetBoard( void )
{
	int	h = 57;				// HEIGHT OF CONTROL PANELS
	int	board_width = Width - ( 2 * margin );
							// MAIN PANEL
	panel_play->Left = margin;
	panel_play->Top  = margin;
	panel_play->Width  = board_width;
	panel_play->Height = Height - ( 3 * margin ) - h;
	panel_play->Color = PLAY_BACKGROUND;
							// IMAGE PLAYING AREA
	image_play->Top = 4;
	image_play->Left = 4;
	image_play->Height = panel_play->Height - 12;
	image_play->Width = panel_play->Width - 12;
	TrailBoard->Color = clBlack;
	image_play->Canvas->Brush->Color = PLAY_BACKGROUND;
	image_play->Canvas->FillRect( image_play->ClientRect );
							// ABANDON BUTTON
	panel_abandon->Visible = true;
	panel_abandon->Color = this->Color;
	panel_abandon->Height = 41;
	panel_abandon->Top = Height - panel_abandon->Height - margin;
	panel_abandon->Left = margin;
}
//---------------------------------------------------------------------------

bool TTrailBoard::playGame( TForm *caller, int gameid )
{
	m_startTime = Now();

	game->m_returnDataHeader.partid= L"0";
	game->m_returnDataHeader.sessionid= L"0";
	game->m_returnDataHeader.sectionid= L"hm05";
	String startDate = m_startTime.FormatString("yyyy-mm-dd") + "T" + m_startTime.FormatString("hh:mm:ss");
	game->m_returnDataHeader.dateStarted=startDate; //L"2013-08-30T10:16:30:000";
	game->m_returnDataHeader.abandon= 0;
	game->m_returnDataHeader.introelapsed=0; //Intro elasped time
	game->m_returnDataHeader.totelapsed=0;
	game->m_returnDataHeader.practiceaelapsed=0;
	game->m_returnDataHeader.practiceaerrors=0;
	game->m_returnDataHeader.practicebelapsed=0;
	game->m_returnDataHeader.practiceberrors=0;
	game->m_returnDataHeader.partaelapsed=0;
	game->m_returnDataHeader.partaerrors=0;
	game->m_returnDataHeader.partbelapsed=0;
	game->m_returnDataHeader.partberrors=0;

	resetBoard();

	game->initGame();
//		MESS::bug( "Error in playGame random allocation" );

	Show();
	panel_abandon->Visible = true;

	caller->Hide();
	if (results)
		delete results;

	game->run( );
	while ( game->isRunning() )	// WAIT FOR END SIGNAL
	{
		Application->ProcessMessages();
	}
	panel_abandon->Visible = false;
	game->m_returnDataHeader.totelapsed = MilliSecondsBetween(Now(),m_startTime);  //milliseconds elapsed since 1 January 1970

	std::stringstream timeString;
	timeString << game->getRunTime();
  /*
	if (!game->wasAbandoned())
		fm_tmessage->msgOk("Congratulations!\n\nYou completed the test in " + timeString.str() + " deci-seconds " + "\n\nWell done.","WHOOOT!", POPUP_COLOUR );
    */
	extractResults();

	std::string resultString = results->serializeOut();

	if (SaveTextFileDialog->Execute())
	{
		int iFileHandle = FileCreate(SaveTextFileDialog->FileName);

		if (iFileHandle > 0)
		{
			FileWrite(iFileHandle,(void*)AnsiString(resultString.c_str()).c_str(),resultString.length());
		}
		FileClose(iFileHandle);
	}

	caller->Show();
	Hide();
	return( true );
}
//---------------------------------------------------------------------------

void __fastcall TTrailBoard::FormDestroy(TObject *Sender)
{
	delete game;
	if (results)
	{
		delete results;
		results = NULL;
	}
}
//---------------------------------------------------------------------------

void __fastcall TTrailBoard::panel_abandonClick(TObject *Sender)
{
	abandonGame();
}
//---------------------------------------------------------------------------

void TTrailBoard::abandonGame( void )
{
	if ( mrYes != fm_tmessage->msgYesNo(
		"You have not yet completed the task."
		"\n\nAre you certain you wish to abandon this game?"
		"\n\nYou will not be able to attempt it again.",
			"Please check!", POPUP_COLOUR ) )
	{
		return;
	}
	game->m_returnDataHeader.abandon= 1;
	game->abandon();
}
//---------------------------------------------------------------------------

void TTrailBoard::doFlashBackGround()
{
	bgFlashTimer->Enabled = false;
	image_play->Canvas->Brush->Color = PLAY_BACKGROUND_ERROR;
	image_play->Canvas->FillRect( game->getGameRec() );
	game->drawTrail(true);

	bgFlashTimer->Interval = 250;
	bgFlashTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TTrailBoard::bgFlashTimerTimer(TObject *Sender)
{
	bgFlashTimer->Enabled = false;
	image_play->Canvas->Brush->Color = PLAY_BACKGROUND;
	image_play->Canvas->FillRect( game->getGameRec() );
	game->drawTrail(true);
}
//---------------------------------------------------------------------------

void __fastcall TTrailBoard::NextButtonClick(TObject *Sender)
{
	game->NextClicked();
}
//---------------------------------------------------------------------------

