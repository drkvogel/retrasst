#ifndef LCDbRetrievalH
#define LCDbRetrievalH

#include <sstream>
#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

class LCDbBoxRetrieval : public LCDbID//, public LDbNames
{
    int rj_box_cid, retrieval_cid, box_id, project_cid, section, status;
    TDateTime time_stamp;
    //static const char * statusStrings[];
public:
    LCDbBoxRetrieval(const LQuery & query);
    enum Status { NEW, PART_FILLED, COLLECTED, NOT_FOUND, DELETED = 99, NUM_STATUSES };
    int getRJId() { return retrieval_cid; }
    void setRJId(int i) { retrieval_cid = i; }
    int getRJBId() { return rj_box_cid; }
    void setRJBId(int i) { rj_box_cid = i; }
    int getBoxID() { return box_id; }
    void setBoxID(int i) { box_id = i; }
    int getProjId() { return project_cid; }
    void setProjId(int i) { project_cid = i; }
    int getSection() { return section; }
    void setSection(int i) { section = i; }
    int getStatus() { return status; }
    void setStatus(int s) { status = s; }
    static const char * statusString(int i); // cannot use status without an object
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

class LCDbCryovialRetrieval : public LCDbID//, public LDbNames
{
    int rj_box_cid, position, aliquot_type_cid, slot_number, process_cid;
    int status;
    TDateTime time_stamp;
public:
    LCDbCryovialRetrieval(const LQuery & query);
    bool saveRecord(LQuery query);
    enum Status { EXPECTED, IGNORED, COLLECTED, COLLECTED_SECONDARY, PROCESSED, DISPOSED, NOT_FOUND, NUM_STATUSES, DELETED = 99 };
    static const char * statusString(int st);
    int getStatus() { return status; }
    void setStatus(int s) { status = s; }
    int getRJBId() { return rj_box_cid; }
    void setRJBId(int i) { rj_box_cid = i; }
    int getPosition() { return position; }
    void setPosition(int i) { position = i; }
    int getAliType() { return aliquot_type_cid; }
    void setAliType(int i) { aliquot_type_cid = i; }
    int getSlot() { return slot_number; }
    void setSlot(int i) { slot_number = i; }
    int getProcId() { return process_cid; }
    void setProcId(int i) { process_cid = i; }
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
