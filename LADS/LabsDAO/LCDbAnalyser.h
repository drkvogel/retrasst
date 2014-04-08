//---------------------------------------------------------------------------

#ifndef LCDbAnalyserH
#define LCDbAnalyserH

#include <set>
#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LCDbAnalyser : public LCDbID, public LDbNames, public LDbValid
{
	enum { UNKNOWN, IS_BUDDY = 1, IS_LAPTOP = 2 } use;

	std::set< int > clusterIDs;
	std::string location;

public:

	LCDbAnalyser( int id = 0 ) : LCDbID( id ), use( UNKNOWN )
	{}

	LCDbAnalyser( const LQuery & query  );

	void makeDefault() const;

	bool isBuddyPC() const { return use == IS_BUDDY; }
	bool isLaptop() const { return use == IS_LAPTOP; }

	std::set< int > getClusterIDs() const { return clusterIDs; }
	void addClusterID( int nid ) { clusterIDs.insert( nid ); }
	void setStatusFlags();

	std::string getLocation() const { return location; }
	void setLocation( const std::string & nl ) { location = nl; }

	bool saveRecord( LQuery central );
};

//---------------------------------------------------------------------------

class LCDbAnalysers : public LDbCache< LCDbAnalyser >, public LCDbSingleton< LCDbAnalysers >
{
	int currentID;

public:

	LCDbAnalysers() : currentID( 0 ) {}

	void setCurrent( const LCDbAnalyser & buddy ) { currentID = buddy.getID(); }
	void setCurrent( const std::string & name );
	static int getCurrentID() { return records().currentID; }

	std::set< int > getMachineIDs( int buddyID = 0 ) const;
	std::set< int > getAnalyserIDs( int machineID = 0 ) const;

	bool read( LQuery central, bool readAll = false );
	const LCDbAnalyser * findByName( const std::string & name ) const;
};

//---------------------------------------------------------------------------

#endif

