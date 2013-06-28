//---------------------------------------------------------------------------

#include <vcl.h>

#include "AddRackSize1.h"
#include "RackSize1.h"
#include "WizardLogin1.h"
#include "LDbBoxType.h"
#include "LCDbBoxStore.h"
#include "LDbBoxSize.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TRackSize *RackSize;

//---------------------------------------------------------------------------

__fastcall TRackSize::TRackSize(TComponent* Owner)
 : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TRackSize::FormActivate(TObject *Sender)
{
	Caption = WizardLogin -> setCaption( "Rack Sizes" );

	RackSizeGrid->ColCount = RT_NUM_COLS;
	RackSizeGrid->RowCount = 2;
	RackSizeGrid->Cells[RT_CID][0] = "ID";
	RackSizeGrid->ColWidths[RT_CID] = 0;
	RackSizeGrid->Cells[RT_DESCRIPTION][0] = "Rack description";
	RackSizeGrid->ColWidths[RT_DESCRIPTION] = 180;
	RackSizeGrid->Cells[RT_CAPACITY][0] = "Capacity";
	RackSizeGrid->ColWidths[RT_CAPACITY] = 55;
	RackSizeGrid->Cells[RT_BOX_SIZE][0] = "Box Size";
	RackSizeGrid->ColWidths[RT_BOX_SIZE] = 108;
	RackSizeGrid->Cells[RT_FIRST_RACK][0] = "First Rack";
	RackSizeGrid->ColWidths[RT_FIRST_RACK] = 55;
	RackSizeGrid->Cells[RT_CURRENT][0] = "Status";
	RackSizeGrid->ColWidths[RT_CURRENT] = 55;

	CopyFromClassToGrid();
}
//---------------------------------------------------------------------------

void __fastcall TRackSize::AddClick(TObject *Sender)
{
	AddRackSize->setRackSize( 0 );
	AddRackSize->ShowModal();
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TRackSize::ChangeClick(TObject *Sender)
{
	bool rowFound = false;
	for( std::set< int >::const_iterator si = selected.begin(); si != selected.end(); ++ si )
	{
		int bsid = RackSizeGrid->Cells[RT_CID][ *si ].ToIntDef( 0 );
		if( bsid != 0 )
		{
			rowFound = true;
			AddRackSize->setRackSize( bsid );
			AddRackSize->ShowModal();
		}
	}

	if( !rowFound )
		Application->MessageBox( "No row changed", NULL, MB_OK);

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void TRackSize::CopyFromClassToGrid()
{
	selected.clear();
	int row = 1;
	for( LCDbRackSizes::Range rs = LCDbRackSizes::records(); rs.isValid(); ++ rs )
	{
		RackSizeGrid->Cells[RT_DESCRIPTION][row] = rs -> getName();
		RackSizeGrid->Cells[RT_CID][row] = rs -> getID();
		RackSizeGrid->Cells[RT_CAPACITY][row] = rs -> getSize();
		const LCDbBoxSize & type = LCDbBoxSizes::records().get( rs -> getBoxSizeID() );
		RackSizeGrid->Cells[RT_BOX_SIZE][row] = type.getName();
		RackSizeGrid->Cells[RT_FIRST_RACK][row] = rs -> getFirstRack();
		RackSizeGrid->Cells[RT_CURRENT][row] = rs -> isActive() ? "Current" : "Obsolete";
		row++;
	}

	RackSizeGrid->RowCount = row > 1 ? row : 2;
	RackSizeGrid->FixedRows = 1;
}

//---------------------------------------------------------------------------

void __fastcall TRackSize::btnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TRackSize::RackSizeGridMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	RackSizeGrid->MouseToCell( X, Y, Col, Row );

	if( selected.count( Row ) == 0 )
		selected.insert( Row );
	else
		selected.erase( Row );

	RackSizeGrid -> Repaint();
}

//---------------------------------------------------------------------------


void __fastcall TRackSize::RackSizeGridDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
	if( !State.Contains( gdFixed ) )
	{
		TCanvas * canvas = RackSizeGrid -> Canvas;
		if( selected.count( ARow ) == 0 )
			canvas -> Brush -> Color = clWhite;
		else
			canvas -> Brush -> Color = clLime;
		canvas -> FillRect( Rect );

		int mid = (Rect.Bottom + Rect.Top - abs( canvas -> Font -> Height )) / 2 - 1;
		canvas -> Font -> Color = clBlack;
		canvas -> TextOut( Rect.Left + 2, mid, RackSizeGrid -> Cells[ ACol ][ ARow ] );
	}

}
//---------------------------------------------------------------------------

