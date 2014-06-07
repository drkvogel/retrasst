//---------------------------------------------------------------------------

#ifndef LCDbProjectH
#define LCDbProjectH

#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"
#include "LDbRange.h"

//---------------------------------------------------------------------------

class TLeaseManager;

class LCDbProject : public LCDbID, public LDbNames, public LDbValid
{
	std::string database, contact, code;
	bool liveData, boxImport;
	struct Version {
		short major, minor, phase;
		Version( short a, short b, short c ) : major(a), minor(b), phase(c)
		{}
	} version;

	int getFlags();

public:

	static const short IS_LIVE_STATUS = 1, BOX_IMPORT_STATUS = 2;
	static const short ANALYSIS_ACTIVITY = 1, COMPLETED_ACTIVITY = 2, SAMPLE_STORAGE = 4, BOX_STORAGE = 8;
	static const int CENTRAL_DB = 0, NONE_SELECTED = -1;

	LCDbProject( int id = NONE_SELECTED )
	 : LCDbID( id ), version( 2, 7, 0 ), liveData( false ), boxImport( false )
	{}

	LCDbProject( const LQuery & central );
	bool saveRecord( LQuery central );

	bool isLive() const { return liveData; }
	bool isCentral() const { return getID() == CENTRAL_DB; }
	bool isInCurrentSystem() const;
	bool hasBoxes() const { return boxImport; }

	const std::string & getDbName() const { return database; }
	void setDbName( const std::string & name ) { database = name; }

	const std::string & getAddress() const { return contact; }
	void setAddress( const std::string & url ) { contact = url; }

	const std::string & getStudyCode() const { return code; }
	void setStudyCode( const std::string & acronym ) { code = acronym; }

	void setVersionString( const std::string & sVer );
	Version getVersion() const { return version; }
	std::string getVersionString() const;
};

//---------------------------------------------------------------------------

class LCDbProjects : public LDbCache< LCDbProject >, public LCDbSingleton< LCDbProjects >
{
	TLeaseManager * leaseManager;
	int currentID;

public:

	static const int leaseID;			// to be specified in main module

	LCDbProjects()
	 : leaseManager( NULL ), currentID( LCDbProject::NONE_SELECTED )
	{}

	bool read( LQuery central, bool readAll = true );
	const LCDbProject * findByName( const std::string & nameOrDB ) const;

	static int getCurrentID() { return records().currentID; }
	void setCurrent( const LCDbProject * proj );
	void clearCurrentID() { currentID = LCDbProject::NONE_SELECTED; }
};

//---------------------------------------------------------------------------
//	Storage policy class – records() returns the current LDbCache
//---------------------------------------------------------------------------

template< typename Values > struct LPDbCacheMap
{
	static Values & records( int projID ) {
		static std::map< int, Values > shared;
		Values & cache = shared[ projID ];
		if( cache.empty() ) {
			LCDbProjects & projList = LCDbProjects::records();
			const LCDbProject * proj = projList.findByID( projID );
			if( proj == NULL || proj->isCentral() ) {
				throw Exception( "Invalid project CID" );
			}
			if( projID != projList.getCurrentID() ) {
				projList.setCurrent( proj );
			}
			cache.read( LIMSDatabase::getProjectDb( proj->getDbName(), true ) );
		}
		return cache;
	}

	static Values & records() {
		return records( LCDbProjects::getCurrentID() );
	}
};

//---------------------------------------------------------------------------

#endif

