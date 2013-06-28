//---------------------------------------------------------------------------

#ifndef LDbQCLotH
#define LDbQCLotH

#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LDbQCValues : public LDbIdBase
{
	double mean, stddev;

public:

	LDbQCValues( int id = 0 ) : LDbIdBase( id ), mean( 0 ), stddev( 0 )
	{}

	LDbQCValues( int source, double avg, double sd )
	 : LDbIdBase( source ), mean( avg ), stddev( sd )
	{}

	double getMean() const { return mean; }
	void setMean( double avg ) { mean = avg; }
	double getSD() const { return stddev; }
	void setSD( double sd ) { stddev = sd; }
};

//---------------------------------------------------------------------------

class LDbQCLot : public LDbNames
{
	String material, level;
	bool finished;

protected:

	LDbQCLot() : finished( false ) {};

	LDbQCLot( const String & lot, const String & lev, const String & mat,
			  const String & description, bool old )
	 : LDbNames( lot, description ), material( mat ), level( lev ), finished( old )
	{}

public:

	const String & getMaterial() const { return material; }
	const String & getLevel() const { return level; }
	String getPrefix() const { return "QC" + getMaterial() + getLevel() + getName(); }

	void setFinished( bool value ) { finished = value; }
	bool isFinished() const { return finished; }
};

//---------------------------------------------------------------------------

class LCDbQCLot : public LCDbID, public LDbQCLot
{
	struct Test : public LDbQCValues
	{
		Test() {}
		Test( const LQuery & query );
	};
public:
	LDbCache< Test > tests;


	
	enum RecordStatus { NEW_RECORD = 0, IN_USE = 1, DELETED = 99 };

	LCDbQCLot() : LCDbID( 0 ) {}
	explicit LCDbQCLot( const LQuery & cQuery );

	typedef Range<Test> Tests;
	Tests getTests() const { return tests; }

	void addQCGate( LQuery qCentral, const LDbQCValues & values ) const;
};

//---------------------------------------------------------------------------

class LCDbQCLots : public LDbCache< LCDbQCLot > // , public LDbSingleton< LCDbQCLots >
{
	class LotMatch;

public:

	bool read( LQuery qCentral, bool readAll );
	const LCDbQCLot * find( const LDbQCLot & qc ) const;
};

//---------------------------------------------------------------------------

class LBDbQCTest : public LBDbID, public LDbQCLot
{
	LDbQCValues assigned, machine;
	int results;
	bool useCalc, wgCheck;

	class AddDiffSq;

public:

	explicit LBDbQCTest( const LQuery & bq );

	LBDbQCTest() : LBDbID( 0 ), useCalc( false ), wgCheck( false ), results( 0 )
	{}

	LBDbQCTest( const LDbQCLot & lot, const LDbQCValues & test, int machineCID )
	 : LDbQCLot( lot ), assigned( test ), machine( machineCID ),
	   LBDbID( 0 ), useCalc( false ), wgCheck( false ), results( 0 )
	{}

	LDbQCValues & getAssigned() { return assigned; }
	const LDbQCValues & getAssigned() const { return assigned; }
	LDbQCValues & getCalculated() { return machine; }
	const LDbQCValues & getCalculated() const { return machine; }

	void setCalc( bool value ) { useCalc = value; }
	bool isCalc() const { return useCalc; }
	void setWestgardCheck( bool value ) { wgCheck = value; }
	bool isWestgardCheck() const { return wgCheck; }

	bool checkResults( LQuery buddyQuery );
	bool saveRecord( LQuery buddyQuery );
};

//---------------------------------------------------------------------------

class LBDbQCTests : public LDbCache< LBDbQCTest > // , public LDbSingleton< LBDbQCTests >
{
//	class Matcher;

public:

	void read( LQuery query );
	const LBDbQCTest * find( const String & barcode, int test ) const;
	const LBDbQCTest * find( const LDbQCLot & qc, int test ) const;
};

//---------------------------------------------------------------------------

#endif
