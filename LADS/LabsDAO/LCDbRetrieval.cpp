#pragma hdrstop
#include "LCDbRetrieval.h"
#pragma package(smart_init)

LCDbBoxRetrieval::LCDbBoxRetrieval(const LQuery & query) {}

const char * LCDbBoxRetrieval::statusString(int st) {
    static const char * statusStrings[] = { "Expected", "In progress", "Completed", "Not found", "Deleted" };
    if (DELETED == st) return "Deleted";
    return st < LCDbBoxRetrieval::Status::NUM_STATUSES ? statusStrings[st] : "Invalid";
};

LCDbCryovialRetrieval::LCDbCryovialRetrieval(const LQuery & query) :
   position(query.readInt("dest_pos")), //??
   aliquot_type_cid(query.readInt("aliquot_type_cid")),
   slot_number(query.readInt("lcr_slot")),
   process_cid(query.readInt("lcr_procid")),
   status(query.readInt("lcr_status")) {}

const char * LCDbCryovialRetrieval::statusString(int st) {
    // enum Status { EXPECTED, IGNORED, COLLECTED, COLLECTED_SECONDARY, PROCESSED, DISPOSED, NOT_FOUND, NUM_STATUSES, DELETED = 99 };
    static const char * statusStrings[] = { "Expected", "Ignored", "Collected", "Collected secondary", "Processed", "Disposed", "Not found" };
    if (DELETED == st) return "Deleted";
    return st < LCDbCryovialRetrieval::Status::NUM_STATUSES ? statusStrings[st] : "Invalid";
};

bool LCDbCryovialRetrieval::saveRecord(LQuery query) {
    throw "todo";
	if (!saved) {
		claimNextID(query);
//		query.setSQL( "insert into cryovial_store (record_id, cryovial_id, box_cid,"
//					 " tube_position, time_stamp, status, note_exists, process_cid, retrieval_cid)"
//					 " values ( :rid, :cid, :bid, :pos, 'now', :sts, 0, :pid, :jcid )" );
//		query.setParam( "cid", getID() );
//		query.setParam( "bid", boxID );
//		query.setParam( "pos", position );
	} else { // update
		std::stringstream fields;
//		fields << "update cryovial_store set status = :sts, retrieval_cid = jcid, process_cid = :pid";
//		switch( status ) {
//			case ALLOCATED:
//			case CONFIRMED:
//				fields << ", time_stamp = 'now'";
//			break;
//			case ANALYSED:
//			case DESTROYED:
//				fields << ", removed = 'now'";
//		}
//		if( volume >= 0 ) {
//			fields << ", sample_volume = " << volume;
//		}
//		fields << " where record_id = :rid";
		query.setSQL(fields.str());
	}
//	query.setParam( "sts", status);
//	query.setParam( "jcid", retrievalID);
//	query.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID());
//	query.setParam( "rid", getID());
	if (query.execSQL()) {
        saved = true;
		return true;
	} else {
		return false;
    }
}

