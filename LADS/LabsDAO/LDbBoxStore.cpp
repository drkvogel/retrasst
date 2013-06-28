//---------------------------------------------------------------------------
//
//	Version history:
//
//		14 July 2005:	Keep rack ID when database updated for later re-use
//      20 July, NG:    Added LCDbRackSize/s to encapsulate c_rack_capacity
//      18 August, NG:	Added LCDbTankStore to encapsulate c_storage_details
//      23 August, NG:	Don't bodge tank name; use list for given location
//      16 Feb 2006:	Added valid flag to LCDbRackSize (always true)
//      22 April 2009:	Expect hive CID for tank, rather than tank position
//                      Calculate correct position for the rack within it
//		16 Nov 09, NG:	Use (ddbq's) box_store, not c_slot_allocation
//		28 July, 2010:	Added job ID as link to owner (LCDbRetrievalJob)
//		15 Nov 2012:    Don't use storeman_ddb after all - it's unreliable
//						Use project's box_store (c_slot_allocation for compatibility)
//      22 April 2013:  Tidied up for db 2.7.2: no c_slot_allocation
//---------------------------------------------------------------------------

#include <vcl.h>

#include "LQuery.h"
#include "LDbBoxStore.h"
#include "LCDbObject.h"
#include "LCDbProject.h"
#include "LCDbTankMap.h"
#include "LCDbStoreDetail.h"
#include "LCDbTankLayout.h"
#include "LCDbAuditTrail.h"
#include "LPDbBoxes.h"
#include "LCDbRack.h"

#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------

LCDbBoxStore::LCDbBoxStore( const LQuery & project ) {
   	copyFields( project );
}

//---------------------------------------------------------------------------

void LCDbBoxStore::copyFields( const LQuery & ddbq )
{
	boxID = ddbq.readInt( "box_cid" );
	setID( ddbq.fieldExists( "record_id" ) ? ddbq.readInt( "record_id" ) : boxID );
	rackID = ddbq.readInt( "rack_cid" );
	slot = ddbq.readInt( "slot_position" );
	slotID = ddbq.fieldExists( "slot_cid" ) ? ddbq.readInt( "slot_cid" ) : 0;
	status = ddbq.readInt( "status" );
	updated = ddbq.readDateTime( "time_stamp" );
	processID = ddbq.readInt( "process_cid" );
	jobID = ddbq.fieldExists( "retrieval_cid" ) ? ddbq.readInt( "retrieval_cid" ) : 0;
	if (jobID != 0 && ddbq.fieldExists( "removed" )) {
		removed = ddbq.readDateTime( "removed" );
	} else {
        removed = 0;
    }
}

//---------------------------------------------------------------------------
//	Check the database for the given tank/rack/slot combination
//---------------------------------------------------------------------------

bool LCDbBoxStore::findStoreRecord( LQuery & ddbq )
{
	if( rackID == 0 ) {
		// find the c_rack_number record in the central database
		LCDbRack rackData( site, tankID, rack );
		if( rackData.findRack( ddbq ) ) {
			rackID = rackData.getID();
		}
	}
	if( rackID != 0 ) {
		// recent programs create box_store records for each stored box
		ddbq.setSQL( "select * from box_store where status = :conf"
					" and rack_cid = :rid and slot_position = :pos" );
		ddbq.setParam( "rid", rackID );
		ddbq.setParam( "pos", slot );
		ddbq.setParam( "conf", LCDbBoxStore::SLOT_CONFIRMED );
		if( ddbq.open() ) {
			copyFields( ddbq );
			return true;
		}
		// older programs store the rack and slot in the box_name table
/*		ddbq.setSQL( "select * from box_name where status = :conf"
					" and rack_cid = :rid and slot_position = :pos" );
		ddbq.setParam( "rid", rackID );
		ddbq.setParam( "pos", slot );
		ddbq.setParam( "conf", LPDbBoxName::IN_TANK );
		if( ddbq.open() ) {
			copyFields( ddbq );
			return true;
		}
*/	}
	return false;
}

//---------------------------------------------------------------------------
//	Check the database for the position of the selected box
//---------------------------------------------------------------------------

bool LCDbBoxStore::findBoxRecord( LQuery & ddbq )
{
	ddbq.setSQL( "select * from box_store where status = :conf and box_cid = :box" );
	ddbq.setParam( "box", boxID );
	ddbq.setParam( "conf", LCDbBoxStore::SLOT_CONFIRMED );
	if( ddbq.open() ) {
		copyFields( ddbq );
		return true;
/*	}
	ddbq.setSQL( "select * from box_name where status = :conf and box_cid = :box" );
	ddbq.setParam( "box", boxID );
	ddbq.setParam( "conf", LPDbBoxName::IN_TANK );
	if( ddbq.open() ) {
		copyFields( ddbq );
		return true;
*/	}
	return false;
}

//---------------------------------------------------------------------------
//	Create or update box_store record, adding to the movement history
//	Also update box_name and/or c_slot_allocation for compatibility
//---------------------------------------------------------------------------

bool LCDbBoxStore::saveRecord( LQuery ddbq )
{
	bool ok = saved ? updateStoreRecord( ddbq ) : insertStoreRecord( ddbq );
/*	if( ok ) {
		switch (status) {
		case LCDbBoxStore::MOVE_EXPECTED:
		case LCDbBoxStore::SLOT_ALLOCATED:
			ok = findSlotRecord( ddbq ) ? updateSlotRecord( ddbq ) : insertSlotRecord( ddbq );
			break;
		case LCDbBoxStore::SLOT_CONFIRMED:
			ok = updateBoxRecord( ddbq );
		}
	}
*/	return ok;
}

//---------------------------------------------------------------------------

bool LCDbBoxStore::insertStoreRecord( LQuery & ddbq )
{
	claimNextID( ddbq );
	ddbq.setSQL( "insert into box_store (record_id, box_cid, rack_cid,"
				" slot_position, status, time_stamp, retrieval_cid, process_cid)"
				" values ( :myid, :box, :rid, :pos, :sts, 'now', :jid, :pid )" );
	ddbq.setParam( "box", boxID );
	ddbq.setParam( "jid", jobID );
	ddbq.setParam( "myid", getID() );
	ddbq.setParam( "rid", rackID );
	ddbq.setParam( "pos", slot );
	ddbq.setParam( "sts", status );
	ddbq.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	saved = ddbq.execSQL();
	return saved;
}

//---------------------------------------------------------------------------

bool LCDbBoxStore::updateStoreRecord( LQuery & ddbq )
{
	std::string fields = "rack_cid = :rid, slot_position = :pos, status = :sts,";
	switch( status ) {
		case EXPECTED:
		case SLOT_ALLOCATED:
		case SLOT_CONFIRMED:
			fields += " time_stamp = 'now', ";
		break;
		case REMOVED:
			fields += " removed = 'now', ";
		break;
		default:
			;	// leave old time stamp when DESTROYED or DELETED
	}
	ddbq.setSQL( "update box_store set " + fields + " process_cid = :pid where record_id = :myid" );
	ddbq.setParam( "myid", getID() );
	ddbq.setParam( "rid", rackID );
	ddbq.setParam( "pos", slot );
	ddbq.setParam( "sts", status );
	ddbq.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	return ddbq.execSQL();
}

//---------------------------------------------------------------------------
/*
bool LCDbBoxStore::updateBoxRecord( LQuery & ddbq ) {
	ddbq.setSQL( "update box_name set rack_cid = :rid, slot_position = :pos, slot_cid = :sid,"
				" time_stamp = 'now', process_cid = :pid where box_cid = :bid" );
	ddbq.setParam( "rid", rackID );
	ddbq.setParam( "pos", slot );
	ddbq.setParam( "sid", slotID );
	ddbq.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	ddbq.setParam( "bid", boxID );
	return ddbq.execSQL();
}
*/
//---------------------------------------------------------------------------
//	Mark record as belonging to given job; return false if claim fails
//---------------------------------------------------------------------------

bool LCDbBoxStore::setJobRef( LQuery ddbq, int jobRef, Status reason )
{
	if( jobID == jobRef )
		return true;                	// already part of the job

	if( jobRef != 0 && jobID != 0 )
		return false;					// already part of another job

	if( saved ) {
		ddbq.setSQL( "update box_store"
					" set retrieval_cid = :newJob, process_cid = :pid, status = :st"
					" where record_id = :myid and retrieval_cid = :oldJob" );
		ddbq.setParam( "oldJob", jobID );
	}
	else
	{	claimNextID( ddbq );
		ddbq.setSQL( "insert into box_store (record_id, box_cid, rack_cid,"
					" slot_position, status, time_stamp, retrieval_cid, process_cid)"
					" values ( :myid, :box, :rid, :pos, :st, 'now', :newJob, :pid )" );
		ddbq.setParam( "box", boxID );
		ddbq.setParam( "rid", rackID );
		ddbq.setParam( "pos", slot );
	}

	ddbq.setParam( "myid", getID() );
	ddbq.setParam( "st", reason );
	ddbq.setParam( "newJob", jobRef );
	ddbq.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	if( !ddbq.execSQL() )
		return false;               	// claimed by another job(?)

	jobID = jobRef;
	status = reason;
//	if( reason == SLOT_CONFIRMED )
//		updateBoxRecord( ddbq );
	return true;                    	// now part of the given job
}

//---------------------------------------------------------------------------
//	Find all the boxes that have been included in the given job
//---------------------------------------------------------------------------

bool LCDbBoxStores::readJob( LQuery pQuery, int jobRef )
{
	pQuery.setSQL( "select * from box_store where retrieval_cid = :job"
				  " order by record_id" );
	pQuery.setParam( "job", jobRef );
	return readData( pQuery );
}

//---------------------------------------------------------------------------
//	maintain c_slot_allocation for compatibility with older programs
//---------------------------------------------------------------------------
/*
bool LCDbBoxStore::findSlotRecord( LQuery & ddbq ) {
	ddbq.setSQL( "select slot_cid from c_slot_allocation"
				" where project_cid = :pid and box_cid = :box" );
	ddbq.setParam( "pid", LCDbProjects::getCurrentID() );
	ddbq.setParam( "box", boxID );
	slotID = ddbq.open() ? ddbq.readInt( 0 ) : 0;
	return (slotID != 0);
}

//---------------------------------------------------------------------------

bool LCDbBoxStore::insertSlotRecord( LQuery & ddbq ) {
	slotID = getID();
	ddbq.setSQL( "insert into c_slot_allocation"
				" (slot_cid, rack_cid, slot_position, status, project_cid, box_cid)"
				" values ( :myid, :rid, :pos, :sts, :pid, :box )" );
	ddbq.setParam( "myid", slotID );
	ddbq.setParam( "rid", rackID );
	ddbq.setParam( "pos", slot );
	ddbq.setParam( "sts", LPDbBoxName::CONFIRMED );
	ddbq.setParam( "pid", LCDbProjects::getCurrentID() );
	ddbq.setParam( "box", boxID );
	return ddbq.execSQL();
}

//---------------------------------------------------------------------------

bool LCDbBoxStore::updateSlotRecord( LQuery & ddbq ) {
	ddbq.setSQL( "update c_slot_allocation set rack_cid = :rid, slot_position = :pos"
				" where project_cid = :pid and box_cid = :box" );
	ddbq.setParam( "rid", rackID );
	ddbq.setParam( "pos", slot );
	ddbq.setParam( "pid", LCDbProjects::getCurrentID() );
	ddbq.setParam( "box", boxID );
	return ddbq.execSQL();
}
*/
//---------------------------------------------------------------------------

