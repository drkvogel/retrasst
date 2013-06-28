//---------------------------------------------------------------------------

#ifndef LPDbBoxesH
#define LPDbBoxesH

#include <vector>

#include "LDbIdClasses.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LCDbObject;
class LPDbBoxType;

class LPDbBoxName : public LPDbID
{
	std::string name;
	short status;
	int boxTypeID;
	const char * event;
	int filledBy;
	std::vector< std::string > cryovials;

public:

	enum Status { EMPTY = 0, IN_USE = 1, CONFIRMED = 2, ANALYSED = 3, IN_TANK = 4,
				  DESTROYED = 8, DELETED = 99 };

	LPDbBoxName( const LQuery & query );

	LPDbBoxName( int id = 0 )
	 : LPDbID( id ), status( EMPTY ), boxTypeID( 0 ), filledBy( 0 ), event( NULL )
	{}

	LPDbBoxName( const std::string & label, int typeID = 0 )
	 : status( IN_USE ), name( label ), boxTypeID( typeID ), filledBy( 0 ), event( NULL )
	{}

	const std::string & getName() const { return name; }
	int getTypeID() const { return boxTypeID; }
	void setFilledBy( int operatorID ) { filledBy = operatorID; }
	int getFilledBy() const { return filledBy; }

	void setStatus( Status update ) { status = update; }
	Status getStatus() const { return Status( status ); }

	short getSize() const;
	short getSpace() const { return getSize() - cryovials.size(); }
	bool hasSpace() const { return short(cryovials.size()) < getSize(); }
	short nextSpace();

	void addCryovial( short position, const std::string & barcode );
	const std::vector< std::string > & getCryovials() const { return cryovials; }

	void create( const LPDbBoxType & type, LQuery query );
	void findSpace( const LPDbBoxType & type, LQuery query );
	bool saveRecord( LQuery query );
	bool addEventRecord( LQuery query, const LCDbObject * event, const std::string & text );
	void addCryovials( LQuery & pq );
	void checkFilledBy( LQuery & pq );

	void confirmAllocation( LQuery pQuery );
	bool matchesGroup( LQuery projectQuery );

	static const LCDbObject * findEvent( const char * eventName );
};

//---------------------------------------------------------------------------

class LPDbBoxNames : public LDbCache< LPDbBoxName >
{
//	class NameMatcher;
//	class SpaceMatcher;

public:

	bool readCurrent( LQuery pq );
	bool readFilled( LQuery ddq );
	void addCryovials( LQuery pq );

	const LPDbBoxName * readRecord( LQuery pQuery, int id );
	const LPDbBoxName * readRecord( LQuery pQuery, std::string name );
	const LPDbBoxName * find( const std::string & name ) const;
	const LPDbBoxName * findSpace( int boxType ) const;
};

//---------------------------------------------------------------------------

#endif
