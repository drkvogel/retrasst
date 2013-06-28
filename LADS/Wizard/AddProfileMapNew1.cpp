//---------------------------------------------------------------------------

#include <vcl.h>

#include "AddProfileMapNew1.h"
#include "ChooseProfile1.h"
#include "Login1.h"
#include "LQuery.h"
#include "LCDbProject.h"
#include "LPDbProfile.h"

#pragma hdrstop

#pragma package(smart_init)
#pragma resource "*.dfm"

TAddProfileMapNew *AddProfileMapNew;

//---------------------------------------------------------------------------

__fastcall TAddProfileMapNew::TAddProfileMapNew(TComponent* Owner)
	: TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TAddProfileMapNew::FormActivate(TObject *Sender)
{
	Caption = Login -> setProjectCaption( "Add Profile Map" );
	theProfileMap = LPDbProfileMap();
	numValuesChosen = 0;
	lbxValuesChosen -> Clear();
	edtProfile -> Text = "";
	btnSave -> Enabled = false;

	if( !nextDescriptor( LPDbDescriptors::records() ) )
	{
		Application->MessageBox( "There are no descriptor values defined",
								 NULL, MB_OK );
		descripID = 0;
		edtDescriptor -> Text = "";
		lbxValues -> Clear();
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddProfileMapNew::btnCancelClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TAddProfileMapNew::lbxValuesClick(TObject *Sender)
{
	LPDbDescriptors::Range dr = LPDbDescriptors::records();
	while( dr.isValid() && dr -> getID() != descripID )
		++ dr;

	const LPDbDescriptor::Value * pVal = NULL;
	if( dr.isValid() && lbxValues -> ItemIndex >= 0 )
		pVal = dr -> findValueByName( lbxValues -> Items -> Strings[ lbxValues -> ItemIndex ] );

	if( pVal != NULL )
	{
		theProfileMap.setValue( descripID, pVal -> getID() );
		lbxValuesChosen -> Items -> Add( dr -> getName() + ": " + pVal -> getName() );
		numValuesChosen ++;
	}
	else if( descripID != 0 )
		theProfileMap.setValue( descripID, 0 );

	if( !nextDescriptor( ++ dr ) )
	{
		ChooseProfile->ShowModal();
		int chosenProfile = ChooseProfile -> getProfileID();
		if( chosenProfile != TChooseProfile::INVALIDPROFILE )
		{
			theProfileMap.setTestProfile( chosenProfile );
			edtProfile->Text = LPDbProfiles::records().get( chosenProfile ).getDescription();
			btnSave -> Enabled = (numValuesChosen > 0);
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddProfileMapNew::btnSaveClick(TObject *Sender)
{
	theProfileMap.saveRecord( QueryProject );
	Close();
}

//---------------------------------------------------------------------------

bool TAddProfileMapNew::nextDescriptor( LPDbDescriptors::Range dr )
{
	while( dr.isValid() )
	{
		if( dr -> isActive() && !dr -> getMapField().IsEmpty() )
		{
			LPDbDescriptor::Values vr = dr -> getValues();
			if( vr.isValid() )
			{
				edtDescriptor -> Text = dr -> getName();
				descripID = dr -> getID();
				lbxValues -> Items -> Text = "(any value)";
				do lbxValues -> Items -> Add( vr -> getName() );
				while( (++ vr).isValid() );
				return true;
			}
		}
		++ dr;
	}
	return false;
}

//---------------------------------------------------------------------------

