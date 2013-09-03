//---------------------------------------------------------------------------

#ifndef LDbIdClassesH
#define LDbIdClassesH

#include <string>
#include "LQuery.h"

//---------------------------------------------------------------------------

class LDbIdBase
{
	int id;

protected:

	bool saved;

	LDbIdBase( int key ) : id( key ), saved( key != 0 )
	{}

	int readID( LQuery & query, std::string sequence );

public:

	int getID() const { return id; }
	void setID( int key );
	bool exists() const { return saved; }

	bool operator<( int otherID ) const { return id < otherID; }
	friend bool operator<( int pID, const LDbIdBase & other ) { return pID < other.id; }
	bool operator<( const LDbIdBase & other ) const { return id < other.id; }
};

//---------------------------------------------------------------------------

struct LCDbID : public LDbIdBase
{
	LCDbID( int id = 0 ) : LDbIdBase( id ) {}
	int claimNextID( LQuery & query );
};

//---------------------------------------------------------------------------

struct LPDbID : public LDbIdBase
{
	LPDbID( int id = 0 ) : LDbIdBase( id ) {}
	int claimNextID( LQuery & query );
};

//---------------------------------------------------------------------------

struct LBDbID : public LDbIdBase
{
	LBDbID( int id = 0 ) : LDbIdBase( id ) {}
	int claimNextID( LQuery & query );
};

//---------------------------------------------------------------------------

#endif

