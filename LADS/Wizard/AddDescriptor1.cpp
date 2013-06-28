//---------------------------------------------------------------------------

#include <vcl.h>

#include "AddDescriptor1.h"
#include "AddValue1.h"
#include "ObjectName1.h"
#include "LCDbProject.h"
#include "Login1.h"
#include "LPDbProfileMap.h"
#include "LQuery.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"

TAddDescriptor *AddDescriptor;

//---------------------------------------------------------------------------

__fastcall TAddDescriptor::TAddDescriptor(TComponent* Owner)
	: TForm(Owner), descripID( 0 )
{}

//---------------------------------------------------------------------------

void __fastcall TAddDescriptor::FormActivate(TObject *Sender)
{
	cbSource -> Items -> Text = "byhand";
	cbSource -> Items -> Add( "specimen" );
	for( LPDbDescriptors::Range d = LPDbDescriptors::records(); d.isValid(); ++ d )
		cbSource -> Items -> Add( d -> getSpecimenField() + ": " + d -> getName() );

	cbObjectType -> Items -> Text = "(value list)";
	for( int ot = LCDbObject::UNKNOWN; (++ ot) < LCDbObject::NUM_TYPES; )
		cbObjectType -> Items -> Add( LCDbObject::findDescription( LCDbObject::Category( ot ) ) );

	const LPDbDescriptor * theDescriptor = LPDbDescriptors::records().findByID( descripID );
	if( theDescriptor == NULL )
	{
		edtName->Text = "";
		edtDescription->Text = "";
		edtParseOrder->Text = "0";
		edtDelimiter->Text = "";
		edtMinLength->Text = "1";
		edtMaxLength->Text = "16";

		edtMapFieldName->Text = LPDbDescriptors::records().getNextMapField();
		edtSpecimenField->Text = LPDbDescriptors::records().getNextSpecimenField();
		cbSource -> Text = source;
		cbObjectType -> ItemIndex = 0;
		cbRequired -> Checked = true;

		Caption = "Add " + Login -> setProjectCaption( "Descriptors" );
		Add->Enabled = false;
	}
	else
	{   edtName->Text = theDescriptor -> getName();
		edtDescription->Text = theDescriptor -> getDescription();
		edtParseOrder->Text = theDescriptor -> getParseOrder();
		edtDelimiter->Text = theDescriptor -> getDelimiter();

		edtSpecimenField->Text = theDescriptor -> getSpecimenField();
		edtMapFieldName->Text = theDescriptor -> getMapField();
		std::pair< short, short > lengths = theDescriptor -> getLengthRange();
		edtMinLength->Text = lengths.first;
		edtMaxLength->Text = lengths.second;
		cbSource -> Text = source = theDescriptor -> getSource();
		cbObjectType -> ItemIndex = theDescriptor -> getObjectType();
		cbRequired -> Checked = theDescriptor -> isActive();

		Caption = "Modify " + Login -> setProjectCaption( "Descriptors" );
		Add->Enabled = true;
	}

	DefineValueGrid();
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void TAddDescriptor::DefineValueGrid()
{
	grdValue->RowCount = 2;
	grdValue->ColCount = VAL_NUM_COLS;
	grdValue->Cells[VAL_NAME][0] = "Value Name";
	grdValue->ColWidths[VAL_NAME] = 170;
	grdValue->Cells[VAL_MINVALUE][0] = "Min value";
	grdValue->ColWidths[VAL_MINVALUE] = 80;
	grdValue->Cells[VAL_MAXVALUE][0] = "Max value";
	grdValue->ColWidths[VAL_MAXVALUE] = 80;
	grdValue->Cells[VAL_VALUEID][0] = "Value ID";
	grdValue->ColWidths[VAL_VALUEID] = 0;
}

//---------------------------------------------------------------------------

void __fastcall TAddDescriptor::SaveClick(TObject *Sender)
{
	if( edtName->Text.IsEmpty() || edtDescription->Text.IsEmpty() || cbSource->Text.IsEmpty() )
	{
		Application->MessageBox( "Please fill in every field", NULL, MB_OK );
		return;
	}

	String name = edtName -> Text.LowerCase();
	if( (name == "analysis" && cbObjectType -> ItemIndex != 0)
	 || (name.Pos( "aliquot" ) == 1) != (cbObjectType -> ItemIndex == LCDbObject::ALIQUOT_TYPE) )
	{
		Application->MessageBox( "Please check object type", NULL, MB_OK );
		return;
	}

	std::pair< short, short > lengths( edtMinLength->Text.ToIntDef(0), edtMaxLength->Text.ToIntDef(0) );
	if( lengths.first < 1 || lengths.second > 16 || lengths.first > lengths.second )
	{
		Application->MessageBox( "Please check minimum and maximum length", NULL, MB_OK );
		return;
	}

	String mfnStart = edtMapFieldName -> Text.LowerCase().SubString( 1, 8 );
	String sfnStart = edtSpecimenField -> Text.LowerCase().SubString( 1, 5 );
	if( (mfnStart != "value_id") || (sfnStart != "value" && sfnStart != "barco") )
	{
		Application->MessageBox( "Invalid specimen field or map field name", NULL, MB_OK );
		return;
	}

	short parseOrder = edtParseOrder->Text.ToIntDef( 0 );
	source = cbSource -> Text;
	if( source == "byhand" || source == "specimen"  )
	{
		if( parseOrder != 0 || !edtDelimiter -> Text.IsEmpty() )
		{
			Application->MessageBox( "No parse order or delimiter allowed for by-hand"
									" or specimen descriptors",	NULL, MB_OK );
			return;
		}
	}
	else
	{	int end = source.Pos( ": " );
		if( end > 1 )
			source.SetLength( end - 1 );

		if( (edtSpecimenField -> Text == source) ? (parseOrder != 0) : (parseOrder == 0) )
		{
			Application->MessageBox( "Parse order should be set"
									" unless specimen field matches source",
									NULL, MB_OK );
			return;
		}

		if( !edtDelimiter -> Text.IsEmpty() && parseOrder == 0 )
		{
			Application->MessageBox( "Parse order should be set"
									" if start delimiter is specified",
									NULL, MB_OK );
			return;
		}
	}

	LPDbDescriptor theDescriptor;
	for( LPDbDescriptors::Range d = LPDbDescriptors::records(); d.isValid(); ++ d )
	{
		if( descripID == d -> getID() )
			theDescriptor = *d;				// copies value list

		else if( checkMatch( d -> getSpecimenField(), edtSpecimenField -> Text )
			  || checkMatch( d -> getMapField(), edtMapFieldName -> Text ) )
		{
			Application->MessageBox( "Specimen field name"
									" and map field name should be unique",
									NULL, MB_OK );
			return;
		}
	}

	theDescriptor.setName( edtName->Text );
	theDescriptor.setDescription( edtDescription->Text );
	theDescriptor.setSource( source );
	theDescriptor.setParseOrder( parseOrder );
	theDescriptor.setLengthRange( lengths );
	theDescriptor.setMapField( edtMapFieldName->Text );
	theDescriptor.setSpecimenField( edtSpecimenField->Text );
	theDescriptor.setDelimiter( edtDelimiter -> Text );
	theDescriptor.setActive( cbRequired -> Checked );
	theDescriptor.setObjectType( cbObjectType -> ItemIndex );
	theDescriptor.saveRecord( QueryProject ) ;

	descripID = theDescriptor.getID();
	edtName -> Text = theDescriptor.getName();
	Add->Enabled = true;
}

//---------------------------------------------------------------------------

bool TAddDescriptor::checkMatch( const String & field1, const String & field2 )
{
	if( field1.IsEmpty() || field2.IsEmpty() || field1 == '.' || field2 == '.' )
		return false;
	return field1.AnsiCompareIC( field2 ) == 0;
}

//---------------------------------------------------------------------------

void __fastcall TAddDescriptor::CancelClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TAddDescriptor::AddClick(TObject *Sender)
{
	AddValue -> setDescriptor( descripID );
	AddValue -> ShowModal();
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TAddDescriptor::ChangeClick(TObject *Sender)
{
	bool rowFound = false;
	TColor colour;
	for( int row=1; row < grdValue->RowCount; row++ )
	{
		if( grdValue -> GetRowColour( row, colour ) )
		{
			String valueID = grdValue -> Cells[ VAL_VALUEID ][ row ];
			if( !valueID.IsEmpty() )
			{
				rowFound = true;
				AddValue -> setValue( valueID.ToIntDef( 0 ) );
				AddValue -> ShowModal();
			}
			grdValue -> DelRowColour( row );
		}
	}

	if( rowFound )
		CopyFromClassToGrid();
	else
		Application->MessageBox( "No row changed", NULL, MB_OK );
}

//---------------------------------------------------------------------------

void TAddDescriptor::CopyFromClassToGrid()
{
	int row = 1;
	const LPDbDescriptor * descrip = LPDbDescriptors::records().findByID( descripID );
	if( descrip != NULL )
	{
		for( LPDbDescriptor::Values dv = descrip -> getValues(); dv.isValid(); ++ dv )
		{
			grdValue->Cells[VAL_NAME][row] = dv -> getName();
			grdValue->Cells[VAL_VALUEID][row] = dv -> getID();
			std::pair< String, String > range = dv -> getRange();
			grdValue->Cells[VAL_MINVALUE][row] = range.first;
			grdValue->Cells[VAL_MAXVALUE][row] = range.second;
			++ row;
		}
	}

	if( row > 1 )
	{
		grdValue -> RowCount = row;
		Change -> Enabled = true;
	}
	else
	{	grdValue -> RowCount = 2;
		grdValue -> DeleteRow( 1 );
		grdValue -> RowCount = 2;
		Change -> Enabled = false;
	}
	grdValue -> FixedRows = 1;
}

//---------------------------------------------------------------------------

void __fastcall TAddDescriptor::grdValueMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	grdValue->MouseToCell( X, Y, Col, Row );
	if( Row == -1 )
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
		if( grdValue->GetRowColour( Row, colour ) )
			grdValue->DelRowColour( Row );
		else
			grdValue->SetRowColour( Row, clLime );
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddDescriptor::btnCloseClick(TObject *Sender)
{
	if( descripID == 0
	 || LPDbDescriptors::records().get( descripID ).getValues().isValid()
	 || Application -> MessageBox( "Do you want to add values for this descriptor?",
								   "No values defined", MB_YESNO ) == ID_NO )
	{
		Close();
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddDescriptor::DeleteClick(TObject *Sender)
{
	LPDbDescriptor theDescriptor = LPDbDescriptors::records().get( descripID );
	bool rowDeleted = false, rowInUse = false;
	TColor colour;
	for( int row=1; row < grdValue->RowCount; row++ )
	{
		if( grdValue->GetRowColour( row, colour ) )
		{
			int valueID = grdValue -> Cells[ VAL_VALUEID ][ row ].ToIntDef( 0 );
			if( inProfileMap( valueID ) )
			{
				String message = grdValue -> Cells[ VAL_NAME ][ row ] + " used in profile map";
				Application->MessageBox( message.c_str(), NULL, MB_OK );
				rowInUse = true;
			}
			else
			{	theDescriptor.removeValue( QueryProject, valueID );
				rowDeleted = true;
			}
			grdValue -> DelRowColour( row );
		}
	}

	if( rowDeleted )
		CopyFromClassToGrid();

	else if( !rowInUse )
		Application->MessageBox( "No row changed", NULL, MB_OK );
}

//---------------------------------------------------------------------------

bool TAddDescriptor::inProfileMap( int valueID )
{
	for( LPDbProfileMaps::Range pmi = LPDbProfileMaps::records(); pmi.isValid(); ++ pmi )
		if( pmi -> includesValue( valueID ) )
			return true;

	return false;
}

//---------------------------------------------------------------------------


