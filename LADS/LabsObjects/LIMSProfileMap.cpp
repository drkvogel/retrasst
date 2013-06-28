//---------------------------------------------------------------------------
//
//	Version history:
//
//      14 June 2005	Split out profile tree as LIMSProfileMap object
//      16 March 2006	Store child nodes as an (unsorted) vector
//      21 Feb. 2007	Sort columns by descriptor ID rather than children
//---------------------------------------------------------------------------

#include <vcl.h>
#include <set>

#include "LCDbProject.h"
#include "LQuery.h"
#include "LIMSProfileMap.h"
#include "LPDbProfile.h"
#include "LPDbDescriptor.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//  Nodes in the tree are kept in an array so it can grow without shifting
//---------------------------------------------------------------------------

unsigned LIMSProfileMap::nextNode;

LIMSProfileMap * LIMSProfileMap::getNode( unsigned index )
{
	static const MAX_NODES = 65;
	static LIMSProfileMap nodes[ MAX_NODES ];
	if( index >= MAX_NODES )
		throw Exception( "Profile map has too many nodes" );

	return nodes + index;
}

//---------------------------------------------------------------------------
//	Function object used to sort profile maps by descripto value IDs
//---------------------------------------------------------------------------

class LIMSProfileMap::LessByColumn
{
	int descriptor;

public:

	LessByColumn( int column ) : descriptor( column )
	{}

	bool operator()( const LPDbProfileMap & lhs, const LPDbProfileMap & rhs ) const
	{
		return lhs.findValueID( descriptor ) < rhs.findValueID( descriptor );
	}
};

//---------------------------------------------------------------------------

const LIMSProfileMap * LIMSProfileMap::create( const LPDbProfileMaps & pm )
{
	Range< LPDbProfileMap > pmr = pm;
	if( !pmr.isValid() )
		throw Exception( "No descriptors in profile map" );

	std::vector< LPDbProfileMap > copy;
	do copy.push_back( *pmr );
	while( (++ pmr).isValid() );

	LIMSProfileMap * root = getNode( 0 );
	nextNode = 1;
	root -> fill( copy.begin(), copy.end() );
	return root;
}

//---------------------------------------------------------------------------
//  Create profile map node from the given range of records - leaf nodes
//  store a test profile ID, otherwise there should be a shared descriptor
//  and this will be a sorted list of pointers, depending on its value
//---------------------------------------------------------------------------

void LIMSProfileMap::fill( RecIter start, RecIter end )
{
	// sort (part of) the table by the next shared column

	setID( nextShared( start, end ) );
	std::sort( start, end, LessByColumn( getID() ) );

	// create node for each value in that column recursively

	children.clear();
	for( RecIter split = start; start != end; start = split )
	{
		int valueID = start -> findValueID( getID() );
		do split -> getDVPairs().erase( getID() );
		while( ++ split != end && split -> findValueID( getID() ) == valueID );

		if( split - start == 1 && start -> getDVPairs().empty() )
			children.push_back( Link( valueID, true, start -> getProfileID() ) );
		else
		{   children.push_back( Link( valueID, false, nextNode ) );
			getNode( nextNode ++ ) -> fill( start, split );
		}
	}
}

//---------------------------------------------------------------------------
//  Find first column with a value in each row; return descriptor ID
//---------------------------------------------------------------------------

int LIMSProfileMap::nextShared( RecIter start, RecIter end )
{
	for( Range< LPDbDescriptor > dr = LPDbDescriptors::records(); dr.isValid(); ++ dr )
	{
		bool shared = true;
		for( RecIter match = start; match != end; ++ match )
			if( match -> findValueID( dr -> getID() ) == 0 )
				shared = false;

		if( shared )
			return dr -> getID();
	}
	throw Exception( "Cannot compile descriptors in profile map" );
}

//---------------------------------------------------------------------------
//  Create external name associated with this link's descriptor value
//---------------------------------------------------------------------------

String LIMSProfileMap::Link::getDescription() const
{
	const LPDbDescriptor * descrip = LPDbDescriptors::records().findByValueID( getID() );
	if( descrip != NULL )
	{
		const LPDbDescriptor::Value * found = descrip -> findValueByID( getID() );
		if( found != NULL )
			return descrip -> getName() + ": " + found -> getName();
	}
	throw Exception( "Descriptor value " + String( getID() ) + " not found" );
}

//---------------------------------------------------------------------------

