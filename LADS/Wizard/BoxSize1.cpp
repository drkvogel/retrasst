//---------------------------------------------------------------------------

#include <vcl.h>

#include "AddBoxSize1.h"
#include "BoxSize1.h"
#include "WizardLogin1.h"
#include "LDbBoxType.h"
#include "LDbBoxSize.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"

TBoxSize *BoxSize;

//---------------------------------------------------------------------------

__fastcall TBoxSize::TBoxSize(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------
void __fastcall TBoxSize::FormActivate(TObject *Sender)
{
	Caption = WizardLogin -> setCaption( "Box Sizes" );

	BoxSizeGrid->ColCount = BS_NUM_COLS;
	BoxSizeGrid->RowCount = 2;
	BoxSizeGrid->Cells[BS_SIZE_ID][0] = "Box ID";
	BoxSizeGrid->ColWidths[BS_SIZE_ID] = 0;
	BoxSizeGrid->Cells[BS_DESCRIPTION][0] = "Box description";
	BoxSizeGrid->ColWidths[BS_DESCRIPTION] = 170;
	BoxSizeGrid->Cells[BS_CAPACITY][0] = "Capacity";
	BoxSizeGrid->ColWidths[BS_CAPACITY] = 65;
	BoxSizeGrid->Cells[BS_EMPTY_POS][0] = "Empty position";
	BoxSizeGrid->ColWidths[BS_EMPTY_POS] = 76;
	BoxSizeGrid->Cells[BS_TUBE_TYPE][0] = "Tube type";
	BoxSizeGrid->ColWidths[BS_TUBE_TYPE] = 65;
	BoxSizeGrid->Cells[BS_VALIDFROM][0] = "Valid from";
	BoxSizeGrid->ColWidths[BS_VALIDFROM] = 76;
	BoxSizeGrid->Cells[BS_VALIDTO][0] = "Valid to";
	BoxSizeGrid->ColWidths[BS_VALIDTO] = 76;
	BoxSizeGrid->Cells[BS_STATUS][0] = "Status";
	BoxSizeGrid->ColWidths[BS_STATUS] = 65;

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TBoxSize::AddClick(TObject *Sender)
{
	AddBoxSize->setBoxSize( 0 );
	AddBoxSize->ShowModal();
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TBoxSize::ChangeClick(TObject *Sender)
{
	bool rowFound = false;
	for( std::set< int >::const_iterator si = selected.begin(); si != selected.end(); ++ si )
	{
		int bsid = BoxSizeGrid -> Cells[ BS_SIZE_ID ][ *si ].ToIntDef( 0 );
		if( bsid != 0 )
		{
			rowFound = true;
			AddBoxSize->setBoxSize( bsid );
			AddBoxSize->ShowModal();
		}
	}

	if( !rowFound )
		Application->MessageBox( "No row changed", NULL, MB_OK);

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void TBoxSize::CopyFromClassToGrid()
{
	selected.clear();
	int row = 1;
	for( LCDbBoxSizes::Range box = LCDbBoxSizes::records(); box.isValid(); ++ box )
	{
		BoxSizeGrid->Cells[BS_DESCRIPTION][row] = box -> getName();
		BoxSizeGrid->Cells[BS_SIZE_ID][row] = box -> getID();
		BoxSizeGrid->Cells[BS_CAPACITY][row] = box -> getSize();
		BoxSizeGrid->Cells[BS_EMPTY_POS][row] = box -> getHole();
		BoxSizeGrid->Cells[BS_TUBE_TYPE][row] = (box -> getTube() == LCDbBoxSize::CRYOVIAL) ? "Cryovial" : "Vacutainer";
		std::pair< TDate, TDate > dates = box -> getValidDates();
		BoxSizeGrid->Cells[BS_VALIDFROM][row] = dates.first;
		BoxSizeGrid->Cells[BS_VALIDTO][row] = dates.second;
		BoxSizeGrid->Cells[BS_STATUS][row] = box -> isActive() ? "Current" : "Inactive";
		row++;
	}

	BoxSizeGrid->RowCount = row > 1 ? row : 2;
	BoxSizeGrid->FixedRows = 1;
}

//---------------------------------------------------------------------------

void __fastcall TBoxSize::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TBoxSize::BoxSizeGridMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	BoxSizeGrid->MouseToCell( X, Y, Col, Row );

	if( selected.count( Row ) == 0 )
		selected.insert( Row );
	else
		selected.erase( Row );

	BoxSizeGrid -> Repaint();
}

//---------------------------------------------------------------------------

void __fastcall TBoxSize::BoxSizeGridDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
	if( !State.Contains( gdFixed ) )
	{
		TCanvas * canvas = BoxSizeGrid -> Canvas;
		if( selected.count( ARow ) == 0 )
			canvas -> Brush -> Color = clWhite;
		else
			canvas -> Brush -> Color = clLime;
		canvas -> FillRect( Rect );

		int mid = (Rect.Bottom + Rect.Top - abs( canvas -> Font -> Height )) / 2 - 1;
		canvas -> Font -> Color = clBlack;
		canvas -> TextOut( Rect.Left + 2, mid, BoxSizeGrid -> Cells[ ACol ][ ARow ] );
	}
}

//---------------------------------------------------------------------------

