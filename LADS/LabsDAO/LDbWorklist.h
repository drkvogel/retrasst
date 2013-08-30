//---------------------------------------------------------------------------

#ifndef LDbWorklistH
#define LDbWorklistH

#include <set>

#include "LDbStage.h"
#include "LPDbProfile.h"
#include "LDbRange.h"

//---------------------------------------------------------------------------

class LDbWorkEntry : public LDbStage
{
	std::pair< int, int > specimen;
	String barcode;
	int profile, categoryID;
	TDateTime timeStamp, inDate;

protected:

	LDbTestLimits limits;
	int resultID;
	double diluent;

	LDbWorkEntry( const String & id, int proj, int sid, TDateTime taken, int testID )
	 : barcode( id ), specimen( proj, sid ), inDate( taken ), limits( testID ),
	   profile( 0 ), resultID( 0 ), categoryID( 0 ), diluent( 0 )
	{}

	LDbWorkEntry()
	 : specimen( 0, 0 ), profile( 0 ), categoryID( 0 ), limits( 0 ), diluent( 0 ), resultID( 0 )
	{}

public:

	LDbStage::Stage makeStage( char status );
	static const char makeStatus( LDbStage::Stage stage );

	static String listMachineIDs();

	const String & getBarcode() const { return barcode; }
	int getProjectID() const { return specimen.first; }
//	int getSpecimenID() const { return specimen.second; }

	void setSpecimen( std::pair< int, int > sample ) { specimen = sample; }
	const std::pair< int, int > & getSpecimen() const { return specimen; }

	bool isQC() const { return getBarcode().Pos( "QC" ) == 1; }

	int getResultID() const { return resultID; }
	void setResultID( int brid ) { resultID = brid; }

	void setDilution( double ratio ) { diluent = ratio; }
	double getDilution() const { return diluent; }

	int getProfileID() const { return profile; }
	void setProfileID( int tpid ) { profile = tpid; }

	int getCategoryID() const { return categoryID; }
	void setCategoryID( int filter ) { categoryID = filter; }
	bool matchCategory( int filter ) const
	{
		return filter == 0 || categoryID == 0 || filter == categoryID;
	}

	TDateTime getInDate() const { return inDate; }
	TDateTime getTimeStamp() const;
	void setTimeStamp( TDateTime time ) { timeStamp = time; }

	const LDbTestLimits & getTestLimits() const { return limits; }
	void setTestLimits( const LDbTestLimits & tl ) { limits = tl; }
};

//---------------------------------------------------------------------------

class LBDbWorkEntry : public LDbWorkEntry
{
	int duplicate;

	static const int TRIGGER_ID = -2, FIRST_RERUN_ID = -4;
	static int nextRerun;

public:

	LBDbWorkEntry() : duplicate( 0 )
	{}

	LBDbWorkEntry( const LBDbWorkEntry & wle )
	 : LDbWorkEntry( wle ), duplicate( wle.duplicate )
	{}

	LBDbWorkEntry( const LDbWorkEntry & wle )
	 : LDbWorkEntry( wle ), duplicate( 0 )
	{}

	LBDbWorkEntry( const String & tube, int testID, int rowID = 0 )
	 : LDbWorkEntry( tube, 0, 0, Now(), testID ), duplicate( rowID )
	{}

	LBDbWorkEntry( const String & tube, int proj, int sample, int testID, TDateTime taken )
	 : LDbWorkEntry( tube, proj, sample, taken, testID ), duplicate( 0 )
	{}

	explicit LBDbWorkEntry( const LQuery & cluster );

	bool createRecord( LQuery cluster, String reason = "" );
	bool updateRecord( LQuery cluster, LDbWorkEntry::Stage status );

	int getOldID() const { return duplicate; }
};

//---------------------------------------------------------------------------

class LBDbWorkList : public LDbCacheMap< LCDbAnalysers, LBDbWorkList >
{
	std::multimap< String, LBDbWorkEntry > cache;

	static String rootString( const String & pattern );

public:

	typedef LBDbWorkEntry value_type;

	bool read( LQuery cluster, bool readAll );
	bool read( LQuery cluster, const String & barcode );

	bool copyCentral( LQuery cluster, LQuery central );
	bool confirm( LQuery query, std::pair< int, int > sample, LDbStage::Stage stage );

	LBDbWorkEntry * insert( const LBDbWorkEntry & wle );
	LBDbWorkEntry * find( const LBDbWorkEntry & wle );

	std::set< const LBDbWorkEntry * > find( const String & barcode, int testCID ) const;

	MapRange< String, LBDbWorkEntry > getTests() const { return cache; }
	MapRange< String, LBDbWorkEntry > getTests( const String & pattern ) const
	{
		return cache.equal_range( rootString( pattern ) );
	}

	void clearQCEntries( LQuery cluster );
	unsigned deleteOld( LQuery cluster, TDateTime flagged, TDateTime oldest );
	String nextWipe( LQuery cluster );
	bool confirm( LQuery cluster, int project, int specimen );
};

//---------------------------------------------------------------------------

class LCDbWorkEntry : public LDbWorkEntry, public LDbIdBase
{
	int groupID;
	String profileName;
	bool copied;

public:

	static const char NEW_RECORD = 'N', COPIED = 'Y', HELD = 'H', FINISHED = 'T';

	LCDbWorkEntry( const String & barcode, int proj, int sample, int testID, TDateTime taken )
	 : LDbIdBase( 0 ), LDbWorkEntry( barcode, proj, sample, taken, testID ), groupID( 0 ), copied( false )
	{}

	explicit LCDbWorkEntry( const LQuery & central );

	const String & getProfile() const { return profileName; }
	bool buddyRead() const { return copied; }

	int getGroupID() const { return groupID; }
	void setGroupID( int group ) { groupID = group; }

	bool createRecord( LQuery cQuery, const String & reason );
	bool updateStatus( LQuery central, LDbWorkEntry::Stage status );
};

//---------------------------------------------------------------------------

class LCDbWorkList : public LDbCache< LCDbWorkEntry > //, public LDbSingleton< LCDbWorkList >
{
//	TStrings * findRemaining( LQuery qCentral, std::pair< TDateTime, TDateTime > range );

public:

	bool read( LQuery central, bool readAll );
	bool read( LQuery central, const String & barcode );
	bool readPending( LQuery central );

	std::set< const LCDbWorkEntry * > find( const String & barcode, int testCID ) const;
	std::set< int > getAnalyserIDs( LQuery cQuery, int projID = 0 ) const;
	std::set< int > getCategoryIDs( LQuery cQuery, int buddyID = 0 ) const;
	int findGroup( LQuery cQuery, const std::set< int > & sampleIDs ) const;
	int findParentAnalysis( LQuery cQuery, const std::set< int > & recordIDs ) const;

	bool confirm( LQuery cQuery, int groupID );
	unsigned addQCRecords( LQuery qCentral, const String & barcode, int testID = 0 );
	unsigned deleteOld( LQuery qCentral, TDateTime flagged, TDateTime oldest );
};

//---------------------------------------------------------------------------

#endif

