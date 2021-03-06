//---------------------------------------------------------------------------

#include <vcl.h>
#include <sstream>
#include "StoreDAO.h"
#include "LCDbObject.h"
#include "LDbBoxStore.h"
#include "LCDbRack.h"
#include "LCDbStoreDetail.h"
#include "LCDbTankMap.h"
#include "LCDbTankLayout.h"
#include "LCDbProject.h"
#include "LPDbBoxes.h"
#include "LIMSDatabase.h"
#include "LQuery.h"
#include "LCDbJob.h"
#include "StoreUtil.h"
#include "LPDbCryovialStore.h"
#include "LPDbDescriptor.h"
#include "LCDbAuditTrail.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)


void StoreDAO::loadSites(std::vector<ROSETTA>& results)
{
	results.clear();
	ROSETTA result;
	for( Range<LCDbObject> ul = LCDbObjects::records(); ul.isValid(); ++ ul ) {
		if( ul->isActive() && ul->getObjectType() == LCDbObject::STORAGE_SITE ) {
			result.setInt( "object_cid", ul->getID() );
			result.setString( "external_full", ul->getDescription().c_str() );
			result.setString( "external_name", ul->getName().c_str() );
			results.push_back(result);
		}
	}
}

//---------------------------------------------------------------------------

bool StoreDAO::saveSite( ROSETTA & data )
{
	LCDbObject site( LCDbObject::STORAGE_SITE );
	site.setName( data.getString("Name") );
	site.setDescription( data.getString("fullname") );
	bool ok = site.saveRecord( LIMSDatabase::getCentralDb() );
	if( ok )
		data.setInt("ID", site.getID() );
	return ok;
}

//---------------------------------------------------------------------------
//	list vessels at the given site (or none if location_id == -1)
//---------------------------------------------------------------------------

void StoreDAO::loadTanks( int location_id, std::vector<ROSETTA>& results )
{
	LQuery cQuery( LIMSDatabase::getCentralDb() );
	if( location_id == 0 ) {
		cQuery.setSQL( "SELECT object_cid as storage_cid, 0 as location_cid, 0 as position,"
					  " external_name as serial_number, external_full as friendly_name"
					  " FROM c_object_name"
					  " WHERE object_type = :typ AND status <> :del AND object_cid not in"
					  " (SELECT storage_cid FROM c_tank_map WHERE status not in (:off, :del))" );
		cQuery.setParam( "off", LCDbTankMap::OFFLINE );
		cQuery.setParam( "typ", LCDbObject::STORAGE_VESSEL );
	} else {
		cQuery.setSQL( "SELECT distinct storage_cid, location_cid, position,"
					  " external_name as serial_number, external_full as friendly_name"
					  " FROM c_tank_map m, c_object_name v"
					  " WHERE m.storage_cid = v.object_cid"
					  " AND m.status <> :del AND m.location_cid = :locid" );
//					  " AND m.status not in (:off, :del) AND m.location_cid = :locid" );
		cQuery.setParam( "locid", location_id );
//		cQuery.setParam( "off", LCDbTankMap::OFFLINE );
	}
	cQuery.setParam( "del", LCDbTankMap::DELETED );
	results.clear();
	for( cQuery.open(); !cQuery.eof(); cQuery.next() ) {
		results.push_back( cQuery.getRecord() );
	}
}

//---------------------------------------------------------------------------

void StoreDAO::loadTankDetails( int storage_cid, std::vector<ROSETTA>& results )
{
	LQuery cQuery(LIMSDatabase::getCentralDb());
	cQuery.setSQL( "SELECT m.*, external_name as content_name, external_full as content_full"
				  " FROM c_tank_map m, c_object_name h"
				  " WHERE m.storage_cid = :sid AND m.tank_cid = h.object_cid" );
	cQuery.setParam( "sid", storage_cid );
	results.clear();
	for( cQuery.open(); !cQuery.eof(); cQuery.next() ) {
		results.push_back( cQuery.getRecord() );
	}
}

//---------------------------------------------------------------------------

bool StoreDAO::saveTankObject( ROSETTA& data )
{
	LCDbObjects & names = LCDbObjects::records();
	const LCDbObject * t = NULL;
	int tid = data.getIntDefault( "tank_cid", 0 );
	if( tid != 0 ) {
		t = names.findByID( tid );
	}
	AnsiString population = data.getString("content_name").c_str();
	if( t == NULL ) {
		t = names.find( population.c_str(), LCDbObject::STORAGE_POPULATION );
	}
	AnsiString description = data.getString("content_full").c_str();
	if( t != NULL && population.AnsiCompareIC( t->getName().c_str() ) == 0
	 && description.AnsiCompareIC( t->getDescription().c_str() ) == 0 ) {
		data.setInt( "tank_cid", t->getID() );
		return true;
	}
	LCDbObject tank( LCDbObject::STORAGE_POPULATION );
	if( t != NULL ) {
		tank = *t;
	}
	tank.setName( population.c_str() );
	tank.setDescription( description.c_str() );
	bool ok = tank.saveRecord( LIMSDatabase::getCentralDb() );
	data.setInt( "tank_cid", tank.getID() );
	return ok;
}

bool StoreDAO::savePhysicalStore( ROSETTA& data )
{
	LCDbObjects & names = LCDbObjects::records();
	const LCDbObject * t = NULL;
	int sid = data.getIntDefault( "storage_cid", 0 );
	if( sid != 0 ) {
		t = names.findByID( sid );
	}
	AnsiString vessel = data.getString( "serial_number" ).c_str();
	if( t == NULL ) {
		t = names.find( vessel.c_str(), LCDbObject::STORAGE_VESSEL );
	}
	AnsiString description = data.getString("Name").c_str();
	if( t != NULL && vessel.AnsiCompareIC( t->getName().c_str() ) == 0
	 && description.AnsiCompareIC( t->getDescription().c_str() ) == 0 ) {
		data.setInt( "storage_cid", t->getID() );
		return true;
	}
	LCDbObject sd( LCDbObject::STORAGE_VESSEL );
	if( t != NULL ) {
		sd = *t;
	}
	sd.setName( vessel.c_str() );
	sd.setDescription( description.c_str() );
	bool ok = sd.saveRecord( LIMSDatabase::getCentralDb() );
	data.setInt( "storage_cid", sd.getID() );
	return ok;
}

bool StoreDAO::saveTankMap( ROSETTA& data )
{
	TDateTime start( data.getTime("valid_from").outputTDateTime() );
	TDateTime end( data.getTime("valid_to").outputTDateTime() );
	LCDbTankMap tmap( start, end, data.getInt("status") );
	tmap.setStorageCID( data.getInt("storage_cid") );
	tmap.setTankCID( data.getInt("tank_cid") );
	tmap.setLayoutCID( data.getInt("layout_cid") );
	tmap.setStoreTypeCID( data.getInt("storage_type") );
	tmap.setLocationCID( data.getInt("location_cid") );
	tmap.setPosition( data.getInt("position") );
	tmap.setPopulation( data.getInt("shelf_number") );
	if( tmap.saveRecord( LIMSDatabase::getCentralDb() ) ) {
		data.setInt("map_cid", tmap.getID() );
		return true;
	} else {
		return false;
    }
}

bool StoreDAO::setLayoutAvailability( ROSETTA& data )
{
	LCDbTankMap map;
	const LCDbTankMap * ul = LCDbTankMaps::records().findByID( data.getInt("ID") );
	if( ul != NULL ) {
		map = *ul;
    }
	map.setStatus( data.getInt("status") );
	std::pair<TDateTime, TDateTime> dateRange;
	dateRange.first = data.getTime("valid_from").outputTDateTime();
	dateRange.second = data.getTime("valid_to").outputTDateTime();
	map.setValidDates( dateRange );
	return map.saveRecord( LIMSDatabase::getCentralDb() );
}

void StoreDAO::loadLayouts( int storage_cid, std::vector<ROSETTA>& results)
{
	LQuery cQuery( LIMSDatabase::getCentralDb() );
	results.clear();
	cQuery.setSQL( "SELECT m.*, l.external_name as layout_name, l.external_full as layout_full,"
					" n.external_name as label_name, n.external_full as label_full"
					" FROM c_tank_map m, c_object_name l, c_object_name n"
					" WHERE m.storage_cid = :sid"
					" AND m.tank_cid = n.object_cid AND m.rack_layout_cid = l.object_cid" );
	cQuery.setParam( "sid", storage_cid );
	for( cQuery.open(); !cQuery.eof(); cQuery.next() ) {
		results.push_back( cQuery.getRecord() );
	}
}

/* void StoreDAO::loadLayout( int map_cid, ROSETTA& result )
{
	std::string q = "SELECT * FROM c_tank_map m, c_object_name l, c_storage_details s ";
	q += " WHERE m.record_cid = :map_cid ";
	q += " AND m.rack_layout_cid = l.object_cid ";
	q += " AND m.storage_cid = s.storage_cid ";

	cQuery.setSQL( q.c_str() );
	cQuery.setParam( "map_cid", map_cid );

	result.clear();
	for( cQuery.open(); !cQuery.eof(); cQuery.next() )
	{
		result.setInt( "storage_cid", cQuery.readInt( "storage_cid" ) );
		result.setString( "layout_name", cQuery.readString("external_name") );
		result.setString( "layout_full", cQuery.readString("external_full") );
		result.setInt( "rack_layout_cid", cQuery.readInt( "rack_layout_cid" ) );
		result.setInt( "tank_cid", cQuery.readInt( "tank_cid" ) );
		result.setInt( "map_cid", cQuery.readInt( "record_cid" ) );
		result.setInt( "storage_type", cQuery.readInt( "storage_type" ) );
		result.setString( "serial_number", cQuery.readString("serial_number") );

		TDate date = cQuery.readDateTime("valid_from");
		result.setInt( "valid_from", (int)date );
		date = cQuery.readDateTime("valid_to");
		result.setInt( "valid_to", (int)date );
		result.setInt( "status", cQuery.readInt( "status" ) );
	}
}
*/

bool StoreDAO::saveLayout( ROSETTA& data )
{
	LCDbObject sections( LCDbObject::TANK_LAYOUT );
	sections.setName( data.getString("layout_name") );
	sections.setDescription( data.getString("layout_full") );
	bool ok = sections.saveRecord( LIMSDatabase::getCentralDb() );
	data.setInt("rack_layout_cid", sections.getID() );
	return ok;
}

void StoreDAO::loadSections( int layout_cid, std::vector<ROSETTA>& results)
{
	LQuery cQuery( LIMSDatabase::getCentralDb() );
	std::string q = "SELECT * FROM c_tank_layout "
					" WHERE rack_layout_cid = :id "
					" ORDER BY fill_order ";
	cQuery.setSQL( q );
	cQuery.setParam( "id", layout_cid );
	results.clear();
	for( cQuery.open(); !cQuery.eof(); cQuery.next() ) {
		results.push_back( cQuery.getRecord() );
	}
}

/*
void StoreDAO::loadSection( int map_id, std::string prefix, ROSETTA& result)
{
	result.clear();
//	LQuery cQuery(frmLogin->qCentral);

	std::string q = "SELECT l.rack_layout_cid, l.fill_order, l.first_rack, ";
	q += " l.last_rack, l.section_prefix, l.rack_capacity, m.tank_cid, m.record_cid ";
	q += " FROM c_tank_map m, c_storage_details s, c_tank_layout l ";
	q += " WHERE m.record_cid = :map_id ";
	q += " AND m.storage_cid = s.storage_cid ";
	q += " AND m.status <> :sts ";
	q += " AND 'now' BETWEEN m.valid_from AND m.valid_to ";
	q += " AND m.rack_layout_cid = l.rack_layout_cid ";
	q += " AND l.section_prefix like :prx ";
	q += " ORDER BY fill_order ";

	cQuery.setSQL( q.c_str() );
	cQuery.setParam( "map_id", map_id );
	cQuery.setParam( "sts", LCDbTankMap::DELETED );
	char partial[128];
	std::strncpy( partial, prefix.c_str(), 1 );
	partial[1] = '%';
	partial[2] = '\0';
	cQuery.setParam( "prx", partial );

	for( cQuery.open(); !cQuery.eof(); cQuery.next() )
	{
		result.setInt( "rack_layout_cid", cQuery.readInt( "rack_layout_cid" ) );
		result.setString( "section_prefix", cQuery.readString("section_prefix").c_str() );
		result.setInt( "fill_order", cQuery.readInt( "fill_order" ) );
		result.setInt( "first_rack", cQuery.readInt( "first_rack" ) );
		result.setInt( "last_rack", cQuery.readInt( "last_rack" ) );
		result.setInt( "rack_capacity", cQuery.readInt("rack_capacity") );
		result.setInt( "tank_cid", cQuery.readInt( "tank_cid" ) );
		result.setInt( "map_cid", cQuery.readInt( "record_cid" ) );
	}
}
*/

bool StoreDAO::saveSection( ROSETTA& data )
{
	LQuery cQuery( LIMSDatabase::getCentralDb() );
	LCDbSectionDef section;
	section.setTankLayoutCID( data.getInt("layout_cid") );
	section.setSectionPrefix( data.getString("Name") );
	section.setFillOrder( data.getInt("fill_order") );
	section.setFirstRack( data.getInt("first_rack") );
	section.setLastRack( data.getInt("last_rack") );
	section.setRackCapacity( data.getInt("rack_capacity") );
	if( section.saveRecord( cQuery ) ) {
		data.setInt("ID", section.getID() );
		return true;
	} else {
		return false;
    }
}

void StoreDAO::loadRacks( int tank_cid, std::vector<ROSETTA>& results, int type )
{
	LQuery cQuery( LIMSDatabase::getCentralDb() );
	if( type == 0 ) {
		cQuery.setSQL( "SELECT * FROM c_rack_number WHERE tank_cid = :id"
					  " ORDER BY position " );
	} else {
		cQuery.setSQL( "SELECT * FROM c_rack_number WHERE tank_cid = :id"
					  " and rack_type_cid = :rtid "
					  " ORDER BY position " );
		cQuery.setParam( "rtid", type );
	}
	cQuery.setParam( "id", tank_cid );
	results.clear();
	for( cQuery.open(); !cQuery.eof(); cQuery.next() ) {
		results.push_back( cQuery.getRecord() );
	}
}

void StoreDAO::loadRacks( const std::set< int > & rackCids, std::vector<ROSETTA>& results )
{
	LQuery cQuery( LIMSDatabase::getCentralDb() );
	results.clear();
	std::set< int >::const_iterator ri = rackCids.begin();
	if( ri != rackCids.end() ) {
		std::stringstream q;
		q << "SELECT * FROM c_rack_number where rack_cid in (" << *ri;
		while( ++ri != rackCids.end() ) {
			q << ',' << *ri;
		}
		q << ") ORDER BY position ";
		cQuery.setSQL( q.str() );
		for( cQuery.open(); !cQuery.eof(); cQuery.next() ) {
			results.push_back( cQuery.getRecord() );
		}
	}
}

bool StoreDAO::occupyRack( ROSETTA& data )
{
	LQuery cQuery( LIMSDatabase::getCentralDb() );
	std::string name = data.getString("Name");
	std::string location = data.getStringDefault("location","");
	int tank_cid = data.getIntDefault("tank_cid", -1);
	LCDbRack st( location, tank_cid, name );
	bool exists = st.findRack( cQuery );
	st.setSectionType(data.getInt("rack_type_cid"));
	st.setPosInTank(data.getInt("pos_in_tank"));
	st.setPosInSection(data.getInt("pos_in_section"));
	st.setBoxTypeID(data.getInt("box_type_cid"));
	st.setProjectCID(data.getInt("project_cid"));
	st.setEmptySlots(data.getInt("empty_slots"));
	bool saved = false;
	try {
		saved = exists ? st.updateRecord( cQuery ) : st.createRack( cQuery );
	} catch (...) {
		saved = false;	// e.g. if layout cannot be calculated
	}
	data.setInt("ID", st.getID() );
	data.setInt("position", st.getPosInTank() );
	return saved;
}

void StoreDAO::loadRackOccupancy( int rack_cid, std::set< int > & occupied )
{
	LQuery cQuery( LIMSDatabase::getCentralDb() );
	// include expected and confirmed boxes but not removed or deleted
	cQuery.setSQL( "select slot_position from c_slot_allocation"
			  " where status in (0,1,2,5,6,7) and rack_cid = :rid" );
	cQuery.setParam( "rid", rack_cid );
	for( cQuery.open(); !cQuery.eof(); cQuery.next() )
		occupied.insert( cQuery.readInt( 0 ) );
}

bool StoreDAO::loadBoxDetails( const std::string & barcode, const std::string & box_type, int proj_id, ROSETTA & result )
{
	LQuery cq( LIMSDatabase::getCentralDb() );
	std::string sql = "select b.box_cid, external_name, barcode, box_type_cid, box_capacity, b.project_cid,"
					" s.slot_cid, s.rack_cid, s.slot_position, s.retrieval_cid, s.status"
					" from c_box_name b left join c_slot_allocation s on b.box_cid = s.box_cid"
					" where b.barcode = :bar and b.project_cid in (0,:proj) and s.status not in (:rmv, :del)";
	if( box_type.empty() ) {
		cq.setSQL( sql );
	} else {
		cq.setSQL( sql + " and exists (select * from c_box_content c where c.external_name = :bt"
						" and b.box_type_cid = c.box_type_cid and c.project_cid in (0,:proj))" );
		cq.setParam( "bt", box_type );
	}
	cq.setParam( "bar", barcode );
	cq.setParam( "proj", proj_id );
	cq.setParam( "rmv", LCDbBoxStore::REMOVED );
	cq.setParam( "del", LCDbBoxStore::DELETED );
	if( cq.open() ) {
		result = cq.getRecord();
		return true;
	} else {
		return false;
    }
}

bool StoreDAO::loadBoxDetails( int box_id, ROSETTA & result )
{
	LQuery cq( LIMSDatabase::getCentralDb() );
	cq.setSQL( "select b.box_cid, b.external_name, barcode, box_type_cid, box_capacity, b.project_cid,"
			  " s.slot_cid, s.rack_cid, s.slot_position, retrieval_cid, s.status"
			  " from c_box_name b left join c_slot_allocation s"
			  " on b.box_cid = s.box_cid and s.status not in ( :rmv, :del )"
			  " where b.box_cid = :bid" );
	cq.setParam( "bid", box_id );
	cq.setParam( "rmv", LCDbBoxStore::REMOVED );
	cq.setParam( "del", LCDbBoxStore::DELETED );
	if( cq.open() ) {
		result = cq.getRecord();
		return true;
	} else {
        return false;
    }
}

void StoreDAO::loadBoxes( int rack_id, std::vector<ROSETTA>& results)
{
	LQuery cq( LIMSDatabase::getCentralDb() );
	cq.setSQL( "select b.box_cid, b.external_name, barcode, box_type_cid, box_capacity, b.project_cid,"
			  " s.slot_cid, s.rack_cid, s.slot_position, retrieval_cid, s.status"
			  " from c_slot_allocation s, c_box_name b"
			  " where b.box_cid = s.box_cid and s.rack_cid = :rid and s.status not in ( :rmv, :del )" );
	cq.setParam( "rid", rack_id );
	cq.setParam( "rmv", LCDbBoxStore::REMOVED );
	cq.setParam( "del", LCDbBoxStore::DELETED );
	for( cq.open(); !cq.eof(); cq.next() ) {
		results.push_back( cq.getRecord() );
	}
}

void StoreDAO::loadBoxesByJobID( int job_id, int proj_id, bool lhs, std::vector<ROSETTA>& results )
{
	std::string sql = lhs ? "SELECT s1.*" : "SELECT s2.*";
	sql += ", box_capacity, box_type_cid, barcode, external_name"
		  " from box_store s1, box_name b, box_store s2"
		  " where b.box_cid = s1.box_cid and b.box_cid = s2.box_cid"
		  " and s1.retrieval_cid = :job and s2.retrieval_cid = 0"
		  " ORDER BY slot_position";
	LQuery pq( LIMSDatabase::getProjectDb( proj_id ) );
	pq.setSQL( sql );
	pq.setParam( "job", job_id );
	for( const ROSETTA & box : pq ) {
		results.push_back( box );
		results.back().setInt( "project_cid", proj_id );
	}
}

bool StoreDAO::addBoxToLHSJobList( ROSETTA& data )
{
	int id = data.getInt("record_id");
	if( id == -1 ) id = 0;
	int box_id = data.getInt("ID");
	int rack_id = data.getInt("rack_cid");
	int tank_id = data.getInt("tank_cid");
	int proj_id = data.getInt("project_cid");
	int pos = data.getInt("slot_position");
	std::string rack_name = data.getString("rack_name");
	int retrieval_cid = data.getInt("retrieval_cid");

	LQuery ddbq = Util::projectQuery( proj_id, true );
	LCDbBoxStore st( id, tank_id, rack_name, pos );
	st.setRackID( rack_id );
	st.setBox( proj_id, box_id );
	return st.setJobRef( ddbq, retrieval_cid, LCDbBoxStore::MOVE_EXPECTED );
}

/*
bool StoreDAO::addBoxToRHSJobList( ROSETTA& data )
{
	int rack_id = data.getInt("rack_cid");
	int tank_id = data.getInt("tank_cid");
	int proj_id = data.getInt("project_cid");
	int pos = data.getInt("slot_position");
	std::string rack_name = data.getString("rack_name");
	int retrieval_cid = data.getInt("retrieval_cid");
	int id = data.getInt("record_id");
	if( id == -1 ) id = 0;
	int box_cid = data.getInt("ID");

	LCDbBoxStore st( id, tank_id, rack_name, pos );
	st.setRackID(rack_id);
//	st.setJobID(retrieval_cid);
	st.setStatus( LCDbBoxStore::EXPECTED );
	st.setBox(proj_id, box_cid);
	LQuery cq( LIMSDatabase::getCentralDb() );
	LQuery pq( LIMSDatabase::getProjectDb( proj_id ) );
	return st.saveRecord( pq, cq );
}
*/

bool StoreDAO::updateBox( ROSETTA& data )
{
	int rack_id = data.getInt("rack_cid");
	int tank_id = data.getInt("tank_cid");
	int proj_id = data.getInt("project_cid");
	int pos = data.getInt("slot_position");
	std::string rack_name = data.getString("rack_name");
	int box_cid = data.getInt("ID");
	int id = data.getIntDefault("record_id",-1);
	if( id == -1 ) id = 0;
	short status = data.getInt("status");

	LCDbBoxStore st( id, tank_id, rack_name, pos );
	st.setRackID(rack_id);
	st.setBox( proj_id, box_cid );
	st.setStatus( status );
	LQuery cq( LIMSDatabase::getCentralDb() );
	LQuery pq( LIMSDatabase::getProjectDb( proj_id ) );
	return st.saveRecord( pq, cq );
}

/*
bool StoreDAO::signoffBox( ROSETTA& data )
{
	int rack_id = data.getInt("rack_cid");
	int tank_id = data.getInt("tank_cid");
	int proj_id = data.getInt("project_cid");
	int pos = data.getInt("slot_position");
	std::string rack_name = data.getString("rack_name");
	int retrieval_cid = data.getInt("retrieval_cid");
	int box_cid = data.getInt("ID");
	int record_id = data.getInt("record_id");

	const LCDbObject & tank = LCDbObjects::records().get( tank_id );
	std::string destination = "to " + tank.getName() + ", " + rack_name;

	LCDbBoxStore st( record_id, tank_id, rack_name.c_str(), pos );
	st.setRackID( rack_id );
	st.setBox( proj_id, box_cid );
	st.setJobID( retrieval_cid );
	LQuery pq( LIMSDatabase::getProjectDb( proj_id ) );
	return st.setJobRef( pq, 0, LCDbBoxStore::SLOT_CONFIRMED );
}
*/

void StoreDAO::loadBoxHistory( int box_id, int proj_id, std::vector<ROSETTA>& results )
{
	LQuery ddbq = Util::projectQuery( proj_id, true );
	ddbq.setSQL( "SELECT s.*, m.*, n.external_full as vessel, r.external_name as rack"
				  " FROM box_store s, c_rack_number r, c_tank_map m, c_object_name n"
				  " WHERE s.box_cid = :bid AND s.rack_cid = r.rack_cid"
				  " and r.tank_cid = m.tank_cid and m.storage_cid = n.object_cid"
				  " ORDER BY s.time_stamp" );
	ddbq.setParam( "bid", box_id );
	results.clear();
	for( const ROSETTA & record : ddbq ) {
		results.push_back( record );
		results.back().setInt( "project_cid", proj_id );
	}
}

void StoreDAO::loadSamples( int box_id, int proj_id, std::vector<ROSETTA>& results )
{
	std::string source_field;
	const LPDbDescriptor * descrip = LPDbDescriptors::records( proj_id ).findByName( "source_name" );
	if( descrip != NULL ) {
		source_field = descrip->getSpecimenField();
	}
	if( source_field.empty() ) {
		source_field = "external_name";
	}
	results.clear();
	LQuery pQuery( LIMSDatabase::getProjectDb( proj_id ) );
	std::string q = "SELECT s.*, c.sample_id, cryovial_barcode, aliquot_type_cid,"
			" sp.barcode as source_barcode, sp." + source_field + " as source_name"
			" FROM cryovial_store s, cryovial c, specimen sp "
			" WHERE s.box_cid = :bid AND s.status in (0,1,2) "	// allocated, confirmed, expected
			" AND s.cryovial_id = c.cryovial_id AND c.sample_id = sp.sample_id";
	pQuery.setSQL( q );
	pQuery.setParam( "bid", box_id );
	results.clear();
	for( const ROSETTA & record : pQuery ) {
		results.push_back( record );
		results.back().setInt( "project_cid", proj_id );
	}
}

void StoreDAO::loadStorageHistory( int cryovial_id, int proj_id, std::vector<ROSETTA>& results )
{
	LQuery pQuery( LIMSDatabase::getProjectDb( proj_id ) );
	pQuery.setSQL( "SELECT s.*, b.external_name as box_name"
					" FROM cryovial_store s, box_name b "
					" WHERE s.cryovial_id = :id AND s.box_cid = b.box_cid" );
	pQuery.setParam( "id", cryovial_id );
	results.clear();
	for( const ROSETTA & record : pQuery ) {
		results.push_back( record );
		results.back().setInt( "project_cid", proj_id );
	}
}

bool StoreDAO::loadAnalysisHistory( const std::string & cryovial_barcode, int aliquot_type_id, int proj_id, std::vector<ROSETTA>& results )
{
	results.clear();
	const LCDbObject * aliquot = LCDbObjects::records().findByID( aliquot_type_id );
	const LPDbDescriptor * descrip = LPDbDescriptors::records(proj_id).findByName( "aliquot_type" );
	if( aliquot == NULL || descrip == NULL ) {
		return false;
	}
	std::string q = "SELECT date_trunc('day',r.time_stamp) as when, count(result_id) as results"
		" FROM specimen s, result r WHERE s.sample_id = r.sample_id "
		" AND " + descrip->getSpecimenField() + " = :atn AND barcode = :bar"
		" GROUP BY date_trunc('day',r.time_stamp)";

	LQuery pQuery( LIMSDatabase::getProjectDb( proj_id ) );
	pQuery.setSQL( q.c_str() );
	pQuery.setParam( "atn", aliquot->getName() );
	pQuery.setParam( "bar", cryovial_barcode );
	for( pQuery.open(); !pQuery.eof(); pQuery.next() ) {
		results.push_back( pQuery.getRecord() );
	}
	return !results.empty();
}

//---------------------------------------------------------------------------

void StoreDAO::loadAliquotTypes( std::vector<ROSETTA>& results )
{
	results.clear();
	const LCDbObjects& objs = LCDbObjects::records();
	ROSETTA result;
	for( Range<LCDbObject> ul = objs; ul.isValid(); ++ ul ) {
		if( ul->isActive() && ul->getObjectType() == LCDbObject::Category(LCDbObject::ALIQUOT_TYPE) ) {
			result.setInt( "object_cid", ul->getID() );
			result.setString( "external_name", ul->getDescription().c_str() );
			results.push_back(result);
		}
	}
}

//---------------------------------------------------------------------------

bool StoreDAO::loadCryovials( short source, const std::string & id, int primary, int secondary, int proj_id, std::vector<ROSETTA>& results )
{
	std::stringstream q;
	q << "SELECT c.cryovial_id, c.aliquot_type_cid, c.cryovial_barcode, c.sample_id,"
			" sp.barcode, cs.box_cid, b.external_name as box_name, cs.cryovial_position"
			" FROM cryovial_store cs, cryovial c, specimen sp, box_name b"
			" WHERE cs.status in (0, 1)" 	// expected or confirmed
			" AND cs.cryovial_id = c.cryovial_id AND c.sample_id = sp.sample_id "
			" AND b.box_cid = cs.box_cid";
	switch( source ) {
		case 0: // sample
			q << " AND sp.barcode = '" << id << '\'';
			break;

		case 1: // cryovial
			q << " AND c.cryovial_barcode = '" << id << '\'';
			break;

		case 2: // box
			q << " AND '" << id << "' in (b.external_name, b.barcode)";
			break;

		default:
			return false;	// filter missing
	}

	if( primary != 0 ) {
		if( secondary == 0 ) {
			q << " AND c.aliquot_type_cid = " << primary;
		} else {
			q << " AND c.aliquot_type_cid in (" << primary << ',' << secondary << ')';
		}
	} else {
		if( secondary != 0 ) {
			q << " AND c.aliquot_type_cid = " << secondary;
		}
	}

	results.clear();
	LQuery pQuery( LIMSDatabase::getProjectDb( proj_id ) );
	pQuery.setSQL( q.str() );
	if( !pQuery.open() ) {
		return false;
	}
	do {
		results.push_back( pQuery.getRecord() );
		results.back().setInt( "project_cid", proj_id );
		pQuery.next();
	} while( !pQuery.eof() );
	return true;
}

//---------------------------------------------------------------------------

bool StoreDAO::addToRetrieval( int jobID, int cryovial_id, int proj_id, int box_cid, short pos ) {
	std::stringstream q1;
	q1 << "UPDATE cryovial_store SET status = 2," 		// removal expected
	  << " retrieval_cid = " << jobID
	  << " WHERE status in (0,1) " 						// current position
	  << " AND cryovial_id = " << cryovial_id;
	LQuery pQuery( LIMSDatabase::getProjectDb( proj_id ) );
	pQuery.setSQL( q1.str() );
	if( !pQuery.execSQL() ) {
		return false;
	}
	std::stringstream q2;
	q2 << "INSERT INTO cryovial_store"
		" (record_id, cryovial_id, box_cid, cryovial_position, time_stamp, status, note_exists, process_cid)"
		" VALUES (next value for id_sequence, " << cryovial_id << ", " << box_cid << ", " << pos
	   << ", 'now', 0,"		// space allocated in new box
	   << " 0, " << LCDbAuditTrail::getCurrent().getProcessID() << ")";
	pQuery.setSQL( q2.str() );
	return pQuery.execSQL();
}

//---------------------------------------------------------------------------

bool StoreDAO::findBox( int box_id, ROSETTA & result )
{
	LQuery cq( LIMSDatabase::getCentralDb() );
	cq.setSQL( "SELECT v.external_full as vessel_name, shelf_number, r.position as rack_pos, m.status,"
			" r.external_name as structure, slot_position, m.position as tank_pos, l.external_name as site_name"
			" FROM c_slot_allocation bs, c_rack_number r, c_tank_map m, c_object_name v, c_object_name l "
			" WHERE box_cid = :bid AND bs.status in (1, 2, 6)" 		// current box position
			" AND bs.rack_cid = r.rack_cid AND r.tank_cid = m.tank_cid AND m.storage_cid = v.object_cid"
			" AND m.location_cid = l.object_cid AND m.status<>99 AND v.status<>99 AND l.status<>99" );
	cq.setParam( "bid", box_id );
	bool found = false;
	for( const ROSETTA & record : cq ) {
		// fix Chris's bug: there should be one mapping and that should be in use
		// if there is one that's off-line, use it if there isn't anything better
		if( !found || result.getInt( "status" ) == LCDbTankMap::OFFLINE ) {
			result = record;
			found = true;
		} else if( record.getInt( "status" ) != LCDbTankMap::OFFLINE ) {
			return false;
		}
	}
	return found;
}

//---------------------------------------------------------------------------

