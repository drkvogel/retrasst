//---------------------------------------------------------------------------

#ifndef LDbBoxTypeH
#define LDbBoxTypeH

#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"
#include "LCDbProject.h"

//---------------------------------------------------------------------------

class LPDbBoxType : public LCDbID, public LDbNames
{
	short group, position, status, uses;
	int sizeID, projectCID;
	std::vector< int > content;

	static bool hasLocalContent();
	bool update( bool central, LQuery & query );
	bool insert( bool central, LQuery & query );

public:

	enum Expectation { SHORT_TERM, LONG_TERM, ANALYSIS, DISPOSAL, DELETED = 99 };

	LPDbBoxType( int id = 0 )
	 : LCDbID( id ), group( 0 ), position( 0 ), status( LONG_TERM ), uses( 0 ), sizeID( 0 ), projectCID( 0 )
	{}

	LPDbBoxType( const LQuery & cQuery );
	bool saveRecord( LQuery pQuery, LQuery cQuery );

	bool isActive() const { return status != DELETED; }
	Expectation getUse() const { return Expectation( status ); }
	void setUse( Expectation ex ) { status = short( ex ); }
	int getProjectCID() const { return projectCID; }
	void setProjectCID( int cid ) { projectCID = cid; }
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
	class NameMatcher;

public:
	bool read( LQuery cQuery, bool readAll = false );
	const LPDbBoxType * find( const std::string & name ) const;
};

//---------------------------------------------------------------------------

#endif

