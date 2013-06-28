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
//		16 Nov 09, NG:	Use (project's) box_store, not c_slot allocation
//		28 July, 2010:	Added job ID as link to owner (LCDbRetrievalJob)
//---------------------------------------------------------------------------

#include <vcl.h>

#include "LQuery.h"
#include "LCDbBoxStore.h"
#include "LCDbObject.h"
#include "LCDbProject.h"
#include "LCDbTankMap.h"
#include "LCDbStoreDetail.h"
#include "LCDbTankLayout.h"
#include "LCDbAuditTrail.h"
#include "LPDbBoxes.h"

#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------

bool LCDbBoxStore::copyFields( const LQuery & project )
{
	rackID = project.readInt( "rack_cid" );
	boxID = project.readInt( "box_cid" );
	slot = project.readShort( "slot_position" );
	status = project.readShort( "status" );
	updated = project.readDateTime( "time_stamp" );
	processID = project.readInt( "process_cid" );

	if( project.fieldExists( "record_id" ) )
		setID( project.readInt( "record_id" ) );
	else
		setID( 0 );

	if( project.fieldExists( "removed" ) )
	{
		removed = project.readDateTime( "removed" );
		jobID = project.readInt( "retrieval_cid" );
	}
	else
	{ 	removed = 0;
		jobID = 0;
	}
	return true;
}

//---------------------------------------------------------------------------
//	Check the database for the given tank/rack/slot combination
//---------------------------------------------------------------------------

bool LCDbBoxStore::findSlotRecord( LQuery project )
{
	if( rackID != 0 )
	{
		// recent programs create box_store records for each stored box
		project.setSQL( "select * from box_store where status = :conf"
					   " and rack_cid = :rid and slot_position = :pos" );
		project.setParam( "rid", rackID );
		project.setParam( "pos", slot );
		project.setParam( "conf", LCDbBoxStore::SLOT_CONFIRMED );
		if( project.open() == 1 && copyFields( project ) )
			return true;

		// older programs store the rack and slot in the box_name table
		project.setSQL( "select * from box_name where status = :conf"
					   " and rack_cid = :rid and slot_position = :pos" );
		project.setParam( "rid", rackID );
		project.setParam( "pos", slot );
		project.setParam( "conf", LPDbBoxName::IN_TANK );
		if( project.open() == 1 && copyFields( project ) )
			return true;
	}
	return false;
}

//---------------------------------------------------------------------------
//	Check the database for the position of the selected box
//---------------------------------------------------------------------------

bool LCDbBoxStore::findBoxRecord( LQuery project )
{
	project.setSQL( "select * from box_store where status = :conf"
				   " and box_cid = :box" );
	project.setParam( "box", boxID );
	project.setParam( "conf", LCDbBoxStore::SLOT_CONFIRMED );
	if( project.open() == 1 && copyFields( project ) )
		return true;

	project.setSQL( "select * from box_name where status = :conf"
				   " and box_cid = :box" );
	project.setParam( "box", boxID );
	project.setParam( "conf", LPDbBoxName::IN_TANK );
	return project.open() == 1 && copyFields( project );
}

//---------------------------------------------------------------------------
//	Create or update slot details, adding to the movement history
//---------------------------------------------------------------------------

bool LCDbBoxStore::saveRecord( LQuery project )
{
	if( saved )
	{
		String fields = "rack_cid = :rid, slot_position = :pos, status = :sts,";
		switch( status )
		{
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

		project.setSQL( "update box_store set " + fields + " process_cid = :pid"
					   " where record_id = :myid" );
	}
	else
	{	claimNextID( project );
		project.setSQL( "insert into box_store (record_id, box_cid, rack_cid,"
						" slot_position, status, time_stamp, retrieval_cid, process_cid)"
					   " values ( :myid, :box, :rid, :pos, :sts, 'now', :jid, :pid )" );
		project.setParam( "box", boxID );
		project.setParam( "jid", jobID );
	}

	project.setParam( "myid", getID() );
	project.setParam( "rid", rackID );
	project.setParam( "pos", slot );
	project.setParam( "sts", status );
	project.setParam( "pid", LCDbAuditTrail::getProcessID() );
	return saved = (project.execSQL() == 1);
}

//---------------------------------------------------------------------------
//	Mark record as belonging to given job; return false if claim fails
//---------------------------------------------------------------------------

bool LCDbBoxStore::setJobRef( LQuery project, int jobRef, Status reason )
{
	if( jobID == jobRef )
		return true;                	// already part of the job

	if( jobRef != 0 && jobID != 0 )
		return false;					// already part of another job

	if( saved )
	{
		project.setSQL( "update box_store"
					   " set retrieval_cid = :newJob, process_cid = :pid, status = :st"
					   " where record_id = :myid and retrieval_cid = :oldJob" );
		project.setParam( "oldJob", jobID );
	}
	else
	{	claimNextID( project );
		project.setSQL( "insert into box_store (record_id, box_cid, rack_cid,"
					   " slot_position, status, time_stamp, retrieval_cid, process_cid)"
					   " values ( :myid, :box, :rid, :pos, :st, 'now', :newJob, :pid )" );
		project.setParam( "box", boxID );
		project.setParam( "rid", rackID );
		project.setParam( "pos", slot );
	}

	project.setParam( "myid", getID() );
	project.setParam( "st", reason );
	project.setParam( "newJob", jobRef );
	project.setParam( "pid", LCDbAuditTrail::getProcessID() );

	if( project.execSQL() != 1 )
		return false;               	// claimed by another job(?)

	jobID = jobRef;
	status = reason;
	return true;                    	// now part of the given job
}

//---------------------------------------------------------------------------
//	Find all the boxes that have been included in the given job
//---------------------------------------------------------------------------

bool LCDbBoxStores::readJob( LQuery pQuery, int jobRef )
{
	pQuery.setSQL( "select * from box_store where retrieval_cid = :job"
				  " order by record_cid" );
	pQuery.setParam( 0, jobRef );
	return readData( pQuery );
}

//---------------------------------------------------------------------------

