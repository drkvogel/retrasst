//---------------------------------------------------------------------------

#ifndef LDbBoxSizeH
#define LDbBoxSizeH

#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LCDbBoxSize : public LCDbID, public LDbValid
{
	std::string name;
	short size, hole, tube;
	int tubeType;

	static bool hasValidFlag( LQuery cQuery );

public:

	LCDbBoxSize( int id = 0 )
	 : LCDbID( id ), size( 0 ), hole( 0 ), tube( 0 ), tubeType( 0 )
	{}

	LCDbBoxSize( const LQuery & central );
	bool saveRecord( LQuery central );

	const std::string & getName() const { return name; }
	void setName( const std::string & desc ) { name = desc; }

	short getHole() const { return hole; }
	void setHole( short empty ) { hole = empty; }

	short getFirst() const { return short( 1 ); }
	short getLast() const { return size; }
	void setLast( short last ) { size = last; }

	short getCapacity() const { return hole == 0 ? size : size - 1; }
	int getTubeType() const { return tubeType; }
};

//---------------------------------------------------------------------------

class LCDbBoxSizes : public LDbCache< LCDbBoxSize >, public LCDbSingleton< LCDbBoxSizes >
{
public:

	bool read( LQuery cQuery, bool readAll = true );
	const LCDbBoxSize * find( const std::string & description ) const;
};

//---------------------------------------------------------------------------

#endif

