#pragma hdrstop
#include "LCDbRetrieval.h"
#pragma package(smart_init)


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
    //???
}
