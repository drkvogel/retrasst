//---------------------------------------------------------------------------

#ifndef LCDbCannedH
#define LCDbCannedH

#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LCDbCanned : public LCDbID, public LDbNames
{
	short status;
	int groupID, processID;

public:

	LCDbCanned( const LQuery & query );
	LCDbCanned( int textID );
	bool saveRecord( LQuery central );

	bool isActive() const { return status != LDbValid::DELETED; }
	void setActive( bool live )	{
		status = live ? LDbValid::RECORD_IN_USE : LDbValid::DELETED;
	}
	int getGroupID() const { return groupID; }
};

//---------------------------------------------------------------------------

class LCDbCanneds : public LDbCache< LCDbCanned >, public LCDbSingleton< LCDbCanneds >
{

public:
	bool read( LQuery central, bool readAll = false );
};

//---------------------------------------------------------------------------

#endif


