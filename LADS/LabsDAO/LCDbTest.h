//---------------------------------------------------------------------------

#ifndef LCDbTestH
#define LCDbTestH

#include <set>

#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"
#include "TestRange.h"

//---------------------------------------------------------------------------

class LCDbTest : public LCDbID, public LDbNames, public LDbValid
{
	ResultRange limits;
	std::string sampleType;   		/// obsolete - moved to c_test_machine
	short precision, dataType;

	static const REAL_DATA = 4, STRING_DATA = 8, REQUIRED_DATA_POINTS = 1;
	static const short ANALYSED = 1, CALCULATED = 2, AS_NOTE = 4;

public:

	LCDbTest( int id = 0 ) : LCDbID( id ), precision( 2 ), dataType( REAL_DATA )
	{}

	LCDbTest( const LQuery & query );
	bool saveRecord( LQuery central );

	bool keepAsNote() const { return dataType != REAL_DATA || (status & AS_NOTE) != 0; }

	std::pair< short, short > getPointCount() const
	{
		return std::pair< short, short >( REQUIRED_DATA_POINTS, REQUIRED_DATA_POINTS );
	}

	template< typename T > void setLimits( T low, T high )
	{
		limits = std::pair< T, T >( low, high );
	}

	const ResultRange & getLimits() const { return limits; }

	class CalcDef : public LCDbID, public LDbValid
	{
		short calc;
		int sourceTest;
		double multiple;

	public:

		enum Operation { SUBTRACTION, DIVISION, ADDITION, MULTIPLICATION };

		CalcDef() : calc( -1 ), sourceTest( 0 ), multiple( 0 )
		{}

		CalcDef( short operation, double mul, int test )
		 : calc( operation ), sourceTest( test ), multiple( mul )
		{}

		CalcDef( const LQuery & query );
		bool saveRecord( LQuery central, short part, int testID );

		Operation getCalculation() const { return Operation( calc ); }
		double getMultiple() const { return multiple; }
		int getSourceTestID() const { return sourceTest; }
	};

	void addCalcDef( const LQuery & defs );

	bool isCalculated() const { return !calculations.empty(); }
	Range< CalcDef > getCalcParts() const { return calculations; }
	void saveCalcParts( LQuery query, Range< CalcDef > parts );

	class MachineDef : public LCDbID, public LDbValid
	{
		int analyser;
		short protocol, testOrder;
		std::string code, sampleType;

	public:

		MachineDef( int id = 0 )
		 : LCDbID( id ), analyser( 0 ), protocol( 0 ), testOrder( 0 )
		{}

		MachineDef( int machine, const std::string & mName, short type, short order )
		 : analyser( machine ), code( mName ), protocol( type ), testOrder( order )
		{}

		MachineDef( const LQuery & query );

		int getMachineID() const { return analyser; }
		const std::string & getCode() const { return code; }
		short getProtocol() const { return protocol; }
		short getTestOrder() const { return testOrder; }

		const std::string & getSampleType() const { return sampleType; }
		void setSampleType( const std::string & type ) { sampleType = type; }
	};

	Range< MachineDef > getCodes() const { return machineDefs; }

	void addMachineDef( const LQuery & defs );
	const MachineDef * findMachineDef( int buddyID ) const;
	bool isConfigured( int machineID ) const;
	String findTestCode( const std::set< int > & machines, short protocol ) const;

	void saveMachine( LQuery query, MachineDef definition );
	bool deleteMachine( LQuery query, int machineCID );

private:

	LDbCache< MachineDef > machineDefs;
	LDbCache< CalcDef > calculations;
};

//---------------------------------------------------------------------------

class LCDbTests : public LDbCache< LCDbTest >, public LDbSingleton< LCDbTests >
{

public:

	bool read( LQuery central, bool readAll );

	const LCDbTest * findByName( const std::string & externalName ) const {
		return findMatch( LDbNames::LCMatcher( externalName ) );
	}

	int findTestID( int buddyID, const String & testCode, const String & fluidCode = "" ) const;
	String findSampleType( int buddyID, std::set< int > testIDs ) const;
};

//---------------------------------------------------------------------------

#endif

