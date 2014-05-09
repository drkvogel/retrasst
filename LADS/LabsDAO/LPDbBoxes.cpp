/*---------------------------------------------------------------------------
 *
 *	12 July 2005, NG:	Store slot_cid with methods to get/set/save it
 *  14 July - bug fix:  Don't assume ID allocated when creating record
 *  19 July 2005, NG:	Added method to read a named box from the database
 *  9/09/05 - bug fix:	Always create database record in createNew()
 *  22 April 2008, NG:	Link to the audit trail to find out who checked entry
 *  23 May 2008, NG:	Added confirmation records using box_event_history
 *  13 June 2008, NG:	Added readOperator() to check box_event_history
 *  18 July 08, NG:		Create event as a side-effect of saving a box
 *  22 July 08, NG:		Keep track of cryovials, not just first and last
 *	29 July 2008, NG:	add method to check if box matches others in group
 *	31 July 08, NG:		bug fix: reinitialise all variables for new box
 *  14 August 08, NG:	include boxes with space in them even if confirmed
 *  24 Feb 2009, NG:	add support for copying box arrival history events
 *	27 May 2009, NG:	set rack_cid + slot_position when appropriate
 *  1 Feb 2010, NG:		count boxes that are ready to take to Worminghall
 *  5 Feb 2013, NG:		set new status (ANALYSED) if ready to be transferred
 * 20 September 2013:	ignore events - no longer required; add barcode
 * 12 December, NG:		Check box_cid not already used in another project
 * 12 March 2014, NG:	Prefer c_box_content for most queries (db 2.7.2)
 *
 *---------------------------------------------------------------------------*/

#include <vcl.h>
#include <sstream>

#include "LPDbBoxes.h"
#include "LCDbProject.h"
#include "LDbBoxType.h"
#include "LCDbOperator.h"
#include "LCDbAuditTrail.h"
#include "LDbBoxSize.h"
#include "LPDbCryovial.h"
#include "LDbBoxStore.h"
#include "LPDbCryovialStore.h"
#include "LPDbDescriptor.h"
#include "LCDbObject.h"

#pragma hdrstop
#pragma package(smart_init)

// static const char * CRYOVIALS_ADDED = "MoreCryovials";
// static const char * CONTENT_CONFIRMED = "BoxConfirmed";

//---------------------------------------------------------------------------
//	read a box_name record and try to work out if there's any space left
//---------------------------------------------------------------------------

LPDbBoxName::LPDbBoxName( const LQuery & query )
 : LPDbID( query.readInt( "box_cid" ) ),
   boxTypeID( query.readInt( "box_type_cid" ) ),
   name( query.readString( "external_name" ) ),
   status( query.readInt( "status" ) ),
   filledBy( 0 )
{
	if( query.fieldExists( "barcode" ) ) {
		barcode = query.readString( "barcode" );
	}
	if( barcode.empty() || barcode == "." ) {
		unsigned n = name.size();
		while( n > 0 && isdigit( name[ n - 1 ] ) ) {
			n --;
		}
		barcode = name.substr( n );
	}
	int space = query.readInt( "box_capacity" );
	short size = getSize();
	if( space >= 0 && space < size ) {
		cryovials.resize( size - space, "?" );
	}
}

//---------------------------------------------------------------------------
// 	read unconfirmed and part-filled boxes into the cache; keep IDs
//---------------------------------------------------------------------------

bool LPDbBoxNames::readCurrent( LQuery pq )
{
	char select[ 120 ];
	std::sprintf( select, "select * from box_name"
				  " where status in ( %d, %d ) or box_capacity > 0"
				  " order by box_cid",
				   LPDbBoxName::EMPTY, LPDbBoxName::IN_USE );
	pq.setSQL( select );
	return readData( pq );
}

//---------------------------------------------------------------------------
//	Find the cryovials the cached boxes contain and who signed them off
//---------------------------------------------------------------------------

void LPDbBoxNames::addCryovials( LQuery pq )
{
	for( Iterator ci = begin(); ci != end(); ci ++ ) {
		ci -> addCryovials( pq );
	}
}

//---------------------------------------------------------------------------

void LPDbBoxName::addCryovials( LQuery & pq )
{
	LPDbCryovials content;
	content.readByBoxID( pq, getID() );
	for( const LPDbCryovial & cr : content ) {
		addCryovial( cr.getPosition(), cr.getBarcode() );
    }
	checkFilledBy( pq );
}

//---------------------------------------------------------------------------

void LPDbBoxName::checkFilledBy( LQuery & pq )
{
/*	FIXME - use audit trail rather than box event history

	pq.setSQL( "select operator_cid, event_date from box_event_history"
			  " where box_id = :bid and event_cid = :eid"
			  " order by event_date" );
	pq.setParam( "bid", getID() );
	pq.setParam( "eid", findEvent( CRYOVIALS_ADDED ) -> getID() );
	filledBy = pq.open() ? pq.readInt( "operator_cid" ) : 0;
*/
}

//---------------------------------------------------------------------------
//	Read the selected box entry from the current project database
//---------------------------------------------------------------------------

const LPDbBoxName * LPDbBoxNames::readRecord( LQuery pQuery, std::string name )
{
	pQuery.setSQL( "select * from box_name"
				  " where upper( :bn ) in (barcode, upper(external_name))" );
	pQuery.setParam( "bn", name );
	return pQuery.open() ? insert( LPDbBoxName( pQuery ) ) : NULL;
}

//---------------------------------------------------------------------------

const LPDbBoxName * LPDbBoxNames::readRecord( LQuery pQuery, int id )
{
	pQuery.setSQL( "select * from box_name where box_cid = :bid" );
	pQuery.setParam( "bid", id );
	return pQuery.open() ? insert( LPDbBoxName( pQuery ) ) : NULL;
}

//---------------------------------------------------------------------------
//	Load boxes in the current project that may be awaiting transfer
//---------------------------------------------------------------------------

bool LPDbBoxNames::readFilled( LQuery pq )
{
	pq.setSQL( "select * from box_name b"
			   " where b.status in ( :conf, :rdy )"
			   " order by box_cid" );
	pq.setParam( "conf", LPDbBoxName::CONFIRMED );
	pq.setParam( "rdy", LPDbBoxName::ANALYSED );
	return readData( pq );
/*
	LPDbDescriptors & descriptors = LPDbDescriptors::records();
	if( descriptors.empty() )
		descriptors.read( ddq, false );
	const LPDbDescriptor * atd = descriptors.findByName( "aliquot_type" );
	if( atd == NULL )
	{
	}
	else
	{	ddq.setSQL( "select b.* from box_name b, c_box_content c"
				" where c.box_type_cid = b.box_type_cid and b.status = :conf and box_capacity = 0"
				" and (expected_use in (:sts,:lts) or (expected_use=:stl and box_cid not in"
				" (select box_cid from cryovial_store where cryovial_id not in"
				" (select cryovial_id from cryovial c, c_object_name n, specimen s, result r"
				" where s.sample_id = r.sample_id and s.sample_id <> c.sample_id"
				" and c.aliquot_type_cid = n.object_cid and cryovial_barcode = s.barcode"
				" and n.external_name = s." + atd -> getSpecimenField() + "))))"
				" order by box_cid" );
		ddq.setParam( "stl", LPDbBoxType::STORE_LATER );
	}
	ddq.setParam( "sts", LPDbBoxType::SHORT_TERM );
	ddq.setParam( "lts", LPDbBoxType::LONG_TERM ); */
}

//---------------------------------------------------------------------------
//  Set up an empty box of the given type with a new name and ID
//---------------------------------------------------------------------------

bool LPDbBoxName::create( const LPDbBoxType & type, LQuery pQuery, LQuery cQuery )
{
	saved = false;
	do { claimNextID( pQuery );
	} while( needsNewID( cQuery ) );
	unsigned code = abs( getID() );

	const LCDbProject & proj = LCDbProjects::records().get( LCDbProjects::getCurrentID() );
	char buff[ 32 ];
	std::sprintf( buff, "%0.5u", code );
	barcode = buff;
	AnsiString projName = proj.getName().c_str();
	AnsiString typeName = type.getName().c_str();
	if( typeName.UpperCase().Pos( projName.UpperCase() ) == 0 ) {
		std::sprintf( buff, "%s %s %u", projName.c_str(), typeName.c_str(), code );
	} else {
		std::sprintf( buff, "%s %u", typeName.c_str(), code );
	}
	name = buff;
	boxTypeID = type.getID();
	filledBy = 0;
	cryovials.clear();
	status = EMPTY;
	return saveRecord( pQuery, cQuery );
}

//---------------------------------------------------------------------------
//	Find total space in this box (including hole, cryovials and free space)
//---------------------------------------------------------------------------

const LCDbBoxSize * LPDbBoxName::getLayout() const {
	const LPDbBoxType * bt = LPDbBoxTypes::records().findByID( boxTypeID );
	return bt == NULL ? NULL : LCDbBoxSizes::records().findByID( bt->getSizeID() );
}

//---------------------------------------------------------------------------

short LPDbBoxName::getSize() const {
	const LCDbBoxSize * bl = getLayout();
	return bl == NULL ? -1 : bl->getLast();
}

//---------------------------------------------------------------------------
//	Find available space in this box (excluding cryovials and hole)
//---------------------------------------------------------------------------

short LPDbBoxName::getSpace() const {
	const LCDbBoxSize * bl = getLayout();
	if( bl == NULL ) {
		return -1;
	}
	short filled = cryovials.size();
	short free = bl->getLast() - filled;
	return filled < bl->getHole() ? free - 1 : free;
}

//---------------------------------------------------------------------------

bool LPDbBoxName::hasSpace() const { return getSpace() > 0; }

//---------------------------------------------------------------------------
//	Add a cryovial to this box once it has been recorded in the database
//---------------------------------------------------------------------------

void LPDbBoxName::addCryovial( short position, const std::string & barcode )
{
	const LCDbBoxSize * bl = getLayout();
	if( bl == NULL || position < 1 || position == bl->getHole() || position > bl->getLast() ) {
		throw Exception( String( name.c_str() ) + "[" + position + "] does not exist" );
	}
	if( position < cryovials.size() ) {
		std::string current = cryovials[ position - 1 ];
		if( !current.empty() && current != "?" && current != barcode ) {
			throw Exception( String( name.c_str() ) + "[" + position + "] is occupied" );
		}
	}
	while( position > cryovials.size() ) {
		cryovials.push_back( "" );
	}
	cryovials[ position - 1 ] = barcode;
	status = IN_USE;
	filledBy = LCDbOperators::getCurrentID();
}

//---------------------------------------------------------------------------
//	Allocate space for another cryovial (don't update the database yet)
//---------------------------------------------------------------------------

short LPDbBoxName::addCryovial( const std::string & barcode )
{
	const LCDbBoxSize * layout = getLayout();
	if( layout == NULL ) {
		throw Exception( "Formation layout not defined" );
	}
	if( cryovials.size() >= layout->getLast() ) {
		throw Exception( "Formation " + String( name.c_str() ) + " is already full" );
	}
	if( layout->getHole() > 0 && cryovials.size() + 1 == layout->getHole() ) {
		cryovials.push_back( "" );
	}
	cryovials.push_back( barcode );
	return cryovials.size();
}

//---------------------------------------------------------------------------
//	Confirm the allocation of cryovials in box_name and related tables
//---------------------------------------------------------------------------

void LPDbBoxName::confirmAllocation( LQuery pQuery, LQuery cQuery )
{
	status = CONFIRMED;
	saveRecord( pQuery, cQuery );

	LPDbCryovialStores contents;
	contents.confirmAllocation( pQuery, getID() );
}

//---------------------------------------------------------------------------
//  Add entry for this box to box_name table, ready to copy to c_box_name
//---------------------------------------------------------------------------

bool LPDbBoxName::saveRecord( LQuery & pQuery, LQuery & cQuery )
{
	if( saved ) {
		pQuery.setSQL( "update box_name set box_capacity = :cap, status = :sts,"
					" time_stamp = 'now', note_exists = note_exists + :nex, process_cid = :pid"
					" where box_cid = :bid" );
	} else {
		while( needsNewID( cQuery ) ) {
			claimNextID( pQuery );
		}
		pQuery.setSQL( "insert into box_name (box_cid, box_type_cid, box_capacity,"
					" external_name, status, time_stamp, process_cid, note_exists)"
///// fixme - include barcode after upgrade
					" values ( :bid, :btid, :cap, :exn, :sts, 'now', :pid, :nex)" );
		pQuery.setParam( "exn", name );
		pQuery.setParam( "btid", boxTypeID );
	}

	pQuery.setParam( "bid", getID() );
	pQuery.setParam( "cap", getSpace() );
	pQuery.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	pQuery.setParam( "sts", status );
	pQuery.setParam( "nex", 0 );
	if( pQuery.execSQL() ) {
		saved = true;
		return true;
	} else {
		return false;
	}
}

//---------------------------------------------------------------------------
//	check the box ID is valid and another box doesn't have the same number
//---------------------------------------------------------------------------

bool LPDbBoxName::needsNewID( LQuery & cQuery ) const
{
	if( getID() != 0 ) {
		cQuery.setSQL( "select count(*) from c_box_name where box_cid in (:pid, :nid)" );
		cQuery.setParam( "pid", getID() );
		cQuery.setParam( "nid", -getID() );
		if( cQuery.open() && cQuery.readInt( 0 ) == 0 ) {
			return false;
		}
	}
	return true;
}

//---------------------------------------------------------------------------
//	compare position of cryovials in this box with others in the same group
//	(don't use c_box_content - distributed database may be far too slow)
//---------------------------------------------------------------------------

bool LPDbBoxName::matchesGroup( LQuery pQuery )
{
	pQuery.setSQL( "select count(*) as mismatched"
				" from box_name n1, box_content t1, cryovial c1, cryovial_store s1,"
				" box_name n2, box_content t2, cryovial c2, cryovial_store s2"
				" where n1.box_cid = :bid and s1.box_cid = :bid"
				" and t1.box_type_cid = n1.box_type_cid and c1.cryovial_id = s1.cryovial_id"
				" and t2.box_set_link = t1.box_set_link and n2.box_type_cid = t2.box_type_cid"
				" and s2.box_cid = n2.box_cid and c2.cryovial_id = s2.cryovial_id"
				" and c1.cryovial_barcode = c2.cryovial_barcode"
				" and s1.cryovial_position <> s2.cryovial_position" );
	pQuery.setParam( "bid", getID() );
	return pQuery.open() && pQuery.readInt( 0 ) == 0;
}

//---------------------------------------------------------------------------
//	Add the most recent sign-off event to the history of this box
//---------------------------------------------------------------------------

const LCDbObject * LPDbBoxName::findEvent( const char * eventName )
{
	return LCDbObjects::records().find( eventName, LCDbObject::SIS_EVENT );
}

//---------------------------------------------------------------------------
//	Add an event from Box Reception to the history of this box
//---------------------------------------------------------------------------
/*
bool LPDbBoxName::addEventRecord( LQuery query, const LCDbObject * event, const std::string & text )
{
	query.setSQL( "insert into box_event_history"
				" (box_id, event_cid, operator_cid, event_date, process_cid, comment)"
				" values ( :bid, :eid, :oid, 'now', :pid, :txt )" );
	query.setParam( "bid", getID() );
	query.setParam( "eid", event == NULL ? 0 : event -> getID() );
	query.setParam( "oid", LCDbOperators::getCurrentID() );
	query.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	query.setParam( "txt", text );
	return query.execSQL();
}
*/
//---------------------------------------------------------------------------
//	Find a box with the given name (usually based on type + ID)
//---------------------------------------------------------------------------

class /* LPDbBoxNames:: */ NameMatcher : public std::unary_function< LPDbBoxName, bool >
{
	const std::string name;

public:

	NameMatcher( const std::string & s ) : name( s ) {}

	operator std::string() const { return name; }
	
	bool operator() ( const LPDbBoxName & other ) const
	{
		return name.compare( other.getName() ) == 0;
	}
};

//---------------------------------------------------------------------------

const LPDbBoxName * LPDbBoxNames::find( const std::string & name ) const
{
	return findMatch( NameMatcher( name ) );
}

//---------------------------------------------------------------------------
//	Find a box of the given type with space for further cryovials
//---------------------------------------------------------------------------

class /* LPDbBoxNames:: */ SpaceMatcher : public std::unary_function< LPDbBoxName, bool >
{
	const int btid;
	const short needed;

public:

	SpaceMatcher( int boxType ) : btid( boxType ),
	 	needed( LPDbBoxTypes::records().get( boxType ).getAliquots().size() )
	{}

	operator std::string() const {
		std::stringstream out;
		out << "ID: " << btid;
		return out.str();
	}

	bool operator() ( const LPDbBoxName & other ) const
	{
		return other.getTypeID() == btid && other.getSpace() >= needed;
	}
};

//---------------------------------------------------------------------------

const LPDbBoxName * LPDbBoxNames::findSpace( int boxType ) const
{
	return findMatch( SpaceMatcher( boxType ) );
}

//---------------------------------------------------------------------------

