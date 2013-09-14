//---------------------------------------------------------------------------

#ifndef LCDbObjectH
#define LCDbObjectH

#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LCDbObject : public LCDbID, public LDbNames
{
	short objType, status;
	int process;
	TDateTime when;

public:

	enum Category { UNKNOWN, CALIBRANT, REAGENT, ANALYSER_EVENT, SIS_EVENT, CLUSTER = 5,
					ALIQUOT_TYPE, STORAGE_TYPE, STORAGE_SITE, STORAGE_POPULATION, TANK_LAYOUT = 10,
					PROGRAM_NAME, SAMPLE_CATEGORY, RESULT_ATTRIBUTE, LAB_NAME, QC_RULE = 15,
					STORAGE_VESSEL, QC_MATERIAL, TUBE_TYPE, CANNED_TEXT, STORAGE_EXERCISE = 20,
					NUM_TYPES };

	LCDbObject( Category type = UNKNOWN )
	 : objType( type ), status( LDbValid::NEW_RECORD )
	{}

	LCDbObject( const LQuery & query );

	Category getObjectType() const { return Category( objType ); }

	bool isActive() const { return status != LDbValid::DELETED; }
	void setActive( bool live )
	{
		status = live ? LDbValid::RECORD_IN_USE : LDbValid::DELETED;
	}

	bool makeLogEntry( LQuery cq, int otherCID, int testCID = 0 ) const;
	bool saveRecord( LQuery central );

	static std::string findDescription( Category type );
	static Category findObjectType( const std::string & description );
	static Category findObjectType( int type )
	{
		return (type <= UNKNOWN || type >= NUM_TYPES) ? UNKNOWN : Category( type );
	}
};

//---------------------------------------------------------------------------

class LCDbObjects : public LDbCache< LCDbObject >, public LCDbSingleton< LCDbObjects >
{

public:
	bool read( LQuery central, bool readAll = false );
	const LCDbObject * findByName( const std::string & name ) const;
	const LCDbObject * find( const std::string & name, LCDbObject::Category type ) const;
};

//---------------------------------------------------------------------------

#endif


