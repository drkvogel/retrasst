//---------------------------------------------------------------------------

#ifndef LCDbProjectH
#define LCDbProjectH

#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"
#include "LDbRange.h"
#include "LIMSDatabase.h"

//---------------------------------------------------------------------------

class TLeaseManager;

class LCDbProject : public LCDbID, public LDbNames, public LDbValid
{
	std::string database, contact, code;
	std::pair< short, short > version;
	bool liveData, boxImport;

	int getFlags();

public:

	static const short IS_LIVE = 1, BOX_IMPORT = 2;
	static const int CENTRAL_DB = 0, NONE_SELECTED = -1;

	LCDbProject( int id = NONE_SELECTED )
	 : LCDbID( id ), version( 2, 6 ), liveData( false ), boxImport( false )
	{}

	LCDbProject( const LQuery & central );
	bool saveRecord( LQuery central );

	bool isLive() const { return liveData; }
	bool isCentral() const { return getID() == CENTRAL_DB; }
	bool isValid() const { return LIMSDatabase::includes( database ); }
	bool hasBoxes() const { return boxImport; }

	const std::string & getDbName() const { return database; }
	void setDbName( const std::string & name ) { database = name; }

	const std::string & getAddress() const { return contact; }
	void setAddress( const std::string & url ) { contact = url; }

	const std::string & getStudyCode() const { return code; }
	void setStudyCode( const std::string & acronym ) { code = acronym; }

	void setVersionString( const std::string & sVer );
	std::pair< short, short > getVersion() const { return version; }
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
	void setCurrent( const LCDbProject & proj );
	void clearCurrentID() { currentID = LCDbProject::NONE_SELECTED; }
};

//---------------------------------------------------------------------------

#endif

