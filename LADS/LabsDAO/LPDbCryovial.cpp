//---------------------------------------------------------------------------
//
//		LIMS class to keep track of cryovial and cryovial_store tables
//
//		18 November 2005:	Modified saveRecord() to update store status
//      19 January 2006:	Use cryovial_store.record_id in saveRecord()
//      13 January 09, NG:	added methods to read specific cryovials
//      1 February 2010:	started to split out cryovial_store methods
//
//---------------------------------------------------------------------------

#include <vcl.h>
#include <sstream>
#include <stdlib.h>
#include "LPDbCryovial.h"
#include "LQuery.h"
#include "LCDbProject.h"
#include "LCDbAuditTrail.h"
#include "LDbBoxType.h"
#include "LPDbCryovialStore.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//	Read a single cryovial using box+position or barcode+aliquot type
//---------------------------------------------------------------------------

LPDbCryovial::LPDbCryovial( const LQuery & query )
 : LPDbID( query.readInt( "Cryovial_id" ) ),
   barcode( query.readString( "Cryovial_Barcode" ) ),
   storeID( query.readInt( "record_id" ) ),
   sampleID( query.readInt( "sample_id" ) ),
   typeID( query.readInt( "aliquot_type_cid" ) ),
   boxID( query.readInt( "box_cid" ) ),
   status( query.readInt( "status" ) ),
   position( query.readInt( "cryovial_position" ) ),
   LDbNoteCount( query.readInt( "Note_Exists" ) ),
   retrievalID( query.readInt( "retrieval_cid" ) )
{}

//---------------------------------------------------------------------------
//	Record the current cryovial in the database, ready to be allocated
//---------------------------------------------------------------------------

bool LPDbCryovial::saveRecord( LQuery query )
{
	if( !saved )
	{
		claimNextID( query );
		query.setSQL( "insert into cryovial (cryovial_id, sample_id, cryovial_barcode,"
					 " aliquot_type_cid, process_cid, time_stamp, note_exists)"
					 " values ( :cid, :sid, :bar, :atid, :pid, 'now', :nc )" );
		query.setParam( "sid", sampleID );
		query.setParam( "bar", barcode );
		query.setParam( "atid", typeID );
		query.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
		query.setParam( "nc", notes );
		query.setParam( "cid", getID() );
		if( !query.execSQL() )
			return false;
	}
	else if( hasNotes() )
	{
		query.setSQL( "update cryovial set note_exists = note_exists + 1"
					 " where cryovial_id = :cid" );
		query.setParam( "cid", getID() );
		if( !query.execSQL() )
			return false;
	}

	LPDbCryovialStore store( storeID, getID(), boxID, position );
	store.setStatus( LPDbCryovialStore::Status(status) );
	return store.saveRecord( query );
}

//---------------------------------------------------------------------------

std::string LPDbCryovials::selectFields()
{
	return "select c.Cryovial_id, c.Cryovial_Barcode, c.sample_id, c.aliquot_type_cid,"
		  " s.record_id, s.box_cid, s.status, s.cryovial_position, s.Note_Exists, s.retrieval_cid"
		  " from cryovial c, cryovial_store s"
		  " where c.cryovial_id = s.cryovial_id and c.status in (0,1) and s.status in (0,1)";
}

//---------------------------------------------------------------------------
//	check the database for the given cryovial (should be unique)
//---------------------------------------------------------------------------

const LPDbCryovial * LPDbCryovials::readRecord( LQuery pQuery, const std::string & barcode, int typeID )
{
	pQuery.setSQL( selectFields() + " and cryovial_barcode = :bar and aliquot_type_cid = :tid" );
	pQuery.setParam( "bar", barcode );
	pQuery.setParam( "tid", typeID );
	return pQuery.open() ? insert( LPDbCryovial( pQuery ) ) : NULL;
}

//---------------------------------------------------------------------------

const LPDbCryovial * LPDbCryovials::readRecord( LQuery pQuery, int box, short pos )
{
	pQuery.setSQL( selectFields() + " and s.box_cid = :bid and s.cryovial_position = :pos" );
	pQuery.setParam( "bid", box );
	pQuery.setParam( "pos", pos );
	return pQuery.open() ? insert( LPDbCryovial( pQuery ) ) : NULL;
}

//---------------------------------------------------------------------------
//	Read cryovials entries taken from given specimen, ignoring status
//---------------------------------------------------------------------------

bool LPDbCryovials::readBySourceID( LQuery pQuery, int specimenID )
{
	pQuery.setSQL( selectFields() + " and c.sample_id = :sid"
	// include cryovial_store time_stamp so that latest entries are kept
				 " order by c.cryovial_id, s.time_stamp" );
	pQuery.setParam( "sid", specimenID );
	return readData( pQuery );
}

//---------------------------------------------------------------------------
//	Read the cryovials that the given box (still) contains
//---------------------------------------------------------------------------

bool LPDbCryovials::readByBoxID( LQuery pQuery, int boxCID )
{
	pQuery.setSQL( selectFields() + " and s.box_cid = :bid"
				 " order by c.cryovial_id, s.time_stamp" );
	pQuery.setParam( "bid", boxCID );
	return readData( pQuery );
}

//---------------------------------------------------------------------------
//	Read all the cryovials with the given status (may be a long list)
//---------------------------------------------------------------------------

bool LPDbCryovials::readByStatus( LQuery pQuery, LPDbCryovial::Status filter )
{
	pQuery.setSQL( selectFields() + " and s.status = :sst"
				 " order by c.cryovial_id, s.time_stamp" );
	pQuery.setParam( "sst", filter );
	return readData( pQuery );
}

//---------------------------------------------------------------------------
//	Find a cryovial from the cache given the barcode and aliquot type
//---------------------------------------------------------------------------

class LPDbCryovials::Matcher : public std::unary_function< LPDbCryovial, bool >
{
	std::string barcode;
	int aliquot;

public:

	Matcher( const std::string & cryovial, int typeCID )
	 : barcode( cryovial ), aliquot( typeCID )
	{}

	operator std::string() const {
		std::stringstream out;
		out << barcode << " type " << aliquot;
		return out.str();
	}

	bool operator()( const LPDbCryovial & other ) const
	{
		return other.getAliquotType() == aliquot && other.getBarcode() == barcode;
	}
};

//---------------------------------------------------------------------------

const LPDbCryovial * LPDbCryovials::find( const std::string & barcode, int typeCID ) const
{
	return findMatch( Matcher( barcode, typeCID ) );
}

//---------------------------------------------------------------------------

std::set<int> LPDbCryovials::getAliquotTypes( LQuery pQuery ) {
	std::set<int> types;
	pQuery.setSQL( "select distinct aliquot_type_cid from cryovial" );
	for( pQuery.open(); !pQuery.eof(); pQuery.next() ) {
		 types.insert( pQuery.readInt( 0 ) );
	}
	return types;
}

//---------------------------------------------------------------------------

