//---------------------------------------------------------------------------

#ifndef LPDbProfileMapH
#define LPDbProfileMapH

#include <map>
#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"
#include "ValueFields.h"

//---------------------------------------------------------------------------

class LPDbProfileMap : public LPDbID
{
	int testProfile, groupBy, storageCID, analysisCID;
	std::map< int, int > columns;

public:

	LPDbProfileMap( int id = 0 )
	 : LPDbID( id ), testProfile( 0 ), groupBy( 0 ), storageCID( 0 ), analysisCID( 0 )
	{}

	explicit LPDbProfileMap( const LQuery & query );
	void saveRecord( LQuery pQuery );

	int getProfileID() const { return testProfile; }
	void setProfileID( int profile ) { testProfile = profile; }

	int getGroupByID() const { return groupBy; }
	int getAnalysisCID() const { return analysisCID; }
	int getStorageCID() const { return storageCID; }

	void setValueID( int descriptor, int valueID );
	int findValueID( int descripID ) const;
	void remove( int descripID ) { columns.erase( descripID ); }
	bool includesValue( int valueID ) const;

	std::map< int, int > & getDVPairs() { return columns; }
	const std::map< int, int > & getDVPairs() const { return columns; }
	bool matches( const ValueFields & dvPairs ) const;
};

//---------------------------------------------------------------------------

class LPDbProfileMaps : public LDbCache< LPDbProfileMap >, public LPDbCacheMap< LPDbProfileMaps >
{
public:

	bool read( LQuery pQuery, bool readAll = true );
	bool remove( LQuery projectQuery, int pmid );
};

//---------------------------------------------------------------------------

#endif

