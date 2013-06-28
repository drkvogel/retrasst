//---------------------------------------------------------------------------
//
//	Modified 19 July 2005	Set status and date range in v2.3 database
//           17 March 2006	Split out of LDbBoxTypes.cpp without changes
//           21 March 2006	Remove hasValidFlag() - assume ldbc >= 2.3
//
//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdlib.h>

#include "LQuery.h"
#include "LDbBoxSize.h"
#include "LCDbProject.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//  Read parameters for the given box size (can never be deleted)
//---------------------------------------------------------------------------

LCDbBoxSize::LCDbBoxSize( const LQuery & query )
 : LCDbID( query.readInt( "box_size_cid" ) ),
   name( query.readString( "description" ) ),
   size( query.readInt( "box_capacity" ) ),
   hole( query.readInt( "empty_position" ) ),
   LDbValid( query.readDateTime( "valid_from" ),
			 query.readDateTime( "valid_to" ),
			 query.readInt( "status" ) )
{}

//---------------------------------------------------------------------------

bool LCDbBoxSizes::read( LQuery cQuery, bool readAll )
{
	if( readAll )
		cQuery.setSQL( "select * from c_box_size"
					  " order by box_size_cid" );
	else
	{	cQuery.setSQL( "select * from c_box_size"
					  " where status <> :sts and 'now' between valid_from and valid_to"
					  " order by box_size_cid" );
		cQuery.setParam( "sts", LDbValid::DELETED );
	}
	return readData( cQuery );
}

//---------------------------------------------------------------------------
//	Create or update the given box size record and copy it into the cache
//---------------------------------------------------------------------------

bool LCDbBoxSize::saveRecord( LQuery cQuery )
{
	if( saved )
	{
		cQuery.setSQL( "Update c_box_size set description = :desc, box_capacity = :cap,"
					  " empty_position = :pos, status = :sts, valid_from = :from, valid_to = :to"
					  " where box_size_cid = :cid" );
	}
	else
	{	claimNextID( cQuery );
		cQuery.setSQL( "Insert into c_box_size (box_size_cid, description, box_capacity,"
					  "  empty_position, status, valid_from, valid_to)"
					  " values ( :cid, :desc, :cap, :pos, :sts, :from, :to )" );
	}

	cQuery.setParam( "cid", getID() );
	cQuery.setParam( "desc", getName() );
	cQuery.setParam( "cap", size );
	cQuery.setParam( "pos", hole );
	cQuery.setParam( "sts", status );
	std::pair< XTIME, XTIME > dates = getValidDates();
	cQuery.setParam( "from", dates.first );
	cQuery.setParam( "to", dates.second );

	if( cQuery.execSQL() )
	{
    	saved = true;
		LCDbBoxSizes::records().insert( *this );
		return true;
	}
	else return false;
}

//---------------------------------------------------------------------------

