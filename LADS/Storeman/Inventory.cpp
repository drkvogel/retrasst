// ---------------------------------------------------------------------------

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
#include "rosebase.h"
#include "XSQL.h"

#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------

IPart::IPart( ) {
	id = -1;
	capacity = position = childCount = -1;
	mapped2 = NULL;
	parent = NULL;
	state = UnknownState;
	type = UnknownType;
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
	capacity = position = childCount = -1;
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
}

void IPart::remove( int row ) {
	if( row >= 0 && row < ( int )partlist.size( ) ) {
		delete partlist[ row ];
		partlist.erase( partlist.begin( ) + row );
		childCount = partlist.size( );
	}
}

/*
std::auto_ptr < ROSETTA > IPart::getProperties( ) {
	std::auto_ptr < ROSETTA > r( new ROSETTA( ) );
	r->setString( getTypeStr( ), name );
	if( mapped2 != NULL && mapped2->id != this->id ) {
		r->setString( "Mapped_to", mapped2->name );
	}
	int n = 1;
	for( auto hi = history.begin(); hi != history.end(); hi ++, n ++ ) {
		AnsiString date = String( n ) + "_" + TDate( hi->first ).FormatString( "d_mmmm_yyyy" );
		r->setString( date.c_str( ), hi->second );
	}
	return r;
}
*/

void IPart::showProperties( TStringGrid *grdProps ) const {
	int row = grdProps -> FixedRows;
	grdProps->Cells[ 0 ][ row ] = getTypeStr();
	grdProps->Cells[ 1 ][ row++ ] = name.c_str();
	if( mapped2 != NULL && mapped2->id != this->id ) {
		grdProps->Cells[ 0 ][ row ] = "Mapped to";
		grdProps->Cells[ 1 ][ row++ ] = mapped2->name.c_str();
	}
	for( auto hi : history ) {
		grdProps->Cells[ 0 ][ row ] = TDate( hi.first ).FormatString( "d mmmm yyyy" );
		grdProps->Cells[ 1 ][ row ++ ] = hi.second.c_str();
	}
	grdProps->RowCount = row;
}

ROSETTA * IPart::getObjectData() const {
	ROSETTA * r( new ROSETTA( ) );
	r->setInt( "ID", id );
	r->setString( "Name", name );
	return r;
}

ROSETTA * Sites::getObjectData() const {
	ROSETTA * r( new ROSETTA( ) );
	r->setString( "Type", "Sites" );
	return r;
}

void Sites::populate( ) {
	if( childCount > 0 ) {
		return;
	}
	StoreDAO dao;
	std::vector < ROSETTA > results;
	dao.loadSites( results );
	for( const ROSETTA & r : results ) {
		Site *s = new Site( r );
		s->setParent( this );
		partlist.push_back( s );
	}
	sortChildren( );
	Site * s = new Site( 0, "(currently off line)" );
	s->setParent( this );
	partlist.push_back( s );
	childCount = partlist.size( );
	for( int i = 0; i < childCount; i++ ) {
		partlist[ i ]->setPosition( i );
	}
}

/*
std::auto_ptr < ROSETTA > Sites::getProperties( ) {
	std::auto_ptr < ROSETTA > r = IPart::getProperties( );
	if( childCount > 0 ) {
		r->setInt( "sites", childCount );
	}
	return r;
}
*/
void Sites::showProperties( TStringGrid *grdProps ) const {
	int row = grdProps -> FixedRows;
	grdProps->Cells[ 0 ][ row ] = "Sites";
	grdProps->Cells[ 1 ][ row ] = childCount;
	grdProps->RowCount = row + 1;
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

/*
std::auto_ptr < ROSETTA > Site::getProperties( ) {
	std::auto_ptr < ROSETTA > r = IPart::getProperties( );
	r->setString( "full_name", fullname );
	if( childCount > 0 ) {
		r->setInt( "storage_vessels", childCount );
	}
	return r;
}
*/
void Site::showProperties( TStringGrid *grdProps ) const {
	IPart::showProperties( grdProps );
	int row = grdProps -> RowCount;
	grdProps->Cells[ 0 ][ row ] = "Full name";
	grdProps->Cells[ 1 ][ row ++ ] = fullname.c_str();
	grdProps->Cells[ 0 ][ row ] = "Storage vessels";
	grdProps->Cells[ 1 ][ row ] = childCount;
	grdProps->RowCount = row + 1;
}

ROSETTA * Site::getObjectData( ) const {
	ROSETTA * r = IPart::getObjectData( );
	r->setString( "fullname", fullname );
	return r;
}

bool Site:: operator< ( const IPart& rhs ) const {
	return getName( ) < rhs.getName( );
}

void Site::populate( ) {
	if( childCount >= 0 || id == -1 ) {
		return;
	}
	std::vector < ROSETTA > results;
	StoreDAO( ).loadTanks( id, results );
	childCount = results.size( );
	for( int i = 0; i < childCount; i++ ) {
		Tank *p = new Tank( results[ i ] );
		p->loadTankDetails( );
		p->setParent( this );
		if( id == 0 ) {
			p->setPosition( 0 ); // no position if no current site
		}
		partlist.push_back( p );
	}
	capacity = 0;
	sortChildren( );
}

bool Site::save( ) {
	StoreDAO dao;
	std::auto_ptr < ROSETTA > data( getObjectData() );
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
		name = srlno.empty( ) ? label : srlno;
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
	tank_cid = data.getTank_cid( );
	store_type_cid = data.getStore_type_cid( );
	map_cid = data.getID( );
	layout_cid = data.getLayout_cid( );
	label = data.getPopulationName( );
	description = data.getPopulationDescription( );
	shelf_number = data.getPosition( );
	if( data.availability( ) == Layout::IS_AVAILABLE ) {
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
	for( IPart * pi : getList( ) ) {
		Layout *lay = ( Layout* ) pi;
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

/*
std::auto_ptr < ROSETTA > Tank::getProperties( ) {
	std::auto_ptr < ROSETTA > r = IPart::getProperties( );
	r->setString( "serial_number", srlno );
	const LCDbObject * type = LCDbObjects::records( ).findByID( store_type_cid );
	if( type != NULL ) {
		r->setString( "storage_type", type->getDescription( ) );
	}
	r->setString( "on_line", availability( ) == IS_AVAILABLE ? "Yes" : "No" );
	return r;
}
*/
void Tank::showProperties( TStringGrid *grdProps ) const {
	IPart::showProperties( grdProps );
	int row = grdProps -> RowCount;
	grdProps->Cells[ 0 ][ row ] = "Serial number";
	grdProps->Cells[ 1 ][ row ++ ] = srlno.c_str();
	const LCDbObject * type = LCDbObjects::records( ).findByID( store_type_cid );
	if( type != NULL ) {
		grdProps->Cells[ 0 ][ row ] = "Storage type";
		grdProps->Cells[ 1 ][ row ++ ] = type->getDescription().c_str();
	}
	grdProps->Cells[ 0 ][ row ] = "On line";
	grdProps->Cells[ 1 ][ row ] = availability( ) == IS_AVAILABLE ? "Yes" : "No";
	grdProps->RowCount = row + 1;
}

std::string Tank::getName( ) const {
	if( position > 0 ) {
		char buff[ 100 ];
		std::sprintf( buff, "%d: %s", position, name.c_str( ) );
		return buff;
	} else {
		return name;
	}
}

std::string Tank::getFullName( ) const {
	std::string result = getName( );
	if( parent != NULL ) {
		result = parent->getName( ) + " " + result;
	}
	return result;
}

bool Tank:: operator < ( const IPart& rhs ) const {
	if( getPosition( ) == rhs.getPosition( ) ) {
		return getName( ) < rhs.getName( );
	} else {
		return getPosition( ) < rhs.getPosition( );
	}
}

ROSETTA * Tank::getObjectData( ) const {
	ROSETTA * r = IPart::getObjectData( );
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
	std::vector < ROSETTA > results;
	StoreDAO( ).loadLayouts( id, results );
	childCount = ( int )results.size( );
	for( int i = 0; i < childCount; i++ ) {
		Layout *p = new Layout( results[ i ] );
		p->setParent( this );
		partlist.push_back( p );
	}
	sortChildren( );
}

void Tank::loadTankDetails( ) {
	const LCDbObjects & names = LCDbObjects::records( );
	bool online = false;
	std::vector < ROSETTA > mapping;
	StoreDAO( ).loadTankDetails( id, mapping );
	int best = -1;
	for( unsigned i = 0; i < mapping.size( ); i++ ) {
		if( mapping[ i ].getInt( "status" ) != LCDbTankMap::DELETED ) {
			// remember first valid mapping for first shelf
			Layout population( mapping[ i ] );
			if( best < 0 || population < Layout( mapping[ best ] ) ) {
				best = i;
			}
			if( population.availability( ) == IPart::Availability::IS_AVAILABLE ) {
				online = true;
			}
			// add to history (may create multiple entries)
			std::stringstream detail;
			const LCDbObject * site = names.findByID( population.getLocation_cid( ) );
			if( site != NULL ) {
				detail << site->getName( ) << ", ";
			}
			detail << "position " << population.getTankPos( );
			const LCDbObject * type = names.findByID( population.getStore_type_cid( ) );
			if( type != NULL ) {
				detail << ": " << type->getName( );
			}
			history.insert( { population.getValidFrom(), detail.str() } );
		}
	}
	if( best >= 0 ) {
		setFields( mapping[ best ] );
	}
	setAvailability( online );
}

bool Tank::save( ) {
	StoreDAO dao;
	std::auto_ptr < ROSETTA > data( getObjectData() );
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
	// l->setStorage_cid( id );
	l->setTank_cid( tank_cid );
	if( l->isNew( ) ) {
		if( l->save( ) ) {
			layout_cid = l->getLayout_cid( );
		} else {
			return false;
		}
	}

	data->setInt( "layout_cid", l->getLayout_cid( ) );
	data->setTime( "valid_from", l->getValidFrom( ) );
	data->setTime( "valid_to", l->getValidUntil( ) );
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
	std::auto_ptr < ROSETTA > data( getObjectData() );
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
		// l->setID( map_cid );
	}
	return result;
}

bool Tank::modifyLayout( ) {
	StoreDAO dao;
	std::auto_ptr < ROSETTA > data( getObjectData() );
	Layout *l = ( Layout* )partlist[ 0 ];
	// l->setStorage_cid( storage_cid );
	l->setTank_cid( tank_cid );
	if( l->isNew( ) && !l->save( ) ) {
		return false;
	}
	data->setInt( "layout_cid", l->getID( ) );
	if( !dao.saveTankMap( *data ) ) {
		return false;
	}
	map_cid = data->getInt( "map_cid" );
	// l->setID( map_cid );
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
	status = -1;
}

Layout::Layout( int p_id, std::string p_name ) : IPart( p_id, p_name ) {
	state = NewState;
	type = LayoutType;
	store_type_cid = tank_cid = layout_cid = -1;
	status = -1;
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
		valid_from = data.getDate( "valid_from" ).outputTDateTime( );
	} else if( data.isTime( "valid_from" ) ) {
		valid_from = data.getTime( "valid_from" ).outputTDateTime( );
	}
	if( data.isDate( "valid_to" ) ) {
		valid_to = data.getDate( "valid_to" ).outputTDateTime( );
	} else if( data.isTime( "valid_to" ) ) {
		valid_to = data.getTime( "valid_to" ).outputTDateTime( );
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

/* ROSETTA > Layout::getProperties( ) {
	std::auto_ptr < ROSETTA > r = IPart::getProperties( );
	if( position != 0 ) {
		r->setInt( "shelf_number", position );
	}
	if( store_type_cid != 0 ) {
		const LCDbObject *st = LCDbObjects::records( ).findByID( store_type_cid );
		if( st != NULL ) {
			r->setString( "storage_type", st->getDescription( ) );
		}
	}
	r->setString( "layout", layout_description );
	r->setString( "on_line", availability( ) == IS_AVAILABLE ? "Yes" : "No" );
	r->setString( "valid", getDateRange( ) );
	return r;
}
*/

void Layout::showProperties( TStringGrid *grdProps ) const {
	IPart::showProperties( grdProps );
	int row = grdProps -> RowCount;
	if( position != 0 ) {
		grdProps->Cells[ 0 ][ row ] = "Shelf";
		grdProps->Cells[ 1 ][ row ++ ] = position;
	}
	const LCDbObject *st = LCDbObjects::records( ).findByID( store_type_cid );
	if( st != NULL ) {
		grdProps->Cells[ 0 ][ row ] = "Storage type";
		grdProps->Cells[ 1 ][ row ++ ] = st->getDescription().c_str( );
	}
	grdProps->Cells[ 0 ][ row ] = "Layout";
	grdProps->Cells[ 1 ][ row ++ ] = layout_description.c_str( );
	grdProps->Cells[ 0 ][ row ] = "On line";
	grdProps->Cells[ 1 ][ row ++ ] = availability( ) == IS_AVAILABLE ? "Yes" : "No";
	grdProps->Cells[ 0 ][ row ] = "Valid";
	grdProps->Cells[ 1 ][ row ++ ] = getDateRange( ).c_str( );
	grdProps->RowCount = row;                                                      }



ROSETTA * Layout::getObjectData( ) const {
	ROSETTA * r = IPart::getObjectData( );
	r->setString( "layout_name", layout_name );
	r->setString( "layout_full", layout_description );
	r->setInt( "store_type_cid", store_type_cid );
	r->setInt( "tank_cid", tank_cid );
	r->setInt( "status", status );
	r->setTime( "valid_from", valid_from );
	r->setTime( "valid_to", valid_to );
	return r;
}

std::string Layout::getDateRange( ) const {
	AnsiString result;
	if( EPOCH_START < valid_from && EPOCH_END > valid_from ) {
		result = valid_from.DateString( );
		if( valid_to >= valid_from && EPOCH_END > valid_to ) {
			result = result + " - " + valid_to.DateString( );
		} else if( valid_from > Now( ) ) {
			result = "From " + result;
		} else {
			result = "Since " + result;
		}
	} else {
		if( EPOCH_START < valid_to && EPOCH_END > valid_to ) {
			result = "Until " + valid_to.DateString( );
		} else if( status == LCDbTankMap::DELETED ) {
			result = "Former content";
		} else {
			result = "Current content";
		}
	}
	return result.c_str( );
}

std::string Layout::getName( ) const {
	std::stringstream result;
	if( position > 0 ) {
		result << position << ": ";
	}
	result << getDateRange( ) << ": " << getLayoutName( );
	return result.str( );
}

std::string Layout::getFullName( ) const {
	return getParent( )->getFullName( ) + ", " + getLayoutName( );
}

IPart::Availability Layout::availability( ) const {
	if( (status == LCDbTankMap::NEW_RECORD || status == LCDbTankMap::RECORD_IN_USE)
	 && (EPOCH_END < valid_from || valid_from < Now()) && (valid_to < valid_from || valid_to > Now()) ) {
		return IS_AVAILABLE;
	}
	return UNAVAILABLE;
}

// sort order for Layouts: by shelf, then status, then start date
bool Layout:: operator < ( const IPart& rhs ) const {
	if( rhs.getType( ) != LayoutType ) {
		return type < rhs.getType( );
	}
	if( getPosition( ) != rhs.getPosition( ) ) {
		return getPosition( ) < rhs.getPosition( );
	}
	Availability myA = availability( ), rhsA = ( ( Layout* )( &rhs ) )->availability( );
	if( myA != rhsA ) {
		return myA == IS_AVAILABLE && rhsA == UNAVAILABLE;
	}
	return valid_from < ( ( Layout* )( &rhs ) )->getValidFrom( );
}

// fill in dates if necessary
void Layout::setAvailability( bool online ) {
	TDateTime change = Now( );
	if( EPOCH_START < valid_from || valid_from > change ) {
		valid_from = change;
	}
	if( online ) {
		status = LCDbTankMap::RECORD_IN_USE;
		valid_to = 0;
	} else {
		status = LCDbTankMap::OFFLINE;
		if( EPOCH_START < valid_to || valid_to > change ) {
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
	// read existing racks in this "population" - sections don't exist in database
	StoreDAO dao;
	std::vector < ROSETTA > racks;
	if( tank_cid != 0 && tank_cid != -1 ) {
		dao.loadRacks( tank_cid, racks );
	}
	// read section definitions for this layout - racks should match one of them
	std::vector < ROSETTA > sections;
	dao.loadSections( layout_cid, sections );
	capacity = sections.size( );
	for( int i = 0; i < capacity; i++ ) {
		Section *s = new Section( sections[ i ] );
		s->populate( racks );
		s->setParent( this );
		partlist.push_back( s );
	}
	childCount = partlist.size( );
}

bool Layout::save( ) {
	StoreDAO dao;
	std::auto_ptr < ROSETTA > data( getObjectData() );
	if( isNew( ) ) {
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
	std::auto_ptr < ROSETTA > data( getObjectData() );
	StoreDAO().setLayoutAvailability( *data );
}

Section::Section( ) : IPart( ) {
	state = NewState;
	type = SectionType;
	rack_layout_cid = tank_cid = map_cid = 0;
	rackSize = first = last = -1;
}

Section::Section( int p_id, std::string p_name ) : IPart( p_id, p_name ) {
	state = NewState;
	type = SectionType;
	rack_layout_cid = tank_cid = map_cid = 0;
	rackSize = first = last = -1;
}

void Section::populate( const std::vector < ROSETTA > & dbRacks ) {
	char buff[ 60 ];
	short posInRack = 1, posInTank = 1;
	const LCDbSectionDef * layout = LCDbSectionDefs::records( ).findByID( id );
	if( layout != NULL ) {
		posInTank = layout->getFirstPosition( );
	}
	clearList( );
	for( short n = first; n <= last; n++, posInRack++, posInTank++ ) {
		Rack * r = NULL;
		for( const ROSETTA & ri : dbRacks ) {
			if( ri.getIntDefault( "position", -999 ) == posInTank ) {
				r = new Rack( ri );
			}
		}
		if( r == NULL ) {
			// add in empty racks to fill the section
			r = new Rack( );
			std::sprintf( buff, "%s%d", name.c_str( ), n );
			r->setName( buff );
			r->setPosInTank( posInTank );
			r->setEmptySlots( rackSize );
		}
		r->setMap_cid( map_cid );
		r->setPosition( posInRack );
		r->setCapacity( rackSize );
		r->setParent( this );
		partlist.push_back( r );
	}
	childCount = partlist.size( );
}

/*
void Section::populate( const std::multimap < short, Rack * > & dbRacks ) {
	const LCDbSectionDef * layout = LCDbSectionDefs::records( ).findByID( id );
	short posInRack = 1, posInTank = ( layout == NULL ? -999 : layout->getFirstPosition( ) );
	std::multimap < short, Rack* > ::const_iterator mmi = dbRacks.begin( );
	clearList( );
	for( short n = first; n <= last || mmi != dbRacks.end( ); n++, posInRack++, posInTank++ ) {
		Rack * r;
		if( mmi == dbRacks.end( ) || posInRack < mmi->first ) {
			char buff[ 60 ];
			// add in empty racks to fill the section
			r = new Rack( );
			std::sprintf( buff, "%s%d", name.c_str( ), n );
			r->setName( buff );
			r->setPosition( posInRack );
			r->setEmptySlots( rackSize );
		} else {
			r = mmi->second;
			mmi++;
		}
		r->setMap_cid( map_cid );
		r->setCapacity( rackSize );
		r->setPosInTank( posInTank );
		r->setParent( this );
		partlist.push_back( r );
	}
	childCount = partlist.size( );
}
*/

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
	capacity = ( last > first ? 1 + last - first : -1 );
	rackSize = data.getIntDefault( "rack_capacity", -1 );
	tank_cid = data.getIntDefault( "tank_cid", -1 );
	map_cid = data.getIntDefault( "map_cid", -1 );

	mapped2 = NULL;
	parent = NULL;
	state = SyncState;
	type = SectionType;
	childCount = -1;
}

/*
std::auto_ptr < ROSETTA > Section::getProperties( ) {
	std::auto_ptr < ROSETTA > r = IPart::getProperties( );
	r->setInt( "first_structure", first );
	r->setInt( "last_structure", last );
	if( rackSize > 0 ) {
		r->setInt( "structure_size", rackSize );
	}
	return r;
}
*/
void Section::showProperties( TStringGrid *grdProps ) const {
	IPart::showProperties( grdProps );
	int row = grdProps -> RowCount;
	grdProps->Cells[ 0 ][ row ] = "First structure";
	grdProps->Cells[ 1 ][ row ++ ] = first;
	grdProps->Cells[ 0 ][ row ] = "Last structure";
	grdProps->Cells[ 1 ][ row ++ ] = last;
	if( rackSize > 0 ) {
		grdProps->Cells[ 0 ][ row ] = "Structure size";
		grdProps->Cells[ 1 ][ row ++ ] = rackSize;
	}
	grdProps->RowCount = row;
}

ROSETTA * Section::getObjectData( ) const {
	ROSETTA * r = IPart::getObjectData( );
	r->setInt( "layout_cid", rack_layout_cid );
	r->setInt( "fill_order", position );
	r->setInt( "first_rack", first );
	r->setInt( "last_rack", last );
	r->setInt( "rack_capacity", rackSize );
	r->setInt( "map_cid", map_cid );
	return r;
}

std::string Section::getFullName( ) const {
	std::string result;
	if( name.length( ) < 3 ) {
		result = "section " + name;
	} else if( stricmp( name.c_str( ), "Rack" ) == 0 ) {
		result = "rack list";
	} else {
		result = name;
	}
	return getParent( )->getFullName( ) + ", " + result;
}

void Section::populate( ) {
	if( childCount >= 0 || tank_cid == -1 ) {
		return;
	}
	std::vector < ROSETTA > results;
	StoreDAO().loadRacks( tank_cid, results, id );
	populate( results );
}

IPart::Availability Section::availability( ) const {
	short empty = 0, full = 0;
	for( int i = 0; i < childCount; i++ ) {
		Rack * r = ( Rack* )( partlist[ i ] );
		switch( r->availability( ) ) {
		case IS_EMPTY:
			empty++;
			break;
		case IS_FULL:
			full++;
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
	rack_layout_cid = t->getLayout_cid( );
	std::auto_ptr < ROSETTA > data( getObjectData() );
	bool result = dao.saveSection( *data );
	if( result ) {
		state = SyncState;
		id = data->getInt( "ID" );
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

	const LCDbSectionDef * layout = LCDbSectionDefs::records( ).findByID( rack_type_cid );
	position = ( layout == NULL ? -1 : posInTank - ( layout->getFirstPosition( ) - 1 ) );
	mapped2 = NULL;
	parent = NULL;
	state = SyncState;
	type = RackType;
	childCount = -1;
}

/*
std::auto_ptr < ROSETTA > Rack::getProperties( ) {
	std::auto_ptr < ROSETTA > r = IPart::getProperties( );
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
*/
void Rack::showProperties( TStringGrid *grdProps ) const {
	IPart::showProperties( grdProps );
	int row = grdProps -> RowCount;
	grdProps->Cells[ 0 ][ row ] = "Position";
	grdProps->Cells[ 1 ][ row ++ ] = position;
	if( capacity > 0 ) {
		grdProps->Cells[ 0 ][ row ] = "Total slots";
		grdProps->Cells[ 1 ][ row ++ ] = capacity;
	}
	if( childCount > 0 ) {
		grdProps->Cells[ 0 ][ row ] = "Slots used";
		grdProps->Cells[ 1 ][ row ++ ] = childCount;
	}
	if( emptySlots > 0 ) {
		grdProps->Cells[ 0 ][ row ] = "Empty slots";
		grdProps->Cells[ 1 ][ row ++ ] = emptySlots;
	}
	grdProps->RowCount = row;
}

ROSETTA * Rack::getObjectData( ) const {
	ROSETTA * r = IPart::getObjectData( );
	r->setInt( "tank_cid", tank_cid );
	r->setInt( "rack_type_cid", rack_type_cid );
	// r->setInt( "map_cid", map_cid );
	r->setInt( "project_cid", project_cid );
	r->setInt( "box_type_cid", box_type_cid );
	r->setInt( "empty_slots", emptySlots );
	r->setInt( "pos_in_tank", posInTank );
	r->setInt( "pos_in_section", position );
	Section * p = dynamic_cast < Section* > ( parent );
	if( p != NULL ) {
		Site * s = dynamic_cast < Site* > ( p->getParent( )->getParent( )->getParent( ) );
		if( s != NULL ) {
			r->setString( "location", s->getName( ) );
		}
	}
	return r;
}

std::string Rack::getFullName( ) const {
	std::string result = name;
	Layout * l = dynamic_cast < Layout* > ( getParent( )->getParent( ) );
	if( l != NULL ) {
		result = l->getFullName( ) + ", " + result;
	}
	return result;
}

void Rack::populate( ) {
	if( childCount >= 0 || id == -1 ) {
		return;
	}
	// find expected number of slots for the current segment
	const LCDbSectionDef * layout = LCDbSectionDefs::records( ).findByID( rack_type_cid );
	if( layout != NULL ) {
		capacity = layout->getRackCapacity( );
	} else if( capacity < 1 ) {
		capacity = 99; // error ??
	}
	// allocate boxes to slots; allow more than one per slot (e.g. during box move)
	StoreDAO dao;
	std::vector < ROSETTA > results;
	dao.loadBoxes( id, results );
	std::multimap < int, Box* > boxes;
	std::set < int > projects, boxTypes;
	for( const ROSETTA & ri : results ) {
		Box * b = new Box( ri );
		boxes.insert({ b->getPosition(), b } );
		projects.insert( b->getProjectCID( ) );
		boxTypes.insert( b->getBoxTypeCID( ) );
	}
	for( auto bi : boxes ) {
		bi.second->setParent( this );
		partlist.push_back( bi.second );
	}
	childCount = partlist.size( );

	// update summary fields in c_rack_number if rack contents have changed
	bool changed = name.empty( );
	int boxProj = ( projects.empty( ) ? -1 : projects.size( ) == 1 ? *projects.begin( ) : 0 );
	if( project_cid != boxProj ) {
		project_cid = boxProj;
		changed = true;
	}
	int boxType = ( boxTypes.size( ) == 1 ? *boxTypes.begin( ) : 0 );
	if( box_type_cid != boxType ) {
		box_type_cid = boxType;
		changed = true;
	}
	short slots = capacity - childCount;
	if( emptySlots != slots ) {
		emptySlots = slots;
		changed = true;
	}
	if( tank_cid != -1 && changed ) {
		save( );
	}
}

IPart::Availability Rack::availability( ) const {
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
bool Rack::save( ) {
	StoreDAO dao;
	std::auto_ptr < ROSETTA > data( getObjectData() );
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

bool IPart::validField( const ROSETTA &record, const std::string &field ) {
	const ROSE_BASE * data = record.pointerRoseBase( field );
	return data != NULL && data->isValid()  && !data->hasTag( XSQL::null );
}

Box::Box( const ROSETTA &data ) {
	if( data.isInt( "box_cid" ) ) {
		id = data.getInt( "box_cid" );
	} else {
		id = data.getIntDefault( "box_id", -1 );
	}
	box_type_id = data.getIntDefault( "box_type_cid", -1 );
	if( data.isInt( "record_id" ) ) {
		record_id = data.getInt( "record_id" );
	} else {
		record_id = data.getIntDefault( "slot_cid", 0 );
	}

	rack_cid = validField( data, "rack_cid" ) ? data.getInt( "rack_cid" ) : -1;
	position = validField( data, "slot_position" ) ? data.getInt( "slot_position" ) : 0;
	emptySlots = validField( data, "box_capacity" ) ? data.getInt( "box_capacity" ) : -1;
	retrieval_cid = validField( data, "retrieval_cid" ) ? data.getInt( "retrieval_cid" ) : 0;
	status = validField( data, "status" ) ? data.getInt( "status" ) : 0;

	if( data.isString( "external_name" ) ) {
		name = data.getString( "external_name" );
	}
	if( data.isString( "barcode" ) ) {
		barcode = data.getString( "barcode" );
	}
	project_cid = data.getIntDefault( "project_cid", -1 );
	if( project_cid == -1 && data.isString( "db_name" ) ) {
		std::string db_name = data.getString( "db_name" );
		const LCDbProject * p = LCDbProjects::records( ).findByName( db_name );
		if( p != NULL ) {
			project_cid = p->getID( );
		}
	}
	mapped2 = NULL;
	parent = NULL;
	state = SyncState;
	type = BoxType;
	childCount = -1;
	capacity = -1;
}

std::string Box::getName( ) const {
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

	if( movement != NULL && mapped2 != NULL && mapped2->getParent( ) != NULL ) {
		Rack * r = ( Rack* )( mapped2->getParent( ) );
		const std::string & rack = r->getName( );
		Tank * t = ( Tank* )( r->getParent( )->getParent( )->getParent( ) );
		const std::string & store = t->getVessel( );
		short slot = ( ( Box* )mapped2 )->getPosition( );
		char label[ 150 ];
		std::sprintf( label, "%s %s %s %s, %d", name.c_str( ), movement, store.c_str( ),
			rack.c_str( ), slot );
		return label;
	} else {
		return name;
	}
}

void Box::showProperties( TStringGrid *grdProps ) const {
	IPart::showProperties( grdProps );
	int row = grdProps -> RowCount;
	if( !barcode.empty() ) {
		grdProps->Cells[ 0 ][ row ] = "Barcode";
		grdProps->Cells[ 1 ][ row ++ ] = barcode.c_str();
	}
	const char * strStat = NULL;
	switch( status ) {
		case LCDbBoxStore::EXPECTED:
			strStat = "Expected";
			break;
		case LCDbBoxStore::MOVE_EXPECTED:
			strStat = "Move expected";
			break;
		case LCDbBoxStore::SLOT_ALLOCATED:
			strStat = "Allocated";
			break;
		case LCDbBoxStore::SLOT_CONFIRMED:
			strStat = "Confirmed";
			break;
		case LCDbBoxStore::REMOVED:
			strStat = "Removed";
			break;
		case LPDbBoxName::DESTROYED:
			strStat = "Destroyed";
			break;
		case LPDbBoxName::DELETED:
			strStat = "Deleted";
			break;
		default:
			;	// invalid status?
	}
	if( strStat != NULL ) {
		grdProps->Cells[ 0 ][ row ] = "Status";
		grdProps->Cells[ 1 ][ row ++ ] = strStat;
	}
	const LPDbBoxType *content = LPDbBoxTypes::records( ).findByID( box_type_id );
	if( content != NULL ) {
		grdProps->Cells[ 0 ][ row ] = "Content";
		grdProps->Cells[ 1 ][ row ++ ] = content->getDescription( ).c_str( );
	}
	grdProps->RowCount = row;
}

ROSETTA * Box::getObjectData( ) const {
	ROSETTA * r = IPart::getObjectData( );
	r->setInt( "record_id", record_id );
	r->setInt( "rack_cid", rack_cid );
	r->setInt( "box_type_cid", box_type_id );
	r->setInt( "tank_cid", ( ( Rack * )parent )->getTank_cid( ) );
	r->setInt( "retrieval_cid", retrieval_cid );
	r->setString( "rack_name", parent->getName( ) );
	r->setInt( "slot_position", position );
	r->setInt( "rack_position", ( ( Rack * )parent )->getPosition( ) );
	r->setInt( "project_cid", project_cid );
	r->setString( "barcode", barcode );
	r->setInt( "status", status );
	return r;
}

std::string Box::getFullName( ) const {
	char buff[ 20 ];
	std::sprintf( buff, ", slot %d", position );
	return parent->getFullName( ) + buff;
}

void Box::populate( ) {
	if( childCount >= 0 || id == -1 ) {
		return;
	}

	StoreDAO dao;
	std::vector < ROSETTA > results;

	// list cryovials in the box, ignoring empty spaces
	dao.loadSamples( id, project_cid, results );
	for( const ROSETTA & bi : results ) {
		Sample * next = new Sample( bi );
		next->setParent( this );
		partlist.push_back( next );
	}
	childCount = partlist.size( );
	sortChildren( );

	const LPDbBoxType * type = LPDbBoxTypes::records( ).findByID( box_type_id );
	if( type != NULL ) {
		const LCDbBoxSize * size = LCDbBoxSizes::records( ).findByID( type->getSizeID( ) );
		if( size != NULL ) {
			capacity = size->getCapacity( );
			if( childCount == 0 ) {
				emptySlots = -1; // no cryovial records, e.g. KADOORIE
			} else {
				emptySlots = capacity - childCount;
			}
		}
	}

	// list previous positions of this box, if available
	dao.loadBoxHistory( id, results );
	for( const ROSETTA & hi : results ) {
		TDateTime when = hi.getTime( "time_stamp" ).outputTDateTime( );
		std::stringstream detail;
		detail << hi.getString( "vessel" );
		short shelf = hi.getIntDefault( "shelf_number", -1 );
		if( shelf > 0 ) {
			detail << '[' << shelf << "] ";
		} else {
			detail << ' ';
		}
		detail << hi.getString( "rack" ) << ", slot " << hi.getInt( "slot_position" );
		history.insert( { when, detail.str() } );
	}
}

bool Box::canMove( ) const {
	return ( status == LCDbBoxStore::SLOT_CONFIRMED );
}

IPart::Availability Box::availability( ) const {
	if( status != LCDbBoxStore::SLOT_CONFIRMED || retrieval_cid != 0 ) {
		return UNAVAILABLE;
	} else if( emptySlots < 1 ) {
		return IS_FULL;
	} else if( childCount == 0 ) {
		return IS_EMPTY;
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
	std::vector < ROSETTA > result;
	std::set < int > ids;
	ids.insert( rack_cid );
	dao.loadRacks( ids, result );
	if( !result.empty( ) ) {
		parent = new Rack( result[ 0 ] );
	}
}

bool Box::addToLHSJobList( int jobID ) {
	if( retrieval_cid == jobID ) {
		return true;
	}
	retrieval_cid = jobID;
	std::auto_ptr < ROSETTA > data( getObjectData() );
	return StoreDAO().addBoxToLHSJobList( *data );
}

bool Box::addToRHSJobList( int jobID ) {
	if( retrieval_cid == jobID ) {
		return true;
	}
	Rack * rack = dynamic_cast < Rack* > ( parent );
	Box * source = dynamic_cast < Box* > ( mapped2 );
	if( source == NULL || rack == NULL || !rack->save( ) ) {
		return false;
	}
	rack_cid = rack->getID( );
	retrieval_cid = jobID;
	project_cid = source->project_cid;
	id = source->getID( );
	status = LCDbBoxStore::EXPECTED;
	std::auto_ptr < ROSETTA > data( getObjectData() );
	return StoreDAO( ).updateBox( *data );
}

bool Box::save( ) {
	std::auto_ptr < ROSETTA > data( getObjectData() );
	return StoreDAO( ).updateBox( *data );
}

/*
bool Box::signoff( ) {
	StoreDAO dao;
	std::auto_ptr < ROSETTA > data( getObjectData() );
	if( !dao.signoffBox( *data ) ) {
		return false;
	}
	retrieval_cid = 0;
	status = LCDbBoxStore::SLOT_CONFIRMED;
	return true;
}

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
	project_cid = data.getIntDefault( "project_cid", -1 );
	sample_id = data.getIntDefault( "sample_id", 0 );
	if( data.isTime( "time_stamp" ) ) {
		stamp = data.getTime( "time_stamp" );
	}
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
	if( data.isInt( "box_id" ) ) {
		box_id = data.getInt( "box_id" );
	} else {
		box_id = data.getIntDefault( "box_cid", -1 );
    }

	mapped2 = NULL;
	parent = NULL;
	state = SyncState;
	type = SampleType;
	childCount = -1;
}

IPart::Availability Sample::availability( ) const {
	return name.empty( ) ? UNKNOWN : IS_AVAILABLE;
}

std::string Sample::getFullName( ) const {
	char buff[ 20 ];
	std::sprintf( buff, ", position %d", position );
	return parent->getName( ) + buff;
}

void Sample::populate( ) {
	if( childCount >= 0 || id == -1 ) {
		return;
	}
	partlist.clear( );
	std::vector < ROSETTA > results;
	StoreDAO dao;
	// add history; analysis before storage if the same day
	dao.loadAnalysisHistory( name, aliquot_type, project_cid, results );
	for( const ROSETTA & hi : results ) {
		std::stringstream detail;
		detail << "Analysed: ";
		int n = hi.getInt( "results" );
		if( n == 1 ) {
			detail << "1 result";
		} else {
			detail << n << " results";
		}
		TDateTime when = hi.getTime( "when" ).outputTDateTime( );
		history.insert( { when, detail.str() } );
	}
	dao.loadStorageHistory( id, project_cid, results );
	for( const ROSETTA & hi : results ) {
		TDateTime when = hi.getTime( "time_stamp" ).outputTDateTime( );
		std::stringstream detail;
		detail << hi.getString( "box_name" );
		int position = hi.getIntDefault( "cryovial_position", -1 );
		if( position > 0 ) {
			detail << ", position " << position;
		}
		float volume = hi.getRealDefault( "sample_volume", -1 );
		if( volume > 0 ) {
			detail << ": " << volume << "ml";
		}
		history.insert( { when, detail.str() } );
	}
	childCount = 0;

}

/*
std::auto_ptr < ROSETTA > Sample::getProperties( ) {
	std::auto_ptr < ROSETTA > r = IPart::getProperties( );
	const LCDbObject *at = NULL;
	if( aliquot_type != 0 && aliquot_type != -1 ) {
		at = LCDbObjects::records( ).findByID( aliquot_type );
	}
	if( at != NULL ) {
		r->setString( "aliquot_type", at->getDescription( ).c_str( ) );
	}
	if( !source_name.empty( ) && source_name != name ) {
		r->setString( "source_name", source_name );
	}
	if( !source_barcode.empty( ) && source_barcode != name ) {
		r->setString( "source_barcode", source_barcode );
	}
	return r;
}
*/
void Sample::showProperties( TStringGrid *grdProps ) const {
	IPart::showProperties( grdProps );
	int row = grdProps -> RowCount;
	const LCDbObject *at = NULL;
	if( aliquot_type != 0 && aliquot_type != -1 ) {
		at = LCDbObjects::records( ).findByID( aliquot_type );
	}
	if( at != NULL ) {
		grdProps->Cells[ 0 ][ row ] = "Aliquot type";
		grdProps->Cells[ 1 ][ row ++ ] = at->getDescription( ).c_str( );
	}
	if( !source_name.empty( ) && source_name != name ) {
		grdProps->Cells[ 0 ][ row ] = "Source name";
		grdProps->Cells[ 1 ][ row ++ ] = source_name.c_str( );
	}
	if( !source_barcode.empty( ) && source_barcode != name ) {
		grdProps->Cells[ 0 ][ row ] = "Source barcode";
		grdProps->Cells[ 1 ][ row ++ ] = source_barcode.c_str( );
	}
	grdProps->RowCount = row + 1;
}

ROSETTA * Sample::getObjectData( ) const {
	ROSETTA * r = IPart::getObjectData( );
	r->setInt( "cryovial_id", id );
	r->setInt( "cryovial_position", position );
	r->setInt( "aliquot_type", aliquot_type );
	r->setInt( "box_id", box_id );
	r->setString( "source_barcode", source_barcode );
	return r;
}

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
	for( Range < LCDbObject > ul = objs; ul.isValid( ); ++ul ) {
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
	const LCDbSectionDefs & sections = LCDbSectionDefs::records( );
	std::vector < ROSETTA > racks;
	StoreDAO( ).loadRacks( tank_cid, racks );
	for( const ROSETTA & ri : racks ) {
		int type = ri.getIntDefault( "rack_type_cid", 0 );
		if( type != 0 ) {
			const LCDbSectionDef * sd = sections.findByID( type );
			if( sd != NULL ) {
				return sd->getTankLayoutCID( );
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
	if( i >= 0 && i < layouts.size( ) ) {
		return layouts[ i ];
	} else {
		return NULL;
	}
}

bool Layouts::isNameDuplicate( bool full, std::string name ) const {
	for( unsigned i = 0; i < layouts.size( ); i++ ) {
		if( full ) {
			if( layouts[ i ]->getLayoutDescription( ) == name ) {
				return true;
			}
		} else {
			if( layouts[ i ]->getLayoutName( ) == name ) {
				return true;
			}
		}
	}
	return false;
}

int Layouts::getDefaultLayoutId( int store_cid ) const {
	int layout_cid = -1;
	for( Range < LCDbTankMap > tmr = LCDbTankMaps::records( ); tmr.isValid( ); ++tmr ) {
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

// ============== Available part classes ==============

void AvlSites::populate( ) {
	if( childCount >= 0 ) {
		return;
	}
	std::vector < ROSETTA > results;
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
	std::vector < ROSETTA > results;
	StoreDAO( ).loadTanks( id, results );
	std::map < int, AvlTank* > tanks;
	for( const ROSETTA & r : results ) {
		AvlTank * p = new AvlTank( r );
		p->loadTankDetails( );
		p->readRackOccupancy( );
		std::map < int, AvlTank* > ::iterator ti = tanks.find( p->getID( ) );
		if( ti == tanks.end( ) ) {
			p->setParent( this );
			partlist.push_back( p );
			tanks.insert( { p->getID(), p } );
		} else {
			ti->second->addRackOccupancy( p );
			delete p; // same tank, another layout
		}
	}
	childCount = partlist.size( );
	sortChildren( );
}

void AvlTank::readRackOccupancy( ) {
	const LCDbSectionDefs & sDefs = LCDbSectionDefs::records( );
	totalSlots = usedSlots = 0;
	for( Range < LCDbSectionDef > sr = sDefs; sr.isValid( ); ++sr ) {
		if( sr->getTankLayoutCID( ) == layout_cid ) {
			totalSlots += ( sr->getCapacity( ) * sr->getRackCapacity( ) );
		}
	}
	std::vector < ROSETTA > results;
	StoreDAO( ).loadRacks( tank_cid, results );
	for( const ROSETTA & ri : results ) {
		Rack r( ri );
		short filled = 1; // empty racks are not recorded in c_rack_number
		if( r.getEmptySlots( ) >= 0 ) {
			const LCDbSectionDef * sd = sDefs.findByID( r.getType_cid( ) );
			if( sd != NULL ) {
				filled = sd->getRackCapacity( ) - r.getEmptySlots( );
			}
		}
		usedSlots += filled;
	}
}

void AvlTank::addRackOccupancy( const AvlTank * other ) {
	usedSlots += other->usedSlots;
	totalSlots += other->totalSlots;
}

float AvlTank::getFillFactor( ) const {
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
	std::vector < ROSETTA > results;
	StoreDAO( ).loadLayouts( id, results );
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
	std::vector < ROSETTA > results;
	StoreDAO( ).loadSections( layout_cid, results );
	capacity = results.size( );
	for( int i = 0; i < capacity; i++ ) {
		AvlSection *p = new AvlSection( results[ i ] );
		p->setTank_cid( tank_cid );
		partlist.push_back( p );
		p->setParent( this );
		p->setMap_cid( getID( ) );
	}
	childCount = partlist.size( );
}

void AvlSection::populate( ) {
	if( childCount >= 0 ) {
		return;
	}
	std::set < std::string > racknames;
	StoreDAO dao;
	std::vector < ROSETTA > results;
	dao.loadRacks( tank_cid, results, id );
	for( std::vector < ROSETTA > ::const_iterator ri = results.begin( ); ri != results.end( );
	++ri ) {
		AvlRack *p = new AvlRack( *ri );
		racknames.insert( p->getName( ) );
		// check the occupancy if it's easy to do so; ignore if full
		std::set < int > occupied;
		dao.loadRackOccupancy( p->getID( ), occupied );
		if( occupied.size( ) >= rackSize ) {
			delete p;
			continue;
		} else {
			p->setOccupancy( occupied );
		}
		p->setCapacity( rackSize );
		p->setParent( this );
		partlist.push_back( p );
	}
	char buf[ 40 ];
	for( short i = first; i <= last; i++ ) {
		std::sprintf( buf, "%s%d", name.c_str( ), i );
		std::string r_name = buf;
		if( racknames.find( r_name ) == racknames.end( ) ) {
			AvlRack *p = new AvlRack( tank_cid, r_name, id, map_cid, name, rackSize, i );
			p->setParent( this );
			partlist.push_back( p );
		}
	}
	capacity = last - first + 1;
	childCount = partlist.size( );
	sortChildren( );
}

float AvlSection::getFillFactor( ) const {
	if( childCount < 0 ) {
		return -1;
	}
	float racks = capacity - childCount;
	for( const IPart * pi : partlist ) {
		AvlRack * r = ( AvlRack* ) pi;
		float fill = r->getFillFactor( );
		if( fill < 0 ) {
			return -1;
		} else {
			racks += fill;
		}
	}
	return racks / capacity;
}

AvlRack::AvlRack( int p_tank_cid, const std::string & p_name, int p_type_cid, int p_map_cid,
	const std::string & p_sec, short p_capacity, short pos ) : Rack( ) {
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
	if( id != -1 && occupied.empty( ) ) {
		StoreDAO( ).loadRackOccupancy( id, occupied );
	}
	for( int i = 1; i <= capacity; i++ ) {
		if( occupied.count( i ) == 0 ) {
			char buf[ 70 ];
			std::sprintf( buf, "%s empty slot %d", name.c_str( ), i );
			AvlBox *p = new AvlBox( -1, buf );
			p->setPosition( i );
			p->setParent( this );
			partlist.push_back( p );
		}
	}
	childCount = partlist.size( );
	emptySlots = 0;
}

void AvlRack::setOccupancy( std::set < int > & positions ) {
	occupied = positions;
	emptySlots = capacity - occupied.size( );
}

float AvlRack::getFillFactor( ) const {
	if( id == -1 || emptySlots == capacity ) {
		return 0;
	} else {
		return occupied.size( ) / float( capacity );
	}
}

AvlBox::AvlBox( int p_rack_cid, std::string p_name ) : Box( ) {
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

// ---------------------------------------------------------------------------
/***** Factory Class **** */

void PartFactory::loadBoxes( const LCDbCryoJob &job ) {

	StoreDAO dao;
	std::vector < ROSETTA > results;
	int projID = job.getProjectID( ); // 0 => multiple; -1 => unknown
	if( projID != 0 && projID != -1 ) {
		dao.loadBoxesByJobID( job.getID( ), projID, lhs, results );
	} else {
		for( Range < LCDbProject > pr = LCDbProjects::records( ); pr.isValid( ); ++pr ) {
			if( pr->isInCurrentSystem( ) && !pr->isCentral( ) ) {
				dao.loadBoxesByJobID( job.getID( ), pr->getID( ), lhs, results );
			}
		}
	}

	// buildMovedTo = true will build RHS tree in SampleMove screen
	for( const ROSETTA & ri : results ) {
		Box *b = new Box( ri );
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

// ---------------------------------------------------------------------------
// add boxes and their racks, sections and layouts to the given site list
// ---------------------------------------------------------------------------

IPart * PartFactory::createSiteList( ) const {
	// create the parent object to be displayed in the UI
	Sites * list = lhs ? new Sites( ) : new AvlSites( );
	list->populate();
	if( boxList.empty( ) ) {
		return list;
	}

	ROSETTA result;
	std::vector < ROSETTA > results;
	StoreDAO dao;
	std::set < int > rackIDs, tankIDs;

	// find valid racks; list the tanks they belong to
	for( Box * bi : boxList ) {
		rackIDs.insert( bi->getRackCID( ) );
	}
	dao.loadRacks( rackIDs, results );
	std::vector < Rack* > rackList;
	for( const ROSETTA & ri : results ) {
		Rack *r = new Rack( ri );
		rackList.push_back( r );
		tankIDs.insert( r->getTank_cid() );
	}
	std::sort( rackList.begin( ), rackList.end( ), Rack::less_than );

	// recreate the tanks they belong to, leaving orphans in rackIDs
	std::vector < Tank* > tankList;
	const LCDbObjects & objs = LCDbObjects::records();
	for( const LCDbTankMap & tm : LCDbTankMaps::records( ) ) {
		if( tm.isActive( ) && tankIDs.count( tm.getTankCID( ) ) != 0 ) {
			const LCDbObject *population = objs.findByID( tm.getTankCID( ) );
			const LCDbObject *layout = objs.findByID( tm.getLayoutCID( ) );
			const LCDbObject *phys = objs.findByID( tm.getStorageCID( ) );
			if( population == NULL || phys == NULL || layout == NULL ) {
				continue;
			}

			result.setInt( "storage_cid", phys->getID( ) );
			result.setString( "friendly_name", phys->getDescription( ) );
			result.setString( "serial_number", phys->getName( ) );
			result.setInt( "location_cid", tm.getLocationCID( ) );
			result.setInt( "position", tm.getPosition( ) );
			result.setInt( "storage_type", tm.getStoreTypeCID( ) );
			result.setInt( "map_cid", tm.getID( ) );
			result.setInt( "tank_cid", tm.getTankCID( ) );
			result.setInt( "rack_layout_cid", tm.getLayoutCID( ) );
			result.setInt( "status", tm.getStatus( ) );
			std::pair < TDateTime, TDateTime > valid = tm.getValidDates( );
			result.setDate( "valid_from", valid.first );
			result.setDate( "valid_to", valid.second );
			Tank *t = new Tank( result );

			result.setString( "layout_name", layout->getName( ) );
			result.setString( "layout_full", layout->getDescription( ) );
			result.setString( "label_name", population->getName( ) );
			result.setString( "label_full", population->getDescription( ) );
			Layout *l = new Layout( result );
			dao.loadSections( tm.getLayoutCID( ), results );
			for( const ROSETTA & ri : results ) {
				Section *s = new Section( ri );
				for( Rack * r : rackList ) {
					if( r->getTank_cid( ) == tm.getTankCID( ) && r->getType_cid( ) == s->getID( ) )	{
						s->addToList( r );
						rackIDs.erase( r->getID() );
					}
				}
				s->setMap_cid( tm.getID( ) );
				s->setTank_cid( tm.getTankCID( ) );
				l->addToList( s );
			}
			t->addToList( l );
			tankList.push_back( t );
		}
	}
	std::sort( tankList.begin( ), tankList.end( ), Tank::less_than );

	// add dummy entry for any racks that haven't been found
	if( !rackIDs.empty() ) {
		Tank * t = new Tank( -1, "Unknown" );
		Layout *l = new Layout( -1, "Unknown" );
		Section * s = new Section( -1, "Unknown" );
		for( int id: rackIDs ) {
			Rack * r = new Rack( id, "Not yet stored" );
			s->addToList( r );
			rackList.push_back( r );
		}
		l->addToList( s );
		t->addToList( l );
		tankList.push_back( t );
	}
	for( IPart * s : list->getList() ) {
		for( Tank * t : tankList ) {
			if( s->getID( ) == t->getLocationID( ) ) {
				s->addToList( t );
			}
		}
	}

	// link in the boxes once the structure is in place
	for( Rack * r : rackList ) {
		if( sibs && rackIDs.count( r->getID() ) == 0 ) {
			r->populate( );
		} else {
			for( Box * bi : boxList ) {
				if( r->getID( ) == bi->getRackCID( ) ) {
					r->addToList( bi );
				}
			}
		}
	}
	return list;
}

// ---------------------------------------------------------------------------
