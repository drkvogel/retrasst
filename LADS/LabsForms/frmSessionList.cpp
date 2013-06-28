/*---------------------------------------------------------------------------
 *
 *      16 April 09, NG:	Guess session end for single audit trail entry
 *      26 May 2009, NG:	Move into shared library; provide callback
 *
 *--------------------------------------------------------------------------*/

#include <vcl.h>
#pragma hdrstop

#include "frmSessionList.h"
#include "LCDbProject.h"
#include "LCDbOperator.h"
#include "LCDbAnalyser.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmSessionHistory *frmSessionHistory;
TForm * TfrmSessionHistory::details;

//---------------------------------------------------------------------------

__fastcall TfrmSessionHistory::TfrmSessionHistory(TComponent* Owner)
 : TForm(Owner)
{
	mcStartDate -> MaxDate = Now();
	mcStartDate -> Date = TDate::CurrentDate() - 1;
	mcEndDate -> MaxDate = TDate::CurrentDate() + 1;
	mcEndDate -> Date = Now();

	grid -> ColCount = NUM_COLS;
	grid -> Cells[ PROCESS ][ 0 ] = "Session";
	grid -> Cells[ OPERATOR ][ 0 ] = "Operator";
	grid -> Cells[ MACHINE ][ 0 ] = "Machine";
	grid -> Cells[ START ][ 0 ] = "Start";
	grid -> Cells[ FINISH ][ 0 ] = "Finish";
}

//---------------------------------------------------------------------------

void __fastcall TfrmSessionHistory::FormActivate(TObject *Sender)
{
	listSessions();
	details -> Hide();
}

//---------------------------------------------------------------------------

void __fastcall TfrmSessionHistory::mcStartDateEnter(TObject *Sender)
{
	mcStartDate -> MaxDate = mcEndDate -> Date;
}

//---------------------------------------------------------------------------

void __fastcall TfrmSessionHistory::mcEndDateEnter(TObject *Sender)
{
	mcEndDate -> MinDate = mcStartDate -> Date;
}

//---------------------------------------------------------------------------

void __fastcall TfrmSessionHistory::mcDateClick(TObject *Sender)
{
	listSessions();
}

//---------------------------------------------------------------------------

void __fastcall TfrmSessionHistory::FormResize(TObject *Sender)
{
	grid -> ColWidths[ PROCESS ] = 58;
	grid -> ColWidths[ START ] = 88;
	grid -> ColWidths[ FINISH ] = 45;
	int remainder = grid -> Width - 216;
	grid -> ColWidths[ OPERATOR ] = remainder * 0.4;
	grid -> ColWidths[ MACHINE ] = remainder * 0.6;
}

//---------------------------------------------------------------------------

void __fastcall TfrmSessionHistory::gridClick(TObject *Sender)
{
	details -> Show();
}

//---------------------------------------------------------------------------

void __fastcall TfrmSessionHistory::FormHide(TObject *Sender)
{
	details -> Hide();
}

//---------------------------------------------------------------------------

std::pair< TDateTime, TDateTime > TfrmSessionHistory::getSelected()
{
	std::pair< TDateTime, TDateTime > range( Now(), 0 );
	TGridRect selected = grid -> Selection;
	for( int row = selected.Top; row <= selected.Bottom; ++ row )
	{
		if( row > 0 && row <= sessions.size() )
		{
			const LCDbAuditTrail::Process & proc = sessions[ row - 1 ];
			if( range.first > proc.start )
				range.first = proc.start;
			if( range.second < proc.finish )
				range.second = proc.finish;
		}
	}
	return range;
}
     
//---------------------------------------------------------------------------

void TfrmSessionHistory::listSessions()
{
	Screen -> Cursor = crSQLWait;
	TDateTime start = mcStartDate -> Date, finish = mcEndDate -> Date + 1;
	int projID = LCDbProjects::getCurrentID();
	sessions = LCDbAuditTrail::getCurrent().read( start, finish, projID );
	TDateTime followOn;
	int row = 1;
	for( ProcessList::iterator si = sessions.begin(); si != sessions.end(); ++ si, ++ row )
	{
		// The current session has not finished yet; include recent records.
		if( row == 1 )
			si -> finish = Now();

		// If there's only one audit trail record, assume it's for the login.
		// The session would have finished by 9pm and/or before the next one.
		else if( si -> finish == si -> start )
		{
			unsigned short year, month, day;
			si -> start.DecodeDate( &year, &month, &day )   ;
			TDateTime cutOff( year, month, day, 21, 0, 0, 0 );
			si -> finish = followOn > cutOff ? cutOff : followOn;
		}
		followOn = si -> start;

		grid -> Cells[ PROCESS ][ row ] = si -> processID;
		grid -> Cells[ START ][ row ] = si -> start.FormatString( "dd/mm/yy hh:nn" );
		grid -> Cells[ FINISH ][ row ] = si -> finish.FormatString( "hh:nn" );

		const LCDbOperator * user = LCDbOperators::records().findByID( si -> operatorID );
		if( user == NULL )
			grid -> Cells[ OPERATOR ][ row ] = "";
		else
			grid -> Cells[ OPERATOR ][ row ] = user -> getDescription();

		const LCDbAnalyser * buddy = LCDbAnalysers::records().findByID( si -> buddyID );
		if( buddy == NULL )
			grid -> Cells[ MACHINE ][ row ] = "";
		else
			grid -> Cells[ MACHINE ][ row ] = buddy -> getDescription();
	}
	setCount( row );
	Screen -> Cursor = crDefault;
}

//---------------------------------------------------------------------------

void TfrmSessionHistory::setCount( int rows )
{
	if( rows < 2 )
	{
		grid -> RowCount = 2;
		for( int i = 0; i < grid -> ColCount; i ++ )
			grid -> Cells[ i ][ 1 ] = "";
	}
	else grid -> RowCount = rows;
}

//---------------------------------------------------------------------------

