/*---------------------------------------------------------------------------
 *
 *		16 Oct 08, NG:		Split out from LDbCache to use stand-alone
 *      9 June 09, NG:		Added similar definition for MapRange
 *      29 Sept 2010, NG:   Added Filter - selects values to return
 *
 *--------------------------------------------------------------------------*/

#ifndef LDbRangeH
#define LDbRangeH

#include <vector>
#include <map>
#include <functional>

//---------------------------------------------------------------------------
//	Wraps a slice from a container [first,last), similar to boost::Range
//---------------------------------------------------------------------------

template< typename T, typename C = std::vector< T > > class Range
{
	typedef typename C::iterator Iterator;
	typedef typename C::const_iterator ConstIter;

	ConstIter start, next, finish;

public:

	typedef C container_type;
	typedef T value_type;

	Range( ConstIter first, ConstIter last )
	 : start( first ), next( first ), finish( last )
	{}

	Range( const C & data )
	 : start( data.begin() ), next( start ), finish( data.end() )
	{}

	Range( std::pair< ConstIter, ConstIter > pair )
	 : start( pair.first ), next( pair.first ), finish( pair.second )
	{}

	Range( std::pair< Iterator, Iterator > pair )
	 : start( pair.first ), next( pair.first ), finish( pair.second )
	{}

	Range( const Range & other )
	 : start( other.start ), next( other.next ), finish( other.finish )
	{}

	bool isValid() const { return next < finish; }
	unsigned remaining() const { return finish - next; }
	bool empty() const { return finish == start; }
	unsigned size() const { return finish - start; }

	void reset() { next = start; }

	const Range & operator++()   			// ++ prefix advances iterator
	{
		if( isValid() )
			++ next;
		return *this;
	}

	const Range & operator++( int ); 		// postfix ++ not implemented

	const value_type * operator->() const { return &(*next); }
	const value_type & operator*() const { return *next; }
};

//---------------------------------------------------------------------------
//	A similar wrapper for iterators for a map or multi-map
//---------------------------------------------------------------------------

template< typename K, typename T, typename C = std::multimap< K, T > > class MapRange
{
	typedef typename std::pair< K, T > MapEntry;
	typedef typename std::multimap< K, T >::const_iterator ConstIter;

	ConstIter start, next, finish;

public:

	typedef C container_type;
	typedef K key_type;
	typedef T value_type;

	MapRange( ConstIter first, ConstIter last )
	 : start( first ), next( first ), finish( last )
	{}

	MapRange( const C & data )
	 : start( data.begin() ), next( start ), finish( data.end() )
	{}

	MapRange( std::pair< ConstIter, ConstIter > pair )
	 : start( pair.first ), next( pair.first ), finish( pair.second )
	{}

	MapRange( const MapRange & other )
	 : start( other.start ), next( other.next ), finish( other.finish )
	{}

	bool isValid() const { return next != NULL && next != finish; }
	bool empty() const { return finish == start; }

	void reset() { next = start; }

	const MapRange & operator++()
	{
		if( isValid() )
			++ next;
		return *this;
	}

	const MapRange & operator++( int );

	const T & operator*() const { return next -> second; }
	const T * operator->() const { return &(next -> second); }
};

//---------------------------------------------------------------------------
//	A filtered range - returns item that match a given predicate
//---------------------------------------------------------------------------

template< typename C, typename P > class Filter
{
	typedef typename C::const_iterator const_iterator;
	typedef typename std::pair< const_iterator, const_iterator > ConstIterPair;
	typedef typename C::value_type value_type;

	const_iterator start, next, finish;
	P predicate;

public:

	Filter( const C & data, const P & pred )
	 : start( data.begin() ), finish( data.end() ), predicate( pred )
	{
		reset();
	}

	Filter( ConstIterPair pair, const P & pred )
	 : start( pair.first ), finish( pair.second ), predicate( pred )
	{
		reset();
	}

	Filter( const Filter & other )
	 : start( other.start ), next( other.next ), finish( other.finish ), predicate( other.predicate )
	{}

	void reset()
	{
		next = std::find_if( start, finish, predicate );
	}

	bool isValid() const { return next != finish; }

	const Filter & operator++()
	{
		if( next != NULL && next < finish )
			next = std::find_if( next + 1, finish, predicate );
		return *this;
	}

	const Filter & operator++( int );

	const value_type * operator->() const { return &(*next); }
	const value_type & operator*() const { return *next; }

};

//---------------------------------------------------------------------------

#endif

