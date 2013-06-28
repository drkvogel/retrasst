//---------------------------------------------------------------------------
//
//	Record analyser ID in the buddy_note table as well as process ID
//
//---------------------------------------------------------------------------

#include <vcl.h>

#include "LCDbProject.h"
#include "LDbNotes.h"
#include "LQuery.h"
#include "LCDbAuditTrail.h"
#include "LCDbOperator.h"
#include "LCDbAnalyser.h"
#include "LDbResult.h"
#include "LDbSource.h"

#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------

LDbNote::LDbNote( const std::string & text )
 : content( text ), user( LCDbOperators::getCurrentID() ),
   created( Now() ), process( LCDbAuditTrail::getCurrent().getProcessID() )
{}

//---------------------------------------------------------------------------

LBDbNote::LBDbNote( const std::string & text, int linkID )
 : LDbNote( text ), objectBID( linkID ), machineCID( LCDbAnalysers::getCurrentID() )
{}

//---------------------------------------------------------------------------

LBDbNote::LBDbNote( const LQuery & cluster )
 : objectBID( cluster.readInt( "buddy_object_id" ) ),
   LDbNote( cluster.readString( "contents" ),
			cluster.readInt( "process_cid" ),
			cluster.readInt( "operator_cid" ),
			cluster.readDateTime( "time_stamp" ) )
{}

//---------------------------------------------------------------------------

LPDbNote::LPDbNote( const LQuery & project )
 : LPDbID( project.readInt( "note_id" ) ),
   objectPID( project.readInt( "object_id" ) ),
   LDbNote( project.readString( "contents" ),
			project.readInt( "process_cid" ),
			project.readInt( "operator_cid" ),
			project.readDateTime( "time_stamp" ) )
{}

//---------------------------------------------------------------------------
//	Read notes for the given object into temporary store
//---------------------------------------------------------------------------

bool LBDbNotes::read( LQuery query, int objID )
{
	cache.clear();
	query.setSQL( "select * from buddy_note"
				 " where buddy_object_id = :oid "
				 " order by time_stamp" );
	query.setParam( "oid", objID );
	query.setParam( "mid", LCDbAnalysers::getCurrentID() );
	for( query.open(); !query.eof(); query.next() )
		cache.push_back( query );
	return !cache.empty();
}

//---------------------------------------------------------------------------
//  Add a note for this sample or result (assumes it has been saved)
//---------------------------------------------------------------------------

bool LBDbNote::saveRecord( LQuery query )
{
	query.setSQL( "INSERT into buddy_note (buddy_object_id,"
				 " operator_cid, contents, time_stamp, process_cid )"
				 " values ( :bid, :uid, :txt, :dta, :pid )" );
	query.setParam( "bid", objectBID );
//	query.setParam( "mid", machineCID );
	query.setParam( "uid", getUserID() );
	query.setParam( "pid", getProcID() );
	query.setParam( "txt", getContent() );
	query.setParam( "dta", XTIME( getTimeStamp() ) );
	return query.execSQL();
}

//---------------------------------------------------------------------------
//	Flag any notes for given object for later deletion
//---------------------------------------------------------------------------

bool LBDbNotes::mark( LQuery query, int objID )
{
	query.setSQL( "update buddy_note set time_stamp = 'now'"
				 " where buddy_object_id = :oid " );
	query.setParam( "oid", objID );
	// query.setParam( "mid", LCDbAnalysers::getCurrentID() );
	return query.execSQL();
}

//---------------------------------------------------------------------------
//	delete copied notes so analyses and results can be deleted
//---------------------------------------------------------------------------

unsigned LBDbNotes::deleteOld( LQuery cluster, TDateTime flagged, TDateTime oldest )
{
	cluster.setSQL( "delete from buddy_note where time_stamp < :old"
					" or time_stamp < :fdt" );
	cluster.setParam( "old", XTIME(oldest) );
	cluster.setParam( "fdt", XTIME(flagged) );
	return cluster.execSQL();
}

//---------------------------------------------------------------------------

bool LPDbNotes::read( LQuery project, int objID )
{
	project.setSQL( "select * from note where object_id = :oid order by note_id" );
	project.setParam( "oid", objID );
	return readData( project );
}

//---------------------------------------------------------------------------
//	Copy notes from cluster database to project sample/note tables
//---------------------------------------------------------------------------

bool LPDbNote::saveRecord( LQuery project )
{
	claimNextID( project );
	project.setSQL( "Insert into note (note_id, object_id,"
				   " operator_cid, process_cid, contents, time_stamp)"
				   " values( :nid, :obj, :uid, :pid, :txt, :dts )" );
	project.setParam( "nid", getID() );
	project.setParam( "obj", objectPID );
	project.setParam( "pid", getProcID() );
	project.setParam( "uid", getUserID() );
	project.setParam( "dts", XTIME(getTimeStamp()) );
	project.setParam( "txt", getContent() );

	if( !project.execSQL() )
		return false;

	saved = true;
//	LPDbNotes::records().insert( *this );
	return true;
}

//---------------------------------------------------------------------------
//	record when the note was created; default to the current time/date
//---------------------------------------------------------------------------

void LDbNote::setTimeStamp( TDateTime when )
{
	created = when;
}

//---------------------------------------------------------------------------

TDateTime LDbNote::getTimeStamp()
{
	static const TDate FIRST_DATE( 2000, 1, 1 );
	if( created < FIRST_DATE )
		created = Now();
	return created;
}

//---------------------------------------------------------------------------

