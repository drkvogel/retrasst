//---------------------------------------------------------------------------

#ifndef LCDbBoxStoreH
#define LCDbBoxStoreH

#include "LDbIdClasses.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LCDbBoxStore : public LPDbID
{
	int rackID, boxID;
	short slot, status;
	TDateTime updated, removed;
	int jobID, processID, map_id;

	String site, rack, tank;
	int projectID, boxTypeID, tankID;
	int rack_pos;

	bool copyFields( const LQuery & project );
	
public:

	enum Status { EXPECTED = 0, MOVE_EXPECTED = 1, REMOVED = 3, SLOT_ALLOCATED = 5,
				  SLOT_CONFIRMED = 6, REFERRED = 7, DELETED = 99 };

	LCDbBoxStore( const LQuery & project );

	LCDbBoxStore( int id, int hive, const String & rak, short pos  )
	 : LPDbID( id ), tankID( hive ), rack( rak ), slot( pos )
	{
		jobID = processID = projectID = boxID = boxTypeID = rackID = map_id = 0;
		status = EXPECTED;
	}

	LCDbBoxStore( const String & place, int hive, const String & rak, short pos )
	 : site( place ), tankID( hive ), rack( rak ), slot( pos )
	{
		jobID = processID = projectID = boxID = boxTypeID = rackID = map_id = 0;
		status = EXPECTED;
	}

	void setTypeID( int type ) { boxTypeID = type; }
	void setBox( int proj, int box ) { projectID = proj; boxID = box; }
	void setStatus( short p_st ) { status = p_st; };
	void setRackID( int p_id ) { rackID = p_id; };
	void setJobID( int p_id ) { jobID = p_id; };
	void setMapID( int p_id ) { map_id = p_id; };

	int getTankID() const { return tankID; }
	int getRackID() const { return rackID; }
	short getSlotPosition() const { return slot; }
	int getBoxID() const { return boxID; }
	int getJobID() const { return jobID; }

//	bool findRack( LQuery central );
//	void createRack( LQuery central );
	int getRackPos() const { return rack_pos; }

	bool findSlotRecord( LQuery project );
	bool findBoxRecord( LQuery project );
	bool saveRecord( LQuery project );
	bool setJobRef( LQuery project, int jobRef, Status reason );
};

//---------------------------------------------------------------------------

class LCDbBoxStores : public LDbCache< LCDbBoxStore >
{
//	class Matcher;

public:

	bool readJob( LQuery project, int jobRef );
};

//---------------------------------------------------------------------------

#endif
