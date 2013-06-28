//---------------------------------------------------------------------------

#ifndef LIMSDatabaseH
#define LIMSDatabaseH

#include <string>
#include <xdb.h>

class ROSETTA;

//---------------------------------------------------------------------------

class LIMSDatabase {

	static std::vector< std::string > errors;
//	static std::vector< LIMSDatabase > pool;

	LIMSDatabase( const std::string & dbName );
	LIMSDatabase & operator =( const LIMSDatabase & other ); // not allowed

	void setErrorCallBacks();
//	static LIMSDatabase * find( const std::string &dbName );

	static void xerrorCallback( const std::string object, const std::string error_txt );
	static bool rosettaCallback( const ROSETTA *r, const int type, const std::string msg );
	static bool xdbErrorCallback( const std::string object, const int instance,
			const int ecount, const int ecode, const std::string error_txt );

public:

	LIMSDatabase( const LIMSDatabase & other );
	~LIMSDatabase();

	enum DbSystem { TEST_DATA, LIVE_DATA, MIRROR_SYSTEM, UNKNOWN = 99 };

	static DbSystem getCurrentSystem() { return current; }
	static void setCurrentSystem( DbSystem system );
	static bool includes( const std::string & dbName );

	static LIMSDatabase getCentralDb();
	static LIMSDatabase getDistributedDb( const std::string & dbName );
	static LIMSDatabase getProjectDb( std::string dbName = "" );

	std::string getDbName() const;
	XDB * connect( bool readLocks = true );
	bool isConnected() const;
	static void confirm( bool success );

private:

	static DbSystem current;

	DbSystem dbs;
	XDB * xdb;
	std::string name;

	static std::string getRootName( const std::string & dbName );
	static std::string getPrefix( DbSystem system );
};

//---------------------------------------------------------------------------
#endif
