//---------------------------------------------------------------------------

#include <vcl.h>
#include <set>
#include <map>
#include <sstream>

#include "Inventory.h"
#include "StoreDAO.h"
#include "LCDbObject.h"
#include "LCDbTankMap.h"
#include "LDbBoxType.h"
#include "LPDbBoxes.h"
#include "LCDbTankLayout.h"
#include "LCDbJob.h"
#include "LDbBoxSize.h"
#include "LDbBoxStore.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

IPart::IPart( ) {
	id = -1;
	capacity = position = childCount = -1;
	mapped2 = NULL;
	parent = NULL;
	state = UnknownState;
	type = UnknownType;
}

IPart::~IPart( ) {
	clearList( );
}

void IPart::clearList( ) {
	for( unsigned i = 0; i < partlist.size( ); i++ ) {
		delete partlist[ i ];
	}
	discardList( );
}

void IPart::discardList( ) {
	partlist.clear( );
	childCount = -1;
}

IPart::Availability IPart::availability( ) const {
	switch( childCount ) {
		case -1:
			return UNKNOWN;
		case 0:
			return IS_EMPTY;
		default:
			return IS_AVAILABLE;
	}
}

void IPart::addToList( IPart *part ) {
	partlist.push_back( part );
	childCount = partlist.size( );
	part->setParent( this );
	sortChildren( );
}

void IPart::remove( int row ) {
	if( row >= 0 && row < ( int )partlist.size( ) ) {
		delete partlist[ row ];
		partlist.erase( partlist.begin( ) + row );
		childCount = partlist.size( );
	}
}

std::auto_ptr< ROSETTA >IPart::getProperties( ) {
	std::auto_ptr< ROSETTA >r( new ROSETTA( ) );
	r->setString( getTypeStr( ), name );
	if( mapped2 != NULL && mapped2->id != this->id ) {
		r->setString( "Mapped_to", mapped2->name );
	}
	std::map< int, std::string >::const_iterator hi;
	for( hi = history.begin( ); hi != history.end( ); ++ hi ) {
		AnsiString date = TDate(hi->first).FormatString( "d_mmmm_yyyy" );
		r->setString( date.c_str(), hi->second );
	}
	return r;
}

std::auto_ptr< ROSETTA >IPart::getObjectData( ) {
	std::auto_ptr< ROSETTA >r( new ROSETTA( ) );
	r->setInt( "ID", id );
	r->setString( "Name", name );
	return r;
}

IPart::IPart( int p_id, std::string p_name ) {
	id = p_id;
	name = p_name;
	mapped2 = NULL;
	type = UnknownType;
	parent = NULL;
	if( id == 0 || id == -1 ) {
		state = NewState;
	} else {
		state = SyncState;
	}
	childCount = -1;
}

void Sites::populate( ) {
	if( childCount > 0 ) {
		return;
	}
	StoreDAO dao;
	std::vector< ROSETTA >results;
	dao.loadSites( results );
	for( unsigned i = 0; i < results.size(); i++ ) {
		Site *s = new Site( results[ i ] );
		s->setParent( this );
		partlist.push_back( s );
	}
	sortChildren();
	Site * s = new Site( 0, "(currently off line)" );
	s->setParent( this );
	partlist.push_back( s );
	childCount = partlist.size( );
	for( int i = 0; i < childCount; i ++ ) {
		partlist[i]->setPosition( i );
	}
}

std::auto_ptr< ROSETTA >Sites::getProperties( ) {
	std::auto_ptr< ROSETTA >r = IPart::getProperties( );
	if( childCount > 0 ) {
		r->setInt( "sites", childCount );
	}
	return r;
}

Site::Site( int p_id, std::string p_name ) : IPart( p_id, p_name ) {
	type = SiteType;
	state = NewState;
}

Site::Site( const ROSETTA &data ) {
	id = data.getIntDefault( "object_cid", -1 );
	if( data.isString( "external_name" ) ) {
		name = data.getString( "external_name" );
	}
	if( data.isString( "external_full" ) ) {
		fullname = data.getString( "external_full" );
	}
	mapped2 = NULL;
	parent = NULL;
	state = SyncState;
	type = SiteType;
	childCount = -1;
}

std::auto_ptr< ROSETTA >Site::getProperties( ) {
	std::auto_ptr< ROSETTA >r = IPart::getProperties( );
	r->setString( "full_name", fullname );
	if( childCount > 0 ) {
		r->setInt( "storage_vessels", childCount );
	}
	return r;
}

std::auto_ptr< ROSETTA >Site::getObjectData( ) {
	std::auto_ptr< ROSETTA >r = IPart::getObjectData( );
	r->setString( "fullname", fullname );
	return r;
}

bool Site::operator<( const IPart& rhs ) const {
	return getName() < rhs.getName();
}

void Site::populate( ) {
	if( childCount >= 0 || id == -1 ) {
		return;
	}
	std::vector< ROSETTA >results;
	StoreDAO().loadTanks( id, results );
	childCount = results.size();
	for( int i = 0; i < childCount; i++ ) {
		Tank *p = new Tank( results[ i ] );
		p->loadTankDetails();
		p->setParent( this );
		if( id == 0 ) {
			p->setPosition( 0 );	// no position if no current site
		}
		partlist.push_back( p );
	}
	capacity = 0;
	sortChildren( );
}

bool Site::save( ) {
	StoreDAO dao;
	std::auto_ptr< ROSETTA >data = getObjectData( );
	bool result = dao.saveSite( *data );
	if( result ) {
		state = SyncState;
		id = data->getIntDefault( "ID", -1 );
	}
	return result;
}

Tank::Tank( ) : IPart( ) {
	state = NewState;
	type = TankType;
	tank_cid = location_cid = layout_cid = map_cid = store_type_cid = 0;
	status = -1;
	shelf_number = 0;
}

Tank::Tank( int p_id, std::string p_name ) : IPart( p_id, p_name ) {
	state = NewState;
	type = TankType;
	tank_cid = location_cid = layout_cid = map_cid = store_type_cid = 0;
	status = -1;
	shelf_number = 0;
}

Tank::Tank( const ROSETTA &data ) {
	status = -1;
	setFields( data );
	mapped2 = NULL;
	parent = NULL;
	state = SyncState;
	type = TankType;
	childCount = -1;
}

void Tank::setFields( const ROSETTA &data ) {
	if( data.isInt( "storage_cid" ) ) {
		id = data.getInt( "storage_cid" );
	}
	if( data.isString( "friendly_name" ) ) {
		name = data.getString( "friendly_name" );
	}
	if( data.isString( "serial_number" ) ) {
		srlno = data.getString( "serial_number" );
	}
	if( data.isString( "content_name" ) ) {
		label = data.getString( "content_name" );
	}
	if( data.isString( "content_full" ) ) {
		description = data.getString( "content_full" );
	}
	if( name.empty( ) ) {
		name = srlno.empty() ? label : srlno;
	}
	if( data.isInt( "map_cid" ) ) {
		map_cid = data.getInt( "map_cid" );
	} else if( data.isInt( "record_cid" ) ) {
		map_cid = data.getInt( "record_cid" );
	}
	if( data.isInt( "tank_cid" ) ) {
		tank_cid = data.getInt( "tank_cid" );
	}
	if( data.isInt( "location_cid" ) ) {
		location_cid = data.getInt( "location_cid" );
	}
	if( data.isInt( "position" ) ) {
		position = data.getInt( "position" );
	}
	if( data.isInt( "shelf_number" ) ) {
		shelf_number = data.getInt( "shelf_number" );
	}
	if( data.isInt( "storage_type" ) ) {
		store_type_cid = data.getInt( "storage_type" );
	}
	if( data.isInt( "rack_layout_cid" ) ) {
		layout_cid = data.getInt( "rack_layout_cid" );
	} else if( data.isInt( "layout_cid" ) ) {
		layout_cid = data.getInt( "layout_cid" );
	}
	if( data.isInt( "status" ) ) {
		status = data.getInt( "status" );
	}
}

// merge tank mapping data into vessel details for editting
void Tank::setFields( const Layout &data ) {
	tank_cid = data.getTank_cid();
	store_type_cid = data.getStore_type_cid();
	map_cid = data.getID();
	layout_cid = data.getLayout_cid();
	label = data.getPopulationName();
	description = data.getPopulationDescription();
	shelf_number = data.getPosition();
	if( data.availability() == Layout::IS_AVAILABLE ) {
		status = LCDbTankMap::RECORD_IN_USE;
	} else {
		status = LCDbTankMap::OFFLINE;
	}
}

IPart::Availability Tank::availability( ) const {
	if( location_cid != 0 && status <= LCDbTankMap::RECORD_IN_USE ) {
		return IS_AVAILABLE;
	} else {
		return UNAVAILABLE;
	}
}

void Tank::takeOffline( ) {
	const std::vector< IPart* >&layouts = getList( );
	for( unsigned i = 0; i < layouts.size( ); i++ ) {
		Layout *lay = ( Layout* ) layouts[ i ];
		if( lay->availability( ) == Layout::IS_AVAILABLE ) {
			lay->setAvailability( false );
			lay->saveAvailability( );
		}
	}
	status = LCDbTankMap::OFFLINE;
}

void Tank::setAvailability( bool online ) {
	status = online ? LCDbTankMap::RECORD_IN_USE : LCDbTankMap::OFFLINE;
}

std::auto_ptr< ROSETTA >Tank::getProperties( ) {
	std::auto_ptr< ROSETTA >r = IPart::getProperties( );
	r->setString( "serial_number", srlno );
	const LCDbObject * type = LCDbObjects::records().findByID( store_type_cid );
	if( type != NULL ) {
		r->setString( "storage_type", type->getDescription() );
	}
	r->setString( "on_line", availability( ) == IS_AVAILABLE ? "Yes" : "No" );
	return r;
}

std::string Tank::getName() const {
	if( position > 0 ) {
		char buff[ 100 ];
		std::sprintf( buff, "%d: %s", position, name.c_str() );
		return buff;
	} else {
		return name;
	}
}

std::string Tank::getFullName() const {
	std::string result = getName();
	if( parent != NULL ) {
		result = parent->getName() + " " + result;
	}
	return result;
}

bool Tank::operator<( const IPart& rhs ) const {
	if( getPosition() == rhs.getPosition() ) {
		return getName() < rhs.getName();
	} else {
		return getPosition() < rhs.getPosition();
	}
}

std::auto_ptr< ROSETTA >Tank::getObjectData( ) {
	std::auto_ptr< ROSETTA >r = IPart::getObjectData( );
	r->setInt( "tank_cid", tank_cid );
	r->setInt( "position", position );
	r->setInt( "storage_type", store_type_cid );
	r->setString( "content_name", label );
	r->setString( "content_full", description );
	r->setString( "serial_number", srlno );
	r->setInt( "location_cid", location_cid );
	r->setInt( "layout_cid", layout_cid );
	r->setInt( "shelf_number", shelf_number );
	r->setInt( "status", status );
	return r;
}

void Tank::populate( ) {
	if( childCount >= 0 || id == -1 ) {
		return;
	}
	std::vector< ROSETTA >results;
	StoreDAO().loadLayouts( id, results );
	childCount = ( int )results.size( );
	for( int i = 0; i < childCount; i++ ) {
		Layout *p = new Layout( results[ i ] );
		p->setParent( this );
		partlist.push_back( p );
	}
	sortChildren( );
}

void Tank::loadTankDetails( ) {
	const LCDbObjects & names = LCDbObjects::records();
	bool online = false;
	std::vector< ROSETTA > mapping;
	StoreDAO().loadTankDetails( id, mapping );
	int best = -1;
	for( unsigned i = 0; i < mapping.size(); i++ ) {
		if( mapping[ i ].getInt( "status" ) != LCDbTankMap::DELETED ) {
			// remember first valid mapping for first shelf
			Layout population( mapping[ i ] );
			if( best < 0 || population < Layout( mapping[ best ] ) ) {
				best = i;
			}
			if( population.availability() == IPart::Availability::IS_AVAILABLE ) {
                online = true;
            }
			// add to history, but only one record for each day
			std::stringstream detail;
			const LCDbObject * site = names.findByID( population.getLocation_cid() );
			if( site != NULL ) {
				detail << site->getName() << ", ";
			}
			detail << "position " << population.getTankPos();
			const LCDbObject * type = names.findByID( population.getStore_type_cid() );
			if( type != NULL ) {
				detail << ": " << type->getName();
			}
			history[ population.getValidFrom() ] = detail.str( );
		}
	}
	if( best >= 0 ) {
		setFields( mapping[ best ] );
	}
	setAvailability( online );
}

bool Tank::save( ) {
	StoreDAO dao;
	std::auto_ptr< ROSETTA >data = getObjectData( );
	if( dao.savePhysicalStore( *data ) ) {
		id = data->getInt( "storage_cid" );
	} else {
		return false;
	}

	if( dao.saveTankObject( *data ) ) {
		tank_cid = data->getInt( "tank_cid" );
	} else {
		return false;
	}

	Layout *l = ( Layout* ) partlist[ 0 ];
//	l->setStorage_cid( id );
	l->setTank_cid( tank_cid );
	if( l->isNew( ) ) {
		if( l->save( ) ) {
			layout_cid = l->getLayout_cid( );
		} else {
			return false;
		}
	}

	data->setInt( "layout_cid", l->getLayout_cid( ) );
	data->setTime( "valid_from", l->getValidFrom());
	data->setTime( "valid_to", l->getValidUntil());
	if( !dao.saveTankMap( *data ) ) {
		return false;
	}

	map_cid = data->getInt( "map_cid" );
	state = SyncState;
	return true;
}

bool Tank::modifySerial( ) {
	StoreDAO dao;
	location_cid = this->parent->getID( );
	std::auto_ptr< ROSETTA >data = getObjectData( );
	bool result = dao.savePhysicalStore( *data );
	if( result ) {
		state = SyncState;
		id = data->getInt( "storage_cid" );
		Layout *l = ( Layout* )partlist[ 0 ];
		data->setInt( "layout_cid", l->getID( ) );
		if( !dao.saveTankMap( *data ) ) {
			return false;
		}
		map_cid = data->getInt( "map_cid" );
//		l->setID( map_cid );
	}
	return result;
}

bool Tank::modifyLayout( ) {
	StoreDAO dao;
	std::auto_ptr< ROSETTA >data = getObjectData( );
	Layout *l = ( Layout* )partlist[ 0 ];
//	l->setStorage_cid( storage_cid );
	l->setTank_cid( tank_cid );
	if( l->isNew( ) && !l->save( ) ) {
		return false;
	}
	data->setInt( "layout_cid", l->getID( ) );
	if( !dao.saveTankMap( *data ) ) {
		return false;
	}
	map_cid = data->getInt( "map_cid" );
//	l->setID( map_cid );
	state = SyncState;
	return true;
}

void Tank::setContent( const std::string& name, const std::string& full ) {
	label = name;
	description = full;
	tank_cid = 0;
}

Layout::Layout( ) : IPart( ) {
	state = NewState;
	type = LayoutType;
	store_type_cid = tank_cid = layout_cid = -1;
}

Layout::Layout( int p_id, std::string p_name ) : IPart( p_id, p_name ) {
	state = NewState;
	type = LayoutType;
	store_type_cid = tank_cid = layout_cid = -1;
}

Layout::Layout( const ROSETTA &data ) {
	if( data.isInt( "map_cid" ) ) {
		id = data.getInt( "map_cid" );
	} else if( data.isInt( "record_cid" ) ) {
		id = data.getInt( "record_cid" );
	} else {
		id = -1;
	}
	if( data.isInt( "rack_layout_cid" ) ) {
		layout_cid = data.getInt( "rack_layout_cid" );
	} else if( data.isInt( "layout_cid" ) ) {
		layout_cid = data.getInt( "layout_cid" );
	} else {
		layout_cid = -1;
	}
	if( data.isString( "layout_name" ) ) {
		layout_name = data.getString( "layout_name" );
	}
	if( data.isString( "layout_full" ) ) {
		layout_description = data.getString( "layout_full" );
	}
	if( data.isString( "label_name" ) ) {
		name = data.getString( "label_name" );
	}
	if( data.isString( "label_full" ) ) {
		fullname = data.getString( "label_full" );
	}
	if( data.isDate( "valid_from" ) ) {
		valid_from = data.getDate( "valid_from" ).outputTDateTime();
	} else if( data.isTime( "valid_from" ) ) {
		valid_from = data.getTime( "valid_from" ).outputTDateTime();
	}
	if( data.isDate( "valid_to" ) ) {
		valid_to = data.getDate( "valid_to" ).outputTDateTime();
	} else if( data.isTime( "valid_to" ) ) {
		valid_to = data.getTime( "valid_to" ).outputTDateTime();
	}
	status = data.getIntDefault( "status", 0 );
	store_type_cid = data.getIntDefault( "storage_type", -150 );
	tank_position = data.getIntDefault( "position", -1 );
	tank_cid = data.getIntDefault( "tank_cid", -1 );
	location_cid = data.getIntDefault( "location_cid", 0 );
	position = data.getIntDefault( "shelf_number", -1 );

	mapped2 = NULL;
	parent = NULL;
	state = SyncState;
	type = LayoutType;
	childCount = -1;
}

std::auto_ptr< ROSETTA >Layout::getProperties( ) {
	std::auto_ptr< ROSETTA >r = IPart::getProperties( );
	if( position != 0 ) {
		r->setInt( "shelf_number", position );
	}
	if( store_type_cid != 0 ) {
		const LCDbObject *st = LCDbObjects::records().findByID( store_type_cid );
		if( st != NULL ) {
			r->setString( "storage_type", st->getDescription( ) );
		}
	}
	r->setString( "layout", layout_description );
	r->setString( "on_line", availability( ) == IS_AVAILABLE ? "Yes" : "No" );
	r->setString( "valid", getDateRange() );
	return r;
}

std::auto_ptr< ROSETTA >Layout::getObjectData( ) {
	std::auto_ptr< ROSETTA >r = IPart::getObjectData( );
	r->setString( "layout_name", layout_name );
	r->setString( "layout_full", layout_description );
	r->setInt( "store_type_cid", store_type_cid );
	r->setInt( "tank_cid", tank_cid );
	r->setInt( "status", status );
	r->setTime( "valid_from", valid_from );
	r->setTime( "valid_to", valid_to );
	return r;
}

std::string Layout::getDateRange() const {
	AnsiString result;
	if( valid_from >= EPOCH_START && valid_from < EPOCH_END ) {
		result = valid_from.DateString();
		if( valid_to >= valid_from && valid_to < EPOCH_END ) {
			result = result + " - " + valid_to.DateString();
		} else if( valid_from > Now() ) {
			result = "From " + result;
		} else {
			result = "Since " + result;
		}
	} else {
		if( valid_to >= EPOCH_START && valid_to < EPOCH_END ) {
			result = "Until " + valid_to.DateString();
		} else if( status == LCDbTankMap::DELETED ) {
			result = "Former content";
		} else {
			result = "Current content";
		}
	}
	return result.c_str();
}

std::string Layout::getName() const {
	std::stringstream result;
	if( position > 0 ) {
		result << position << ": ";
	}
	result << getDateRange() << ": " << getLayoutName();
	return result.str();
}

std::string Layout::getFullName() const {
	return getParent()->getFullName() + ", " + getLayoutName();
}

IPart::Availability Layout::availability( ) const {
	if( status == LCDbTankMap::NEW_RECORD || status == LCDbTankMap::RECORD_IN_USE ) {
		if( valid_from >= EPOCH_END || valid_from < Now() ) {
			if( valid_to < valid_from || valid_to > Now() ) {
				return IS_AVAILABLE;
			}
		}
	}
	return UNAVAILABLE;
}

// sort order for Layouts: by shelf, then status, then start date
bool Layout::operator<( const IPart& rhs ) const {
	if( rhs.getType() != LayoutType ) {
		return type < rhs.getType();
	}
	if( getPosition() != rhs.getPosition() ) {
		return getPosition() < rhs.getPosition();
	}
	Availability myA = availability(), rhsA = ((Layout*)(&rhs))->availability();
	if( myA != rhsA ) {
		return myA == IS_AVAILABLE && rhsA == UNAVAILABLE;
	}
	return valid_from < ((Layout*)(&rhs))->getValidFrom();
}

// fill in dates if necessary
void Layout::setAvailability( bool online ) {
	TDateTime change = Now();
	if( valid_from <= EPOCH_START || valid_from > change ) {
		valid_from = change;
	}
	if( online ) {
		status = LCDbTankMap::RECORD_IN_USE;
		valid_to = EPOCH_END;
	} else {
		status = LCDbTankMap::OFFLINE;
		if( valid_to <= EPOCH_START || valid_to > change ) {
			valid_to = change;
		}
	}
}

void Layout::setLayoutType( const std::string & p_name, const std::string & p_desc ) {
	layout_name = p_name;
	layout_description = p_desc;
	// layout_cid = 0;
}

void Layout::populate( ) {
	if( childCount >= 0 ) {
		return;
	}
	StoreDAO dao;
	// read existing racks in this "population" - sections don't exist in the database
	std::vector< Rack * > allRacks;
	if( tank_cid != 0 && tank_cid != -1 ) {
		std::vector< ROSETTA > racks;
		dao.loadRacks( tank_cid, racks );
		for( std::vector< ROSETTA >::const_iterator ri = racks.begin(); ri != racks.end(); ++ ri ) {
			allRacks.push_back( new Rack( *ri ) );
		}
	}
	// read section definitions for this layout - racks should match one of them
	std::vector< ROSETTA > sections;
	dao.loadSections( layout_cid, sections );
	capacity = sections.size( );
	for( int i = 0; i < capacity; i++ ) {
		Section *s = new Section( sections[ i ] );
		std::multimap< short, Rack * > matchedRacks;
		for( std::vector< Rack * >::iterator ri = allRacks.begin(); ri != allRacks.end(); ++ ri ) {
			if( (**ri).getType_cid() == s->getID() ) {
				matchedRacks.insert( std::pair< short, Rack * >( (**ri).getPosition(), *ri ) );
			}
		}
		s->setMap_cid( id );
		s->setTank_cid( tank_cid );
		s->populate( matchedRacks );
		s->setParent( this );
		partlist.push_back( s );
	}
	childCount = partlist.size( );
}

bool Layout::save( ) {
	StoreDAO dao;
	std::auto_ptr< ROSETTA >data = getObjectData( );
	if( isNew() ) {
		bool result = dao.saveLayout( *data );
		if( !result ) {
			return false;
		}
		layout_cid = data->getInt( "rack_layout_cid" );
		state = SyncState;
	}
	return true;
}

void Layout::saveAvailability( ) {
	StoreDAO dao;
	std::auto_ptr< ROSETTA >data = getObjectData( );
	dao.setLayoutAvailability( *data );
}

Section::Section( ) : IPart( ) {
	state = NewState;
	type = SectionType;
	tank_cid = map_cid = 0;
	first = last = -1;
}

Section::Section( int p_id, std::string p_name ) : IPart( p_id, p_name ) {
	state = NewState;
	type = SectionType;
	tank_cid = map_cid = 0;
	first = last = -1;
}

void Section::populate( const std::multimap< short, Rack * > & dbRacks ) {
	const LCDbSectionDef * layout = LCDbSectionDefs::records().findByID( id );
	short posInRack = 1, posInTank = (layout == NULL ? -999 : layout->getFirstPosition());
	std::multimap< short, Rack * >::const_iterator mmi = dbRacks.begin();
	clearList();
	for( short n = first; n <= last || mmi != dbRacks.end(); n ++, posInRack ++, posInTank ++ ) {
		Rack * r;
		if( mmi == dbRacks.end() || posInRack < mmi->first ) {
			char buff[ 60 ];
			// add in empty racks to fill the section
			r = new Rack();
			std::sprintf( buff, "%s%d", name.c_str(), n );
			r -> setName( buff );
			r -> setPosition( posInRack );
			r -> setEmptySlots( rackSize );
		} else {
			r = mmi -> second;
			mmi ++;
		}
		r -> setMap_cid( map_cid );
		r -> setCapacity( rackSize );
		r -> setPosInTank( posInTank );
		r -> setParent( this );
		partlist.push_back( r );
	}
	childCount = partlist.size();
}

Section::Section( const ROSETTA &data ) {
	if( data.isString( "section_prefix" ) ) {
		name = data.getString( "section_prefix" );
	}
	if( data.isInt( "rack_type_cid" ) ) {
		id = data.getInt( "rack_type_cid" );
	} else if( data.isInt( "record_cid" ) ) {
		id = data.getInt( "record_cid" );
	} else {
		id = -1;
    }
	rack_layout_cid = data.getIntDefault( "rack_layout_cid", -1 );
	position = data.getIntDefault( "fill_order", 0 );
	first = data.getIntDefault( "first_rack", 0 );
	last = data.getIntDefault( "last_rack", 0 );
	capacity = (last > first ? 1 + last - first : -1);
	rackSize = data.getIntDefault( "rack_capacity", -1 );
	tank_cid = data.getIntDefault( "tank_cid", -1 );
	map_cid = data.getIntDefault( "map_cid", -1 );

	mapped2 = NULL;
	parent = NULL;
	state = SyncState;
	type = SectionType;
	childCount = -1;
}

std::auto_ptr< ROSETTA >Section::getProperties( ) {
	std::auto_ptr< ROSETTA >r = IPart::getProperties( );
	// r->setInt( "tank_cid", tank_cid );
	// r->setInt( "map_cid", map_cid );
	// r->setInt( "position", position );
	r->setInt( "first_structure", first );
	r->setInt( "last_structure", last );
	r->setInt( "structure_size", rackSize );
	return r;
}

std::auto_ptr< ROSETTA >Section::getObjectData( ) {
	std::auto_ptr< ROSETTA >r = IPart::getObjectData( );
	r->setInt( "layout_cid", rack_layout_cid );
	r->setInt( "fill_order", position );
	r->setInt( "first_rack", first );
	r->setInt( "last_rack", last );
	r->setInt( "rack_capacity", rackSize );
	r->setInt( "map_cid", map_cid );
	return r;
}

std::string Section::getFullName() const {
	std::string result;
	if( name.length() < 3 ) {
		result = "section " + name;
	} else if( stricmp( name.c_str(), "Rack" ) == 0 ) {
		result = "rack list";
	} else {
		result = name;
	}
	return getParent()->getFullName() + ", " + result;
}

void Section::populate( ) {
	if( childCount >= 0 || tank_cid == -1 ) {
		return;
	}
	StoreDAO dao;
	std::vector< ROSETTA >results;
	dao.loadRacks( tank_cid, results, id );
	std::multimap< short, Rack * > dbRacks;
	for( std::vector< ROSETTA >::const_iterator ri = results.begin(); ri != results.end(); ++ ri ) {
		Rack *r = new Rack( *ri );
		dbRacks.insert( std::pair< short, Rack * >( r->getPosition(), r ) );
	}
	populate( dbRacks );
}

IPart::Availability Section::availability() const {
	short empty = 0, full = 0;
	for( int i = 0; i < childCount; i++ ) {
		Rack * r = (Rack *)( partlist[ i ] );
		switch( r->availability() ) {
			case IS_EMPTY:
				empty ++;
				break;
			case IS_FULL:
				full ++;
		}
	}
	if( empty == childCount ) {
		return IS_EMPTY;
	} else if( full == childCount ) {
		return IS_FULL;
	} else {
		return PART_FULL;
	}
}

bool Section::save( ) {
	Layout *t = ( Layout* )this->parent;
	StoreDAO dao;
	// map_cid = t->getID();
	rack_layout_cid = t->getLayout_cid();
	std::auto_ptr< ROSETTA > data = getObjectData( );
	bool result = dao.saveSection( *data );
	if( result ) {
		state = SyncState;
		id = data->getInt("ID");
	}
	return result;
}

Rack::Rack( ) : IPart( ) {
	state = NewState;
	type = RackType;
	project_cid = rack_type_cid = box_type_cid = tank_cid = map_cid = -1;
	posInTank = emptySlots = -1;
}

Rack::Rack( int p_id, std::string p_name ) : IPart( p_id, p_name ) {
	state = NewState;
	type = RackType;
	project_cid = rack_type_cid = box_type_cid = tank_cid = map_cid = -1;
	posInTank = emptySlots = -1;
}

Rack::Rack( const ROSETTA &data ) {
	id = data.getIntDefault( "rack_cid", -1 );
	name = data.getStringDefault( "external_name", "" );
	tank_cid = data.getIntDefault( "tank_cid", -1 );
	emptySlots = data.getIntDefault( "capacity", -1 );
	rack_type_cid = data.getIntDefault( "rack_type_cid", -1 );
	posInTank = data.getIntDefault( "position", -1 );
	box_type_cid = data.getIntDefault( "default_box_type", -1 );
	project_cid = data.getIntDefault( "project_cid", -1 );
	map_cid = data.getIntDefault( "map_cid", -1 );

	const LCDbSectionDef * layout = LCDbSectionDefs::records().findByID( rack_type_cid );
	position = (layout == NULL ? -1 : posInTank - (layout->getFirstPosition() - 1));
	mapped2 = NULL;
	parent = NULL;
	state = SyncState;
	type = RackType;
	childCount = -1;
}

std::auto_ptr< ROSETTA >Rack::getProperties( ) {
	std::auto_ptr< ROSETTA >r = IPart::getProperties( );
	r->setInt( "position", position );
	if( capacity > 0 ) {
		r->setInt( "total_slots", capacity );
	}
	if( childCount > 0 ) {
		r->setInt( "slots_used", childCount );
	}
	if( emptySlots > 0 ) {
		r->setInt( "empty_slots", emptySlots );
	}
	return r;
}

std::auto_ptr< ROSETTA >Rack::getObjectData( ) {
	std::auto_ptr< ROSETTA >r = IPart::getObjectData( );
	r->setInt( "tank_cid", tank_cid );
	r->setInt( "rack_type_cid", rack_type_cid );
	// r->setInt( "map_cid", map_cid );
	r->setInt( "project_cid", project_cid );
	r->setInt( "box_type_cid", box_type_cid );
	r->setInt( "empty_slots", emptySlots );
	r->setInt( "pos_in_tank", posInTank );
	r->setInt( "pos_in_section", position );
	Section * p = dynamic_cast< Section * >( parent );
	if( p != NULL ) {
		Site * s = dynamic_cast< Site * >( p->getParent()->getParent()->getParent() );
		if( s != NULL ) {
			r->setString("location",s->getName());
		}
	}
	return r;
}

std::string Rack::getFullName() const {
	std::string result = name;
	Layout * l = dynamic_cast< Layout * >( getParent()->getParent() );
	if( l != NULL ) {
		result = l->getFullName() + ", " + result;
	}
	return result;
}

void Rack::populate( ) {
	if( childCount >= 0 || id == -1 ) {
		return;
	}
	// find expected number of slots for the current segment
	const LCDbSectionDef * layout = LCDbSectionDefs::records().findByID( rack_type_cid );
	if( layout != NULL ) {
		capacity = layout -> getRackCapacity();
	} else if( capacity < 1 ) {
		capacity = 99; 	// error ??
	}
	// read boxes from database - most racks are filled with boxes from one project
	StoreDAO dao;
	std::vector< ROSETTA > results;
	if( isSingleProject() ) {
		dao.loadBoxes( id, project_cid, results );
		if( results.size() < capacity ) {
			results.clear();	// not enough from this project; should check again
		}
	}
	if( results.empty() ) {
		for( Range< LCDbProject > pr = LCDbProjects::records(); pr.isValid(); ++ pr ) {
			if( pr->isInCurrentSystem() && !pr -> isCentral() ) {
				dao.loadBoxes( id, pr -> getID(), results );
			}
		}
	}
	// allocate boxes to slots; allow more than one per slot (e.g. during box move)
	std::multimap< int, Box * > boxes;
	std::set< int > projects, boxTypes;
	for( std::vector< ROSETTA >::const_iterator ri = results.begin(); ri != results.end(); ++ ri ) {
		Box * b = new Box( *ri );
		boxes.insert( std::pair< int, Box * >( b->getPosition(), b ) );
		projects.insert( b->getProjectCID() );
		boxTypes.insert( b->getBoxTypeCID() );
	}
	for( std::multimap< int, Box * >::const_iterator bi = boxes.begin(); bi != boxes.end(); ++ bi) {
		Box * next = bi->second;
		next->setParent( this );
		partlist.push_back( next );
	}
	childCount = partlist.size();

	// update summary fields of c_rack_number record if rack contents have changed
	bool changed = name.empty();
	int boxProj = (projects.size() == 1 ? *projects.begin() : 0);
	if( project_cid != boxProj ) {
		project_cid = boxProj;
		changed = true;
	}
	int boxType = (boxTypes.size() == 1 ? *boxTypes.begin() : 0);
	if( box_type_cid != boxType ) {
		box_type_cid = boxType;
		changed = true;
	}
	short slots = capacity - childCount;
	if( emptySlots != slots ) {
		emptySlots = slots;
		changed = true;
	}
	if( changed ) {
		save();
	}
}

IPart::Availability Rack::availability() const {
	if( id == -1 || id == 0 || childCount == 0 ) {
		return IS_EMPTY;
	} else if( emptySlots == 0 ) {
		return IS_FULL;
	} else {
		return PART_FULL;
	}
}

/*
 void Rack::loadParent( ) {
 StoreDAO &dao = StoreDAO::records( );

 ROSETTA result;
 dao.loadTank( tank_cid, result );
 if( result.count( ) > 0 ) {
 parent = new Tank( result );        ?? or section
 }
 }
 */
bool Rack::save() {
	StoreDAO dao;
	std::auto_ptr< ROSETTA >data = getObjectData( );
	if( dao.occupyRack( *data ) ) {
		id = data->getInt( "ID" );
		return true;
	} else {
		return false;
    }
}

Box::Box( ) : IPart( ) {
	status = 0;
	emptySlots = record_id = rack_cid = retrieval_cid = project_cid = box_type_id = -1;
	type = BoxType;
}

Box::Box( int p_id, std::string p_name ) : IPart( p_id, p_name ) {
	status = 0;
	emptySlots = record_id = rack_cid = retrieval_cid = project_cid = box_type_id = -1;
	type = BoxType;
}

Box::Box( const ROSETTA &data ) {
	record_id = data.getIntDefault( "record_id", 0 );
	rack_cid = data.getIntDefault( "rack_cid", -1 );
	position = data.getIntDefault( "slot_position", 0 );
	emptySlots = data.getIntDefault( "box_capacity", -1 );

	if( data.isInt( "box_cid" ) ) {
		id = data.getInt( "box_cid" );
	} else if( data.isInt( "box_id" ) ) {
		id = data.getInt( "box_id" );
	} else {
		id = -1;
	}
	box_type_id = data.getIntDefault( "box_type_cid", -1 );
	retrieval_cid = data.getIntDefault( "retrieval_cid", 0 );

	if( data.isString( "external_name" ) ) {
		name = data.getString( "external_name" );
	}
	project_cid = data.getIntDefault( "project_cid", -1 );
	if( project_cid == -1 && data.isString( "db_name" ) ) {
		std::string db_name = data.getString("db_name");
		const LCDbProject * p = LCDbProjects::records().findByName( db_name );
		if( p != NULL ) {
			project_cid = p -> getID();
		}
	}
	status = data.getIntDefault( "status", 0 );
	mapped2 = NULL;
	parent = NULL;
	state = SyncState;
	type = BoxType;
	childCount = -1;
	capacity = -1;
}

std::string Box::getName() const {
	const char * movement;
	switch( status ) {
	case LCDbBoxStore::EXPECTED:
		movement = "<-";
		break;
	case LCDbBoxStore::MOVE_EXPECTED:
		movement = "->";
		break;
	default:
		movement = NULL;
	}

	if( movement != NULL && mapped2 != NULL && mapped2->getParent() != NULL ) {
		Rack * r = (Rack*)(mapped2->getParent());
		const std::string & rack = r -> getName();
		Tank * t = (Tank*)(r->getParent()->getParent()->getParent());
		const std::string & store = t -> getVessel();
		short slot = ((Box*)mapped2)->getPosition();
		char label[ 150 ];
		std::sprintf( label, "%s %s %s %s, %d", name.c_str(), movement, store.c_str(), rack.c_str(), slot );
		return label;
	} else {
		return name;
	}
}

std::auto_ptr< ROSETTA >Box::getProperties( ) {
	std::auto_ptr< ROSETTA >r = IPart::getProperties( );
	const LPDbBoxTypes &boxTypes = LPDbBoxTypes::records( project_cid );
	const LPDbBoxType *content = boxTypes.findByID( box_type_id );
	if( content != NULL ) {
		r->setString( "content", content->getDescription( ) );
	}
	// r->setInt( "slot_position", position );

	const char *statusStr;
	switch( status ) {
	case LCDbBoxStore::EXPECTED:
		statusStr = "Expected";
		break;
	case LCDbBoxStore::MOVE_EXPECTED:
		statusStr = "Move expected";
		break;
	case LCDbBoxStore::SLOT_ALLOCATED:
		statusStr = "Allocated";
		break;
	case LCDbBoxStore::SLOT_CONFIRMED:
		statusStr = "Confirmed";
		break;
	case LCDbBoxStore::REMOVED:
		statusStr = "Removed";
		break;
	case LPDbBoxName::DESTROYED:
		statusStr = "Destroyed";
		break;
	case LPDbBoxName::DELETED:
		statusStr = "Deleted";
		break;
	default:
		statusStr = "Unknown";
	}
	r->setString( "status", statusStr );
	return r;
}

std::auto_ptr< ROSETTA >Box::getObjectData( ) {
	std::auto_ptr< ROSETTA >r = IPart::getObjectData( );
	r->setInt( "record_id", record_id );
	r->setInt( "rack_cid", rack_cid );
	r->setInt( "box_type_cid", box_type_id );
	r->setInt( "tank_cid", ( ( Rack * )parent )->getTank_cid( ) );
	r->setInt( "retrieval_cid", retrieval_cid );
	r->setString( "rack_name", parent->getName( ) );
	r->setInt( "slot_position", position );
	r->setInt( "rack_position", ( (Rack *)parent )->getPosition( ) );
	r->setInt( "project_cid", project_cid );
	//	r->setString( "db_name", db_name );
	r->setInt( "status", status );
	return r;
}

std::string Box::getFullName() const {
	char buff[ 20 ];
	std::sprintf( buff, ", slot %d", position );
	return parent->getFullName() + buff;
}

void Box::populate( ) {
	if( childCount >= 0 || id == -1 ) {
		return;
	}

	StoreDAO dao;
	std::vector< ROSETTA >results;
	dao.loadSamples( id, project_cid, results );

	// list cryovials in the box, ignoring empty spaces
	childCount = 0;
	for( std::vector< ROSETTA >::const_iterator bi = results.begin(); bi != results.end(); bi ++ ) {
		Sample * next = new Sample( *bi );
		next->setParent( this );
		partlist.push_back( next );
		childCount ++;
	}

	const LPDbBoxType * type = LPDbBoxTypes::records( project_cid ).findByID( box_type_id );
	if( type != NULL ) {
		const LCDbBoxSize * size = LCDbBoxSizes::records().findByID( type->getSizeID() );
		if( size != NULL ) {
			capacity = size -> getCapacity();
			emptySlots = capacity - childCount;
		}
	}

	// list previous positions of this box, if available
	dao.loadBoxHistory( id, project_cid, results );
	for( std::vector< ROSETTA >::const_iterator hi = results.begin( ); hi != results.end( ); ++hi ) {
		TDateTime when = hi->getTime( "time_stamp" ).outputTDateTime();
		std::stringstream detail;
		detail << hi->getString( "vessel" );
		short shelf = hi->getIntDefault( "shelf_number", -1 );
		if( shelf > 0 ) {
			detail << '[' << shelf << "] ";
		} else {
			detail << ' ';
		}
        detail << hi->getString( "rack" ) << ", slot " << hi->getInt( "slot_position" );
		history[ when ] = detail.str( );
	}
}

bool Box::canMove() const {
	return (status == LCDbBoxStore::SLOT_CONFIRMED);
}

IPart::Availability Box::availability( ) const {
	if( status != LCDbBoxStore::SLOT_CONFIRMED || retrieval_cid != 0 ) {
		return UNAVAILABLE;
	} else if( childCount == 0 ) {
		return IS_EMPTY;
	} else if( emptySlots == 0 ) {
		return IS_FULL;
	} else {
		return PART_FULL;
	}
}

bool Box::isLHSAssigned( ) const {
	return (status == LCDbBoxStore::MOVE_EXPECTED || status == LCDbBoxStore::SLOT_CONFIRMED)
		&& (retrieval_cid != 0 || mapped2 != NULL);
}

bool Box::isLHSDone( ) const {
	return status == LCDbBoxStore::REMOVED && (retrieval_cid != 0 || mapped2 != NULL);
}

bool Box::isRHSAssigned( ) const {
	return status == LCDbBoxStore::EXPECTED && (retrieval_cid != 0 || mapped2 != NULL);
}

bool Box::isRHSDone( ) const {
	return status == LCDbBoxStore::SLOT_ALLOCATED && (retrieval_cid != 0 || mapped2 != NULL);
}

void Box::loadParent( ) {
	StoreDAO dao;
	std::vector< ROSETTA >result;
	std::set< int >ids;
	ids.insert( rack_cid );
	dao.loadRacks( ids, result );
	if( !result.empty( ) ) {
		parent = new Rack( result[ 0 ] );
	}
}

bool Box::addToLHSJobList( int jobID ) {
	if( retrieval_cid != 0 ) {
		return true;
	}
	StoreDAO dao;
	retrieval_cid = jobID;
	std::auto_ptr< ROSETTA >data = getObjectData( );
	return dao.addBoxToLHSJobList( *data );
}

bool Box::addToRHSJobList( int jobID ) {
	if( retrieval_cid != 0 ) {
		return true;
	}
	Rack * rack = dynamic_cast< Rack* >( parent );
	Box * source = dynamic_cast< Box* >( mapped2 );
	if( source == NULL || rack == NULL || !rack->save() ) {
		return false;
	}
	rack_cid = rack->getID();
	retrieval_cid = jobID;
	project_cid = source->project_cid;
	id = source->getID();
	std::auto_ptr< ROSETTA >data = getObjectData( );
	return StoreDAO().addBoxToRHSJobList( *data );
}

bool Box::save( ) {
	StoreDAO dao ;
	std::auto_ptr< ROSETTA >data = getObjectData( );
	return dao.updateBox( *data );
}

bool Box::signoff( ) {
	StoreDAO dao ;
	std::auto_ptr< ROSETTA >data = getObjectData( );
	if( !dao.signoffBox( *data ) ) {
		return false;
	}
	retrieval_cid = 0;
	status = LCDbBoxStore::SLOT_CONFIRMED;
	return true;
}

/*
bool Box::occupy( ) {
	StoreDAO &dao = StoreDAO::records( );
	if( parent != NULL && parent->occupy( ) ) {
		rack_cid = parent->getID( );
		project_cid = ( ( Box* )mapped2 )->project_cid; //move the box in the same project
		id = ( ( Box* )mapped2 )->getID( ); //keep box_id same as the mapped one
		std::auto_ptr< ROSETTA >data = getObjectData( );
		return dao.occupyBox( *data );
	}
	return false;
}
*/

Sample::Sample( ) : IPart( ) {
	sample_id = project_cid = box_id = aliquot_type = 0;
	type = SampleType;
}

Sample::Sample( int p_id, std::string p_name ) : IPart( p_id, p_name ) {
	sample_id = project_cid = box_id = aliquot_type = 0;
	type = SampleType;
}

Sample::Sample( const ROSETTA &data ) {
	id = data.getIntDefault( "cryovial_id", 0 );
	position = data.getIntDefault( "cryovial_position", -1 );

	if( data.isTime( "time_stamp" ) ) {
		stamp = data.getTime( "time_stamp" );
	}
	project_cid = data.getIntDefault( "project_cid", -1 );
	sample_id = data.getIntDefault( "sample_id", 0 );

	if( data.isString( "cryovial_barcode" ) ) {
		name = data.getString( "cryovial_barcode" );
	}
	if( data.isString( "source_name" ) ) {
		source_name = data.getString( "source_name" );
	}
	if( data.isString( "source_barcode" ) ) {
		source_barcode = data.getString( "source_barcode" );
	}
	aliquot_type = data.getIntDefault( "aliquot_type_cid", -1 );
	box_id = data.getIntDefault( "box_id", -1 );

	mapped2 = NULL;
	parent = NULL;
	state = SyncState;
	type = SampleType;
	childCount = -1;
}

IPart::Availability Sample::availability( ) const {
	return name.empty() ? UNKNOWN : IS_AVAILABLE;
}

std::string Sample::getFullName() const {
	char buff[ 20 ];
	std::sprintf( buff, ", position %d", position );
	return parent->getName() + buff;
}

void Sample::populate( ) {
	if( childCount >= 0 || id == -1 ) {
		return;
	}
	partlist.clear( );
	std::vector< ROSETTA >results;
	StoreDAO dao;
	dao.loadStorageHistory( id, project_cid, results );
	for( std::vector< ROSETTA >::const_iterator hi = results.begin( ); hi != results.end( ); ++hi ) {
		TDateTime when = hi->getTime( "time_stamp" ).outputTDateTime();
		std::stringstream detail;
		detail << hi->getString( "box_name" ) << ", position " << hi->getInt( "cryovial_position" );
		history[ when ] = detail.str( );
	}
	dao.loadAnalysisHistory( name, aliquot_type, project_cid, results );
	for( std::vector< ROSETTA >::const_iterator hi = results.begin( ); hi != results.end( ); ++hi ) {
		TDateTime when = hi->getTime( "when" ).outputTDateTime();
		std::stringstream detail;
		detail << "analysed (" << hi->getInt( "results" ) << " result/s)";
		history[ when ] = detail.str( );
	}
	childCount = 0;

}

/* RETHINK

 void Sample::loadParent( ) {

 StoreDAO& dao = StoreDAO::records();

 ROSETTA result;
 std::string e = dao.loadBox( box_cid, result );
 if( !e.empty() )
 {
 Application->MessageBoxA(e.c_str(), "Info", MB_OK );
 return;
 }

 Box* b;
 if( result.count() > 0 )
 {
 b = new Box( result );
 parent = b;
 result.clear();
 e = dao.loadBoxName( box_cid, db_name, result );
 if( !e.empty() )
 {
 Application->MessageBoxA(e.c_str(), "Info", MB_OK );
 return;
 }

 b->fill(result);
 }
 }
 */

std::auto_ptr< ROSETTA >Sample::getProperties( ) {
	std::auto_ptr< ROSETTA >r = IPart::getProperties( );
	// r->setInt( "cryovial_id", cryovial_id );
	// r->setInt( "cryovial_position", position );

	const LCDbObject *at = NULL;
	if( aliquot_type != 0 && aliquot_type != -1 ) {
		at = LCDbObjects::records( ).findByID( aliquot_type );
	}
	if( at != NULL ) {
		r->setString( "aliquot_type", at->getDescription( ).c_str( ) );
	}

	// r->setInt( "box_cid", box_cid );
	// r->setString( "time_stamp", stamp.DateTimeString() );

	if( !source_name.empty( ) && source_name != name ) {
		r->setString( "source_name", source_name );
	}
	if( !source_barcode.empty( ) && source_barcode != name ) {
		r->setString( "source_barcode", source_barcode );
	}
	return r;
}

std::auto_ptr< ROSETTA >Sample::getObjectData( ) {
	std::auto_ptr< ROSETTA >r = IPart::getObjectData( );
	r->setInt( "cryovial_id", id );
	r->setInt( "cryovial_position", position );
	//r->setString( "time_stamp", stamp.DateTimeString() );   ???
	r->setInt( "aliquot_type", aliquot_type );
	r->setInt( "box_id", box_id );
	r->setString( "source_barcode", source_barcode );
	return r;
}

	/*
IPart *Project::loadSample( std::string qry ) {
	 StoreDAO& dao = StoreDAO::records();

	 ROSETTA result;
	 std::string e = dao.loadSample( qry, db_name, result );
	 if( !e.empty() )
	 {
	 Application->MessageBoxA(e.c_str(), "Info", MB_OK );
	 return NULL;
	 }

	 Sample* s = NULL;
	 if( result.count() > 0 )
	 {
	 s = new Sample( result );
	 //		s->setDb_name( db_name );
	 return s;
	 }
	return NULL;
}

void Project::clearSamples( ) {
	for( int i = 0; i < ( int ) samplist.size( ); i++ ) {
		delete samplist[ i ];
	}
	samplist.clear( );
}

void Project::loadAllSamples( std::string qry ) {
	 StoreDAO& dao = StoreDAO::records();
	 clearSamples();

	 std::vector<ROSETTA> results;
	 std::string e = dao.loadAllSamples( qry, db_name, results );
	 if( !e.empty() )
	 {
	 Application->MessageBoxA(e.c_str(), "Info", MB_OK );
	 return;
	 }

	 Sample* s = NULL;
	 for( int i = 0; i < (int)results.size(); i++ )
	 {
	 s = new Sample( results[i] );
	 //		s->setDb_name( db_name );
	 samplist.push_back( s );
	 }
}

Project::Project( const ROSETTA &data ) {
	project_cid = data.getInt( "project_cid" );
	project_name = data.getString( "external_name" );
	db_name = data.getString( "db_name" );
}

Projects::~Projects( ) {
	for( int i = 0; i < ( int ) projlist.size( ); i++ ) {
		delete projlist[ i ];
	}
	projlist.clear( );
}

void Projects::loadAll( ) {
	projlist.clear( );
	StoreDAO &dao = StoreDAO::records( );

	std::vector< ROSETTA >results;
	dao.loadProjects( results );
	Project *p = NULL;
	for( int i = 0; i < ( int )results.size( ); i++ ) {
		p = new Project( results[ i ] );

		projlist.push_back( p );
	}

}

Project *Projects::findProject( int projID ) {
	for( int i = 0; i < ( int )projlist.size( ); i++ ) {
		if( projlist[ i ]->getProject_cid( ) == projID ) {
			return projlist[ i ];
		}
	}

	return NULL;
}

AliquotType::AliquotType( const ROSETTA &data ) {
	id = data.getInt( "object_cid" );
	name = data.getString( "external_name" );
}

AliquotTypes::~AliquotTypes( ) {
	for( int i = 0; i < ( int ) typelist.size( ); i++ ) {
		delete typelist[ i ];
	}
	typelist.clear( );
}

void AliquotTypes::loadAll( ) {
	typelist.clear( );
	StoreDAO &dao = StoreDAO::records( );
	std::vector< ROSETTA >results;
	dao.loadAliquotTypes( results );
	AliquotType *p = NULL;
	for( int i = 0; i < ( int )results.size( ); i++ ) {
		p = new AliquotType( results[ i ] );

		typelist.push_back( p );
	}
}

AliquotType *AliquotTypes::find( int p_id ) {
	for( int i = 0; i < ( int ) typelist.size( ); i++ ) {
		if( p_id == typelist[ i ]->getID( ) ) {
			return typelist[ i ];
		}
	}

	return NULL;
}
*/

Layouts::~Layouts( ) {
	clearList( );
}

void Layouts::clearList( ) {
	for( int i = 0; i < ( int ) layouts.size( ); i++ ) {
		delete layouts[ i ];
	}
	layouts.clear( );
}

void Layouts::loadAll( ) {
	clearList( );
	LCDbObjects &objs = LCDbObjects::records( );
	ROSETTA result;
	for( Range< LCDbObject >ul = objs; ul.isValid( ); ++ul ) {
		if( ul->isActive( ) && ul->getObjectType( ) == LCDbObject::TANK_LAYOUT ) {
			result.setInt( "rack_layout_cid", ul->getID( ) );
			result.setString( "layout_full", ul->getDescription( ) );
			result.setString( "layout_name", ul->getName( ) );
			Layout *p = new Layout( result );
			p->populate( );
			layouts.push_back( p );
		}
	}
}

int Layouts::getLayoutId( int tank_cid ) {
	const LCDbSectionDefs & sections = LCDbSectionDefs::records();
	std::vector<ROSETTA> racks;
	StoreDAO().loadRacks( tank_cid, racks );
	for( std::vector<ROSETTA>::const_iterator ri = racks.begin(); ri != racks.end(); ++ ri ) {
		int type = ri->getIntDefault( "rack_type_cid", 0 );
		if( type != 0 ) {
			const LCDbSectionDef * sd = sections.findByID( type );
			if( sd != NULL ) {
				return sd->getTankLayoutCID();
			}
		}
	}
	return 0;
}

int Layouts::find( int p_id ) const {
	for( int i = 0; i < ( int ) layouts.size( ); i++ ) {
		if( p_id == layouts[ i ]->getLayout_cid( ) ) {
			return i;
		}
	}
	return -1;
}


Layout * Layouts::getLayout( int i ) const {
	if ( i >= 0 && i < layouts.size() ) {
		return layouts[ i ];
	} else {
		return NULL;
	}
}

bool Layouts::isNameDuplicate( bool full, std::string name ) const {
	for( unsigned i = 0; i < layouts.size( ); i++ ) {
		if( full ) {
			if( layouts[ i ]->getLayoutDescription() == name ) {
				return true;
			}
		} else {
			if( layouts[ i ]->getLayoutName() == name ) {
				return true;
			}
		}
	}
	return false;
}

int Layouts::getDefaultLayoutId( int store_cid ) const {
	int layout_cid = -1;
	for( Range< LCDbTankMap >tmr = LCDbTankMaps::records( ); tmr.isValid( ); ++tmr ) {
		// use any layout but keep the first active one
		if( tmr->getStorageCID( ) == store_cid ) {
			layout_cid = tmr->getLayoutCID( );
			if( tmr->isActive( ) ) {
				break;
			}
		}
	}
	return layout_cid;
}

//============== Available part classes ==============

void AvlSites::populate( ) {
	if( childCount >= 0 ) {
		return;
	}
	std::vector< ROSETTA >results;
	StoreDAO( ).loadSites( results );
	childCount = ( int )results.size( );
	for( int i = 0; i < childCount; i++ ) {
		AvlSite *s = new AvlSite( results[ i ] );
		s->setParent( this );
		partlist.push_back( s );
	}
}

void AvlSite::populate( ) {
	if( childCount >= 0 ) {
		return;
	}
	std::vector< ROSETTA >results;
	StoreDAO().loadTanks( id, results );
	std::map< int, AvlTank * > tanks;
	for( int i = 0; i < results.size(); i++ ) {
		AvlTank * p = new AvlTank( results[ i ] );
		p->loadTankDetails( );
		p->readRackOccupancy();
		std::map< int, AvlTank * >::iterator ti = tanks.find( p->getID() );
		if( ti == tanks.end() ) {
			p->setParent( this );
			partlist.push_back( p );
			tanks.insert( std::pair< int, AvlTank * >( p->getID(), p ) );
		} else {
			ti->second->addRackOccupancy(p);
			delete p; 	// same tank, another layout
		}
	}
	childCount = partlist.size();
	sortChildren( );
}

void AvlTank::readRackOccupancy() {
	const LCDbSectionDefs & sDefs = LCDbSectionDefs::records();
	totalSlots = usedSlots = 0;
	for( Range< LCDbSectionDef > sr = sDefs; sr.isValid(); ++ sr ) {
		if( sr->getTankLayoutCID() == layout_cid ) {
			totalSlots += (sr->getCapacity() * sr->getRackCapacity());
		}
	}
	std::vector< ROSETTA > results;
	StoreDAO().loadRacks( tank_cid, results );
	for( int i = 0; i < results.size(); i++ ) {
		Rack r( results[ i ] );
		short filled = 1;	// empty racks are not recorded in c_rack_number
		if( r.getEmptySlots() >= 0 ) {
			const LCDbSectionDef * sd = sDefs.findByID( r.getType_cid() );
			if( sd != NULL ) {
				filled = sd->getRackCapacity() - r.getEmptySlots();
			}
		}
		usedSlots += filled;
	}
}

void AvlTank::addRackOccupancy( const AvlTank * other ) {
	usedSlots += other->usedSlots;
	totalSlots += other->totalSlots;
}

float AvlTank::getFillFactor() const {
	if( totalSlots < 1 || usedSlots < 0 ) {
		return -1;
	} else {
		return float( usedSlots ) / totalSlots;
	}
}

void AvlTank::populate( ) {
	if( childCount >= 0 ) {
		return;
	}
	std::vector< ROSETTA >results;
	StoreDAO().loadLayouts( id, results );
	childCount = ( int )results.size( );
	for( int i = 0; i < childCount; i++ ) {
		AvlLayout *p = new AvlLayout( results[ i ] );
		p->setParent( this );
		partlist.push_back( p );
	}
	sortChildren( );
}

void AvlLayout::populate( ) {
	if( childCount >= 0 ) {
		return;
	}
	std::vector< ROSETTA >results;
	StoreDAO().loadSections( layout_cid, results );
	capacity = results.size( );
	for( int i = 0; i < capacity; i++ ) {
		AvlSection *p = new AvlSection( results[ i ] );
		p->setTank_cid( tank_cid );
		partlist.push_back( p );
		p->setParent( this );
		p->setMap_cid( getID() );
	}
	childCount = partlist.size( );
}

void AvlSection::populate( ) {
	if( childCount >= 0 ) {
		return;
	}
	std::set< std::string >racknames;
	StoreDAO dao;
	std::vector< ROSETTA >results;
	dao.loadRacks( tank_cid, results, id );
	for( std::vector< ROSETTA >::const_iterator ri = results.begin( ); ri != results.end( ); ++ri ) {
		AvlRack *p = new AvlRack( *ri );
		racknames.insert( p->getName() );
		// check the occupancy if it's easy to do so; ignore if full
		if( p->isSingleProject() ) {
			std::set< int > occupied;
			dao.loadRackOccupancy( p->getID(), p->getProject_cid(), occupied );
			if( occupied.size() >= rackSize ) {
				delete p;
				continue;
			} else {
				p->setOccupancy( occupied );
			}
		}
		p->setCapacity( rackSize );
		p->setParent( this );
		partlist.push_back( p );
	}
	char buf[ 40 ];
	for( short i = first; i <= last; i++ ) {
		std::sprintf( buf, "%s%d", name.c_str(), i );
		std::string r_name = buf;
		std::set< std::string >::const_iterator it = racknames.find( r_name );
		if( it == racknames.end( ) ) {
			AvlRack *p = new AvlRack( tank_cid, r_name, id, map_cid, name, rackSize, i );
			p->setParent( this );
			partlist.push_back( p );
		}
	}
	capacity = last - first + 1;
	childCount = partlist.size();
	sortChildren( );
}

float AvlSection::getFillFactor() const {
	if( childCount < 0 ) {
		return -1;
	}
	float racks = capacity - childCount;
	for( std::vector<IPart*>::const_iterator pi = partlist.begin(); pi != partlist.end(); ++ pi ) {
		AvlRack * r = (AvlRack *)(*pi);
		float fill = r -> getFillFactor();
		if( fill < 0 ) {
			return -1;
		} else {
			racks += fill;
		}
	}
	return racks / capacity;
}

AvlRack::AvlRack( int p_tank_cid, const std::string & p_name, int p_type_cid,
				  int p_map_cid, const std::string & p_sec, short p_capacity, short pos )
 : Rack() {
	name = p_name;
	tank_cid = p_tank_cid;
	map_cid = p_map_cid;
	rack_type_cid = p_type_cid;
	capacity = p_capacity;
	position = pos;
	mapped2 = NULL;
	parent = NULL;
}

void AvlRack::populate( ) {
	if( childCount >= 0 ) {
		return;
	}
	if( id != -1 && occupied.empty() ) {
		StoreDAO dao;
		if( isSingleProject() ) {
			dao.loadRackOccupancy( id, project_cid, occupied );
		} else {
			for( Range< LCDbProject > pr = LCDbProjects::records(); pr.isValid(); ++ pr ) {
				if( pr -> isInCurrentSystem() && !pr -> isCentral() ) {
					dao.loadRackOccupancy( id, pr -> getID(), occupied );
				}
			}
		}
	}

//	Tank * t = dynamic_cast< Tank * >( parent->getParent()->getParent() );
//	std::string rn = (t == NULL ? name : t->getName() + " " + name);
	for( int i = 1; i <= capacity; i++ ) {
		if( occupied.count( i ) == 0 ) {
			char buf[ 70 ];
			std::sprintf( buf, "%s empty slot %d", name.c_str(), i );
			AvlBox *p = new AvlBox( -1, buf );
			p->setPosition( i );
			p->setParent( this );
			partlist.push_back( p );
		}
	}
	childCount = partlist.size();
	emptySlots = 0;
}

void AvlRack::setOccupancy( std::set<int>& positions ) {
	occupied = positions;
	emptySlots = capacity - occupied.size();
}

float AvlRack::getFillFactor() const {
	if( id == -1 || emptySlots == capacity ) {
		return 0;
	} else {
		return occupied.size() / float( capacity );
	}
}

AvlBox::AvlBox( int p_rack_cid, std::string p_name ) : Box() {
	name = p_name;
	rack_cid = p_rack_cid;
	mapped2 = NULL;
	parent = NULL;
	retrieval_cid = 0;
	status = LCDbBoxStore::EXPECTED;
}

void AvlBox::populate( ) {
	partlist.clear( );
	childCount = 0;
}

//---------------------------------------------------------------------------
/***** Factory Class **** */

void PartFactory::loadBoxes( const LCDbCryoJob &job ) {

	StoreDAO dao ;
	std::vector< ROSETTA >results;
	int projID = job.getProjectID();	 // 0 => multiple; -1 => unknown
	if( projID != 0 && projID != -1 ) {
		dao.loadBoxesByJobID( job.getID(), projID, results );
	} else {
		for( Range< LCDbProject > pr = LCDbProjects::records(); pr.isValid(); ++ pr ) {
			if( pr -> isInCurrentSystem() && !pr -> isCentral() ) {
				dao.loadBoxesByJobID( job.getID(), pr->getID(), results );
			}
		}
	}

	// buildMovedTo = true will build RHS tree in SampleMove screen
	for( int i = 0; i < ( int ) results.size( ); i++ ) {
		Box *b = new Box( results[ i ] );
		bool included;
		switch( b->getStatus( ) ) {
			case LCDbBoxStore::MOVE_EXPECTED:
			case LCDbBoxStore::REMOVED:
				included = lhs;
				break;

			case LCDbBoxStore::EXPECTED:
			case LCDbBoxStore::SLOT_ALLOCATED:
				included = !lhs;
				break;

			default:
				included = false;
		}

		if( included ) {
			boxList.push_back( b );
		} else {
			delete b;
		}
	}
}

//---------------------------------------------------------------------------
//	add boxes and their racks, sections and layouts to the given site list
//---------------------------------------------------------------------------

IPart * PartFactory::createSiteList() const {
	if( boxList.empty() ) {
		return (lhs ? new Sites() : new AvlSites());
	}

	const LCDbObjects &objs = LCDbObjects::records( );
//	const LCDbStorageDetails &stores = LCDbStorageDetails::records( );
	StoreDAO dao ;
	std::set< int > parentIDs;
	std::vector< ROSETTA >results;
	for( std::vector< Box* >::const_iterator bi = boxList.begin(); bi != boxList.end(); ++ bi ) {
		parentIDs.insert( (**bi).getRackCID( ) );
	}
	dao.loadRacks( parentIDs, results );
	if( results.empty() ) {
		return NULL;			// error - boxes should be in racks
	}

	std::vector< Rack* > rackList;
	parentIDs.clear( );
	for( std::vector< ROSETTA >::const_iterator ri = results.begin( ); ri != results.end( ); ri++ ) {
		Rack *r = new Rack( *ri );
		if( sibs ) {
			r->populate(); 		// all children of boxList's parents
		} else {
			for( int i = 0; i < ( int ) boxList.size( ); i++ ) {
				if( r->getID( ) == boxList[ i ]->getRackCID( ) ) {
					r->addToList( boxList[ i ] );
				}
			}
		}
		rackList.push_back( r );
		parentIDs.insert( r->getTank_cid( ) );
	}
	std::sort( rackList.begin(), rackList.end(), Rack::less_than );

	std::vector< Tank* >tankList;
	ROSETTA result;
	for( Range< LCDbTankMap >tmi = LCDbTankMaps::records( ); tmi.isValid( ); ++tmi ) {
		if( tmi->isActive() && parentIDs.count( tmi->getTankCID() ) != 0 ) {
			const LCDbObject *population = objs.findByID( tmi->getTankCID( ) );
			const LCDbObject *layout = objs.findByID( tmi->getLayoutCID( ) );
			const LCDbObject *phys = objs.findByID( tmi->getStorageCID( ) );
			if( population == NULL || phys == NULL || layout == NULL ) {
				return NULL;
			}

			result.setInt( "storage_cid", phys->getID( ) );
			result.setString( "friendly_name", phys->getDescription() );
			result.setString( "serial_number", phys->getName( ) );
			result.setInt( "location_cid", tmi->getLocationCID( ) );
			result.setInt( "position", tmi->getPosition( ) );
			result.setInt( "storage_type", tmi->getStoreTypeCID( ) );

			result.setInt( "map_cid", tmi->getID( ) );
			result.setInt( "tank_cid", tmi->getTankCID( ) );
			result.setInt( "rack_layout_cid", tmi->getLayoutCID( ) );
			result.setInt( "status", tmi->getStatus( ) );
			std::pair< TDateTime, TDateTime > valid = tmi->getValidDates();
			result.setDate( "valid_from", valid.first );
			result.setDate( "valid_to", valid.second );
			Tank *t = new Tank( result );

			result.setString( "layout_name", layout->getName( ) );
			result.setString( "layout_full", layout->getDescription( ) );
			result.setString( "label_name", population->getName( ) );
			result.setString( "label_full", population->getDescription( ) );
			Layout *l = new Layout( result );

			dao.loadSections( tmi->getLayoutCID( ), results );
			for( int i = 0; i < ( int )results.size(); i++ ) {
				Section *s = new Section( results[ i ] );
				for( int j = 0; j < ( int )rackList.size( ); j++ ) {
					Rack *r = rackList[ j ];
					if( r->getTank_cid() == tmi->getTankCID()
					 && r->getType_cid() == s->getID() ) {
						s->addToList( r );
					}
				}
				s->setMap_cid( tmi->getID() );
				s->setTank_cid( tmi->getTankCID() );
				l->addToList( s );
			}
			t->addToList( l );
			tankList.push_back( t );
			parentIDs.insert( t->getLocationID( ) );
		}
	}
	std::sort( tankList.begin( ), tankList.end( ), Tank::less_than );

	std::vector< Site* >siteList;
	dao.loadSites( results );
	for( std::vector< ROSETTA >::const_iterator ri = results.begin( ); ri != results.end( ); ri++ ) {
		if( parentIDs.count( ri->getInt( "object_cid" ) ) != 0 ) {
			Site *site = new Site( *ri );
			for( int i = 0; i < ( int ) tankList.size( ); i++ ) {
				if( site->getID() == tankList[ i ]->getLocationID( ) ) {
					site->addToList( tankList[ i ] );
				}
			}
			siteList.push_back( site );
		}
	}
	std::sort( siteList.begin( ), siteList.end( ), Site::less_than );
	Sites * list = lhs ? new Sites() : new AvlSites();
	for( int i = 0; i < ( int ) siteList.size( ); i++ ) {
		list->addToList( siteList[ i ] );
	}
	return list;
}

//---------------------------------------------------------------------------

