//---------------------------------------------------------------------------
#include <vcl.h>

#include "Profile1.h"
#include "CentralTest1.h"
#include "EditProjectDb1.h"
#include "ProfileReport1.h"
#include "Machine1.h"
#include "ObjectName1.h"
#include "AddProfile1.h"
#include "LCDbProject.h"
#include "Login1.h"
#include "LQuery.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TProfile *Profile;

//---------------------------------------------------------------------------

__fastcall TProfile::TProfile(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TProfile::FormActivate(TObject *Sender)
{
	Caption = Login -> setProjectCaption( "Test Profiles" );

	ProfileTable->RowCount = 2;
	ProfileTable->ColCount = TP_NUMCOLS;
	ProfileTable->Cells[TP_PROFILEID][0] = "Profile ID";
	ProfileTable->ColWidths[TP_PROFILEID] = 0;
	ProfileTable->Cells[TP_PROFILE][0] = "Profile";
	ProfileTable->ColWidths[TP_PROFILE] = 156;
	ProfileTable->Cells[TP_DESCRIPTION][0] = "Description";
	ProfileTable->ColWidths[TP_DESCRIPTION] = 332;
	ProfileTable->Cells[TP_DATELASTMOD][0] = " Modified";
	ProfileTable->ColWidths[TP_DATELASTMOD] = 110;
	ProfileTable->Cells[TP_CURRENT][0] = "Current?";
	ProfileTable->ColWidths[TP_CURRENT] = 45;
	ProfileTable->Cells[TP_TEST][0] = "Test?";
	ProfileTable->ColWidths[TP_TEST] = 35;
	ProfileTable->Cells[TP_ALIQUOT][0] = "Aliquot?";
	ProfileTable->ColWidths[TP_ALIQUOT] = 43;
	ProfileTable->Cells[TP_STORE][0] = "Store?";
	ProfileTable->ColWidths[TP_STORE] = 37;
	CopyFromClassToGrid();
}
//---------------------------------------------------------------------------

void __fastcall TProfile::AddClick(TObject *Sender)
{
	AddProfile->Caption = "Add Test Profile";
	AddProfile->setProfile( 0 );
	AddProfile->ShowModal();
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TProfile::ChangeClick(TObject *Sender)
{
	bool rowFound = false;
	for( std::set< int >::const_iterator si = selected.begin(); si != selected.end(); ++ si )
	{
		int btid = ProfileTable->Cells[TP_PROFILEID][ *si ].ToIntDef( 0 );
		if( btid != 0 )
		{
			AddProfile->Caption = "Change Test Profile";
			rowFound = true;
			AddProfile->setProfile( btid );
			AddProfile -> ShowModal();
		}
	}

	if( rowFound )
		CopyFromClassToGrid();
	else
		Application->MessageBox( "No row changed", NULL, MB_OK );
}

//---------------------------------------------------------------------------

void __fastcall TProfile::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void TProfile::CopyFromClassToGrid()
{
	selected.clear();
	int row = 1;
	for( LPDbProfiles::Range pr = LPDbProfiles::records(); pr.isValid(); ++ pr )
	{
		ProfileTable->Cells[TP_PROFILEID][row] = pr -> getID();
		ProfileTable->Cells[TP_PROFILE][row] = pr -> getName();
		ProfileTable->Cells[TP_DESCRIPTION][row] = pr -> getDescription();
		ProfileTable->Cells[TP_DATELASTMOD][row] = pr -> getTimeStamp();
		ProfileTable->Cells[TP_CURRENT][row] = pr -> isActive() ? "Yes" : "No";
		ProfileTable->Cells[TP_TEST][row] = pr -> isTested() ? "Yes" : "No";
		ProfileTable->Cells[TP_STORE][row] = pr -> isStored() ? "Yes" : "No";
		ProfileTable->Cells[TP_ALIQUOT][row] = pr -> isAliquoted() ? "Yes" : "No";
		row ++;
	}

	ProfileTable -> RowCount = row > 1 ? row : 2;
	ProfileTable->FixedRows = 1;
}

//---------------------------------------------------------------------------

void __fastcall TProfile::ProfileTableMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	ProfileTable->MouseToCell( X, Y, Col, Row );

	if( selected.count( Row ) == 0 )
		selected.insert( Row );
	else
		selected.erase( Row );

	ProfileTable -> Repaint();
}

//---------------------------------------------------------------------------

void __fastcall TProfile::btnReportClick(TObject *Sender)
{
	LQuery( ProfileReport->ddbQuery ).open();
	ProfileReport->QuickRep1->ReportTitle = Login -> setProjectCaption( "Test Report" );
	ProfileReport->QuickRep1->PreviewModal();
	ProfileReport->ddbQuery->Active = false;
}

//---------------------------------------------------------------------------


void __fastcall TProfile::ProfileTableDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
	if( !State.Contains( gdFixed ) )
	{
		TCanvas * canvas = ProfileTable -> Canvas;
		if( selected.count( ARow ) == 0 )
			canvas -> Brush -> Color = clWhite;
		else
			canvas -> Brush -> Color = clLime;
		canvas -> FillRect( Rect );

		int mid = (Rect.Bottom + Rect.Top - abs( canvas -> Font -> Height )) / 2 - 1;
		canvas -> Font -> Color = clBlack;
		canvas -> TextOut( Rect.Left + 2, mid, ProfileTable -> Cells[ ACol ][ ARow ] );
	}
}
//---------------------------------------------------------------------------

