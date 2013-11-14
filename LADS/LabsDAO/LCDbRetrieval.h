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

class LCDbBoxRetrievals : public LDbCache< LCDbBoxRetrieval >, public LCDbSingleton< LCDbBoxRetrieval >
{
	//int currentID;
public:
	LCDbBoxRetrievals(bool keepAlive = true);
	//static int getCurrentID() { return records().currentID; }
	//void clearCurrentID() { currentID = 0; }
	//void setCurrent(const LCDbBoxRetrieval & job) { currentID = job.getID(); }
	const LCDbBoxRetrieval * readRecord(LQuery cQuery, int sid);
	//bool read( LQuery cQuery, LCDbBoxRetrieval::JobKind type = LCDbBoxRetrieval::UNKNOWN, bool all = false );
	const LCDbBoxRetrieval * findByName(const std::string & name) const {
		return findMatch(LDbNames::LCMatcher(name));
	}
};

class LCDbCryovialRetrieval : public LCDbID, public LDbNames
{
    int rj_box_cid, position, aliquot_type_cid, slot_number, process_cid;
    enum Status { EXPECTED, IGNORED, COLLECTED, NOT_FOUND, DELETED = 99, NUM_STATUSES } status;
    TDateTime time_stamp;
};

class LCDbCryovialRetrievals : public LDbCache< LCDbCryovialRetrieval >, public LCDbSingleton< LCDbCryovialRetrieval >
{
	//int currentID;
public:
	LCDbCryovialRetrievals(bool keepAlive = true);
	//static int getCurrentID() { return records().currentID; }
	//void clearCurrentID() { currentID = 0; }
	//void setCurrent(const LCDbCryovialRetrieval & job) { currentID = job.getID(); }
	const LCDbCryovialRetrieval * readRecord(LQuery cQuery, int sid);
	//bool read( LQuery cQuery, LCDbCryovialRetrieval::JobKind type = LCDbCryovialRetrieval::UNKNOWN, bool all = false );
	const LCDbCryovialRetrieval * findByName(const std::string & name) const {
		return findMatch(LDbNames::LCMatcher(name));
	}
};

#endif
