/*---------------------------------------------------------------------------
 *
 * 	Methods for the cryovial_store table - use with LPDbCryovial
 *
 *	27 Sepember 2013	Read sample volume (if available)
 ---------------------------------------------------------------------------*/

#include <vcl.h>
#include <stdlib.h>
#include <sstream>

#include "LPDbCryovialStore.h"
#include "LQuery.h"
#include "LCDbProject.h"
#include "LCDbAuditTrail.h"
#include "LDbBoxType.h"
#include "LCDbJob.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

LPDbCryovialStore::LPDbCryovialStore( const LQuery & query )
 : LPDbID( query.readInt( "Cryovial_id" ) ),
   LDbNoteCount( query.readInt( "Note_Exists" ) ),
   retrievalID( query.readInt( "retrieval_cid" ) ),
   boxID( query.readInt( "box_cid" ) ),
   status( query.readInt( "status" ) ),
   cryovialID( query.readInt( "cryovial_id" ) ),
   position( query.readInt( "cryovial_position" ) )
{
	volume = query.fieldExists( "sample_volume" ) ? query.readDouble( "sample_volume" ) : -1;
}

//---------------------------------------------------------------------------
//	Mark record as belonging to given job; return false if claim fails
//---------------------------------------------------------------------------

bool LPDbCryovialStore::setJobRef( LQuery project, int jobRef )
{
	if( retrievalID == jobRef )
		return true;

	if( retrievalID != 0 && jobRef != 0 )
		return false;

	project.setSQL( "update cryovial_store set retrieval_cid = :newJob"
				   " where record_id = :myid and job_ref = :oldJob" );
	project.setParam( "newJob", jobRef );
	project.setParam( "myid", getID() );
	project.setParam( "oldJob", retrievalID );
	if( project.execSQL() ) {
		retrievalID = jobRef;
		return true;
	} else {
		return false;
	}
}

//---------------------------------------------------------------------------
//	Update the cryovial store to confirm a boxful have been allocated
//---------------------------------------------------------------------------

int LPDbCryovialStores::confirmAllocation( LQuery query, int boxID )
{
	query.setSQL( "update cryovial_store set status = :sts, time_stamp = 'now'"
				 " where box_cid = :bid" );
	query.setParam( "bid", boxID );
	query.setParam( "sts", LPDbCryovialStore::CONFIRMED );
	return query.execSQL();
}

//---------------------------------------------------------------------------
//	Check how many cryovials are waiting in unfinished removal jobs
//---------------------------------------------------------------------------

bool LPDbCryovialStores::readMarked( LQuery ddq )
{
	ddq.setSQL( "select s.* from c_retrieval_job j, cryovial_store s"
			   " where j.retrieval_cid = s.retrieval_cid"
			   " and job_type = :jt and j.status <> :js and s.status = :css"
			   " order by record_id" );
	ddq.setParam( "jt", LCDbCryoJob::SAMPLE_DISCARD );
	ddq.setParam( "js", LCDbCryoJob::DELETED );
	ddq.setParam( "css", LPDbCryovialStore::CONFIRMED );
	return readData( ddq );
}

//---------------------------------------------------------------------------
//	Create or update cryovial_store record.  TODO: set volume on insert
//---------------------------------------------------------------------------

bool LPDbCryovialStore::saveRecord( LQuery query )
{
	if( !saved ) {
		claimNextID( query );
		query.setSQL( "insert into cryovial_store (record_id, cryovial_id, box_cid,"
					 " cryovial_position, time_stamp, status, note_exists, process_cid, retrieval_cid)"
					 " values ( :rid, :cid, :bid, :pos, 'now', :sts, 0, :pid, :jcid )" );
		query.setParam( "cid", getID() );
		query.setParam( "bid", boxID );
		query.setParam( "pos", position );
	} else {
		std::stringstream fields;
		fields << "update cryovial_store set status = :sts, retrieval_cid = :jcid, process_cid = :pid";
		switch( status ) {
			case ALLOCATED:
			case CONFIRMED:
				fields << ", time_stamp = 'now'";
			break;
			case ANALYSED:
			case DESTROYED:
				fields << ", removed = 'now'";
		}
		if( volume >= 0 ) {
			fields << ", sample_volume = " << volume;
		}
		fields << " where record_id = :rid";
		query.setSQL( fields.str() );
	}
	query.setParam( "sts", status );
	query.setParam( "jcid", retrievalID );
	query.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	query.setParam( "rid", getID() );
	if( query.execSQL() ) {
        saved = true;
		return true;
	} else {
		return false;
    }
}

//---------------------------------------------------------------------------

