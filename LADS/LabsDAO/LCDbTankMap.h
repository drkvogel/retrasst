//---------------------------------------------------------------------------

#ifndef LCDbTankMapH
#define LCDbTankMapH
//---------------------------------------------------------------------------
#include "LSDbBox.h"
#include "LDbIdClasses.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

class LCDbTankMap : public LCDbID, public LDbValid
{
	int tankCID, storageCID, layoutCID, locationCID, storeTypeCID;
	short position, shelf;

public:

	static const short OFFLINE = 5;  	// extends status

	LCDbTankMap()
	 : tankCID(0),storageCID(0),layoutCID(0),locationCID(0),position(0),storeTypeCID(0),shelf(0)
	{}

	LCDbTankMap( TDateTime from, TDateTime to, short rs ) : LDbValid( from, to, rs ),
	   tankCID(0),storageCID(0),layoutCID(0),locationCID(0),position(0),storeTypeCID(0),shelf(0)
	{}

	explicit LCDbTankMap( const LQuery & query );

	int getTankCID() const { return tankCID; }
	int getStorageCID() const { return storageCID; }
	int getStoreTypeCID() const { return storeTypeCID; }
	int getLayoutCID() const { return layoutCID; }
	int getLocationCID() const { return locationCID; }
	short getPosition() const { return position; }
	short getPopulation() const { return shelf; }
	short getStatus() const { return status; }

	void setTankCID( int id ) { tankCID = id; }
	void setStorageCID( int id ) { storageCID = id; }
	void setStoreTypeCID( int id ) { storeTypeCID = id; }
	void setLayoutCID( int id ) { layoutCID = id; }
	void setLocationCID( int id ) { locationCID = id; }
	void setPosition( short pos ) { position = pos; };
	void setPopulation( short num ) { shelf = num; };
	void setStatus( short p_st ) { status = p_st; };

	bool saveRecord( LQuery central );
};

//---------------------------------------------------------------------------

class LCDbTankMaps : public LDbCache< LCDbTankMap >, public LDbSingleton< LCDbTankMaps >
{
	class Matcher;

public:

//	bool readCurrent( LQuery query );
	bool read( LQuery central, bool readAll );
	const LCDbTankMap * find( int pTankCID, int pStorageCID = 0 ) const;
};

//---------------------------------------------------------------------------
#endif
