//---------------------------------------------------------------------------
#include <vcl.h>

#include "AddObjectName1.h"
#include "ObjectName1.h"
#include "ChooseItem1.h"
#include "CentralTest1.h"
#include "WizardLogin1.h"
#include "LCDbTest.h"
#include "LCDbAnalyser.h"
#include "AddTankMapping1.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"

TAddObjectName *AddObjectName;

//---------------------------------------------------------------------------

__fastcall TAddObjectName::TAddObjectName(TComponent* Owner)
 : TForm(Owner), objectCID( 0 ), objectType( LCDbObject::UNKNOWN )
{}

//---------------------------------------------------------------------------

void __fastcall TAddObjectName::FormActivate(TObject *Sender)
{
	linkedGrid -> ColCount = OT_NUMCOLS;
	linkedGrid -> Cells[OT_LINKID][0] = "ID";
	linkedGrid -> ColWidths[OT_LINKID] = 0;
	linkedGrid -> ColWidths[OT_LINKNAME] = 130;

	bool modify = false;
	const LCDbObject * theObject = LCDbObjects::records().findByID( objectCID );
	if( theObject != NULL )
	{
		objectType = theObject -> getObjectType();
		edtObjectName -> Text = theObject -> getName();
		edtFullName -> Text = theObject -> getDescription();
		cbActive -> Checked = theObject -> isActive();
		modify = true;
	}
	else
	{	edtObjectName -> Text = "";
		edtFullName -> Text = "";
		cbActive -> Checked = true;
	}

	Caption = String( modify ? "Modify " : "Add " )
			+ WizardLogin -> setCaption( LCDbObject::findDescription( objectType ) );
	copyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TAddObjectName::SaveClick(TObject *Sender)
{
	if( edtObjectName -> Text.IsEmpty() )
		Application->MessageBox( "Invalid object name", NULL, MB_OK );

	else if( edtFullName -> Text.IsEmpty() )
		Application->MessageBox( "Invalid description", NULL, MB_OK );

	else
	{	LCDbObject theObject( objectCID, objectType );
		if( objectCID != 0 )
			theObject = LCDbObjects::records().get( objectCID );

		theObject.setName( edtObjectName -> Text );
		theObject.setDescription( edtFullName -> Text );
		theObject.setActive( cbActive -> Checked );
		theObject.saveRecord( QueryCentral );

		if( objectCID == 0 )
			objectCID = theObject.getID();

		if( objectType == LCDbObject::CALIBRANT || objectType == LCDbObject::REAGENT )
			Add -> Enabled = true;
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddObjectName::CancelClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void TAddObjectName::copyFromClassToGrid()
{
	int rows = 0;
	switch( objectType )
	{
		case LCDbObject::CLUSTER:
			rows = copyMachines();
			Delete -> Enabled = false;
			Add -> Enabled = false;
			Panel2 -> Visible = true;
			break;

		case LCDbObject::TANK_LOCATION:
			rows = copyTanks();
			Delete -> Enabled = false;
			Add -> Enabled = false;
			Panel2 -> Visible = true;
			break;

		case LCDbObject::CALIBRANT:
		case LCDbObject::REAGENT:
			rows = copyCRTests();
			Delete -> Enabled = (rows > 1);
			Add -> Enabled = (objectCID != 0);
			Panel2 -> Visible = true;
			break;

		default:
			Panel2 -> Visible = false;
	}

	if( rows > 1 )
		linkedGrid -> RowCount = rows;
	else
	{	linkedGrid->RowCount = 2;
		linkedGrid->DeleteRow(1);
		linkedGrid->RowCount = 2;
	}
	linkedGrid->FixedRows = 1;
}

//---------------------------------------------------------------------------

int TAddObjectName::copyCRTests()
{
	linkedGrid -> Cells[OT_LINKNAME][0] = "Test name";
	int row = 1;
	if( objectCID != 0 )
	{
		const LCDbObject & theObject = LCDbObjects::records().get( objectCID );
		for( LCDbObject::Links lt = theObject.getLinks(); lt.isValid(); ++ lt )
		{
			const LCDbTest & test = LCDbTests::records().get( lt -> getID() );
			linkedGrid -> Cells[ OT_LINKNAME ][ row ] = test.getName();
			linkedGrid -> Cells[ OT_LINKID ][ row ] = test.getID();
			row ++;
		}
	}
	return row;
}

//---------------------------------------------------------------------------

int TAddObjectName::copyTanks()
{
	linkedGrid -> Cells[OT_LINKNAME][0] = "Tank label";
	int row = 1;
	if( objectCID != 0 )
	{
		const LCDbObject & theObject = LCDbObjects::records().get( objectCID );
		for( LCDbObject::Links lt = theObject.getLinks(); lt.isValid(); ++ lt )
		{
			const LCDbObject & tank = LCDbObjects::records().get( lt -> getID() );
			linkedGrid -> Cells[ OT_LINKID ][ row ] = tank.getID();
			linkedGrid -> Cells[ OT_LINKNAME ][ row ] = tank.getName();
			row ++;
		}
	}
	return row;
}

//---------------------------------------------------------------------------

int TAddObjectName::copyMachines()
{
	linkedGrid -> Cells[OT_LINKNAME][0] = "Analyser";
	int row = 1;
	if( objectCID != 0 )
	{
		for( LCDbAnalysers::Range lt = LCDbAnalysers::records(); lt.isValid(); ++ lt )
		{
			if( lt -> getClusterID() == objectCID )
			{
				linkedGrid -> Cells[ OT_LINKID ][ row ] = lt -> getID();
				linkedGrid -> Cells[ OT_LINKNAME ][ row ] = lt -> getName();
				row ++;
			}
		}
	}
	return row;
}

//---------------------------------------------------------------------------

void __fastcall TAddObjectName::AddClick(TObject *Sender)
{
	if( objectType == LCDbObject::CALIBRANT || objectType == LCDbObject::REAGENT )
		addCRTests();

	copyFromClassToGrid();
}

//---------------------------------------------------------------------------

void TAddObjectName::addCRTests()
{
	ChooseItem->SetItemType( "Test" );
	for( LCDbTests::Range tr = LCDbTests::records(); tr.isValid(); ++ tr )
	{
		if( tr -> isActive() )
		{
			bool testUsed = false;
			for (int row=1; row < linkedGrid->RowCount; row++)
				if( linkedGrid->Cells[OT_LINKNAME][row] == tr -> getName() )
					testUsed = true;

			if( !testUsed )
				ChooseItem -> AddItem( tr -> getName() );
		}
	}

	// add the selected tests
	if( ChooseItem->GetNumItems() == 0 )
		Application->MessageBox( "No tests to be added", NULL, MB_OK );
	else
	{	ChooseItem -> ShowModal();
		StrSet setTests = ChooseItem->GetSelectedItems();
		LCDbObject theObject = LCDbObjects::records().get( objectCID );
		StrSet::const_iterator itTest = setTests.begin();
		while( itTest != setTests.end() )
		{
			const LCDbTest * test = LCDbTests::records().findByName( *itTest );
			if( test != NULL )
				theObject.addLinkedTest( QueryCentral, test -> getID() );
			++ itTest;
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddObjectName::DeleteClick(TObject *Sender)
{
	std::set< int > tests;
	TColor colour;
	for( int row=0; row < linkedGrid->RowCount; row++ )
	{
		if( linkedGrid->GetRowColour( row, colour ) )
		{
			String test = linkedGrid -> Cells[ OT_LINKID ][ row ];
			if( !test.IsEmpty() )
				tests.insert( test.ToInt() );
			linkedGrid -> DelRowColour( row );
		}
	}

	if( !tests.empty() && Application->MessageBox(
			"Are you sure you want to delete these tests?",
			"Confirmation", MB_YESNO | MB_ICONQUESTION ) == IDYES )
	{
		LCDbObject theObject = LCDbObjects::records().get( objectCID );
		std::set< int >::const_iterator ti = tests.begin();
		do theObject.deleteTest( QueryCentral, *ti );
		while( (++ ti) != tests.end() );
		copyFromClassToGrid();
	}
	else Application->MessageBox( "No row deleted", NULL, MB_OK );
}

//---------------------------------------------------------------------------

void __fastcall TAddObjectName::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TAddObjectName::linkedGridMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	linkedGrid->MouseToCell( X, Y, Col, Row );
	if( Row < 1 )
	{
		Application->MessageBox(
			"Can not access row.\n"
			"Scroll down and try again.",
			"AddCalibrant Message", MB_OK );
	}
	else
	{

// if colour is already set, unset it and remove row from list of selected rows
// if colour is not set, set it and add row to list of selected rows

	TColor colour;
	if ( linkedGrid->GetRowColour( Row, colour ) )
		linkedGrid->DelRowColour( Row );
	else
		linkedGrid->SetRowColour( Row, clLime );
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddObjectName::edtObjectNameExit(TObject *Sender)
{
	String description = edtFullName -> Text;
	if( description.IsEmpty() || description == edtObjectName -> Text )
	{
		edtFullName -> Text = LCDbObject::findDescription( objectType )
							+ ' ' + edtObjectName -> Text;
	}
}

//---------------------------------------------------------------------------


