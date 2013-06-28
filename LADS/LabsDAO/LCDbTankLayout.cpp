//---------------------------------------------------------------------------

#include <vcl.h>
#include "LCDbTankLayout.h"
#include "LQuery.h"

#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------

LCDbSectionDef::LCDbSectionDef( const LQuery & query )
 : LCDbID( query.readInt( "rack_type_cid" ) )
	,tankLayoutCID( query.readInt( "rack_layout_cid" ) )
	,fillOrder( query.readInt( "fill_order" ) )
	,sectionPrefix( query.readString( "section_prefix" ) )
	,firstRack( query.readInt( "first_rack" ) )
	,lastRack( query.readInt( "last_rack" ) )
	,rackCapacity( query.readInt( "rack_Capacity" ))
	,status( query.readInt( "status" ))
	,confirmRack( query.readInt("confirm_rack" ))
{}

//---------------------------------------------------------------------------

bool LCDbSectionDefs::read( LQuery central, bool readAll )
{
	if( readAll ) {
		central.setSQL( "select * from c_tank_layout"
					   " order by rack_type_cid" );
	} else {
		central.setSQL( "select * from c_tank_layout where status <> :sts"
					   " order by rack_type_cid" );
		central.setParam( "sts", LDbValid::DELETED );
	}
	return readData( central );
}

//---------------------------------------------------------------------------

bool LCDbSectionDef::saveRecord( LQuery query )
{
	if( saved ) {
		query.setSQL( "Update c_tank_layout"
					 " set section_prefix = :sp, first_rack = :fr, last_rack = :lr,"
					 " fill_order = :fo, status = :sts, rack_capacity = :rc, confirm_rack = :cr"
					 " where rack_type_cid = :tid " );
	} else {
		claimNextID( query );
		query.setSQL( "Insert into c_tank_layout"
					" (rack_layout_cid, rack_type_cid, fill_order, section_prefix,"
					" first_rack, last_rack, status, rack_capacity, confirm_rack )"
					" values ( :lid, :tid, :fo, :sp, :fr, :lr, :sts, :rc, :cr )" );
		query.setParam( "lid", tankLayoutCID );
	}
	query.setParam( "tid", getID() );
	query.setParam( "fo", fillOrder );
	query.setParam( "sp", sectionPrefix );
	query.setParam( "fr", firstRack );
	query.setParam( "lr", lastRack );
	query.setParam( "sts", status );
	query.setParam( "rc", rackCapacity );
	query.setParam( "cr", confirmRack );
	if( query.execSQL() ) {
		saved = true;
		LCDbSectionDefs::records().insert( *this );
		return true;
	} else {
		return false;
	}
}

//---------------------------------------------------------------------------
//	find section definition given section name and tank layout ID
//---------------------------------------------------------------------------

const LCDbSectionDef * LCDbSectionDefs::find( int tankLayout, const std::string & name ) const {
	const LCDbSectionDef * found = NULL;
	for( const_iterator seg = begin(); seg != end(); ++ seg ) {
		if( seg->getTankLayoutCID() == tankLayout && seg->getSectionPrefix() == name ) {
			if( found == NULL ) {
				found = &(*seg);
			} else {
				throw Exception( "Duplicate record in c_tank_layout" );
			}
		}
	}
	return found;
}

//---------------------------------------------------------------------------
// identify expected position of this section's first rack within the tank
//---------------------------------------------------------------------------

short LCDbSectionDef::getFirstPosition() const {
	short position = 1;
	for( Range< LCDbSectionDef > tli = LCDbSectionDefs::records(); tli.isValid(); ++ tli ) {
		if( tankLayoutCID == tli->getTankLayoutCID() && fillOrder > tli->getFillOrder() ) {
			position += tli->getCapacity();
		}
	}
	return position;
}

//---------------------------------------------------------------------------

