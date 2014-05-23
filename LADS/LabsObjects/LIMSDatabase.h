//---------------------------------------------------------------------------

#ifndef LIMSDatabaseH
#define LIMSDatabaseH

#include <string>
#include <map>
#include <vector>

class ROSETTA;
class XDB;

//---------------------------------------------------------------------------

class LIMSDatabase {

	static std::vector< std::string > errors;

	LIMSDatabase( const std::string & dbName );
	LIMSDatabase & operator =( const LIMSDatabase & other ); // not allowed

	void setErrorCallBacks();

	static void xerrorCallback( const std::string object, const std::string error_txt );
	static bool rosettaCallback( const ROSETTA *r, const int type, const std::string msg );
	static bool xdbErrorCallback( const std::string object, const int instance,
			const int ecount, const int ecode, const std::string error_txt );

public:

	LIMSDatabase( const LIMSDatabase & other );
	~LIMSDatabase();

	enum DbSystem { VLAB_LIVE, VLAB_TEST, LABDEV_MIRROR, LABDEV_DEV, LABDEV_TEST, UNKNOWN = 99 };

	static DbSystem getCurrentSystem() { return current; }
	static void setCurrentSystem( DbSystem system );
	static bool includes( const std::string & dbName );

	static LIMSDatabase getCentralDb();
	static LIMSDatabase getProjectDb( const std::string & dbName, bool distributed = false );
	static LIMSDatabase getProjectDb( int projID = 0 );
	static std::string getConnectionName( const std::string & dbName );

	std::string getDbName() const;
	XDB * connect( bool readLocks = true );
	bool isConnected() const;
	static void confirm( bool success );

private:

	static DbSystem current;

	XDB * xdb;
	std::string name, vnode;
	DbSystem owner;

	static std::string getRootName( const std::string & dbName );
	static std::string getVNode( DbSystem system );
	static std::string getPrefix( DbSystem system );
};

//---------------------------------------------------------------------------
//	Storage policy class – records() returns the current LDbCache
//---------------------------------------------------------------------------

template< typename Values > struct LCDbSingleton
{
	static Values & records() {
		static Values cdb[ 5 ];
		Values & cache = cdb[ LIMSDatabase::getCurrentSystem() ];
		if( cache.empty() ) {
			cache.read( LIMSDatabase::getCentralDb() );
		}
		return cache;
	}
};

//---------------------------------------------------------------------------
#endif
