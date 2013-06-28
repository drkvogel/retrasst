//---------------------------------------------------------------------------
#include <vcl.h>

#include "Operator1.h"
#include "AddOperator1.h"
#include "WizardLogin1.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"

TOperator *Operator;

//---------------------------------------------------------------------------

__fastcall TOperator::TOperator(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TOperator::FormActivate(TObject *Sender)
{
	Caption = WizardLogin -> setCaption( "Operator" );
	OperatorTable->RowCount = 2;

	OperatorTable->ColCount = O_OPERATORCOLS;
	OperatorTable->Cells[O_OPERATOR][0] = "Operator";
	OperatorTable->ColWidths[O_OPERATOR] = 72;
	OperatorTable->Cells[O_OPERATORID][0] = "Operator ID";
	OperatorTable->ColWidths[O_OPERATORID] = 0;
	OperatorTable->Cells[O_DESCRIPTION][0] = "             Full name";
	OperatorTable->ColWidths[O_DESCRIPTION] = 190;
	OperatorTable->Cells[O_VALIDFROM][0] = "Valid from";
	OperatorTable->ColWidths[O_VALIDFROM] = 76;
	OperatorTable->Cells[O_VALIDTO][0] = "Valid to";
	OperatorTable->ColWidths[O_VALIDTO] = 76;
	OperatorTable->Cells[O_STATUS][0] = "Status";
	OperatorTable->ColWidths[O_STATUS] = 62;

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void TOperator::CopyFromClassToGrid()
{
	selected.clear();
	int row = 1;
	for( LCDbOperators::Range ur = LCDbOperators::records(); ur.isValid(); ++ ur, ++ row )
	{
		OperatorTable->Cells[O_OPERATOR][ row ] = ur -> getName();
		OperatorTable->Cells[O_OPERATORID][ row ] = ur -> getID();
		OperatorTable->Cells[O_DESCRIPTION][ row ] = ur -> getDescription();

		std::pair< TDate, TDate > dates = ur -> getValidDates();
		OperatorTable->Cells[O_VALIDFROM][row] = dates.first;
		OperatorTable->Cells[O_VALIDTO][row] =  dates.second;
		OperatorTable->Cells[O_STATUS][row] = ur -> isActive() ? "Current" : "Inactive";
	}
	OperatorTable->RowCount = row;
	OperatorTable->FixedRows = 1;
}

//---------------------------------------------------------------------------

void __fastcall TOperator::AddClick(TObject *Sender)
{
	AddOperator -> setOperator( 0 );
	AddOperator -> ShowModal();
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TOperator::ChangeClick(TObject *Sender)
{
	bool rowFound = false;
	for( std::set< int >::const_iterator si = selected.begin(); si != selected.end(); ++ si )
	{
		int btid = OperatorTable->Cells[ O_OPERATORID ][ *si ].ToIntDef( 0 );
		if( btid != 0 )
		{
			rowFound = true;
			AddOperator -> setOperator( btid );
			AddOperator -> ShowModal();
		}
	}

	if( !rowFound )
		Application->MessageBox( "No row changed", NULL, MB_OK );

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TOperator::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TOperator::OperatorTableMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	OperatorTable->MouseToCell( X, Y, Col, Row );

	if( selected.count( Row ) == 0 )
		selected.insert( Row );
	else
		selected.erase( Row );

	OperatorTable -> Repaint();
}

//---------------------------------------------------------------------------



void __fastcall TOperator::OperatorTableDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
	if( !State.Contains( gdFixed ) )
	{
		TCanvas * canvas = OperatorTable -> Canvas;
		if( selected.count( ARow ) == 0 )
			canvas -> Brush -> Color = clWhite;
		else
			canvas -> Brush -> Color = clLime;
		canvas -> FillRect( Rect );

		int mid = (Rect.Bottom + Rect.Top - abs( canvas -> Font -> Height )) / 2 - 1;
		canvas -> Font -> Color = clBlack;
		canvas -> TextOut( Rect.Left + 2, mid, OperatorTable -> Cells[ ACol ][ ARow ] );
	}

}
//---------------------------------------------------------------------------

