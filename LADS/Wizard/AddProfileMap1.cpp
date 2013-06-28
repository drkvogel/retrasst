//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AddProfileMap1.h"
#include "ProfileMap1.h"
#include "Descriptor1.h"
#include "ChooseProfile1.h"
#include "LDbIdClasses.h"
#include "Login1.h"
#include "LQuery.h"

#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"

TAddProfileMap *AddProfileMap;

//---------------------------------------------------------------------------
__fastcall TAddProfileMap::TAddProfileMap(TComponent* Owner)
	: TForm(Owner),numProfileMaps(-1)
{}

//---------------------------------------------------------------------------

void __fastcall TAddProfileMap::FormActivate(TObject *Sender)
{
	Caption = Login -> setProjectCaption( "Add Profile Map" );
	ProfileMap->SetupGrid( AddProfileMapTable, false );
}

//---------------------------------------------------------------------------

bool TAddProfileMap::PutProfileMapToGrid()
{
    bool success = true;
    if ( !CreateProfileMapClass() )
    {
        success = false;
    }
    else if ( !CopyFromClassToGrid() )
    {
        Application->MessageBox(
            "All profile maps are present; there are none to add.",
            "Add Profile Map Message",MB_OK);
        success = false;
    }
    return success;
}
//---------------------------------------------------------------------------

bool TAddProfileMap::CopyFromClassToGrid()
{
    AddProfileMapTable->RowCount = 2;
    AddProfileMapTable->FixedRows = 1;
    AddProfileMapTable->DeleteRow(1);
    AddProfileMapTable->RowCount = 2;
    AddProfileMapTable->FixedRows = 1;

    SortProfileMapsByDescriptor();

    bool itemCopied = false;
    int row = 0;
    for (int profile=0; profile<numProfileMaps; profile++)
    {
        if ( !CheckProfileMapExists( profile ) )
        {
            row++;
            AddProfileMapTable->RowCount = row + 1;
            CopyFromClassItemToGrid( row, profile );
            itemCopied = true;
        }
    }
//  resize grid
    int fudgeFactor = 2;
    AddProfileMapTable->Height = ( AddProfileMapTable->RowCount *
        ( AddProfileMapTable->DefaultRowHeight +
        AddProfileMapTable->GridLineWidth ) ) +
        AddProfileMapTable->GridLineWidth + fudgeFactor;
    if ( AddProfileMapTable->Height > 465 )
    {
        AddProfileMapTable->Height = 465;
    }
    return itemCopied;
}
//---------------------------------------------------------------------------
void TAddProfileMap::SortProfileMapsByDescriptor()
{
//    qsort( theProfileMaps, numProfileMaps, sizeof(ProfileMapClass*),
  //      SortByDescriptor);
}
//---------------------------------------------------------------------------
bool TAddProfileMap::CheckProfileMapExists( int profile )
{
    bool mapExists = false;
    bool rowMatches;
/*
	for ( int index=0; index<ProfileMap->numProfileMaps && !mapExists; index++ )
	{
		rowMatches = true;
		for ( int col=0; col<TProfileMap::PM_NUM_VALUES; col++ )
		{
			// if either map has "any value", that constitutes a match
			// ACC, 10/10/02
//            if ( !( ( ProfileMap->theProfileMaps[index]->GetValueID(col) == 0 ) ||
//                ( theProfileMaps[profile]->GetValueID(col) == 0 ) ) )
			if ( theProfileMaps[profile]->GetValueID(col) !=
				ProfileMap->theProfileMaps[index]->GetValueID(col) )
			{
				if ( !( ( ProfileMap->theProfileMaps[index]->GetValueID(col) == 0 ) ||
					( theProfileMaps[profile]->GetValueID(col) == 0 ) ) )
				{
					rowMatches = false;
				}
			}
		}
		if ( rowMatches )
		{
			mapExists = true;
		}
	}
*/	return mapExists;
}
//---------------------------------------------------------------------------

void TAddProfileMap::CopyFromClassItemToGrid( int row, int profile )
{
/*    AddProfileMapTable->Cells[TProfileMap::PM_PROFILE_ID][row] =
		IntToStr(theProfileMaps[profile]->GetProfileMapID());
	AddProfileMapTable->Cells[TProfileMap::PM_PROFILE_INDEX][row] = IntToStr(profile);
	for ( int descrip=0; descrip < numDescriptors; descrip++ )
	{
		int vid = theProfileMaps[profile]->GetValueID(
				theDescriptors[descrip]->GetMapNum() - 1 );
		if ( vid == 0 )
		{
			AddProfileMapTable->Cells[descrip][row] = "any value";
		}
		else
		{
			ValueClass* vc =
				theDescriptors[descrip]->GetValueByID( vid );
			AddProfileMapTable->Cells[descrip][row] = vc->GetName();
		}
	}
*/
}
//---------------------------------------------------------------------------

bool TAddProfileMap::CreateProfileMapClass()
{
    DeleteProfileMapClass();
//    int* numVals = (int*) malloc( TProfileMap::PM_NUM_VALUES * sizeof( int ) );
    int* numVals = new int[TProfileMap::PM_VALUE_COLS];

    // find number of instances of the profile map class
    numProfileMaps = 1;
/*    for(int descrip = 0; descrip < numDescriptors; descrip++)
    {
        numVals[descrip] = theDescriptors[descrip]->GetNumValues();
        if ( numVals[descrip] > 1)
        {
            // work out the new row count before expansion to include profiles.
            // numvals added to cope with optional descriptor,
            // when it is not present. ACC, 9/12/99
            // this includes "any value". ACC, 10/10/02
            numProfileMaps = numProfileMaps * (numVals[descrip] + 1);
        }
        else if ( numVals[descrip] < 1 )
        {
            AnsiString strMessage =
                "Descriptor <" +
                theDescriptors[descrip]->GetName() +
                "> has no values.\n"
                "Descriptors must have at least one value.";
            Application->MessageBox(
                strMessage.c_str(),
                "Profile Map Message",MB_OK);
            delete[] numVals;
            return false;
        }
	}

	theProfileMaps = new ProfileMapClass*[numProfileMaps];
	for(int map = 0; map < numProfileMaps; map++)
	{
		theProfileMaps[map] = NULL;
	}

	// output to profile map class
	// insert values of multi-value descriptors
	int numConsecRows = numProfileMaps;
    for ( int descrip = 0; descrip < numDescriptors; descrip++ )
    {
        int mapCol = theDescriptors[descrip]->GetMapNum() - 1;
        if ( numVals[descrip] > 1 )
        {
            int row = 0;
            numConsecRows = (numConsecRows / (numVals[descrip]+1));
            int numRepeats =
                numProfileMaps/(numConsecRows * (numVals[descrip]+1));
            for ( int repeat = 0; repeat<numRepeats; repeat++ )
            {
                for ( int val_count = 0; val_count < numVals[descrip]+1; val_count++ )
                {
                    for ( int rowCount = 0;
                        rowCount < numConsecRows; rowCount++ )
                    {
                        if ( theProfileMaps[row] == NULL )
                        {
                            theProfileMaps[row] = new ProfileMapClass();
                        }
                        if ( val_count == 0 )
                        {
                            // allow for "any value", 10/10/02, ACC
                            theProfileMaps[row]->SetValueID(mapCol,0);
                        }
                        else
                        {
                            int outStr = theDescriptors[descrip]->
                                GetValue(val_count-1)->GetValueID();
                            theProfileMaps[row]->SetValueID(mapCol,outStr);
                        }
                        row++;
                    }
                }
            }
        }
	}

	// new code 07/11/02, ACC
	// insert values of single value descriptors
	// modified 13/11/02, ACC
	// correctd 12/02/04, ACC
	numProfileMaps;
	for ( int descrip = 0; descrip < numDescriptors; descrip++ )
	{
		int mapCol = theDescriptors[descrip]->GetMapNum() - 1;
		if ( numVals[descrip] == 1 )
		{
			int outStr = theDescriptors[descrip]->
				GetValue(0)->GetValueID();
			for ( int row=0; row<numProfileMaps; row++ )
			{
				if ( theProfileMaps[row] == NULL )
				{
					theProfileMaps[row] = new ProfileMapClass();
				}
				theProfileMaps[row]->SetValueID(mapCol,outStr);
			}
		}
	}
	delete[] numVals;
*/	return true;
}

//---------------------------------------------------------------------------
void TAddProfileMap::DeleteProfileMapClass()
{
/*    if ( theProfileMaps != NULL )
	{
		for (int index=0; index < numProfileMaps; index++)
		{
			if ( theProfileMaps[index] != NULL )
			{
				delete theProfileMaps[index];
			}
		}
		delete[] theProfileMaps;
		theProfileMaps = NULL;
		numProfileMaps = -1;
	}
	*/
}
//---------------------------------------------------------------------------

void __fastcall TAddProfileMap::btnAddClick(TObject *Sender)
{
// copy selected rows to database
    TColor colour;
    bool rowFound = false;

    // has a row been selected?
    for ( int row=1; row<AddProfileMapTable->RowCount; row++ )
    {
        if ( AddProfileMapTable->GetRowColour( row, colour ) )
        {
            rowFound = true;
        }
    }
    // if not, issue message
    if ( !rowFound )
    {
        Application->MessageBox(
            "No row selected.",
            "Profile Map Message",MB_OK);
    }
    // if so, continue
    else
    {
        ChooseProfile->ShowModal();
        int chosenProfile = ChooseProfile -> getProfileID();
        if ( chosenProfile == TChooseProfile::INVALIDPROFILE )
        {
            Application->MessageBox(
                "No profile selected.",
                "Profile Map Message",MB_OK);
        }
        else
        {
            for ( int row=1; row<AddProfileMapTable->RowCount; row++ )
            {
                if ( AddProfileMapTable->GetRowColour( row, colour ) )
                {
                    rowFound = true;
/*                    int profile = StrToInt(
						AddProfileMapTable->Cells[TProfileMap::PM_PROFILE_INDEX][row] );
					int id = LPDbID::allocateID( LQuery( QueryProject ), 1 );
					theProfileMaps[profile]->SetProfileMapID( id );
					theProfileMaps[profile]->SetProfileID( chosenProfile );
					theProfileMaps[profile]->InsertToDatabase(QueryProject);
*/			AddProfileMapTable->DelRowColour( row );
				}
            }
            DeleteProfileMapClass();
            Close();
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TAddProfileMap::CancelClick(TObject *Sender)
{
    DeleteProfileMapClass();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TAddProfileMap::AddProfileMapTableMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
	int Col = 0, Row = 0;
	AddProfileMapTable->MouseToCell( X, Y, Col, Row );
	if( Row < 1 )
	{
        Application->MessageBox(
            "Can not access row.\n"
            "Scroll down and try again.",
            "AddProfileMap Message", MB_OK );
    }
    else
    {

// if colour is already set, unset it and remove row from list of selected rows
// if colour is not set, set it and add row to list of selected rows

    TColor colour;
    if ( AddProfileMapTable->GetRowColour( Row, colour ) )
    {
        AddProfileMapTable->DelRowColour( Row );
    }
    else
    {
        AddProfileMapTable->SetRowColour( Row, clLime );
    }
    }
}
//---------------------------------------------------------------------------


