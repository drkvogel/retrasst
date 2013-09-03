/*---------------------------------------------------------------------------
 *
 *	Modified 16 June 2005: 		don't throw exception if cache is empty
 *		14 March 2006, NG:      cache includes rather than extends vector
 *		18 September 2008:		add default constructor for an empty Range
 *		16 October 08, NG:		Split out from LDbCache to use stand-alone
 *      18 June 2009, NG:		Added LPDbCacheMap extending LDbCacheMap
 *      29 Sept 2010, NG:		Adapted to work with Filter template
 *      27 April 2012, NG:		Allow out-of-order insertion by readData
 *		2 September 2013, NG:	File cache from database on first use
 *
 *--------------------------------------------------------------------------*/

#ifndef LDbCacheBaseH
#define LDbCacheBaseH

#include <vector>
#include <map>
#include <algorithm>
#include <typeinfo>

#include "LDbRange.h"
#include "XMLFile.h"
#include "LQuery.h"
#include "rosetta.h"

//---------------------------------------------------------------------------
//	A sorted container used to cache LIMS database/XML records
//---------------------------------------------------------------------------

template< typename T > class LDbCache : public std::vector< T >
{

protected:

	typedef typename std::vector< T > Vector;
	typedef typename Vector::const_iterator ConstIter;
	typedef typename std::pair< ConstIter, ConstIter > ConstIterPair;
	typedef typename Vector::iterator Iterator;
	typedef typename std::pair< Iterator, Iterator > IteratorPair;

	T * find( int key ) {
		IteratorPair range = std::equal_range( Vector::begin(), Vector::end(), key );
		return range.first == range.second ? NULL : &(*range.first);
	}

	const T * find( int key ) const {
		ConstIterPair range = std::equal_range( Vector::begin(), Vector::end(), key );
		return range.first == range.second ? NULL : &(*range.first);
	}

	void logError( std::string value, const char * tag ) const {
		if( value.empty() ) {
			value = "\" \"";
		}
		std::string type = typeid( T ).name();
		XMLFile::logError( tag, type + ": " + value );
	}

	bool readData( LQuery & query ) {
		Vector::clear();
		for( query.open(); !query.eof(); query.next() ) {
			T record( query );
			if( Vector::empty() || record.getID() > Vector::back().getID() ) {
				Vector::push_back( record );
			} else {
				IteratorPair range = std::equal_range( Vector::begin(), Vector::end(), record.getID() );
				if( range.first == range.second ) {
					std::vector< T >::insert( range.first, record );
				} else {
					String type = typeid( T ).name();
					throw Exception( type + " does not have unique IDs" );
				}
			}
		}
		return !Vector::empty();
	}

	template< typename O > const T * findMatch( const O & matcher ) const {
		const T * found = NULL;
		short n = 0;
		for( ConstIter i = Vector::begin(); i != Vector::end(); ++ i ) {
			if( matcher( *i ) ) {
				found = &(*i);
				n ++;
			}
		}
		switch( n ) {
			case 0:
				logError( matcher, "not-found" );
				return NULL;
			case 1:
				return found;
			default:
				logError( matcher, "duplicate-values" );
				return NULL;
		}
	}

public:

	typedef T value_type;

	const T * insert( const T & rec ) {
		if( Vector::empty() || Vector::back() < rec ) {
			Vector::push_back( rec );
			return &(Vector::back());
		}
		IteratorPair range = std::equal_range( Vector::begin(), Vector::end(), rec );
		if( range.first != range.second ) {
			*(range.first) = rec;
			return &(*range.first);
		} else {
			Iterator position = std::vector< T >::insert( range.first, rec );
			return &(*position);
		}
	}

	void erase( const T & rec ) {
		IteratorPair range = std::equal_range( Vector::begin(), Vector::end(), rec );
		if( range.first != range.second )
			std::vector< T >::erase( range.first, range.second );
	}

	const T * findByID( int key ) const {
		const T * found = find( key );
		if( found == NULL && key != 0 ) {
			char buff[ 12 ];
			std::sprintf( buff, "%d", key );
			logError( buff, "not-found" );
		}
		return found;
	}

	const T & get( int id ) const {
		const T * found = find( id );
		if( found == NULL ) {
			String type = typeid( T ).name();
			throw Exception( type + " " + id + " not found" );
		}
		return *found;
	}
};

//---------------------------------------------------------------------------
//	Storage policy classes – records() returns the (current) LDbCache
//---------------------------------------------------------------------------

template< typename Values > struct LCDbSingleton
{
	static Values & records() {
		static Values cache;
		if( cache.empty() ) {
			cache.read( LIMSDatabase::getCentralDb() );
		}
		return cache;
	}
};

//---------------------------------------------------------------------------

template< typename Values > struct LPDbCacheMap
{
	static Values & records( int key ) {
		static std::map< int, Values > shared;
		Values & cache = shared[ key ];
		if( cache.empty() ) {
			cache.read( LIMSDatabase::getProjectDb() );
		}
		return cache;
	}

	static Values & records() {
		return records( LCDbProjects::getCurrentID() );
	}
};

//---------------------------------------------------------------------------

#endif

