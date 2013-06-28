#ifndef DbProjectH
#define DbProjectH

#include "HasDbId.h"
#include "HasName.h"
#include "SortedVector.h"

//---------------------------------------------------------------------------

class DbProject : public HasDbID, public HasName
{
	String database;
	int major, minor;

	static int currentID;

	static SortedVector< DbProject > projects;
	friend SortedVector< DbProject >;

	class Matcher;

	DbProject() {}
    DbProject( TQuery * query );

public:

	void useDatabase( TDatabase* dbComponent ) const;
	const String & getDbName() const { return database; }

	typedef SortedVector< DbProject >::Range Range;

	static Range readAll( TQuery * query );
	static Range getAll() { return projects; }

	static int getCurrentID() { return currentID; }
	static const DbProject & lookup( const String & nameOrDB );
	static const DbProject & lookup( int id ) { return projects.seek( id ); }
	static const DbProject & lookup() { return projects.seek( currentID ); }
};

//---------------------------------------------------------------------------

#endif


