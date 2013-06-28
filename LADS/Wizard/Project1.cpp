//---------------------------------------------------------------------------
#include <vcl.h>

#include "Project1.h"
#include "LCDbProject.h"
#include "AddProject1.h"
#include "WizardLogin1.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------

TProject *Project;

__fastcall TProject::TProject(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TProject::FormActivate(TObject *Sender)
{
	Caption = WizardLogin -> setCaption( "Project" );

	ProjectTable->RowCount = 2;
	ProjectTable->ColCount = NUM_PROJECT_COLS;
	ProjectTable->Cells[P_PROJECT][0] = "Project";
	ProjectTable->ColWidths[P_PROJECT] = 82;
	ProjectTable->Cells[P_PROJECTID][0] = "Project ID";
	ProjectTable->ColWidths[P_PROJECTID] = 0;
	ProjectTable->Cells[P_FULLNAME][0] = "                 Description";
	ProjectTable->ColWidths[P_FULLNAME] = 265;
	ProjectTable->Cells[P_VALIDFROM][0] = "Valid from";
	ProjectTable->ColWidths[P_VALIDFROM] = 70;
	ProjectTable->Cells[P_VALIDTO][0] = "Valid to";
	ProjectTable->ColWidths[P_VALIDTO] = 70;
	ProjectTable->Cells[P_STATUS][0] = "Status";
	ProjectTable->ColWidths[P_STATUS] = 50;
	ProjectTable->Cells[P_DATABASE][0] = "Database";
	ProjectTable->ColWidths[P_DATABASE] = 70;
	ProjectTable->Cells[P_URL][0] = "URL";
	ProjectTable->ColWidths[P_URL] = 105;
	ProjectTable->Cells[P_DBVERSION][0] = "Version";
	ProjectTable->ColWidths[P_DBVERSION] = 40;

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void TProject::CopyFromClassToGrid()
{
	int row = 1;
	for( LCDbProjects::Range pr = LCDbProjects::records(); pr.isValid(); ++ pr, ++ row )
	{
		ProjectTable->Cells[P_PROJECT][ row] = pr -> getName();
		ProjectTable->Cells[P_PROJECTID][row] = pr -> getID();
		ProjectTable->Cells[P_FULLNAME][row] = pr -> getDescription();
		ProjectTable->Cells[P_DATABASE][row] = pr -> getDbName();
		ProjectTable->Cells[P_URL][row] = pr -> getAddress();

		String status = "Inactive";
		if( pr -> isActive() )
			status = (pr -> isTest() ? "Test" : "Live");
		ProjectTable->Cells[P_STATUS][row] = status;

		std::pair< TDate, TDate > dates = pr -> getValidDates();
		ProjectTable->Cells[P_VALIDFROM][row] = dates.first;
		ProjectTable->Cells[P_VALIDTO][row] = dates.second;
		ProjectTable->Cells[P_DBVERSION][row] = pr -> getVersionString();
	}

	ProjectTable->RowCount = row;
	ProjectTable->FixedRows = 1;
}

//---------------------------------------------------------------------------

void __fastcall TProject::AddClick(TObject *Sender)
{
	AddProject -> setProject( LCDbProject::NONE_SELECTED );
	AddProject -> ShowModal();
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TProject::ChangeClick(TObject *Sender)
{
	bool rowFound = false;
	TColor colour;
	for( int row=1; row<ProjectTable->RowCount; row++ )
	{
		if( ProjectTable->GetRowColour( row, colour ) )
		{
			String projID = ProjectTable->Cells[P_PROJECTID][row];
			if( !projID.IsEmpty() )
			{
				rowFound = true;
				AddProject->setProject( projID.ToIntDef( LCDbProject::NONE_SELECTED ) );
				AddProject->ShowModal();
			}
			ProjectTable->DelRowColour( row );
		}
	}

	if( rowFound )
		CopyFromClassToGrid();
	else
		Application->MessageBox( "No row changed", "Project Message", MB_OK );
}

//---------------------------------------------------------------------------

void __fastcall TProject::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TProject::ProjectTableMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	ProjectTable->MouseToCell( X, Y, Col, Row );
	if( Row < 1 )
	{
		Application->MessageBox(
			"Can not access row.\n"
			"Scroll down and try again.",
			"Project Message", MB_OK );
	}
	else
	{  	TColor colour;

	// if colour is set, unset it and remove row from list of selected rows
	// if colour is not set, set it and add row to list of selected rows

		if( ProjectTable->GetRowColour( Row, colour ) )
			ProjectTable->DelRowColour( Row );
		else
			ProjectTable->SetRowColour( Row, clLime );
	}
}

//---------------------------------------------------------------------------



