/*---------------------------------------------------------------------------
 *
 *	20 July 2005: 		Added method to delete calibrant/reagent Link entry
 *				 		LCDbObject::findDescription() replaces LCDbObjects::getDescription()
 *  26 July 2005:		Add tank IDs from c_tank_map to location entries
 *  9 September:    	Check records deleted from database in deleteTest()
 *  10 March 2006:		Modified cache handling in preparation for garbage collection
 *	20 June 08, NG:		Remove laptop definitions - now held in c_machine
 *  2 September 08:		Added record_id and status for c_calibreag_test
 *	19 Aug 09, NG:		Removed links to c_calibreag_test, c_tank_map etc.
 *  3 Feb 2010, NG:		Use 'now' for time stamps, rather than local time
 *-------------------------------------------------------------------------*/

#include <vcl.h>
#include "LCDbObject.h"
#include "LCDbAnalyser.h"
#include "LQuery.h"
#include "LCDbAuditTrail.h"
#include "LCDbOperator.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

std::string LCDbObject::findDescription( Category type )
{
	switch( type )
	{
		case CALIBRANT:
			return "Calibrant";

		case REAGENT:
			return "Reagent";

		case ANALYSER_EVENT:
			return "Analyzer Event";

		case SIS_EVENT:
			return "Sample Inventory Event";

		case CLUSTER:
			return "Analyser Cluster";

		case ALIQUOT_TYPE:
			return "Aliquot Type";

		case STORAGE_TYPE:
			return "Storage Type";

		case STORAGE_SITE:
			return "Storage Site";

		case STORAGE_POPULATION:
			return "Storage Population";

		case TANK_LAYOUT:
			return "Storage Layout";

		case PROGRAM_NAME:
			return "LIMS Application";

		case SAMPLE_CATEGORY:
			return "Sample Category";

		case LAB_NAME:
			return "Laboratory";

		case TUBE_TYPE:
			return "Tube type";

		case STORAGE_VESSEL:
			return "Storage vessel";

		case RESULT_ATTRIBUTE:
			return "Result attribute";

		case QC_RULE:
			return "Westgard rule";

		case QC_MATERIAL:
			return "QC material";

		case CANNED_TEXT:
			return "Canned text";

		case STORAGE_EXERCISE:
			return "Storage exercise";

		case ANALYSER_MODEL:
			return "Analyser model";

		default:
			return "(unknown)";
	}
}

//---------------------------------------------------------------------------

LCDbObject::Category LCDbObject::findObjectType( const std::string & description )
{
	for( short n = 0; n < NUM_TYPES; n ++ ) {
		Category type = Category( n );
		if( description.compare( findDescription( type ) ) == 0 ) {
			return type;
		}
	}
	return UNKNOWN;
}

//---------------------------------------------------------------------------
//	Read the names of all the objects in the system
//---------------------------------------------------------------------------

LCDbObject::LCDbObject( const LQuery & query  )
 : LCDbID( query.readInt( "object_cid" ) ),
   process( query.readInt( "process_cid" ) ),
   LDbNames( query.readString( "external_name" ),
			 query.readString( "external_full" ) ),
   objType( query.readInt( "object_type" ) ),
   status( query.readInt( "status" ) ),
   when( query.readDateTime( "time_stamp" ) )
{}

//---------------------------------------------------------------------------

bool LCDbObjects::read( LQuery cQuery, bool readAll )
{
	if( readAll )
		cQuery.setSQL( "select * from c_object_name order by object_cid" );
	else
	{	cQuery.setSQL( "select * from c_object_name where status <> :sts"
					  " order by object_cid" );
		cQuery.setParam( "sts", LDbValid::DELETED );
	}
	return readData( cQuery );
}

//---------------------------------------------------------------------------
//	Find entry for object with the given name if it's in the cache
//---------------------------------------------------------------------------

const LCDbObject * LCDbObjects::findByName( const std::string & name ) const
{
	return findMatch( LDbNames::LCMatcher( name ) );
}

//---------------------------------------------------------------------------

const LCDbObject * LCDbObjects::find( const std::string & name, LCDbObject::Category type ) const
{
	const LCDbObject * found = findByName( name );
	if( found == NULL || found ->getObjectType() != type ) {
		return NULL;
	} else {
		return found;
	}
}

//---------------------------------------------------------------------------
//	Add an entry to the event log, refering to the current object
//---------------------------------------------------------------------------

bool LCDbObject::makeLogEntry( LQuery central, int other, int link ) const
{
	static LCDbID records;
	records.claimNextID( central );
	central.setSQL( "insert into c_event_log (event_cid, event_code, machine_cid,"
				   " test_cid, related_id, operator_cid, event_date, process_cid)"
				   // log_date will be filled in by Ingres
				   " values( :eid, :cid, :mid, :tid, :rid, :oid, 'now', :pid )" );
	central.setParam( "eid", records.getID() );
	central.setParam( "cid", getID() );
	central.setParam( "mid", LCDbAnalysers::getCurrentID() );
	central.setParam( "tid", link );
	central.setParam( "rid", other );
	central.setParam( "oid", LCDbOperators::getCurrentID() );
	central.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	return central.execSQL();
}

//---------------------------------------------------------------------------
//	Create or update a record in c_object_name for this object
//---------------------------------------------------------------------------

bool LCDbObject::saveRecord( LQuery query )
{
	if( saved ) {
		query.setSQL( "update c_object_name"
					" set external_name = :nme, external_full = :full,"
					" status = :stat, process_cid = :pcid, time_stamp = 'now'"
					" where object_cid = :cid" );
	} else {
		claimNextID( query );
		query.setSQL( "INSERT into c_object_name ( object_cid, external_name,"
					" external_full, status, object_type, process_cid, time_stamp )"
					" values ( :cid, :nme, :full, :stat, :typ, :pcid, 'now' )" );
		query.setParam( "typ", objType );
	}
	query.setParam( "cid", getID() );
	query.setParam( "nme", getName() );
	query.setParam( "full", getDescription() );
	query.setParam( "stat", status );
	query.setParam( "pcid", LCDbAuditTrail::getCurrent().getProcessID() );
	if( query.execSQL() ) {
		saved = true;
		when = Now();
		LCDbObjects::records().insert( *this );
		return true;
	} else {
		return false;
	}
}

//---------------------------------------------------------------------------

