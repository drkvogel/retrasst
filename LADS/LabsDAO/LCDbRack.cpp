//---------------------------------------------------------------------------
//
//	Version history:
//
//		29 July 2010	Split rack handling out from LCDBoxStore
//		10 Sep 10, NG:	Link to c_tank_map to support reconfiguration
//		23 July 2011	Added section name and position in section
//		25/02/13, NG:	calculate missing fields where possible before insert
//
//---------------------------------------------------------------------------

#include <vcl.h>

#include "LQuery.h"
#include "LCDbRack.h"
#include "LCDbObject.h"
#include "LCDbProject.h"
#include "LCDbTankMap.h"
// #include "LCDbStoreDetail.h"
#include "LCDbTankLayout.h"
#include "LCDbAuditTrail.h"

#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------

LCDbRack::LCDbRack( const std::string & place, int tank, const std::string & rak )
 : site( place ), populationCID( tank ), fullName( rak ), mapCID( 0 ), posInTank( -1 ),
   posInSection( -1 ), capacity( -1 ), sectionCID( 0 ), projectID( 0 ), boxTypeID( 0 ) {
}

//---------------------------------------------------------------------------

void LCDbRack::copyFields( const LQuery & central ) {
	setID( central.readInt( "rack_cid" ) );
	populationCID = central.readInt( "tank_cid" );
	sectionCID = central.readInt( "rack_type_cid" );
	fullName = central.readString( "external_name" );
	posInTank = central.readInt( "position" );
	capacity = central.readInt( "capacity" );
	projectID = central.readInt( "project_cid" );
	boxTypeID = central.readInt( "default_box_type" );
}

//---------------------------------------------------------------------------
//	check for an existing record for this rack; return true if found
//---------------------------------------------------------------------------

bool LCDbRack::findRack( LQuery central ) {
	central.setSQL( "select * from c_rack_number where external_name = :nam and tank_cid = :tid" );
	central.setParam( "nam", fullName );
	central.setParam( "tid", populationCID );
	if( central.open( ) ) {
		copyFields( central );
		return true;
	} else {
		return false;
	}
}

//---------------------------------------------------------------------------
// Create record for rack in c_rack_number, filling in missing fields
//---------------------------------------------------------------------------

bool LCDbRack::createRack( LQuery central ) {
	claimNextID( central );
	calculatePosition( central );
	central.setSQL( "insert into c_rack_number"
				  " (rack_cid, external_name, rack_type_cid, default_box_type, project_cid, "
				  "  tank_cid, position, capacity)" //, tank_map_cid, section, rack_position )"
				  " values( :rid, :nam, :rty, :bty, :pid, :tid, :pos, :cap )" ); //, :tmi, :sec, :srp )" );
	central.setParam( "rid", getID() );
	central.setParam( "nam", fullName );
	central.setParam( "rty", sectionCID );
	central.setParam( "bty", boxTypeID );
	central.setParam( "pid", projectID );
	central.setParam( "tid", populationCID );
	central.setParam( "pos", posInTank );
	central.setParam( "cap", capacity );
//	central.setParam( "tmi", mapCID );
//	central.setParam( "sec", section );
//	central.setParam( "srp", posInSection );
	return central.execSQL();
}

//---------------------------------------------------------------------------
// Create record for rack in c_rack_number, filling in missing fields
//---------------------------------------------------------------------------

bool LCDbRack::updateRecord( LQuery central ) {
	calculatePosition( central );
	central.setSQL( "update c_rack_number"
					" set external_name=:nam, rack_type_cid=:rty, default_box_type=:bty,"
					" project_cid=:pid, tank_cid=:tid, position=:pos, capacity=:cap"
					" where rack_cid=:rid" );
	central.setParam( "nam", fullName );
	central.setParam( "rty", sectionCID );
	central.setParam( "bty", boxTypeID );
	central.setParam( "pid", projectID );
	central.setParam( "tid", populationCID );
	central.setParam( "pos", posInTank );
	central.setParam( "cap", capacity );
	central.setParam( "rid", getID() );
	return central.execSQL();
}

//---------------------------------------------------------------------------
//	fill in missing fields: where rack belongs and what it should be called
//---------------------------------------------------------------------------

void LCDbRack::calculatePosition( LQuery & central )
{
	// we can work most things out but owner must be defined
	LCDbObjects & names = LCDbObjects::records();
	if( names.empty() ) {
		names.read( central, false );
	}
	const LCDbObject * owner = NULL;
	if( populationCID != 0 ) {
		owner = names.findByID( populationCID );
	}
	if( owner == NULL ) {
		throw Exception( "Content not defined for this rack" );
	}

	// find (current) mapping of owner to layout and vessel
	LCDbTankMaps & maps = LCDbTankMaps::records();
	if( maps.empty() ) {
		maps.read( central, false );
	}
	const LCDbTankMap * mapping = NULL;
	if( mapCID != 0 ) {
		mapping = maps.findByID( mapCID );
	}
	if( mapping == NULL ) {
		for( Range< LCDbTankMap > mi = maps; mi.isValid(); ++ mi ) {
			if( mi->getTankCID() == populationCID ) {
				if( mapping == NULL || !mapping->isActive() ) {
					mapping = &(*mi);
				}
			}
		}
	}
	if( mapping == NULL ) {
		throw Exception( "Mapping not defined for this rack" );
	} else {
		mapCID = mapping -> getID();
	}

	// find where the rack is from the vessel it's mapped to
	const LCDbObject * store = names.findByID( mapping->getStorageCID() );
	if( store == NULL ) {
		throw Exception( "Vessel not defined for this rack" );
	}
	const LCDbObject * location = names.findByID( mapping->getLocationCID() );
	if( location == NULL && !site.empty() ) {
		location = names.find( site, LCDbObject::STORAGE_SITE );
	}
	if( location == NULL ) {
		throw Exception( "Location not defined for this rack" );
	} else {
		site = location -> getName();
	}

	// identify section name and/or section ID if not yet known
	LCDbSectionDefs & layouts = LCDbSectionDefs::records();
	if( layouts.empty() ) {
		layouts.read( central, false );
	}
	const LCDbSectionDef * layout = NULL;
	if( sectionCID != 0 ) {
		layout = layouts.findByID( sectionCID );
	}
	if( layout == NULL ) {
		if( section.empty() ) {
			unsigned i = 0;
			while( i < fullName.length() && isalpha( fullName[ i ] ) ) {
				i ++;
			}
			section = fullName.substr( 0, i );
		}
		layout = layouts.find( mapping -> getLayoutCID(), section );
		if( layout == NULL ) {
			throw Exception( "Cannot identify section layout" );
		}
		sectionCID = layout -> getID();
	} else {
		if( layout->getTankLayoutCID() == mapping->getLayoutCID() ) {
			section = layout -> getSectionPrefix();
		} else {
			throw Exception( "Rack layout does not match mapping" );
		}
	}

	// identify position within section and/or population if not yet known
	short offset = layout->getFirstPosition() - 1;
	if( posInTank < 1 ) {
		if( posInSection < 1 ) {
			std::string numberPart = fullName.substr( section.length(), 4 );
			posInSection = atoi( numberPart.c_str() ) - layout->getFirstRack() + 1;
		}
		posInTank = posInSection + offset;
	} else {
		if( posInSection < 1 ) {
			posInSection = posInTank - offset;
		}
	}
}

//---------------------------------------------------------------------------
//	Find part-populated structures in the section given by tankCID
//	and rackTypeCID, or the whole population if rackTypeCID == 0
//---------------------------------------------------------------------------

bool LCDbRacks::read( LQuery qCentral, int tankCID, int rackTypeCID ) {
	if( rackTypeCID == 0 ) {
		qCentral.setSQL( "select * from c_rack_number where tank_cid = :pop"
						" order by rack_cid" );
	} else {
		qCentral.setSQL( "select * from c_rack_number where tank_cid = :pop"
						" and rack_type_cid = :sec"
						" order by rack_cid" );
		qCentral.setParam( "sec", rackTypeCID );
	}
	qCentral.setParam( "pop", tankCID );
	return readData( qCentral );
}

//---------------------------------------------------------------------------

