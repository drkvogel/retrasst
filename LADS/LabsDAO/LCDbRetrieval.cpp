#pragma hdrstop
#include "LCDbRetrieval.h"
#pragma package(smart_init)

LCDbBoxRetrieval::LCDbBoxRetrieval(const LQuery & query) {
    //statusStrings = {"NEW", "PART_FILLED, COLLECTED, NOT_FOUND, DELETED"};
}

const char * LCDbBoxRetrieval::statusString(int st) {
    static const char * statusStrings[] = { "Expected", "In progress", "Completed", "Not found", "Deleted" };
    return st < LCDbBoxRetrieval::Status::NUM_STATUSES ? statusStrings[st] : "Invalid";
};

LCDbCryovialRetrieval::LCDbCryovialRetrieval(const LQuery & query)
 :
// int rj_box_cid, position, aliquot_type_cid, slot_number, process_cid; int status; TDateTime time_stamp;
//fixme LPDbID(query.readInt("rj_box_cid")),
   //LDbNoteCount( query.readInt( "Note_Exists" ) ),
   position(query.readInt("dest_pos")), //??
   aliquot_type_cid(query.readInt("aliquot_type_cid")),
   slot_number(query.readInt("lcr_slot")),
   process_cid(query.readInt("lcr_procid")),
   status(query.readInt("lcr_status"))
{
    //statusStrings[] = {"NEW, PART_FILLED, COLLECTED, NOT_FOUND, DELETED"};
}

const char * LCDbCryovialRetrieval::statusString(int st) {
    // EXPECTED, IGNORED, COLLECTED, NOT_FOUND, DELETED = 99, NUM_STATUSES };
    static const char * statusStrings[] = { "Expected", "Ignored", "Collected", "Not found", "Deleted" };
    return st < LCDbCryovialRetrieval::Status::NUM_STATUSES ? statusStrings[st] : "Invalid";
};

