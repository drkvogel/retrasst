//---------------------------------------------------------------------------

#ifndef LCDbQCLevelH
#define LCDbQCLevelH

#include "LDbIdClasses.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LCDbQCLevel : public LCDbID
{
	String material, level, description;
	short status;

public:

	class Test : public LCDbID, public LDbValid
	{
		int testID, machineID;
		double diluent;

	public:

		Test( const LQuery & query );
		int getMachineID() const { return machineID; }
		int getTestID() const { return testID; }
		double getDilution() const { return diluent; }
	};


	LCDbQCLevel( const LQuery & query );
	bool saveRecord( LQuery central );

	const String & getMaterial() const { return material; }
	const String & getLevel() const { return level; }
	const String & getDescription() const { return description; }

	void addTest( const LQuery & query )
	{
		tests.insert( Test( query ) );
	}

    Range< Test > getTests() const { return tests; }

private:

	LDbCache< Test > tests;
};

//---------------------------------------------------------------------------

class LCDbQCLevels : public LDbCache< LCDbQCLevel >, public LDbSingleton< LCDbQCLevels >
{
//	class Matcher;

public:

	bool read( LQuery central, bool readAll );
	const LCDbQCLevel * find( const String & material, const String & qcLevel ) const;
};

//---------------------------------------------------------------------------

#endif