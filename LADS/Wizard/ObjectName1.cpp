//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AddObjectName1.h"
#include "ObjectName1.h"
#include "CentralTest1.h"
#include "WizardLogin1.h"
#include "AddTankMapping1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"

TObjectName *ObjectName;

//---------------------------------------------------------------------------

__fastcall TObjectName::TObjectName(TComponent* Owner)
	: TForm(Owner), objectType( LCDbObject::UNKNOWN )
{}

//---------------------------------------------------------------------------

void __fastcall TObjectName::FormActivate(TObject *Sender)
{
	Caption = WizardLogin -> setCaption( LCDbObject::findDescription( objectType ) );

	ObjectGrid->RowCount = 2;
	ObjectGrid->ColCount = OBJ_NUMCOLS;
	ObjectGrid->Cells[OBJ_OBJECTID][0] = "ID";
	ObjectGrid->ColWidths[OBJ_OBJECTID] = 0;
	ObjectGrid->ColWidths[OBJ_OBJECT] = 125;
	ObjectGrid->Cells[OBJ_FULL][0] = "Full name";
	ObjectGrid->ColWidths[OBJ_FULL] = 230;
	ObjectGrid->Cells[OBJ_STATUS][0] = "Status";
	ObjectGrid->ColWidths[OBJ_STATUS] = 50;

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TObjectName::AddClick(TObject *Sender)
{
	if( objectType == LCDbObject::STORAGE_HIVE )
		AddTankMapping -> ShowModal();
	else
	{	AddObjectName -> setObjectID( 0 );
		AddObjectName -> setObjectType( objectType );
		AddObjectName -> ShowModal();
	}
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TObjectName::ChangeClick(TObject *Sender)
{
	TColor colour;
	bool rowFound = false;
	for ( int row=1; row<ObjectGrid->RowCount; row++ )
	{
		if( ObjectGrid->GetRowColour( row, colour ) )
		{
			String objid = ObjectGrid->Cells[OBJ_OBJECTID][row];
			if( !objid.IsEmpty() )
			{
				rowFound = true;
				AddObjectName->setObjectID( objid.ToIntDef( 0 ) );
				AddObjectName->ShowModal();
			}
			ObjectGrid->DelRowColour( row );
		}
	}

	if ( rowFound )
		CopyFromClassToGrid();
	else
		Application->MessageBox( "No row changed", NULL, MB_OK );
}

//---------------------------------------------------------------------------

void __fastcall TObjectName::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TObjectName::ObjectGridMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	ObjectGrid->MouseToCell( X, Y, Col, Row );
	if( Row < 1 )
	{
		Application->MessageBox(
			"Can not access row.\n"
			"Scroll down and try again.",
			"Object Message", MB_OK );
	}
	else
	{

// if colour is already set, unset it and remove row from list of selected rows
// if colour is not set, set it and add row to list of selected rows

	TColor colour;
	if ( ObjectGrid->GetRowColour( Row, colour ) )
		ObjectGrid->DelRowColour( Row );
	else
		ObjectGrid->SetRowColour( Row, clLime );
	}
}
//---------------------------------------------------------------------------

void TObjectName::CopyFromClassToGrid()
{
	ObjectGrid->Cells[OBJ_OBJECT][0] = LCDbObject::findDescription( objectType );
	int row = 1;
	for( LCDbObjects::Range or = LCDbObjects::records(); or.isValid(); ++ or )
	{
		if( or -> getObjectType() == objectType )
		{
			ObjectGrid->Cells[ OBJ_OBJECT ][ row ] = or -> getName();
			ObjectGrid->Cells[ OBJ_OBJECTID ][ row ] = or -> getID();
			ObjectGrid->Cells[ OBJ_FULL ][ row ] = or -> getDescription();
			ObjectGrid->Cells[ OBJ_STATUS ][ row ] = or -> isActive() ? "In use" : "obsolete";
			row ++;
		}
	}

	if( row > 1 )
		ObjectGrid -> RowCount = row;
	else
	{	ObjectGrid -> DeleteRow( 1 );
		ObjectGrid -> RowCount = 2;
	}
	ObjectGrid->FixedRows = 1;
}

//---------------------------------------------------------------------------



