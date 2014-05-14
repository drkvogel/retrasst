//---------------------------------------------------------------------------

#ifndef LPDbBoxesH
#define LPDbBoxesH

#include <vector>

#include "LDbIdClasses.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LCDbObject;
class LPDbBoxType;
class LCDbBoxSize;

class LPDbBoxName : public LPDbID
{
	std::string name, barcode;
	short status;
	int boxTypeID;
//	const char * event;
	int filledBy;
	std::vector< std::string > cryovials;

	bool needsNewID( LQuery & cQuery ) const;

public:

	enum Status { EMPTY = 0, IN_USE = 1, CONFIRMED = 2, ANALYSED = 3, IN_TANK = 4, DESTROYED = 8, DELETED = 99 };

	LPDbBoxName( const LQuery & query );

	LPDbBoxName( int id = 0 )
	 : LPDbID( id ), status( EMPTY ), boxTypeID( 0 ), filledBy( 0 )
	{}

	LPDbBoxName( const std::string & label, int typeID = 0 )
	 : name( label ), status( IN_USE ), boxTypeID( typeID ), filledBy( 0 )
	{}

	const std::string & getName() const { return name; }
    void setTypeID(int  id) { boxTypeID = id; }
	int getTypeID() const { return boxTypeID; }
	void setFilledBy( int operatorID ) { filledBy = operatorID; }
	int getFilledBy() const { return filledBy; }

	void setStatus( Status update ) { status = update; }
	Status getStatus() const { return Status( status ); }

	const LCDbBoxSize * getLayout() const;

	short getSize() const;
	short getSpace() const;
	bool hasSpace() const;

	void addCryovial( short position, const std::string & barcode );
	short addCryovial( const std::string & barcode );
	const std::vector< std::string > & getCryovials() const { return cryovials; }

	bool create( const LPDbBoxType & type, LQuery pQuery, LQuery cQuery );
	void findSpace( const LPDbBoxType & type, LQuery query );
	bool saveRecord( LQuery & pQuery, LQuery & cQuery );
//	bool addEventRecord( LQuery query, const LCDbObject * event, const std::string & text );
	void addCryovials( LQuery & pq );
	void checkFilledBy( LQuery & pq );

	void confirmAllocation( LQuery pQuery, LQuery cQuery );
	bool matchesGroup( LQuery pQuery );

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
	const LPDbBoxName * readRecord( LQuery pQuery, const std::string & name );
	const LPDbBoxName * find( const std::string & name ) const;
	const LPDbBoxName * findSpace( int boxType ) const;
};

//---------------------------------------------------------------------------

#endif
