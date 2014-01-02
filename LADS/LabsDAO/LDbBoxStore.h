//---------------------------------------------------------------------------

#ifndef LDbBoxStoreH
#define LDbBoxStoreH

#include "LDbIdClasses.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LCDbBoxStore : public LPDbID
{
	int rackID, boxID, slotID;
	short slot, status;
	TDateTime updated, removed;
	int jobID, processID;

	std::string site, rack, tank;
	int projectID, boxTypeID, tankID;

	void copyFields( const LQuery & ddbq );

	bool findSlotRecord( LQuery & ddbq );
	bool updateBoxRecord( LQuery & ddbq );
	bool insertStoreRecord( LQuery & ddbq );
	bool updateStoreRecord( LQuery & ddbq );
	bool insertSlotRecord( LQuery & ddbq );
	bool updateSlotRecord( LQuery & ddbq );

public:

	enum Status { EXPECTED = 0, UNCONFIRMED = 1, MOVE_EXPECTED = 2, REMOVED = 3,
				SLOT_ALLOCATED = 5, SLOT_CONFIRMED = 6, REFERRED = 7, DELETED = 99 };

	LCDbBoxStore( const LQuery & project );

	LCDbBoxStore( int id, int population, const std::string & rak, short pos )
	 : LPDbID( id ), tankID( population ), rack( rak ), slot( pos )
	{
		slotID = jobID = processID = projectID = boxID = boxTypeID = rackID = 0;
		status = EXPECTED;
	}

	LCDbBoxStore( const std::string & place, int population, const std::string & rak, short pos )
	 : site( place ), tankID( population ), rack( rak ), slot( pos )
	{
		slotID = jobID = processID = projectID = boxID = boxTypeID = rackID = 0;
		status = EXPECTED;
	}

	void setTypeID( int type ) { boxTypeID = type; }
	void setBox( int proj, int box ) { projectID = proj; boxID = box; }
	void setStatus( short p_st ) { status = p_st; };
	void setRackID( int p_id ) { rackID = p_id; };
	void setJobID( int p_id ) { jobID = p_id; };

	bool findBoxRecord( LQuery & ddbq );
	bool findStoreRecord( LQuery & ddbq );
	bool setJobRef( LQuery ddbq, int jobRef, Status reason );
	bool saveRecord( LQuery ddbq );

	int getTankID() const { return tankID; }
	int getRackID() const { return rackID; }
	short getSlotPosition() const { return slot; }
	int getBoxID() const { return boxID; }
	int getJobID() const { return jobID; }
};

//---------------------------------------------------------------------------

class LCDbBoxStores : public LDbCache< LCDbBoxStore >
{
public:
	bool readJob( LQuery project, int jobRef );
};

//---------------------------------------------------------------------------

#endif
