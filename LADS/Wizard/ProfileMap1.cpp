//---------------------------------------------------------------------------

#include <vcl.h>
#include <set>

#include "ProfileMap1.h"
#include "Descriptor1.h"
#include "Profile1.h"
#include "ChooseProfile1.h"
#include "AddProfileMapNew1.h"
#include "Login1.h"
#include "LCDbProject.h"
#include "LPDbProfile.h"
#include "LPDbProfileMap.h"
#include "LPDbDescriptor.h"
#include "ChooseItem1.h"
#include "LQuery.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TProfileMap *ProfileMap;

//---------------------------------------------------------------------------

__fastcall TProfileMap::TProfileMap(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TProfileMap::FormActivate(TObject *Sender)
{
	Caption = Login -> setProjectCaption( "Profile Map" );
	SetupNoProfilesBox();
	SetupGrid();
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void TProfileMap::SetupNoProfilesBox()
{
	NoProfilesBox->Clear();
	for( LPDbDescriptors::Range dr = LPDbDescriptors::records(); dr.isValid(); ++ dr )
	{
		LPDbDescriptor::Values vr = dr -> getValues();
		switch( vr.size() )
		{
		case 0:
			NoProfilesBox->Items->Add( dr -> getName() + " - any value" );
			break;

		case 1:
			NoProfilesBox->Items->Add( dr -> getName() + ": " + vr -> getName() );
		}
	}
}

//---------------------------------------------------------------------------

void TProfileMap::SetupGrid()
{
	ProfileMapTable->Cells[PM_PROFILE_MAP_ID][0] = "Profile Map ID";
	ProfileMapTable->Cells[PM_TEST_PROFILE_ID][0] = "Test Profile ID";
	int col = PM_VALUE_COLS;
	for( LPDbDescriptors::Range dr = LPDbDescriptors::records(); dr.isValid(); ++ dr )
		ProfileMapTable->Cells[ col ++ ][ 0 ] = dr -> getName();
	ProfileMapTable->Cells[ col ++ ][ 0 ] = "Profile";
	ProfileMapTable->ColCount = col;
}

//---------------------------------------------------------------------------

void TProfileMap::CopyFromClassToGrid()
{
	std::set< int > columns;
	selected.clear();
	ProfileMapTable->RowCount = 2;
	int row = 1;
	for( LPDbProfileMaps::Range pm = LPDbProfileMaps::records(); pm.isValid(); ++ pm )
	{
		ProfileMapTable->Cells[PM_PROFILE_MAP_ID][row] = pm -> getID();

		// fill grid with descriptor values; record columns used

		int colIndex = PM_VALUE_COLS;
		LPDbDescriptors::Range dr = LPDbDescriptors::records();
		while( dr.isValid() )
		{
			int vid = pm -> findValue( dr -> getID() );
			const LPDbDescriptor::Value * val = dr -> findValueByID( vid );
			if( val == NULL )
				ProfileMapTable->Cells[colIndex][row] = "any value";
			else
			{	ProfileMapTable->Cells[colIndex][row] = val -> getName();
				columns.insert( colIndex );
			}
			++ colIndex, ++ dr;
		}

		// add name of matching profile if available and required

		int pid = pm -> getTestProfile();
		ProfileMapTable->Cells[PM_TEST_PROFILE_ID][row] = pid;
		const LPDbProfile * profile = LPDbProfiles::records().findByID( pid );
		if( profile != NULL && colIndex < ProfileMapTable->ColCount )
			ProfileMapTable->Cells[colIndex][row] = profile -> getName();
		else
			ProfileMapTable->Cells[colIndex][row] = "";

		++ row;
	}

	// make columns with useful values visible; make the last wider

	int colWidth = ProfileMapTable -> Width / (columns.size() + 2);
	for( int col = 0; col < ProfileMapTable -> ColCount; ++ col )
		ProfileMapTable -> ColWidths[ col ] = (columns.count( col ) == 0) ? 0 : colWidth;
	int space = (ProfileMapTable -> Width) - (ProfileMapTable -> GridWidth + 20);
	ProfileMapTable -> ColWidths[ ProfileMapTable -> ColCount - 1 ] += space;

	ProfileMapTable->RowCount = row > 1 ? row : 2;
	ProfileMapTable->FixedRows = 1;
}

//---------------------------------------------------------------------------


void __fastcall TProfileMap::btnAddRowNewClick(TObject *Sender)
{
	AddProfileMapNew->ShowModal();
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TProfileMap::ChangeProfileClick(TObject *Sender)
{
	if( selected.empty() )
	{
		Application->MessageBox( "No row selected", NULL, MB_OK);
		return;
	}

	ChooseProfile->ShowModal();
	int chosenProfile = ChooseProfile -> getProfileID();
	if( chosenProfile == TChooseProfile::INVALIDPROFILE )
	{
		Application->MessageBox( "No profile selected", NULL, MB_OK);
		selected.clear();
		return;
	}

	for( std::set< int >::const_iterator si = selected.begin(); si != selected.end(); ++ si )
	{
		int pmid = ProfileMapTable->Cells[PM_PROFILE_MAP_ID][*si].ToIntDef( 0 );
		const LPDbProfileMap * map = LPDbProfileMaps::records().findByID( pmid );
		if( map != NULL )
		{
			LPDbProfileMap copy = *map;
			copy.setTestProfile( chosenProfile );
			copy.saveRecord( QueryProject );
		}
	}

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TProfileMap::DeleteRowClick(TObject *Sender)
{
	if( selected.empty() )
	{
		Application->MessageBox( "No row selected", NULL, MB_OK);
		return;
	}

	if( Application->MessageBox( "Are you sure you want to delete these profile maps?",
								 "Confirmation", MB_YESNO | MB_ICONQUESTION ) == IDYES )
	{
		for( std::set< int >::const_iterator si = selected.begin(); si != selected.end(); ++ si )
		{
			int pmid = ProfileMapTable->Cells[PM_PROFILE_MAP_ID][*si].ToIntDef( 0 );
			LPDbProfileMaps::records().remove( QueryProject, pmid );
		}
	}

	SetupNoProfilesBox();
	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TProfileMap::ProfileMapTableMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	ProfileMapTable->MouseToCell( X, Y, Col, Row );
	if( selected.count( Row ) == 0 )
		selected.insert( Row );
	else
		selected.erase( Row );

	ProfileMapTable -> Repaint();
}

//---------------------------------------------------------------------------

void __fastcall TProfileMap::btnCloseClick(TObject *Sender)
{
	if( noDuplicateMapEntries() && profileAssigned() )
		Close();
}

//---------------------------------------------------------------------------

bool TProfileMap::noDuplicateMapEntries()
{
	for( LPDbProfileMaps::Range pm1 = LPDbProfileMaps::records(); pm1.isValid(); ++ pm1 )
	{
		LPDbProfileMaps::Range pm2 = pm1;
		while( (++ pm2).isValid() )
		{
			bool dupCol = true;
			for( LPDbDescriptors::Range dr = LPDbDescriptors::records(); dr.isValid(); ++ dr )
			{
				int dv1 = pm1 -> findValue( dr -> getID() );
				int dv2 = pm2 -> findValue( dr -> getID() );
				if( dv1 != 0 && dv2 != 0 && dv1 != dv2 )
					dupCol = false;
			}

			if( dupCol )
			{
				Application->MessageBox( "Two map entries have the same values.\n\n"
										 "Please delete one of them", NULL, MB_OK );
				return false;
			}
		}
	}
	return true;
}

//---------------------------------------------------------------------------

bool TProfileMap::profileAssigned()
{
	for( LPDbProfileMaps::Range pm = LPDbProfileMaps::records(); pm.isValid(); ++ pm )
	{
		if( LPDbProfiles::records().findByID( pm -> getTestProfile() ) == NULL )
		{
			Application->MessageBox( "A profile must be assigned to each map entry",
									 NULL, MB_OK );
			return false;
		}
	}
	return true;
}

//---------------------------------------------------------------------------

void __fastcall TProfileMap::btnInsertValuesClick(TObject *Sender)
{
	ChooseItem -> SetItemType( "Descriptor" );
	for( LPDbDescriptors::Range dr = LPDbDescriptors::records(); dr.isValid(); ++ dr )
		if( dr -> isActive() && dr -> getValues().size() == 1 )
			ChooseItem -> AddItem( dr -> getName() );

	ChooseItem -> ShowModal();
	StrSet descriptors = ChooseItem->GetSelectedItems();
	if( descriptors.empty() )
		return;

	for( LPDbProfileMaps::Range pm = LPDbProfileMaps::records(); pm.isValid(); ++ pm )
	{
		LPDbProfileMap copy = *pm;
		bool updated = false;
		StrSet::const_iterator it = descriptors.begin();
		do
		{	const LPDbDescriptor * dr = LPDbDescriptors::records().findByName( *it );
			LPDbDescriptor::Values vr = dr -> getValues();
			if( pm -> findValue( dr -> getID() ) != vr -> getID() )
			{
				copy.setValue( dr -> getID(), vr -> getID() );
				updated = true;
			}
		} while( (++ it) != descriptors.end() );

		if( updated )
			copy.saveRecord( QueryProject );
	}

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TProfileMap::btnClearValuesClick(TObject *Sender)
{
	ChooseItem -> SetItemType( "Descriptor" );
	for( LPDbDescriptors::Range dr = LPDbDescriptors::records(); dr.isValid(); ++ dr )
		if( dr -> isActive() && dr -> getValues().size() < 2 )
			ChooseItem -> AddItem( dr -> getName() );

	ChooseItem -> ShowModal();
	StrSet descriptors = ChooseItem -> GetSelectedItems();
	if( descriptors.empty() )
		return;

	for( LPDbProfileMaps::Range pm = LPDbProfileMaps::records(); pm.isValid(); ++ pm )
	{
		LPDbProfileMap copy = *pm;
		bool updated = false;
		StrSet::const_iterator it = descriptors.begin();
		do
		{	const LPDbDescriptor * dr = LPDbDescriptors::records().findByName( *it );
			if( dr != NULL && pm -> findValue( dr -> getID() ) != 0 )
			{
				copy.setValue( dr -> getID(), 0 );
				updated = true;
			}
		} while( (++ it) != descriptors.end() );

		if( updated )
			copy.saveRecord( QueryProject );
	}

	CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void __fastcall TProfileMap::ProfileMapTableDrawCell(TObject *Sender,
	  int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
	if( !State.Contains( gdFixed ) )
	{
		TCanvas * canvas = ProfileMapTable -> Canvas;
		if( selected.count( ARow ) == 0 )
			canvas -> Brush -> Color = clWhite;
		else
			canvas -> Brush -> Color = clLime;
		canvas -> FillRect( Rect );

		int mid = (Rect.Bottom + Rect.Top - abs( canvas -> Font -> Height )) / 2 - 1;
		canvas -> Font -> Color = clBlack;
		canvas -> TextOut( Rect.Left + 2, mid, ProfileMapTable -> Cells[ ACol ][ ARow ] );
	}

}

//---------------------------------------------------------------------------

