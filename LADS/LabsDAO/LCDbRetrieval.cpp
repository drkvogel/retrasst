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
    status(query.readInt("status")),
    time_stamp(query.readDateTime("time_stamp"))
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
	} else {
        query.setSQL(
            "UPDATE c_box_retrieval "
            "SET retrieval_cid = :rtid, box_id = :bxid, project_cid, :prid, section = :sect, status = :stat) "
            "WHERE rj_box_cid = :rjbid "
        );
	}
    query.setParam("rjbid",rj_box_cid); // Unique ID for this retrieval list entry (also determines retrieval order for box retrievals)
    query.setParam("rtid", retrieval_cid);
	query.setParam("bxid", box_id); 	// The box being retrieved (for box retrieval/disposal) or retrieved into (for sample retrieval/disposal)
    query.setParam("prid", project_cid);
	query.setParam("sect", section); 	// 0 = retrieve all boxes in parallel
	query.setParam("stat", status); 	// 0: new record; 1: part-filled, 2: collected; 3: not found; 99: record deleted
    if (query.execSQL()) {
        saved = true;
		return true;
	} else {
		return false;
    }
}

//-------- LCDbCryovialRetrieval ---------

LCDbCryovialRetrieval::LCDbCryovialRetrieval(const LQuery & query) : //LCDbID(1), //saved(true),
    rj_box_cid(query.readInt("rj_box_cid")),
    position(query.readInt("lcr_position")), // NOT cryovial_position - this is the position in the plan
    cryovial_barcode(query.readString("cryovial_barcode")),
    aliquot_type_cid(query.readInt("aliquot_type_cid")),
    old_box_cid(query.readInt("old_box_cid")),
    old_position(query.readInt("old_position")),
    new_position(query.readInt("new_position")), //slot_number(query.readInt("lcr_slot")),
    process_cid(query.readInt("lcr_procid")),
    status(query.readInt("lcr_status")),
	time_stamp(query.readDateTime("time_stamp"))
{
    saved = true;
}

const char * LCDbCryovialRetrieval::statusString(int st) {
	static const char * statusStrings[] = { "Expected", "Ignored", "Collected", "Disposed", "Not found" };
    if (DELETED == st) return "Deleted";
    return st < LCDbCryovialRetrieval::Status::NUM_STATUSES ? statusStrings[st] : "Invalid";
};

bool LCDbCryovialRetrieval::saveRecord(LQuery query) {
    if (IGNORED == status) { // don't save, so will revert back to EXPECTED on next load of plan
        return true; // not an error
    }
	if (!saved) {
		query.setSQL(
			//"INSERT INTO l_cryovial_retrieval (rj_box_cid, position, cryovial_barcode, aliquot_type_cid, slot_number, process_cid, time_stamp, status) "
            "INSERT INTO l_cryovial_retrieval "
                " (rj_box_cid, position, cryovial_barcode, aliquot_type_cid, "
                " old_box_cid, old_position, new_position, "
                " process_cid, status) "
            "VALUES (:rjbid, :pos, :barc, :aliq, "
                " :oldbox, :oldpos, :newpos,"
                " :pid, :st)"
		);
	} else {
		query.setSQL(
			"UPDATE l_cryovial_retrieval "
			//"SET cryovial_barcode = :barc, aliquot_type_cid = :aliq, slot_number = :slot, process_cid = :pid, time_stamp = 'now', status = :st "
            "SET cryovial_barcode = :barc, aliquot_type_cid = :aliq, "
                " old_box_cid = :oldbox, old_position = :oldpos, new_position = :newpos,"
                " process_cid = :pid, status = :st "
			"WHERE rj_box_cid = :rjbid AND position = :pos"
		);
	}
	query.setParam("rjbid",     rj_box_cid); 		// rj_box_cid must be an existing id in c_box_retrieval
	query.setParam("pos",       position);
	query.setParam("barc",      cryovial_barcode);
	query.setParam("aliq",      aliquot_type_cid);
	query.setParam("oldbox",    old_box_cid);
    query.setParam("oldpos",    old_position);
    query.setParam("newpos",    new_position);
    //query.setParam("slot",  slot_number); // deprecated??
	query.setParam("pid",       process_cid);
	query.setParam("st",        status);
	if (query.execSQL()) {
		saved = true;
		return true;
	} else {
		return false;
	}
}

