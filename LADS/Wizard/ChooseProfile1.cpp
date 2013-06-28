//---------------------------------------------------------------------------
#include <vcl.h>

#include "ChooseProfile1.h"
#include "Profile1.h"
#include "LCDbProject.h"
#include "LPDbProfile.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TChooseProfile *ChooseProfile;
const int TChooseProfile::INVALIDPROFILE = 0;

//---------------------------------------------------------------------------

__fastcall TChooseProfile::TChooseProfile(TComponent* Owner)
	: TForm(Owner), profileID( INVALIDPROFILE )
{}

//---------------------------------------------------------------------------

void __fastcall TChooseProfile::FormActivate(TObject *Sender)
{
	ChooseProfile->ChooseProfileBox->Clear();
	for( LPDbProfiles::Range pr = LPDbProfiles::records(); pr.isValid(); ++ pr )
		ChooseProfileBox -> Items -> Add( pr -> getDescription() );
}

//---------------------------------------------------------------------------

void __fastcall TChooseProfile::CancelClick(TObject *Sender)
{
	profileID = INVALIDPROFILE;
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TChooseProfile::SaveClick(TObject *Sender)
{
	if( ChooseProfileBox->ItemIndex == -1 )
	{
		Application->MessageBox( "No profile chosen", NULL, MB_OK );
		return;
	}

	LPDbProfiles::Range pr = LPDbProfiles::records();
	for( int i = 0; i < ChooseProfileBox->ItemIndex; i ++ )
		++ pr;

	profileID = pr -> getID();
	Close();
}

//---------------------------------------------------------------------------

