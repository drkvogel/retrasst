//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CentralTest1.h"
#include "AddCentralTest1.h"
#include "Profile1.h"
#include "WizardLogin1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"

TCentralTest *CentralTest;

//---------------------------------------------------------------------------

__fastcall TCentralTest::TCentralTest(TComponent* Owner) : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TCentralTest::FormActivate(TObject *Sender)
{
	Caption = WizardLogin -> setCaption( "Test" );
	TestTable -> RowCount = 2;

	TestTable->Cells[T_TESTID][0] = "ID";
	TestTable->ColWidths[T_TESTID] = 48;
	TestTable->ColCount = T_TESTCOLS;
	TestTable->Cells[T_TEST][0] = "Test";
	TestTable->ColWidths[T_TEST] = 54;
	TestTable->Cells[T_DESCRIPTION][0] = "Description";
	TestTable->ColWidths[T_DESCRIPTION] = 205;
	TestTable->Cells[T_VALIDFROM][0] = "Valid from";
	TestTable->ColWidths[T_VALIDFROM] = 66;
	TestTable->Cells[T_VALIDTO][0] = "Valid to";
	TestTable->ColWidths[T_VALIDTO] = 66;
	TestTable->Cells[T_STATUS][0] = "Status";
	TestTable->ColWidths[T_STATUS] = 46;
	TestTable->Cells[T_SOURCE][0] = "Source";
	TestTable->ColWidths[T_SOURCE] = 58;
	TestTable->Cells[T_DATATYPE][0] = "Data type";
	TestTable->ColWidths[T_DATATYPE] = 36;
	TestTable->Cells[T_MINDATAPOINTS][0] = "Min DP";
	TestTable->ColWidths[T_MINDATAPOINTS] = 31;
	TestTable->Cells[T_MAXDATAPOINTS][0] = "Max DP";
	TestTable->ColWidths[T_MAXDATAPOINTS] = 31;
	TestTable->Cells[T_SAMPLETYPE][0] = "Type";
	TestTable->ColWidths[T_SAMPLETYPE] = 32;
	TestTable->Cells[T_DEFAULTLOWERLIMIT][0] = "Low Lim";
	TestTable->ColWidths[T_DEFAULTLOWERLIMIT] = 40;
	TestTable->Cells[T_DEFAULTUPPERLIMIT][0] = "Upr Lim";
	TestTable->ColWidths[T_DEFAULTUPPERLIMIT] = 40;

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void TCentralTest::CopyFromClassToGrid()
{
	selected.clear();
	int row = 1;
	for( LCDbTests::Range tr = LCDbTests::records(); tr.isValid(); ++ tr )
	{
		TestTable->Cells[T_TEST][ row ] = tr -> getName();
		TestTable->Cells[T_TESTID][ row ] = tr -> getID();
		TestTable->Cells[T_DESCRIPTION][ row ] = tr -> getDescription();
		std::pair< TDate, TDate > dates = tr -> getValidDates();
		TestTable->Cells[T_VALIDFROM][row] = dates.first;
		TestTable->Cells[T_VALIDTO][row] = dates.second;
		std::pair< double, double > limits = tr -> getLimits();
		TestTable->Cells[T_DEFAULTLOWERLIMIT][row] = limits.first;
		TestTable->Cells[T_DEFAULTUPPERLIMIT][row] = limits.second;
		TestTable->Cells[T_DATATYPE][row] = "Float";
		std::pair< short, short > points = tr -> getPointCount();
		TestTable->Cells[T_MINDATAPOINTS][row] = points.first;
		TestTable->Cells[T_MAXDATAPOINTS][row] = points.second;
		TestTable->Cells[T_SAMPLETYPE][row] = tr -> getSampleType();
		TestTable->Cells[T_STATUS][row] = tr -> isActive() ? "Current" : "Inactive";
		TestTable->Cells[T_SOURCE][row] = tr -> getCalcParts().isValid() ? "Calculated"
										: tr -> getCodes().isValid() ? "Analysed" : "";
		row ++;
	}

	TestTable -> RowCount = row;
	TestTable -> FixedRows = 1;
}

//---------------------------------------------------------------------------

void __fastcall TCentralTest::AddClick(TObject *Sender)
{
	AddCentralTest -> setTest( 0 );
	AddCentralTest -> ShowModal();
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------
void __fastcall TCentralTest::ChangeClick(TObject *Sender)
{
	bool rowFound = false;
	for( std::set< int >::const_iterator si = selected.begin(); si != selected.end(); ++ si )
	{
		int tid = TestTable -> Cells[ T_TESTID ][ *si ].ToIntDef( 0 );
		if( tid != 0 )
		{
			rowFound = true;
			AddCentralTest -> setTest( tid );
			AddCentralTest -> ShowModal();
		}
	}

	if( !rowFound )
		Application->MessageBox( "No row changed", NULL, MB_OK );

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TCentralTest::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TCentralTest::TestTableMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	TestTable->MouseToCell( X, Y, Col, Row );

	if( selected.count( Row ) == 0 )
		selected.insert( Row );
	else
		selected.erase( Row );

	TestTable -> Repaint();
}

//---------------------------------------------------------------------------


void __fastcall TCentralTest::TestTableDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
	if( !State.Contains( gdFixed ) )
	{
		TCanvas * canvas = TestTable -> Canvas;
		if( selected.count( ARow ) == 0 )
			canvas -> Brush -> Color = clWhite;
		else
			canvas -> Brush -> Color = clLime;
		canvas -> FillRect( Rect );

		int mid = (Rect.Bottom + Rect.Top - abs( canvas -> Font -> Height )) / 2 - 1;
		canvas -> Font -> Color = clBlack;
		canvas -> TextOut( Rect.Left + 2, mid, TestTable -> Cells[ ACol ][ ARow ] );
	}
}

//---------------------------------------------------------------------------

