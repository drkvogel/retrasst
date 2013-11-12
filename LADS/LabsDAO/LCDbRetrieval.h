#ifndef LCDbRetrievalH
#define LCDbRetrievalH

#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

class LCDbBoxRetrieval : public LCDbID, public LDbNames
{
    int rj_box_cid, retrieval_cid, box_id, project_cid, section;
    enum Status { NEW, PART_FILLED, COLLECTED, NOT_FOUND, DELETED = 99, NUM_STATUSES } status;
    TDateTime time_stamp;
};

class LCDbCryovialRetrieval : public LCDbID, public LDbNames
{
    int rj_box_cid, position, aliquot_type_cid, slot_number, process_cid;
    enum Status { EXPECTED, IGNORED, COLLECTED, NOT_FOUND, DELETED = 99, NUM_STATUSES } status;
    TDateTime time_stamp;
};

#endif
