//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddRackSize1.h"
#include "LDbIdClasses.h"
#include "WizardLogin1.h"
#include "LCDbBoxStore.h"
#include "LDbBoxSize.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TAddRackSize *AddRackSize;

//---------------------------------------------------------------------------

__fastcall TAddRackSize::TAddRackSize(TComponent* Owner)
 : TForm(Owner), rackSizeID( 0 )
{}

//---------------------------------------------------------------------------

void __fastcall TAddRackSize::FormActivate(TObject *Sender)
{
	cbBoxSize -> Clear();
	for( LCDbBoxSizes::Range bs = LCDbBoxSizes::records(); bs.isValid(); ++ bs )
		cbBoxSize -> Items -> Add( bs -> getName() );

	const LCDbRackSize * theRackSize = LCDbRackSizes::records().findByID( rackSizeID );
	if( theRackSize != NULL )
	{
		Caption = "Modify " + WizardLogin -> setCaption( "Rack Size" );
		edtDescription->Text = theRackSize -> getName();
		edtCapacity->Text = theRackSize -> getSize();
		edtFirstRack->Text = theRackSize -> getFirstRack();
		cbBoxSize -> Text = LCDbBoxSizes::records().get( theRackSize -> getBoxSizeID() ).getName();
		cbActive -> Checked = theRackSize -> isActive();
	}
	else
	{	Caption = "Add " + WizardLogin -> setCaption( "Rack Size" );
		edtDescription->Text = "";
		edtCapacity->Text = "";
		edtFirstRack->Text = "";
		cbActive -> Checked = true;
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddRackSize::SaveClick(TObject *Sender)
{
	if( edtDescription->Text.Length() < 3 )
	{
		Application->MessageBox( "Invalid Description", NULL, MB_OK );
		return;
	}

	short size = edtCapacity->Text.ToIntDef( 0 );
	if( size < 2 )
	{
		Application->MessageBox( "Invalid Capacity", NULL, MB_OK );
		return;
	}

	const LCDbBoxSize * bs = LCDbBoxSizes::records().find( cbBoxSize -> Text );
	if( bs == NULL )
	{
		Application->MessageBox( "Invalid Box Size", NULL, MB_OK );
		return;
	}

	LCDbRackSize theRackSize( rackSizeID );
	theRackSize.setName( edtDescription->Text );
	theRackSize.setFirstRack( edtFirstRack->Text );
	theRackSize.setSize( size );
	theRackSize.setBoxSizeID( bs -> getID() );
	theRackSize.setActive( cbActive -> Checked );
	theRackSize.saveRecord( QueryCentral );
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TAddRackSize::CancelClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------
