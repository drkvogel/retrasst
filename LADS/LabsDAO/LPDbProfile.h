//---------------------------------------------------------------------------

#ifndef LPDbProfileH
#define LPDbProfileH

#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"
#include "LDbNotes.h"
#include "TestRange.h"

//---------------------------------------------------------------------------

class LDbTestLimits
{
	int test, analyser, trigger;

protected:

	ResultRange vRange, tRange, wRange;
	bool local;

public:

	LDbTestLimits( int testID, int buddyID = 0 )
	 : test( testID ), analyser( buddyID ), trigger( 0 ), local( false )
	{}

	int getAnalyserID() const { return analyser; }
	void setAnalyserID( int buddyID ) { analyser = buddyID; }

	bool isPrivate() const { return local; }
	void setPrivate( bool pf ) { local = pf; }

	const ResultRange & getRange() const { return vRange; }
	template< typename T > void setRange( const T & low, const T & high )
	{
		vRange = std::pair< T, T >( low, high );
	}

	bool isRangeSet() const { return vRange.isSet(); }
	bool rangeError( const ResultValue & value ) const { return !vRange.includes( value ); }

	const ResultRange & getWarnLevels() const { return wRange; }
	template< typename T > void setWarnLevels( const T & low, const T & high )
	{
		wRange = std::pair< T, T >( low, high );
	}

	bool isWarnSet() const { return wRange.isSet(); }
	bool warns( const ResultValue & value ) const { return !wRange.includes( value ); }

	template< typename T > void setTriggerLimits( int triggerID, const T & low, const T & high )
	{
		trigger = triggerID;
		if( trigger == 0 )
			tRange = ResultRange();
		else
			tRange = std::pair< T, T >( low, high );
	}

	int getTriggerProfile() const { return trigger; }
	const ResultRange & getTriggerLimits() const { return tRange; }
	bool triggers( const ResultValue & value ) const
	{
		return (trigger != 0) && !tRange.includes( value );
	}
};

//---------------------------------------------------------------------------

class LPDbProfile : public LPDbID, public LDbNames, public LDbNoteCount
{
	bool active, runTests, storeSource, takeAliquots;
	TDateTime changed;

public:

	static const int STORE_ALIQUOTS = 1, TEST_SAMPLE = 2, STORE_SAMPLE = 4;

	class BoxType : public LPDbID, public LDbValid
	{
		short position;

		void saveRecord( LQuery pQuery );

	public:

		BoxType( int id = 0 ) : LPDbID( id )
		{}

		BoxType( const LQuery & query );

		bool isAliquot() const { return status == 0; }
		bool isSample() const { return status == 1; }
	};

	class TestLimits : public LPDbID, public LDbTestLimits, public LDbValid
	{
		static const int DEFAULT_LOWER = 1;			// no lower_test_limit specified
		static const int PRIVATE_RESULT = 2;		// result cannot be released to project
		static const int DEFAULT_UPPER = 4;			// no upper_test_limit specified
		static const int NO_LOWER_WARNING = 8;		// no warning for very low values
		static const int NO_UPPER_WARNING = 16;		// no warning for very high values

		friend class LPDbProfile;
		friend class LPDbProfiles;

		TestLimits( const LQuery & pQuery );
		int getFlagBits() const;
	};

	LPDbProfile( int id = 0 ) : LPDbID( id ), LDbNoteCount( 0 ), active( false ),
		runTests( true ), storeSource( true ), takeAliquots( true )
	{}

	LPDbProfile( const LQuery & pQuery );
	void saveRecord( LQuery pQuery );

	int getFlagBits() const;
	TDateTime getTimeStamp() const { return changed; }

	bool isTested() const { return runTests; }
	void setTested( bool t ) { runTests = t; }

	bool isStored() const { return storeSource; }
	void setStored( bool t ) { storeSource = t; }

	bool isAliquoted() const { return takeAliquots; }
	void setAliquoted( bool t ) { takeAliquots = t; }

	Range< BoxType > getBoxTypes() const { return boxTypes; }
	void addBox( BoxType type ) { boxTypes.insert( type ); }
	void addBoxType( LQuery pQuery, int boxTypeID );

	Range< TestLimits > getTests() const { return tests; }
	void addTest( const TestLimits & limits ) { tests.insert( limits ); }

	void addTestLimits( LQuery pQuery, const TestLimits & tpe );
	const LDbTestLimits * findTestLimits( int testID ) const
	{
		return tests.findByID( testID );
	}

	LDbCache< BoxType > boxTypes;
	LDbCache< TestLimits > tests;

	bool validate() const;

	static void setValue( LQuery & query, const std::string & pName, double value );
	static double getValue( LQuery & query, const std::string & pName );
};

//---------------------------------------------------------------------------

class LPDbProfiles : public LDbCache< LPDbProfile >, public LPDbCacheMap< LPDbProfiles >
{
public:
	bool read( LQuery pQuery, bool readAll = false );
	const LPDbProfile * findByName( const std::string & name ) const {
		return findMatch( LDbNames::LCMatcher( name ) );
	}
};

//---------------------------------------------------------------------------

#endif
