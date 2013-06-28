//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddTankMapping1.h"
#include "LCDbProject.h"
#include "Login1.h"
#include "LCDbObject.h"
#include "LCDbBoxStore.h"
#include "LQuery.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TAddTankMapping *AddTankMapping;

//---------------------------------------------------------------------------

__fastcall TAddTankMapping::TAddTankMapping(TComponent* Owner) : TForm( Owner )
{}

//---------------------------------------------------------------------------

void __fastcall TAddTankMapping::FormActivate(TObject *Sender)
{
	edtSerial->Text = "";
	edtRackCount->Text = "";

	cbLocation -> Clear();
	cbRackType -> Clear();
	for( LCDbObjects::Range or = LCDbObjects::records(); or.isValid(); ++ or )
	{
		if( or -> getObjectType() == LCDbObject::TANK_LOCATION )
			cbLocation -> Items -> Add( or -> getName() );

		else if( or -> getObjectType() == LCDbObject::TANK_LAYOUT )
			cbRackType -> Items -> Add( or -> getName() );
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddTankMapping::SaveClick(TObject *Sender)
{
	int racks = edtRackCount -> Text.ToIntDef( 0 );
	int pos = edtPosition -> Text.ToIntDef( 0 );
	int cold = edtTemperature -> Text.ToIntDef( 0 );
	const LCDbObjects & objs = LCDbObjects::records();
	const LCDbObject * type = objs.find( cbRackType -> Text, LCDbObject::TANK_LAYOUT );
	const LCDbObject * location = objs.find( cbLocation -> Text, LCDbObject::TANK_LOCATION );
	if( edtSerial->Text.Length() < 4 || type == NULL || location == NULL
	 || racks < 1 || racks > 500 || pos < 1 || pos > 99 || cold >= 0 )
	{
		Application -> MessageBox( "All fields must be filled in", NULL, MB_OK );
		return;
	}

	String name = type -> getName().SubString( 1, 1 ) + edtPosition -> Text;
	String description = cbLocation -> Text + " tank " + edtPosition -> Text;

	LCDbObject hive( 0, LCDbObject::STORAGE_HIVE );
	const LCDbObject * np = objs.find( name, LCDbObject::STORAGE_HIVE );
	if( np == NULL )
		np = objs.find( description, LCDbObject::STORAGE_HIVE );

	if( np == NULL )
	{
		hive.setName( name );
		hive.setDescription( description );
		hive.saveRecord( QueryCentral );
	}
	else hive = *np;

	LCDbObject place = *location;
	LCDbTankStore tank( place.getID(), edtSerial -> Text, description, pos, racks, cold, type -> getID() );
	tank.saveRecord( QueryCentral );
	place.addLinkedTank( QueryCentral, hive.getID(), tank.getID() );
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TAddTankMapping::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------




