//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BoxContent1.h"
#include "LCDbProject.h"
#include "AddBoxContent1.h"
#include "LDbBoxType.h"
#include "Login1.h"
#include "LCDbObject.h"
#include "LDbBoxSize.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TBoxContent *BoxContent;

//---------------------------------------------------------------------------

__fastcall TBoxContent::TBoxContent(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TBoxContent::FormActivate(TObject *Sender)
{
	Caption = Login -> setProjectCaption( "Box Content" );

	BoxContentGrid->RowCount = 2;
	BoxContentGrid->ColCount = BC_NUM_COLS;
	BoxContentGrid->Cells[BC_NAME][0] = "Box";
	BoxContentGrid->ColWidths[BC_NAME] = 112;
	BoxContentGrid->Cells[BC_DESCRIP][0] = "Description";
	BoxContentGrid->ColWidths[BC_DESCRIP] = 240;
	BoxContentGrid->Cells[BC_STATUS][0] = "Status";
	BoxContentGrid->ColWidths[BC_STATUS] = 65;
	BoxContentGrid->Cells[BC_SIZE][0] = "Box size";
	BoxContentGrid->ColWidths[BC_SIZE] = 105;
	BoxContentGrid->Cells[BC_ALIQUOT1][0] = "Aliquot 1";
	BoxContentGrid->ColWidths[BC_ALIQUOT1] = 53;
	BoxContentGrid->Cells[BC_ALIQUOT2][0] = "Aliquot 2";
	BoxContentGrid->ColWidths[BC_ALIQUOT2] = 53;
	BoxContentGrid->Cells[BC_ALIQUOT3][0] = "Aliquot 3";
	BoxContentGrid->ColWidths[BC_ALIQUOT3] = 53;
	BoxContentGrid->Cells[BC_SET_LINK][0] = "Set";
	BoxContentGrid->ColWidths[BC_SET_LINK] = 30;
	BoxContentGrid->Cells[BC_POSITION][0] = "Pos";
	BoxContentGrid->ColWidths[BC_POSITION] = 30;
	BoxContentGrid->Cells[BC_TYPE_ID][0] = "Box Type ID";
	BoxContentGrid->ColWidths[BC_TYPE_ID] = 0;

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TBoxContent::AddClick(TObject *Sender)
{
	AddBoxContent -> setBoxContent( 0 );
	AddBoxContent -> ShowModal();
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TBoxContent::ChangeClick(TObject *Sender)
{
	bool rowFound = false;
	for( std::set< int >::const_iterator si = selected.begin(); si != selected.end(); ++ si )
	{
		int btid = BoxContentGrid -> Cells[ BC_TYPE_ID ][ *si ].ToIntDef( 0 );
		if( btid != 0 )
		{
			rowFound = true;
			AddBoxContent -> setBoxContent( btid );
			AddBoxContent -> ShowModal();
		}
	}

	if( !rowFound )
		Application->MessageBox( "No row changed", NULL, MB_OK );

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void TBoxContent::CopyFromClassToGrid()
{
	static String empty;
	selected.clear();
	int row = 1;
	for( LPDbBoxTypes::Range box = LPDbBoxTypes::records(); box.isValid(); ++ box )
	{
		BoxContentGrid->Cells[BC_NAME][row] = box -> getName();
		BoxContentGrid->Cells[BC_DESCRIP][row] = box -> getDescription();
		BoxContentGrid->Cells[BC_TYPE_ID][row] = box -> getID();
		BoxContentGrid->Cells[BC_SET_LINK][row] = box -> getLink();
		BoxContentGrid->Cells[BC_POSITION][row] = box -> getPosition();

		BoxContentGrid->Cells[BC_STATUS][row] = box -> isActive() ? "Current" : "Deleted";
		const LCDbBoxSize * size = LCDbBoxSizes::records().findByID( box -> getSizeID() );
		if( size != NULL )
			BoxContentGrid->Cells[BC_SIZE][row] = size -> getName();
		else
			BoxContentGrid->Cells[BC_SIZE][row] = "Unknown";

		const LCDbObjects & objs = LCDbObjects::records();
		LPDbBoxType::Aliquots alr = box -> getAliquots();
		BoxContentGrid->Cells[BC_ALIQUOT1][row] = alr.isValid() ? objs.get( *alr ).getName() : empty;
		BoxContentGrid->Cells[BC_ALIQUOT2][row] = (++ alr).isValid() ? objs.get( *alr ).getName() : empty;
		BoxContentGrid->Cells[BC_ALIQUOT3][row] = (++ alr).isValid() ? objs.get( *alr ).getName() : empty;
		row ++;
	}

	BoxContentGrid->RowCount = (row > 1) ? row : 2;
	BoxContentGrid->FixedRows = 1;
}

//---------------------------------------------------------------------------

void __fastcall TBoxContent::BoxContentGridMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	BoxContentGrid->MouseToCell( X, Y, Col, Row );

	if( selected.count( Row ) == 0 )
		selected.insert( Row );
	else
		selected.erase( Row );

	BoxContentGrid -> Repaint();
}

//---------------------------------------------------------------------------

void __fastcall TBoxContent::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TBoxContent::BoxContentGridDrawCell(TObject *Sender,
	  int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
	if( !State.Contains( gdFixed ) )
	{
		TCanvas * canvas = BoxContentGrid -> Canvas;
		if( selected.count( ARow ) == 0 )
			canvas -> Brush -> Color = clWhite;
		else
			canvas -> Brush -> Color = clLime;
		canvas -> FillRect( Rect );

		int mid = (Rect.Bottom + Rect.Top - abs( canvas -> Font -> Height )) / 2 - 1;
		canvas -> Font -> Color = clBlack;
		canvas -> TextOut( Rect.Left + 2, mid, BoxContentGrid -> Cells[ ACol ][ ARow ] );
	}
}

//---------------------------------------------------------------------------

