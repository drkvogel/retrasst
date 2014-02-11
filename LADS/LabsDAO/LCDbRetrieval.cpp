#pragma hdrstop
#include "LCDbRetrieval.h"
#pragma package(smart_init)

//-------- LCDbBoxRetrieval ---------

LCDbBoxRetrieval::LCDbBoxRetrieval(const LQuery & query)
// :
//    int rj_box_cid, position, aliquot_type_cid, old_box_cid, old_position, new_position, process_cid, status;
//    std::string cryovial_barcode;
//    TDateTime time_stamp;
{

}

const char * LCDbBoxRetrieval::statusString(int st) {
    static const char * statusStrings[] = { "Expected", "In progress", "Completed", "Not found", "Deleted" };
    if (DELETED == st) return "Deleted";
    return st < LCDbBoxRetrieval::Status::NUM_STATUSES ? statusStrings[st] : "Invalid";
};



//-------- LCDbCryovialRetrieval ---------

LCDbCryovialRetrieval::LCDbCryovialRetrieval(const LQuery & query) :
    //LCDbID(1),
    //saved(true),
    position(query.readInt("dest_pos")), //??
    aliquot_type_cid(query.readInt("aliquot_type_cid")),
    //slot_number(query.readInt("lcr_slot")),
    process_cid(query.readInt("lcr_procid")),
    status(query.readInt("lcr_status"))
{
    saved = true;
}

const char * LCDbCryovialRetrieval::statusString(int st) {
    // enum Status { EXPECTED, IGNORED, COLLECTED, COLLECTED_SECONDARY, PROCESSED, DISPOSED, NOT_FOUND, NUM_STATUSES, DELETED = 99 };
    static const char * statusStrings[] = { "Expected", "Ignored", "Collected", "Collected secondary", "Processed", "Disposed", "Not found" };
    if (DELETED == st) return "Deleted";
    return st < LCDbCryovialRetrieval::Status::NUM_STATUSES ? statusStrings[st] : "Invalid";
};

bool LCDbCryovialRetrieval::saveRecord(LQuery query) {
    //throw "todo";
	if (!saved) {
		claimNextID(query);
        query.setSQL(
            "INSERT INTO l_cryovial_retrieval (rj_box_cid, position, cryovial_barcode, aliquot_type_cid, slot_number, process_cid, time_stamp, status)"
            " VALUES (:rjbid, :pos, :barc, :aliq, :slot, :pid, 'now', :st)"
        );
        rj_box_cid = getID(); //???

	} else { // update
        query.setSQL(
            "UPDATE l_cryovial_retrieval"
            " SET cryovial_barcode = :barc, aliquot_type_cid = :aliq, slot_number = :slot, process_cid = :pid, time_stamp = 'now', status = :st)"
            " WHERE rj_box_cid = :rjbid AND position = :pos"
        );
//		std::stringstream fields;
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
//		query.setSQL(fields.str());
	}
    query.setParam("rjbid", rj_box_cid);
    query.setParam("pos",  position); //?? //qc.setParam("pos",  sampleRow->store_record->getPosition()); //??
    query.setParam("barc", cryovial_barcode); //??
    query.setParam("aliq", aliquot_type_cid);
    query.setParam("slot", new_position); // new pos or old pos?
    query.setParam("pid",  process_cid);
    query.setParam("st",   status); //??

	if (query.execSQL()) {
        saved = true;
		return true;
	} else {
		return false;
    }
}

