/*---------------------------------------------------------------------------
 *
 *  Routines to look after the box worklist
 *
 *	 	25 Apr 08, NG:  ts_sequence set by Marty if time_stamp not unique
 *		15 May 08, NG: 	use ddb to update worklist from cryovial data
 *      24 June 08:		DON'T use ddb for worklist - it may use wrong ldbc
 *      5 January 09:	Added c_box_worklist category ID and group ID
 *      30 Jun 09, NG:	added deleteOld() method to be used by NightShift
 *		19 Aug 09, NG:	incorporated record_no field from v2.5 database
 *
 *---------------------------------------------------------------------------*/

#include <vcl.h>

#include "LDbSource.h"
#include "LPDbBoxWork.h"
#include "LDbBoxType.h"
#include "LDbBoxSize.h"
#include "LCDbProject.h"
#include "LPDbProfile.h"
#include "LQuery.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

LPDbBoxEntry::LPDbBoxEntry( const LPDbSource & sample, int btid )
  : LDbIdBase( 0 ), status( NEW_ENTRY ), boxTypeID( btid ),
	sampleID( sample.getID() ), source( sample.getBarcode() )
{}

//---------------------------------------------------------------------------

LPDbBoxEntry::LPDbBoxEntry( const LQuery & query )
 : LDbIdBase( query.readInt( "record_no" ) ),
   source( query.readString( "barcode" ) ),
   profile( query.readString( "profile_name" ) ),
   sampleID( query.readInt( "sample_id" ) ),
   groupID( query.readInt( "group_id" ) ),
   categoryID( query.readInt( "category_id" ) ),
//   projectID( query.readInt( "project_cid" ) ),
   status( query.readChar( "status" ) ),
   boxTypeID( query.readInt( "box_type_cid" ) ),
   timeStamp( query.readDateTime( "time_stamp" ) )
{
	String cryovial = query.readString( "cryovial_barcode" );
	if( status != NEW_ENTRY && cryovial != "." )
		destination = cryovial;
}

//---------------------------------------------------------------------------
//	Read new and allocated worklist entries for current project, ordered by
// 	swipe time and/or edit time (ts_sequence from RFP ensures correct order)
//---------------------------------------------------------------------------

bool LPDbBoxList::readCurrent( LQuery query )
{
	query.setSQL( "select * from c_box_worklist where project_cid = :pid"
				 " order by group_id, time_stamp, ts_sequence" );
	query.setParam( 0, LCDbProjects::getCurrentID() );
	return readData( query );
}

//---------------------------------------------------------------------------
//	Find recent worklist entries for the given samples; return group ID
//---------------------------------------------------------------------------

int LPDbBoxList::findGroup( LQuery cQuery, const std::set< int > & sampleIDs ) const
{
	if( sampleIDs.empty() )
		return 0;

	std::set< int >::const_iterator si = sampleIDs.begin();
	String list = *si;
	while( ++ si != sampleIDs.end() )
		list = list + ", " + *si;

	cQuery.setSQL( "select group_id from c_box_worklist"
				  " where project_cid = :pid and sample_id in (" + list + ")"
				  " and time_stamp >= date_trunc('day', 'now')" );
	cQuery.setParam( 0, LCDbProjects::getCurrentID() );
	return cQuery.open() >= 1 ? cQuery.readInt( 0 ) : 0;
}

//---------------------------------------------------------------------------
//	Update box worklist record in the database (and the cache)
//---------------------------------------------------------------------------

void LPDbBoxEntry::saveRecord( LQuery query, const String & reason )
{
	if( boxTypeID == 0 || source.IsEmpty() )
		throw Exception( "Invalid box worklist entry" );

	if( saved )
	{
		query.setSQL( "update c_box_worklist"
					" set status = :sts, cryovial_barcode = :dcb, time_stamp = 'now', profile_name = :pnm"
					// record_no and ts_sequence set by Marty's RFPs
					" where project_cid = :pcid and box_type_cid = :btid and barcode = :bar" );
	}
	else
	{   query.setSQL( "insert into c_box_worklist (barcode, sample_id, status, group_id, category_id,"
					 " project_cid, profile_name, box_type_cid, cryovial_barcode, time_stamp)"
					 " values ( :bar, :sid, :sts, :grp, :cat, :pcid, :pnm, :btid, :dcb, 'now' )" );
		query.setParam( "sid", sampleID );
		query.setParam( "grp", groupID );
		query.setParam( "cat", categoryID );
	}

	if( status == NEW_ENTRY || destination.IsEmpty() )
		query.setParam( "dcb", "." );
	else
		query.setParam( "dcb", destination );

	query.setParam( "pcid", LCDbProjects::getCurrentID() );
	query.setParam( "pnm", reason );
	query.setParam( "bar", source );
	query.setParam( "sts", status );
	query.setParam( "btid", boxTypeID );
	if( query.execSQL() > 0 )
	{
		setTimeStamp( Now() );
		profile = reason;
		LPDbBoxList::records().insert( *this );
	}
}

//---------------------------------------------------------------------------

TDateTime LPDbBoxEntry::getTimeStamp()
{
	static const TDateTime ALREADY_SET( 2000, 1, 1 );
	if( timeStamp < ALREADY_SET )
		timeStamp = ALREADY_SET;
	return timeStamp;
}

//---------------------------------------------------------------------------
//	Find worklist entry by barcode - assumes barcode + box type is unique
//---------------------------------------------------------------------------

class LPDbBoxList::BCMatch : public std::unary_function< LPDbBoxEntry, bool >
{
	const int boxType;
	const String barcode;

public:

	BCMatch( const String & source, int btid ) : barcode( source ), boxType( btid )
	{}

	bool operator() ( const LPDbBoxEntry & other ) const
	{
		return (other.getBoxTypeID() == boxType)
			&& (other.getSource() == barcode || other.getDestination() == barcode);
	}

	operator String() const { return barcode + ", box type: " + boxType; }
};

//---------------------------------------------------------------------------

const LPDbBoxEntry * LPDbBoxList::find( const String & barcode, int boxType ) const
{
	return findMatch( BCMatch( barcode, boxType ) );
}

//---------------------------------------------------------------------------
//	Find worklist entry by sample ID and box type - assumes this is unique
//---------------------------------------------------------------------------

class LPDbBoxList::IDMatch : public std::unary_function< LPDbBoxEntry, bool >
{
	const int sampleID, boxType;

public:

	IDMatch( int source, int btid ) : sampleID( source ), boxType( btid )
	{}

	bool operator() ( const LPDbBoxEntry & other ) const
	{
		return (sampleID == other.getSampleID() && boxType == other.getBoxTypeID());
	}

	operator String() const
	{
		return "Sample " + String( sampleID ) + ", box type: " + boxType;
	}
};

//---------------------------------------------------------------------------

const LPDbBoxEntry * LPDbBoxList::find( int sampleID, int boxType ) const
{
	return findMatch( IDMatch( sampleID, boxType ) );
}

//---------------------------------------------------------------------------
//	delete box worklist records 20 hours after all aliquots confirmed
//---------------------------------------------------------------------------

unsigned LPDbBoxList::deleteOld( LQuery central, TDateTime flagged, TDateTime oldest )
{
	central.setSQL( "delete from c_box_worklist where time_stamp < :old"
					" or (time_stamp < :ftd and group_id not in"
				  " (select group_id from c_box_worklist where status <> :tra))" );
	central.setParam( "old", oldest );
	central.setParam( "ftd", flagged );
	central.setParam( "tra", LPDbBoxEntry::CONFIRMED );
	return central.execSQL();
}

//---------------------------------------------------------------------------

