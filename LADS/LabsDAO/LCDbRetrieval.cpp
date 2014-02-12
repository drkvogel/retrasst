#pragma hdrstop
#include "LCDbRetrieval.h"
#pragma package(smart_init)

//-------- LCDbBoxRetrieval ---------

LCDbBoxRetrieval::LCDbBoxRetrieval(const LQuery & query) :
    rj_box_cid(query.readInt("rj_box_cid")),
    retrieval_cid(query.readInt("retrieval_cid")),
    box_id(query.readInt("box_id")),
    project_cid(query.readInt("project_cid")),
    section(query.readInt("section")),
    status(query.readInt("status"))
    //TDateTime time_stamp;
{
    saved = true;
}

const char * LCDbBoxRetrieval::statusString(int st) {
    static const char * statusStrings[] = { "Expected", "In progress", "Completed", "Not found", "Deleted" };
    if (DELETED == st) return "Deleted";
    return st < LCDbBoxRetrieval::Status::NUM_STATUSES ? statusStrings[st] : "Invalid";
};

bool LCDbBoxRetrieval::saveRecord(LQuery query) {
	if (!saved) {
        claimNextID(query);
        rj_box_cid = getID();
        query.setSQL(
            "INSERT INTO c_box_retrieval (rj_box_cid, retrieval_cid, box_id, project_cid, section, status) "
            " VALUES (:rjbid, :rtid, :bxid, :prid, :sect, :stat)"
        );
	} else { // update
        query.setSQL(
            "UPDATE c_box_retrieval "
            "SET retrieval_cid = :rtid, box_id = :bxid, project_cid, :prid, section = :sect, status = :stat) "
            "WHERE rj_box_cid = :rjbid "
        );
	}
    query.setParam("rjbid",rj_box_cid); // Unique ID for this retrieval list entry (also determines retrieval order for box retrievals)
    query.setParam("rtid", retrieval_cid);
    query.setParam("bxid", box_id); // The box being retrieved (for box retrieval/disposal) or retrieved into (for sample retrieval/disposal)
    query.setParam("prid", project_cid);
    query.setParam("sect", section); // 0 = retrieve all boxes in parallel
    query.setParam("stat", status); // 0: new record; 1: part-filled, 2: collected; 3: not found; 99: record deleted
    if (query.execSQL()) {
        saved = true;
		return true;
	} else {
		return false;
    }
}

//-------- LCDbCryovialRetrieval ---------

LCDbCryovialRetrieval::LCDbCryovialRetrieval(const LQuery & query) : //LCDbID(1), //saved(true),
    position(query.readInt("dest_pos")), //??
    aliquot_type_cid(query.readInt("aliquot_type_cid")), //???slot_number(query.readInt("lcr_slot")),
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
	if (!saved) {
		// claimNextID(query); // NO! rj_box_cid must be an existing id in c_box_retrieval
        query.setSQL(
            "INSERT INTO l_cryovial_retrieval (rj_box_cid, position, cryovial_barcode, aliquot_type_cid, slot_number, process_cid, time_stamp, status) "
            "VALUES (:rjbid, :pos, :barc, :aliq, :slot, :pid, 'now', :st)"
        );
        //rj_box_cid = getID(); //???
	} else { // update
        query.setSQL(
            "UPDATE l_cryovial_retrieval"
            "SET cryovial_barcode = :barc, aliquot_type_cid = :aliq, slot_number = :slot, process_cid = :pid, time_stamp = 'now', status = :st "
            "WHERE rj_box_cid = :rjbid AND position = :pos"
        );
	}
    query.setParam("rjbid", rj_box_cid);
    query.setParam("pos",   position); //?? //qc.setParam("pos",  sampleRow->store_record->getPosition()); //??
    query.setParam("barc",  cryovial_barcode); //??
    query.setParam("aliq",  aliquot_type_cid);
    query.setParam("slot",  new_position); // new pos or old pos?
    query.setParam("pid",   process_cid);
    query.setParam("st",    status);
	if (query.execSQL()) {
        saved = true;
		return true;
	} else {
		return false;
    }
}

