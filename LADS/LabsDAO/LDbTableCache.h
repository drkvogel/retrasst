//---------------------------------------------------------------------------

#ifndef LDbTableCacheH
#define LDbTableCacheH

#include <algorithm>

#include "LDbCacheBase.h"

//---------------------------------------------------------------------------
//	Storage policy classes – records() returns the (current) LDbCache
//---------------------------------------------------------------------------

template< typename Record > class LDbStaticCache
{
protected:

	typedef Record RecType;

	static LDbCache< Record > & records()
	{
		static LDbCache< Record > shared;
		return shared;
	}
};

//---------------------------------------------------------------------------

template< typename Record > class LDbCacheMap
{
protected:

	typedef Record RecType;

	static LDbCache< Record > & load( int project )
	{
		static std::map< int, LDbCache< Record > > shared;
		return shared[ project ];
	}

	static LDbCache< Record > & records()
	{
		return load( LCDbProjects::getCurrentID() );
	}
};

//---------------------------------------------------------------------------
//	The shared base class used to create a static database or XML cache
//---------------------------------------------------------------------------

template< typename Store > struct LDbTableCache : public Store
{
	typedef LDbCache< typename Store::RecType >::Range Records;
	static Records getAll() { return records(); }

	static void clear()
	{
		records().clear();
	}

	static bool includes( int id )
	{
		return records().findByID( id ) != NULL;
	}

	static const Store::RecType & get( int id )
	{
		const Store::RecType * found = records().findByID( id );
		if( found == NULL )
			throw Exception( records().getType() + " " + id + " not found" );
		return *found;
	}

	static void copy( const Store::RecType & item )
	{
		records().append( item );
	}
};

//---------------------------------------------------------------------------
//	A template for a static cache of records from the central database
//---------------------------------------------------------------------------

template< typename RecType > class LCDbCache : public LDbTableCache< LDbStaticCache< RecType > >
{
	static int currentID;

protected:

	static void read( LQuery & central, bool readAll );
	static void setCurrentID( int id ) { currentID = id; }

public:

	static int getCurrentID() { return currentID; }
	static void readCurrent( LQuery central ) { read( central, false ); }
	static void readAll( LQuery central ) { read( central, true ); }
};

//---------------------------------------------------------------------------
//	A simple template for a static cache of cluster controller records
//---------------------------------------------------------------------------

template< typename RecType > struct LBDbCache : public LDbTableCache< LDbStaticCache< RecType > >
{
	static void readAll( LQuery cluster );
	static void readCurrent( LQuery cluster );
};

//---------------------------------------------------------------------------
//	A template for a cache of records from all the current projects
//---------------------------------------------------------------------------

template< typename RecType > struct LPDbCache : public LDbTableCache< LDbCacheMap< RecType > >
{
	static void readCurrent( LQuery pQuery );
	static void readAll( LQuery pQuery )
	{
		for( LCDbProjects::Records p = LCDbProjects::getAll(); p.isValid(); ++ p )
		{
			p -> claimDatabase( pQuery.getDatabase() );
			readCurrent( pQuery );
		}
	}
};

//---------------------------------------------------------------------------
//	A template for a cache of records read from or written to a file
//---------------------------------------------------------------------------

template< typename RecType > class LXDbCache : public LDbTableCache< LDbStaticCache< RecType > >
{
	static void fillRecords( XMLTokens & xml )
	{
		LDbCache< RecType > & cache = records();
		cache.clear();

		//	<data> <history>...</history> <records>...</records> </data>

		XMLTokens::Tag root = xml.currentTag();
		xml.checkHeader();

		XMLTokens::Tag section = xml.nextTag();
		while( xml.nextTag() != section.endTag() )
			cache.push_back( RecType( xml.parseFields() ) );

		if( xml.nextTag() != root.endTag() )
			throw XMLTokens::EInvalidFormat();
	}

public:

	static void readFile( const String & fileName )
	{
		std::auto_ptr< TStrings > file( new TStringList );
		file -> LoadFromFile( fileName );
		fillRecords( XMLTokens( file -> Text ) );
	}

	static void writeFile( const String & filename )
	{
		LDbCache< RecType > & cache = records();
		XMLFile output( "data", false, "", filename );
		output.addHeader();
		output.start( "records" );
		for( LDbCache< RecType >::iterator ri = cache.begin(); ri != cache.end(); ++ ri )
			ri -> writeRecord( output );
		output.endTag();
	}
};

//---------------------------------------------------------------------------

#endif
