//---------------------------------------------------------------------------

#include <vcl.h>

#include "Machine1.h"
#include "AddMachine1.h"
#include "MachineCReport1.h"
#include "ObjectName1.h"
#include "LCDbAnalyser.h"
#include "LCDbObject.h"
#include "WizardLogin1.h"

#pragma hdrstop

#pragma package(smart_init)
#pragma resource "*.dfm"

TMachine *Machine;

//---------------------------------------------------------------------------

__fastcall TMachine::TMachine(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------
void __fastcall TMachine::FormActivate(TObject *Sender)
{
	Caption = WizardLogin -> setCaption( "Machine" );
	MachineTable->RowCount = 2;

	MachineTable->ColCount = M_NUMCOLS;
	MachineTable->Cells[M_MACHINE][0] = " Machine";
	MachineTable->ColWidths[M_MACHINE] = 110;
	MachineTable->ColWidths[M_MACHINEID] = 0;
	MachineTable->Cells[M_DESCRIPTION][0] = "                   Description";
	MachineTable->ColWidths[M_DESCRIPTION] = 260;
	MachineTable->Cells[M_VALIDFROM][0] = "Valid from";
	MachineTable->ColWidths[M_VALIDFROM] = 77;
	MachineTable->Cells[M_VALIDTO][0] = "Valid to";
	MachineTable->ColWidths[M_VALIDTO] = 77;
	MachineTable->Cells[M_STATUS][0] = "Status";
	MachineTable->ColWidths[M_STATUS] = 54;
	MachineTable->Cells[M_LOCATION][0] = "Location";
	MachineTable->ColWidths[M_LOCATION] = 54;
	MachineTable->Cells[M_CLUSTER][0] = " Cluster";
	MachineTable->ColWidths[M_CLUSTER] = 100;

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void TMachine::CopyFromClassToGrid()
{
	selected.clear();
	int row = 1;
	for( LCDbAnalysers::Range mr = LCDbAnalysers::records(); mr.isValid(); ++ mr, ++ row )
	{
		MachineTable->Cells[M_MACHINE][row] = mr -> getName();
		MachineTable->Cells[M_MACHINEID][row] = mr -> getID();
		MachineTable->Cells[M_DESCRIPTION][row] = mr -> getDescription();
		std::pair< TDate, TDate > dates = mr -> getValidDates();
		MachineTable->Cells[M_VALIDFROM][row] = dates.first;
		MachineTable->Cells[M_VALIDTO][row] = dates.second;
		MachineTable->Cells[M_LOCATION][row] = mr -> getLocation();
		if( mr -> getClusterID() == 0 )
			MachineTable->Cells[M_CLUSTER][row] = "";
		else
		{	const LCDbObject & cluster = LCDbObjects::records().get( mr -> getClusterID() );
			MachineTable->Cells[M_CLUSTER][row] = cluster.getName();
		}
		MachineTable->Cells[M_STATUS][row] = mr -> isActive() ? "active" : "inactive";
	}
	MachineTable->RowCount = row;
	MachineTable -> FixedRows = 1;
}

//---------------------------------------------------------------------------

void __fastcall TMachine::AddClick(TObject *Sender)
{
	AddMachine->SetMachine( 0 );
	AddMachine->ShowModal();
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TMachine::ChangeClick(TObject *Sender)
{
	bool rowFound = false;
	for( std::set< int >::const_iterator si = selected.begin(); si != selected.end(); ++ si )
	{
		int mcid = MachineTable -> Cells[ M_MACHINEID ][ *si ].ToIntDef( 0 );
		if( mcid != 0 )
		{
			rowFound = true;
			AddMachine -> SetMachine( mcid );
			AddMachine -> ShowModal();
		}
	}

	if( !rowFound )
		Application->MessageBox( "No row changed", NULL, MB_OK );

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TMachine::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TMachine::MachineTableMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	MachineTable->MouseToCell( X, Y, Col, Row );

	if( selected.count( Row ) == 0 )
		selected.insert( Row );
	else
		selected.erase( Row );

	MachineTable -> Repaint();
}

//---------------------------------------------------------------------------

void __fastcall TMachine::PreviewReportClick(TObject *Sender)
{
	MachineCReport->Query1->Active = true;
	MachineCReport->QuickRep1->PreviewModal();
	MachineCReport->Query1->Active = false;
}

//---------------------------------------------------------------------------


void __fastcall TMachine::MachineTableDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
	if( !State.Contains( gdFixed ) )
	{
		TCanvas * canvas = MachineTable -> Canvas;
		if( selected.count( ARow ) == 0 )
			canvas -> Brush -> Color = clWhite;
		else
			canvas -> Brush -> Color = clLime;
		canvas -> FillRect( Rect );

		int mid = (Rect.Bottom + Rect.Top - abs( canvas -> Font -> Height )) / 2 - 1;
		canvas -> Font -> Color = clBlack;
		canvas -> TextOut( Rect.Left + 2, mid, MachineTable -> Cells[ ACol ][ ARow ] );
	}
}
//---------------------------------------------------------------------------

