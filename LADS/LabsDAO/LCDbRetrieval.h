#ifndef LCDbRetrievalH
#define LCDbRetrievalH

#include <sstream>
#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

using namespace std;

class LCDbBoxRetrieval : public LCDbID//, public LDbNames
{
	int rj_box_cid, retrieval_cid, box_id, project_cid, section, status;
    TDateTime time_stamp;
public:
	LCDbBoxRetrieval(const LQuery & query);
    LCDbBoxRetrieval(int rj, int bid, int prj, int sec, int st) : //int rjb, int typ,
        rj_box_cid(0), retrieval_cid(rj), box_id(bid), project_cid(prj), section(sec), status(st)
	{};
	enum Status { NEW, PART_FILLED, COLLECTED, NOT_FOUND, DELETED = 99, NUM_STATUSES };
	bool saveRecord(LQuery query);
    int     getRJBId() { return rj_box_cid; }
    void    setRJBId(int i) { rj_box_cid = i; }
    int     getRJId() { return retrieval_cid; }
	void    setRJId(int i) { retrieval_cid = i; }
    int     getBoxID() { return box_id; }
    void    setBoxID(int i) { box_id = i; }
    int     getProjId() { return project_cid; }
    void    setProjId(int i) { project_cid = i; }
    int     getSection() { return section; }
    void    setSection(int s) { section = s; }
    int     getStatus() { return status; }
    void    setStatus(int s) { status = s; }
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
//	const LCDbBoxRetrieval * findByName(const std::string & name) const {
//		return findMatch(LDbNames::LCMatcher(name));
//	}
};

class LCDbCryovialRetrieval : public LCDbID//, public LDbNames
{
    int rj_box_cid, position, aliquot_type_cid,
    old_box_cid, old_position, new_position,
    process_cid, status;
    std::string cryovial_barcode;
    TDateTime time_stamp; // defaults to 'now'
    //int slot_number; // deprecated
public:
    LCDbCryovialRetrieval(const LQuery & query);
    LCDbCryovialRetrieval(int rjbid, int pos, std::string barcode, int aliq,
            int oldbid, int oldpos, int newpos,
            //int pid, int st, int slot)
            int pid, int st)
        : rj_box_cid(rjbid), position(pos), cryovial_barcode(barcode), aliquot_type_cid(aliq),
        old_box_cid(oldbid), old_position(oldpos), new_position(newpos),
        process_cid(pid), status(st) //, slot_number(slot)
        {}
    bool saveRecord(LQuery query);
    enum Status { EXPECTED, IGNORED, COLLECTED, DISPOSED, NOT_FOUND, NUM_STATUSES, DELETED = 99 }; //??? COLLECTED_SECONDARY
    static const char * statusString(int st);
    int     getRJBId() { return rj_box_cid; }
    void    setRJBId(int i) { rj_box_cid = i; }
    int     getPosition() { return position; }
    void    setPosition(int i) { position = i; }
    string  getBarcode() { return cryovial_barcode; }
    void    setBarcode(string b) { cryovial_barcode = b; }
    int     getAliType() { return aliquot_type_cid; }
    void    setAliType(int i) { aliquot_type_cid = i; }
    int     getOldBox() { return old_box_cid; }
    void    setOldBox(int i) { old_box_cid = i; }
    int     getOldPosition() { return old_position; }
    void    setOldPosition(int i) { old_position = i; }
    int     getNewPosition() { return new_position; }
    void    setNewPosition(int i) { new_position = i; }
//    int     getSlot() { return slot_number; }
//    void    setSlot(int i) { slot_number = i; }
    int     getProcessId() { return process_cid; }
    void    setProcessId(int i) { process_cid = i; }
    int     getStatus() { return status; }
    void    setStatus(int s) { status = s; }
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
//	const LCDbCryovialRetrieval * findByName(const std::string & name) const {
//		return findMatch(LDbNames::LCMatcher(name));
// 	}
};

#endif
