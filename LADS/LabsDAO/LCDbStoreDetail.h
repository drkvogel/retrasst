//---------------------------------------------------------------------------

#ifndef LCDbStoreDetailH
#define LCDbStoreDetailH

#include "LSDbBox.h"
#include "LDbIdClasses.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

class LCDbStorageDetail : public LCDbID
{
	int storageType;
	int locationCID;
	short position;
	std::string serialNumber;
	short status;
	TDateTime timeStamp;
	std::string physDesc;

public:

	LCDbStorageDetail()
	 : LCDbID( 0 ),storageType(0),locationCID(0),position(0),status(0)
	{}

	explicit LCDbStorageDetail( const LQuery & query );

	int getStorageCID() const { return getID(); }
	int getStorageType() const { return storageType; }
	int getLocationCID() const { return locationCID; }
	int getPosition() const {return position; }
	const std::string & getSerialNumber() const { return serialNumber; }
	bool isActive() const { return status != LDbValid::DELETED; }
	TDateTime getTimeStamp() const {return timeStamp; }
	const std::string & getDescription() const { return physDesc; }

	void setStorageType( int st ) { storageType = st; }
	void setLocation( int id ) { locationCID = id; }
	void setPosition( short pos ) { position = pos; }
	void setSerialNum( std::string srl ){ serialNumber = srl; }
	void setStatus( short st ) { status = st; }
	void setTimestamp( TDateTime ts ){ timeStamp = ts; }
	void setDescription( std::string ps ){ physDesc = ps; }

	bool saveRecord( LQuery query );
};
//---------------------------------------------------------------------------

class LCDbStorageDetails : public LDbCache< LCDbStorageDetail >, public LDbSingleton< LCDbStorageDetails >
{
	class Matcher;

public:

	bool read( LQuery central, bool readAll );
	const LCDbStorageDetail * find( const std::string & serial ) const;
};

//---------------------------------------------------------------------------

#endif
