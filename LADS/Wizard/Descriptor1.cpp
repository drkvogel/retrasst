//---------------------------------------------------------------------------
#include <vcl.h>

#include "Descriptor1.h"
#include "ProfileMap1.h"
#include "EditProjectDb1.h"
#include "AddDescriptor1.h"
#include "LCDbProject.h"
#include "WizardLogin1.h"
#include "Login1.h"
#include "LPDbDescriptor.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"

TDescriptor *Descriptor;

//---------------------------------------------------------------------------

__fastcall TDescriptor::TDescriptor(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TDescriptor::FormActivate(TObject *Sender)
{
	Caption = Login -> setProjectCaption( "Descriptors" );

	LPDbDescriptors::Range existing = LPDbDescriptors::records();
	if( !existing.isValid() )
		AddDefaultDescriptors();

	DescriptorTable->RowCount = 2;
	DescriptorTable->Cells[DL_DESCRIPTOR][0] = "Descriptor";
	DescriptorTable->ColWidths[DL_DESCRIPTOR] = 76;
	DescriptorTable->Cells[DL_DESCRIPTION][0] = "                       Description";
	DescriptorTable->ColWidths[DL_DESCRIPTION] = 200;
	DescriptorTable->Cells[DL_MINLENGTH][0] = "Min len";
	DescriptorTable->ColWidths[DL_MINLENGTH] = 43;
	DescriptorTable->Cells[DL_MAXLENGTH][0] = "Max len";
	DescriptorTable->ColWidths[DL_MAXLENGTH] = 43;
	DescriptorTable->Cells[DL_SOURCE][0] = "Source";
	DescriptorTable->ColWidths[DL_SOURCE] = 60;
	DescriptorTable->Cells[DL_PARSEORDER][0] = "Parse order";
	DescriptorTable->ColWidths[DL_PARSEORDER] = 62;
	DescriptorTable->Cells[DL_DELIMITER][0] = "Start";
	DescriptorTable->ColWidths[DL_DELIMITER] = 32;
	DescriptorTable->Cells[DL_SPECIMEN_FIELD][0] = "Specimen field";
	DescriptorTable->ColWidths[DL_SPECIMEN_FIELD] = 75;
	DescriptorTable->Cells[DL_MAP_FIELD][0] = "Map field";
	DescriptorTable->ColWidths[DL_MAP_FIELD] = 75;
	DescriptorTable->Cells[DL_ACTIVE][0] = "Status";
	DescriptorTable->ColWidths[DL_ACTIVE] = 50;
	DescriptorTable->Cells[DL_DESCRIPTORID][0] = "Descriptor ID";
	DescriptorTable->ColWidths[DL_DESCRIPTORID] = 0;
	DescriptorTable->ColCount = DL_NUM_COLS;

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void TDescriptor::CopyFromClassToGrid()
{
	int row = 1;
	for( LPDbDescriptors::Range pr = LPDbDescriptors::records(); pr.isValid(); ++ pr )
	{
		DescriptorTable->Cells[DL_DESCRIPTOR][row] = pr -> getName();
		DescriptorTable->Cells[DL_DESCRIPTORID][row] = pr -> getID();
		DescriptorTable->Cells[DL_DESCRIPTION][row] = pr -> getDescription();
		DescriptorTable -> Cells[DL_SOURCE][row] = pr -> getSource();
		DescriptorTable->Cells[DL_PARSEORDER][row] = pr -> getParseOrder();
		DescriptorTable->Cells[DL_DELIMITER][row] = pr -> getDelimiter();
		std::pair< short, short > range = pr -> getLengthRange();
		DescriptorTable->Cells[DL_MINLENGTH][row] = range.first;
		DescriptorTable->Cells[DL_MAXLENGTH][row] = range.second;
		DescriptorTable->Cells[DL_SPECIMEN_FIELD][row] = pr -> getSpecimenField();
		DescriptorTable->Cells[DL_MAP_FIELD][row] = pr -> getMapField();
		DescriptorTable->Cells[DL_ACTIVE][row] = pr -> isActive() ? "In use" : "Obsolete";
		++ row;
	}
	DescriptorTable -> RowCount = row;
	DescriptorTable -> FixedRows = 1;
}

//---------------------------------------------------------------------------

void __fastcall TDescriptor::BarcodeClick(TObject *Sender)
{
	addDescriptor( "barcode" );
}

//---------------------------------------------------------------------------

void __fastcall TDescriptor::ByhandClick(TObject *Sender)
{
	addDescriptor( "byhand" );
}

//---------------------------------------------------------------------------

void __fastcall TDescriptor::SpecimenClick(TObject *Sender)
{
	addDescriptor( "specimen" );
}

//---------------------------------------------------------------------------

void TDescriptor::addDescriptor( AnsiString Source )
{
	AddDescriptor->SetDescriptor( 0 );
	AddDescriptor->SetSource( Source );
	AddDescriptor->ShowModal();
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void TDescriptor::AddDefaultDescriptors()
{
	// create root descriptor
	LPDbDescriptor theDescriptor;
	theDescriptor.setName( "analysis" );
	theDescriptor.setDescription( "Analysis" );
	theDescriptor.setSource( "byhand" );
	theDescriptor.setSpecimenField( LPDbDescriptors::records().getNextSpecimenField() );
	theDescriptor.setMapField( LPDbDescriptors::records().getNextMapField() );
	theDescriptor.saveRecord( QueryProject );

	//  add barcode descriptor
	theDescriptor.setID( 0 );
	theDescriptor.setName( "barcode" );
	theDescriptor.setDescription( "Specimen ID" );
	theDescriptor.setSource( "specimen" );
	theDescriptor.setSpecimenField( "barcode" );
	theDescriptor.setMapField( "." );
	theDescriptor.saveRecord( QueryProject );

	//  add aliquot type descriptor
	theDescriptor.setID( 0 );
	theDescriptor.setName( "aliquot_type" );
	theDescriptor.setDescription( "Type of aliquot" );
	theDescriptor.setSource( "byhand" );
	theDescriptor.setObjectType( LCDbObject::ALIQUOT_TYPE );
	theDescriptor.setSpecimenField( LPDbDescriptors::records().getNextSpecimenField() );
	theDescriptor.setMapField( LPDbDescriptors::records().getNextMapField() );
	theDescriptor.saveRecord( QueryProject );
}

//---------------------------------------------------------------------------

void __fastcall TDescriptor::ChangeClick(TObject *Sender)
{
	bool rowFound = false;
	TColor colour;
	for( int row=1; row < DescriptorTable->RowCount; row++ )
	{
		if( DescriptorTable->GetRowColour( row, colour ) )
		{
			String descripID = DescriptorTable -> Cells[DL_DESCRIPTORID][row];
			if( !descripID.IsEmpty() )
			{
				rowFound = true;
				AddDescriptor->SetDescriptor( descripID.ToIntDef( 0 ) );
				AddDescriptor->SetSource( DescriptorTable -> Cells[DL_SOURCE][row] );
				AddDescriptor->ShowModal();
			}
			DescriptorTable->DelRowColour( row );
		}
	}

	if( rowFound )
		CopyFromClassToGrid();
	else
		Application->MessageBox( "No row changed", NULL, MB_OK );
}

//---------------------------------------------------------------------------

void __fastcall TDescriptor::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TDescriptor::DescriptorTableMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	DescriptorTable->MouseToCell( X, Y, Col, Row );
	if( Row < 1 )
	{
		Application->MessageBox(
			"Can not access row.\n"
			"Scroll down and try again.",
			"Descriptor Message", MB_OK );
	}
	else
	{
		// if colour is already set, unset it and remove row from list of selected rows
		// if colour is not set, set it and add row to list of selected rows

		TColor colour;
		if ( DescriptorTable->GetRowColour( Row, colour ) )
			DescriptorTable->DelRowColour( Row );
		else
			DescriptorTable->SetRowColour( Row, clLime );
	}
}

//---------------------------------------------------------------------------



