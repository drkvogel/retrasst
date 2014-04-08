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
	std::vector< int > content;

	static bool hasLocalContent();

public:

	enum Expectation { SHORT_TERM, LONG_TERM, ANALYSIS, DISPOSAL, DELETED = 99 };

	LPDbBoxType( int id = 0 )
	 : LPDbID( id ), group( 0 ), position( 0 ), status( LONG_TERM ), uses( 0 ), sizeID( 0 )
	{}

	LPDbBoxType( const LQuery & cQuery );
	bool saveRecord( LQuery cQuery );

	bool isActive() const { return status != DELETED; }
	Expectation getUse() const { return Expectation( status ); }
	void setUse( Expectation ex ) { status = short( ex ); }
	int getSizeID() const { return sizeID; }
	void setSizeID( int cid ) { sizeID = cid; }
	short getLink() const { return group; }
	void setLink( short link ) { group = link; }
	short getPosition() const { return position; }
	void setPosition( short pos ) { position = pos; }
	short getExpectedUses() const { return uses; }
	void setExpectedUses( short count ) { uses = count; }
	const std::vector< int > & getAliquots() const { return content; }
	bool hasAliquot( int atid ) const;
	void setAliquots( const std::vector< int > & aliquotTypes );
};

//---------------------------------------------------------------------------

class LPDbBoxTypes : public LDbCache< LPDbBoxType >, public LCDbSingleton< LPDbBoxTypes >
{
public:
	bool read( LQuery cQuery, bool readAll = false );
	const LPDbBoxType * find( const std::string & name ) const {
		return findMatch( LDbNames::LCMatcher( name ) );
	}
};

//---------------------------------------------------------------------------

#endif

