//---------------------------------------------------------------------------

#ifndef LDbBoxTypeH
#define LDbBoxTypeH

#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"
#include "LCDbProject.h"

//---------------------------------------------------------------------------

class LPDbBoxType : public LPDbID, public LDbNames
{
	short group, position, status, uses;
	int sizeID;

	static bool hasLocalContent();

	std::vector< int > content;

public:

	enum Expectation { SHORT_TERM, LONG_TERM, SEND_AWAY, DISPOSAL, DELETED = 99 };

	LPDbBoxType( int id )
	 : LPDbID( id ), sizeID( 0 ), group( 0 ), position( 0 ), status( LONG_TERM ), uses( 0 )
	{}

	LPDbBoxType( const LQuery & query );
	bool saveRecord( LQuery pQuery );

	bool isActive() const { return status != DELETED; }
	Expectation getStatus() const { return Expectation( status ); }

	int getSizeID() const { return sizeID; }
	void setSizeID( int cid ) { sizeID = cid; }
	short getLink() const { return group; }
	void setLink( short link ) { group = link; }
	short getPosition() const { return position; }
	void setPosition( short pos ) { position = pos; }
	short getExpectedUses() const { return uses; }
	void setExpectedUses( short count ) { uses = count; }

	typedef Range< int > Aliquots;
	Aliquots getAliquots() const { return content; }
	void setAliquots( Aliquots aliquotTypes );
};

//---------------------------------------------------------------------------

class LPDbBoxTypes : public LDbCache< LPDbBoxType >, public LPDbCacheMap< LPDbBoxTypes >
{
public:
	bool read( LQuery pQuery, bool readAll = false );
	const LPDbBoxType * find( const std::string & name ) const {
		return findMatch( LDbNames::LCMatcher( name ) );
	}
};

//---------------------------------------------------------------------------

#endif

