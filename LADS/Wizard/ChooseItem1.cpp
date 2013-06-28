//---------------------------------------------------------------------------

#include <vcl.h>

#include "ChooseItem1.h"
#include "WizardLogin1.h"

#pragma hdrstop

#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"

TChooseItem *ChooseItem;

//---------------------------------------------------------------------------

__fastcall TChooseItem::TChooseItem(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TChooseItem::FormActivate(TObject *Sender)
{
	Caption = WizardLogin -> setCaption( "Choose " + ItemType );
	if( items.empty() )
	{
		Application->MessageBox( "All items already selected", NULL, MB_OK );
		Close();
	}
	else RedrawGrid();
}

//---------------------------------------------------------------------------

void TChooseItem::RedrawGrid()
{
	int row = 0;
	for( StrSet::const_iterator i = items.begin(); i != items.end(); ++ i )
		ItemGrid -> Cells[ 0 ][ row ++ ] = *i;

	ItemGrid -> RowCount = row;
	int height = (ItemGrid->DefaultRowHeight + 1) * row + 10;
	ItemGrid -> Height = height < 500 ? height : 500;
	this -> Height = ItemGrid -> Height + 90;
}

//---------------------------------------------------------------------------

void __fastcall TChooseItem::ItemGridMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = -1;
	ItemGrid->MouseToCell( X, Y, Col, Row );
	if( Row < 0 )
	{
		Application->MessageBox(
			"Can not access row.\n"
			"Scroll down and try again.",
			"ChooseItem Message", MB_OK );
	}
	else
	{

// if colour is already set, unset it and remove row from list of selected rows
// if colour is not set, set it and add row to list of selected rows

		TColor colour;
		if ( ItemGrid->GetRowColour( Row, colour ) )
			ItemGrid->DelRowColour( Row );
		else
			ItemGrid->SetRowColour( Row, clLime );
	}
}

//---------------------------------------------------------------------------

void TChooseItem::SetItemType( AnsiString type )
{
	ItemType = type;
	items.clear();
}

//---------------------------------------------------------------------------

StrSet TChooseItem::GetSelectedItems()
{
	StrSet setItems;
	TColor colour;
	for( int row=0; row < ItemGrid->RowCount; row++ )
	{
		if( ItemGrid->GetRowColour( row, colour ) )
		{
			setItems.insert(ItemGrid->Cells[0][row]);
			ItemGrid->DelRowColour( row );
		}
	}
	return setItems;
}

//---------------------------------------------------------------------------

void __fastcall TChooseItem::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

