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
	int projectCID;
	std::vector< std::string > cryovials;

	std::string createName( const std::string & type ) const;
	bool needsNewID( LQuery & cQuery ) const;
	bool update( bool central, LQuery & query );
	bool insert( bool central, LQuery & query );

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
	void setProjectCID( int cid ) { projectCID = cid; }
	int getProjectCID() const { return projectCID; }

	void setStatus( Status update ) { status = update; }
	Status getStatus() const { return Status( status ); }

	const LPDbBoxType * getContent() const;
	const LCDbBoxSize * getLayout() const;
	short getSpace() const;
	bool hasSpace() const;

	void addCryovial( short position, const std::string & barcode );
	short addCryovial( const std::string & barcode );
	const std::vector< std::string > & getCryovials() const { return cryovials; }

	bool create( const LPDbBoxType & type, short boxSet, LQuery pQuery, LQuery cQuery );
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
	class NameMatcher;
	class SpaceMatcher;

public:
	bool readFilled( LQuery pQuery );
	void addCryovials( LQuery pQuery );

	const LPDbBoxName * find( const std::string & name ) const;
	const LPDbBoxName * findSpace( int boxType ) const;

	bool readCurrent( LQuery central );
	const LPDbBoxName * readRecord( LQuery central, int id );
	const LPDbBoxName * readRecord( LQuery central, const std::string & name );
};

//---------------------------------------------------------------------------

#endif
