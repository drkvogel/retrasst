//---------------------------------------------------------------------------

#ifndef LDbIdClassesH
#define LDbIdClassesH

#include "LQuery.h"

//---------------------------------------------------------------------------

class LDbIdBase
{
	int id;

protected:

	bool saved;

	LDbIdBase( int key ) : id( key ), saved( key != 0 )
	{}

public:

	bool exists() const { return saved; }

	int getID() const { return id; }
	void setID( int key );

	bool operator<( int otherID ) const { return id < otherID; }
	friend bool operator<( int id, const LDbIdBase & other ) { return id < other.id; }
	bool operator<( const LDbIdBase & other ) const { return id < other.id; }
};

//---------------------------------------------------------------------------

class LDbNextID
{
	static int dbChanges;

	int nextId, remaining, changeStamp;

public:

	static void clearAll();

	LDbNextID();
	void setBlock( int first, int available );
	int getBlock( int size );

};

//---------------------------------------------------------------------------

class LDbAlloc : public LDbIdBase
{
	virtual int readIDs( LQuery & query ) = 0;
	virtual LDbNextID & getCache() const = 0;
	void waitForIDs( LQuery & query, int required );

protected:

	LDbAlloc( int id ) : LDbIdBase( id )
	{}

public:

	int claimNextID( LQuery & query, int block = 1 );
};

//---------------------------------------------------------------------------

class LCDbID : public LDbAlloc
{
	LDbNextID & getCache() const;
	int readIDs( LQuery & query );

public:

	LCDbID( int id = 0 ) : LDbAlloc( id )
	{}
};

//---------------------------------------------------------------------------

class LPDbID : public LDbAlloc
{
	LDbNextID & getCache() const;
	int readIDs( LQuery & query );

public:

	LPDbID( int id = 0 ) : LDbAlloc( id )
	{}
};

//---------------------------------------------------------------------------

class LBDbID : public LDbAlloc
{
	LDbNextID & getCache() const;
	int readIDs( LQuery & query );

public:

	LBDbID( int id = 0 ) : LDbAlloc( id )
	{}
};

//---------------------------------------------------------------------------

#endif

