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
//						Use box_store + c_slot_allocation for compatibility
//      16 Dec 13, NG:	Updated to use c_ tables again for database v2.7.2
//		4 April 14, NG:	Use same ID for c_slot_allocation and box_store
//
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

LCDbBoxStore::LCDbBoxStore( const LQuery & pq ) {
	copyFields( pq );
}

//---------------------------------------------------------------------------

void LCDbBoxStore::copyFields( const LQuery & ddbq )
{
	boxID = ddbq.readInt( "box_cid" );
	int slotID = ddbq.fieldExists( "slot_cid" ) ? ddbq.readInt( "slot_cid" ) : 0;
	int record = ddbq.fieldExists( "record_id" ) ? ddbq.readInt( "record_id" ) : slotID;
	setID( record == 0 ? boxID : record );

	rackID = ddbq.fieldExists( "rack_cid" ) ? ddbq.readInt( "rack_cid" ) : 0;
	slot = ddbq.fieldExists( "slot_position" ) ? ddbq.readInt( "slot_position" ) : 0;
	jobID = ddbq.fieldExists( "retrieval_cid" ) ? ddbq.readInt( "retrieval_cid" ) : 0;
	status = ddbq.readInt( "status" );
	processID = ddbq.fieldExists( "process_cid" ) ? ddbq.readInt( "process_cid" ) : 0;
	if( ddbq.fieldExists( "time_stamp" ) ) {
		updated =  ddbq.readDateTime( "time_stamp" );
	}
	if( jobID != 0 && ddbq.fieldExists( "removed" ) ) {
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
		// prefer c_slot_allocation if details available
		ddbq.setSQL( "select * from c_slot_allocation where status = :conf"
					" and rack_cid = :rid and slot_position = :pos" );
		ddbq.setParam( "rid", rackID );
		ddbq.setParam( "pos", slot );
		ddbq.setParam( "conf", LCDbBoxStore::SLOT_CONFIRMED );
		if( ddbq.open() ) {
			copyFields( ddbq );
			return true;
		}
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
		ddbq.setSQL( "select * from box_name where status = :conf"
					" and rack_cid = :rid and slot_position = :pos" );
		ddbq.setParam( "rid", rackID );
		ddbq.setParam( "pos", slot );
		ddbq.setParam( "conf", LPDbBoxName::IN_TANK );
		if( ddbq.open() ) {
			copyFields( ddbq );
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------
//	Check the database for the position of the selected box
//---------------------------------------------------------------------------

bool LCDbBoxStore::findBoxRecord( LQuery & ddbq )
{
	ddbq.setSQL( "select * from c_slot_allocation where status = :conf and box_cid = :box" );
	ddbq.setParam( "box", boxID );
	ddbq.setParam( "conf", LCDbBoxStore::SLOT_CONFIRMED );
	if( ddbq.open() ) {
		copyFields( ddbq );
		return true;
	}
	ddbq.setSQL( "select * from box_store where status = :conf and box_cid = :box" );
	ddbq.setParam( "box", boxID );
	ddbq.setParam( "conf", LCDbBoxStore::SLOT_CONFIRMED );
	if( ddbq.open() ) {
		copyFields( ddbq );
		return true;
	}
	ddbq.setSQL( "select * from box_name where status = :conf and box_cid = :box" );
	ddbq.setParam( "box", boxID );
	ddbq.setParam( "conf", LPDbBoxName::IN_TANK );
	if( ddbq.open() ) {
		copyFields( ddbq );
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------
//	Create or update box_store record, adding to the movement history
//	Also update box_name and/or c_slot_allocation for compatibility
//---------------------------------------------------------------------------

bool LCDbBoxStore::saveRecord( LQuery pq, LQuery cq )
{
	bool bs = saved && updateStoreRecord( pq );
	if( !bs ) {
		while( needsNewID( cq ) ) {
			claimNextID( pq );
		}
		bs = insertStoreRecord( pq );
	}
	bool csa = updateSlotRecord( cq );
	if( !csa ) {
		csa = insertSlotRecord( cq );
	}
	if( status == LCDbBoxStore::SLOT_CONFIRMED ) {
		updateBoxRecord( pq );
	}
	return bs && csa;
}

//---------------------------------------------------------------------------

bool LCDbBoxStore::insertStoreRecord( LQuery & pq )
{
	pq.setSQL( "insert into box_store (record_id, box_cid, rack_cid,"
				" slot_position, status, time_stamp, retrieval_cid, process_cid)"
				" values ( :myid, :box, :rid, :pos, :sts, 'now', :jid, :pid )" );
	pq.setParam( "box", boxID );
	pq.setParam( "jid", jobID );
	pq.setParam( "myid", getID() );
	pq.setParam( "rid", rackID );
	pq.setParam( "pos", slot );
	pq.setParam( "sts", status );
	pq.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	saved = pq.execSQL();
	return saved;
}

//---------------------------------------------------------------------------

bool LCDbBoxStore::updateStoreRecord( LQuery & pq )
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
	pq.setSQL( "update box_store set " + fields + " process_cid = :pid where record_id = :myid" );
	pq.setParam( "myid", getID() );
	pq.setParam( "rid", rackID );
	pq.setParam( "pos", slot );
	pq.setParam( "sts", status );
	pq.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	return pq.execSQL();
}

//---------------------------------------------------------------------------

bool LCDbBoxStore::updateBoxRecord( LQuery & pq ) {
	pq.setSQL( "update box_name set slot_cid = :sid, time_stamp = 'now', process_cid = :pid"
			  " where box_cid = :bid" );
	pq.setParam( "sid", getID() );
	pq.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	pq.setParam( "bid", boxID );
	return pq.execSQL();
}

//---------------------------------------------------------------------------
//	Mark record as belonging to given job; return false if claim fails
//---------------------------------------------------------------------------

bool LCDbBoxStore::setJobRef( LQuery ddbq, int jobRef, Status reason )
{
	if( jobID == jobRef )
		return true;                	// already part of the job

	if( jobRef != 0 && jobID != 0 )
		return false;					// already part of another job

	if( !setJobRef( ddbq, true, jobRef, reason )
	 || !setJobRef( ddbq, false, jobRef, reason ) ) {
		return false;               	// claimed by another job(?)
	}
	jobID = jobRef;
	status = reason;
	if( reason == SLOT_CONFIRMED ) {
		updateBoxRecord( ddbq );
	}
	return true;                    	// now part of the given job
}

//---------------------------------------------------------------------------

bool LCDbBoxStore::setJobRef( LQuery & ddbq, bool central, int jobRef, Status reason )
{
	if( saved ) {
		if( central ) {
			ddbq.setSQL( "update c_slot_allocation"
						" set retrieval_cid = :newJob, status = :st, time_stamp = 'now'"
						" where slot_cid = :myid and retrieval_cid = :oldJob" );
		} else {
			ddbq.setSQL( "update box_store"
						" set retrieval_cid = :newJob, process_cid = :pid, status = :st"
						" where record_id = :myid and retrieval_cid = :oldJob" );
			ddbq.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
		}
		ddbq.setParam( "oldJob", jobID );
	}
	else
	{	claimNextID( ddbq );
		if( central ) {
			ddbq.setSQL( "insert into c_slot_allocation (slot_cid,"
					" box_cid, rack_cid, slot_position, status, time_stamp, retrieval_cid)"
					" values ( :myid, :box, :rid, :pos, :st, 'now', :newJob )" );
		} else {
			ddbq.setSQL( "insert into box_store (record_id, process_cid,"
					" box_cid, rack_cid, slot_position, status, time_stamp, retrieval_cid)"
					" values ( :myid, :pid, :box, :rid, :pos, :st, 'now', :newJob )" );
			ddbq.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
		}
		ddbq.setParam( "box", boxID );
		ddbq.setParam( "rid", rackID );
		ddbq.setParam( "pos", slot );
	}
	ddbq.setParam( "myid", getID() );
	ddbq.setParam( "st", reason );
	ddbq.setParam( "newJob", jobRef );
	return ddbq.execSQL();
}

//---------------------------------------------------------------------------
//	Find all the boxes that have been included in the given job
//---------------------------------------------------------------------------

bool LCDbBoxStores::readJob( LQuery pq, int jobRef )
{
	pq.setSQL( "select * from box_store where retrieval_cid = :job order by record_id" );
	pq.setParam( "job", jobRef );
	return readData( pq );
}

//---------------------------------------------------------------------------
//	maintain c_slot_allocation for compatibility with older programs
//---------------------------------------------------------------------------

bool LCDbBoxStore::findSlotRecord( LQuery & cq ) {
	cq.setSQL( "select slot_cid from c_slot_allocation where box_cid = :box" );
	cq.setParam( "box", boxID );
	int slotID = cq.open() ? cq.readInt( 0 ) : 0;
	return (slotID != 0);
}

//---------------------------------------------------------------------------

bool LCDbBoxStore::insertSlotRecord( LQuery & cq ) {
	cq.setSQL( "insert into c_slot_allocation"
			  " (slot_cid, rack_cid, slot_position, status, project_cid, box_cid)"
			  " values ( :myid, :rid, :pos, :sts, :pid, :box )" );
	cq.setParam( "myid", getID() );
	cq.setParam( "pid", LCDbProjects::getCurrentID() );
	cq.setParam( "box", boxID );
	cq.setParam( "rid", rackID );
	cq.setParam( "pos", slot );
	cq.setParam( "sts", status );
	return cq.execSQL();
}

//---------------------------------------------------------------------------

bool LCDbBoxStore::updateSlotRecord( LQuery & cq ) {
	cq.setSQL( "update c_slot_allocation"
			  " set rack_cid = :rid, slot_position = :pos, status = :sts, time_stamp = 'now'"
			  " where slot_cid = :sid" );
	cq.setParam( "rid", rackID );
	cq.setParam( "pos", slot );
	cq.setParam( "sts", status );
	cq.setParam( "sid", getID() );
	return cq.execSQL();
}

//---------------------------------------------------------------------------
//	check record ID is valid and another slot doesn't have the same one
//---------------------------------------------------------------------------

bool LCDbBoxStore::needsNewID( LQuery & cq ) const {
	if( getID() != 0 ) {
		cq.setSQL( "select count(*) from c_slot_allocation where slot_cid = :sid" );
		cq.setParam( "sid", getID() );
		if( cq.open() && cq.readInt( 0 ) == 0 ) {
			return false;
		}
	}
	return true;
}

//---------------------------------------------------------------------------

