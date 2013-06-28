//---------------------------------------------------------------------------
#include <vcl.h>

#include "CentralTestMachine1.h"
#include "AddTestMachine1.h"
#include "ChooseItem1.h"
#include "Machine1.h"
#include "ObjectName1.h"
#include "LCDbAnalyser.h"
#include "LCDbObject.h"
#include "WizardLogin1.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"

TCentralTestMachine *CentralTestMachine;

//---------------------------------------------------------------------------

__fastcall TCentralTestMachine::TCentralTestMachine(TComponent* Owner)
   : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TCentralTestMachine::FormActivate(TObject *Sender)
{
	Caption = WizardLogin -> setCaption( "Central Test Machine" );
	if( testID != 0 )
		Caption = Caption + " for " + LCDbTests::records().get( testID ).getName();

	CentralTestMachineGrid->RowCount = 2;
	CentralTestMachineGrid->ColCount = CTM_NUMCOLS;
	CentralTestMachineGrid->Cells[CTM_MACHINEID][0] = "ID";
	CentralTestMachineGrid->ColWidths[CTM_MACHINEID] = 50;
	CentralTestMachineGrid->Cells[CTM_MACHINE][0] = "Analyser/Cluster";
	CentralTestMachineGrid->ColWidths[CTM_MACHINE] = 115;
	CentralTestMachineGrid->Cells[CTM_TESTNAME][0] = "Machine Test Name";
	CentralTestMachineGrid->ColWidths[CTM_TESTNAME] = 115;
	CentralTestMachineGrid->Cells[CTM_PROTOCOL][0] = "Protocol";
	CentralTestMachineGrid->ColWidths[CTM_PROTOCOL] = 48;
	CentralTestMachineGrid->Cells[CTM_TESTORDER][0] = "Order";
	CentralTestMachineGrid->ColWidths[CTM_TESTORDER] = 40;

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TCentralTestMachine::AddClick(TObject *Sender)
{
	if( FillChooseMachineGrid() == 0 )
	{
		Application->MessageBox( "All machines already selected", NULL, MB_OK );
		return;
	}

	ChooseItem->ShowModal();
	StrSet setMachines = ChooseItem->GetSelectedItems();
	StrSet::const_iterator itMachine = setMachines.begin();
	if( itMachine == setMachines.end() )
	{
		Application->MessageBox( "No machines selected for this test", NULL, MB_OK );
		return;
	}

	AddTestMachine -> setTest( testID );
	do
	{	const LCDbObject * obj = LCDbObjects::records().find( *itMachine, LCDbObject::CLUSTER );
		if( obj == NULL )
		{
			const LCDbAnalyser * an = LCDbAnalysers::records().findByName( *itMachine );
			AddTestMachine -> setMachine( an == NULL ? 0 : an -> getID() );
		}
		else AddTestMachine -> setMachine( obj -> getID() );

		AddTestMachine->ShowModal();

	} while( (++ itMachine) != setMachines.end() );
	
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TCentralTestMachine::ChangeClick(TObject *Sender)
{
	bool rowFound = false;
	TColor colour;
	for ( int row=1; row<CentralTestMachineGrid->RowCount; row++ )
	{
		if( CentralTestMachineGrid->GetRowColour( row, colour ) )
		{
			String buddy = CentralTestMachineGrid->Cells[CTM_MACHINEID][row];
			if( !buddy.IsEmpty() )
			{
				rowFound = true;
				AddTestMachine -> setMachine( buddy.ToIntDef( 0 ) );
				AddTestMachine -> setTest( testID );
				AddTestMachine -> ShowModal();
			}
			CentralTestMachineGrid -> DelRowColour( row );
		}
	}

	if( rowFound )
		CopyFromClassToGrid();
	else
	{	Application->MessageBox( "No machines selected.",
				"CentralTestMachine",MB_OK);
	}
}

//---------------------------------------------------------------------------

int TCentralTestMachine::FillChooseMachineGrid()
{
	// copy to MachineGrid all machines except
	// those already chosen for this test

	ChooseItem->SetItemType( "Machine" );
	for( LCDbAnalysers::Range mr = LCDbAnalysers::records(); mr.isValid(); ++ mr )
	{
		if( mr -> isActive() )
		{
			const String & MachineName = mr -> getName();
			bool found = false;
			for (int row=1; row < CentralTestMachineGrid -> RowCount; row++)
				if( MachineName == CentralTestMachineGrid->Cells[CTM_MACHINE][row] )
					found = true;

			if ( !found )
				ChooseItem->AddItem( MachineName );
		}
	}

	// copy to MachineGrid all clusters except
	// those already chosen for this test
	for( LCDbObjects::Range or = LCDbObjects::records(); or.isValid(); ++ or )
	{
		if( or -> getObjectType() == LCDbObject::CLUSTER && or -> isActive() )
		{
			const String & cluster = or -> getName();
			bool found = false;
			for( int row=1; row < CentralTestMachineGrid -> RowCount; row++ )
				if( cluster == CentralTestMachineGrid->Cells[ CTM_MACHINE][row] )
					found = true;

			if ( !found )
				ChooseItem -> AddItem( cluster );
		}
	}
	return ChooseItem -> GetNumItems();
}
//---------------------------------------------------------------------------

void __fastcall TCentralTestMachine::DeleteClick(TObject *Sender)
{
	LCDbTest theTest = LCDbTests::records().get( testID );
	TColor colour;
	bool ctmDeleted = false;
	for( int row=0; row<CentralTestMachineGrid->RowCount; row++ )
	{
		if( CentralTestMachineGrid->GetRowColour( row, colour ) )
		{
			String buddy = CentralTestMachineGrid->Cells[CTM_MACHINEID][row];
			if( theTest.deleteMachine( QueryCentral, buddy.ToIntDef( 0 ) ) )
				ctmDeleted = true;
			CentralTestMachineGrid->DelRowColour( row );
		}
	}

	if( ctmDeleted )
		CopyFromClassToGrid();
	else
		Application->MessageBox( "No row deleted", NULL, MB_OK );
}

//---------------------------------------------------------------------------

void TCentralTestMachine::CopyFromClassToGrid()
{
	String name;
	int row = 0;
	const LCDbTest & theTest = LCDbTests::records().get( testID );
	for( LCDbTest::MachineDefs mn = theTest.getCodes(); mn.isValid(); ++ mn )
	{
		int mcid = mn -> getID();
		CentralTestMachineGrid -> Cells[ CTM_MACHINEID ][ ++ row ] = mcid;
		const LDbNames * item = LCDbAnalysers::records().findByID( mcid );
		if( item == NULL )
			item = LCDbObjects::records().findByID( mcid );
		CentralTestMachineGrid -> Cells[ CTM_MACHINE ][ row ] = item -> getName();

		CentralTestMachineGrid -> Cells[ CTM_TESTNAME ][ row ] = mn -> getCode();
		CentralTestMachineGrid -> Cells[ CTM_PROTOCOL ][ row ] = mn -> getProtocol();
		CentralTestMachineGrid -> Cells[ CTM_TESTORDER ][ row ] = mn -> getTestOrder();
	}

	if( row == 0 )
	{
		CentralTestMachineGrid->DeleteRow( 1 );
		CentralTestMachineGrid->RowCount = 2;
	}
	else CentralTestMachineGrid->RowCount = row + 1;

	CentralTestMachineGrid->FixedRows = 1;
}

//---------------------------------------------------------------------------

void __fastcall TCentralTestMachine::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TCentralTestMachine::CentralTestMachineGridMouseDown(
	  TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	CentralTestMachineGrid->MouseToCell( X, Y, Col, Row );
	if( Row < 1 )
	{
		Application->MessageBox(
			"Can not access row.\n"
			"Scroll down and try again.",
			"CentralTestMachine Message", MB_OK );
	}
	else
	{	TColor colour;


	// if colour is set, unset it and remove row from list of selected rows
	// if colour is not set, set it and add row to list of selected rows

		if( CentralTestMachineGrid->GetRowColour( Row, colour ) )
			CentralTestMachineGrid->DelRowColour( Row );
		else
			CentralTestMachineGrid->SetRowColour( Row, clLime );
	}
}

//---------------------------------------------------------------------------

