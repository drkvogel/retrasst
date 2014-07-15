/*---------------------------------------------------------------------------
 *
 *	Modified 16 June 2005: 		don't throw exception if cache is empty
 *		14 March 2006, NG:      cache includes rather than extends vector
 *		18 September 2008:		add default constructor for an empty Range
 *		16 October 08, NG:		Split out from LDbCache to use stand-alone
 *      18 June 2009, NG:		Added LPDbCacheMap extending LDbCacheMap
 *      29 Sept 2010, NG:		Adapted to work with Filter template
 *      27 April 2012, NG:		Allow out-of-order insertion by readData
 *		2 September 13, NG:		Fill cache from database on first use
 *      19 March 2014, NG:		Added begin() and end() for range-based for
 *      31 March 2014, NG:		Use LogFile in place of XMLFile for errors
 *
 *--------------------------------------------------------------------------*/

#ifndef LDbCacheBaseH
#define LDbCacheBaseH

#include <vector>
#include <algorithm>
#include <typeinfo>
#include <SysUtils.hpp>
#include "LQuery.h"
#include "LogFile.h"

//---------------------------------------------------------------------------
//	A sorted container used to cache LIMS database/XML records
//---------------------------------------------------------------------------

template< typename T > class LDbCache
{
	std::vector< T > data;

protected:
	typedef typename std::vector< T > Vector;
	typedef typename Vector::const_iterator ConstIter;
	typedef typename std::pair< ConstIter, ConstIter > ConstIterPair;
	typedef typename Vector::iterator Iterator;
	typedef typename std::pair< Iterator, Iterator > IteratorPair;

	T * find( int key ) {
		IteratorPair range = std::equal_range( begin(), end(), key );
		return range.first == range.second ? NULL : &(*range.first);
	}

	const T * find( int key ) const {
		ConstIterPair range = std::equal_range( begin(), end(), key );
		return range.first == range.second ? NULL : &(*range.first);
	}

	void logError( std::string value, const char * tag ) const {
		if( value.empty() ) {
			value = "\" \"";
		}
		std::string type = typeid( T ).name();
		LogFile::logError( tag, type + ": " + value );
	}

	bool readData( LQuery & query ) {
		clear();
		for( query.open(); !query.eof(); query.next() ) {
			T record( query );
			if( empty() || record.getID() > data.back().getID() ) {
				data.push_back( record );
			} else {
				IteratorPair range = std::equal_range( begin(), end(), record.getID() );
				if( range.first == range.second ) {
					data.insert( range.first, record );
				} else {
					String type = typeid( T ).name();
					throw Exception( type + " does not have unique IDs" );
				}
			}
		}
		return !empty();
	}

	template< typename O > const T * findMatch( const O & matcher ) const {
		const T * found = NULL;
		short n = 0;
		for( ConstIter x = begin(); x != end(); x ++ ) {
			if( matcher( *x ) ) {
				found = &(*x);
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
		if( empty() || data.back() < rec ) {
			data.push_back( rec );
			return &(data.back());
		}
		IteratorPair range = std::equal_range( begin(), end(), rec );
		if( range.first != range.second ) {
			*(range.first) = rec;
			return &(*range.first);
		} else {
			Iterator position = data.insert( range.first, rec );
			return &(*position);
		}
	}

	void erase( const T & rec ) {
		IteratorPair range = std::equal_range( begin(), end(), rec );
		if( range.first != range.second ) {
			data.erase( range.first, range.second );
        }
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

	void clear() {
		data.clear();
	}

	bool empty() const {
		return data.empty();
	}

	size_t size() const {
		return data.size();
	}

	Iterator begin() {
		return data.begin();
	}

	ConstIter begin() const {
		return data.begin();
	}

	Iterator end() {
		return data.end();
	}

	ConstIter end() const {
		return data.end();
	}
};

//---------------------------------------------------------------------------

#endif

