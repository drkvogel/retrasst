//---------------------------------------------------------------------------
//
//	Version history:
//
//		7 June 2005		Throw exception if cannot build LPDbProfileTree
//      14 June 2005	Split out profile tree as LIMSProfile object
//      9 September		Added methods to update and delete database records
//      31 Jan 2006		Added method to check if map includes value ID
//      18 Sep 08, NG:	Check descriptor matches value if available
//      16 Dec 08, NG:	added (optional) group_by and category_id fields
//      21 Jan 09, NG:	split storage and analytical categories
//
//---------------------------------------------------------------------------

#include <vcl.h>
#include <set>
#include <sstream>
#include "LCDbProject.h"
#include "LQuery.h"
#include "LPDbProfileMap.h"
#include "LPDbProfile.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//	Read values from profile_map; throw exception if positions don't match
//---------------------------------------------------------------------------

LPDbProfileMap::LPDbProfileMap( const LQuery & query )
 : LPDbID( query.readInt( "profile_map_id" ) ),
   testProfile( query.readInt( "test_profile_id" ) ),
   groupBy( query.readInt( "group_id" ) ),
   analysisCID( query.readInt( "analysis_cat_id" ) ),
   storageCID( query.readInt( "storage_cat_id" ) )
{
	const LPDbDescriptors & descriptors = LPDbDescriptors::records();
	char mapField[ 20 ];
	for( int i = 1; i < 32; i ++ )
	{
		std::sprintf( mapField, "value_id%d", i );
		if( query.fieldExists( mapField ) )
		{
			int valueID = query.readInt( mapField );
			if( valueID != 0 )
			{
				const LPDbDescriptor * descriptor = descriptors.findByField( mapField );
				if( descriptor == NULL || descriptor -> findValueByID( valueID ) == NULL )
					throw Exception( "Profile map configuration error" );

				setValueID( descriptor -> getID(), valueID );
			}
		}
	}
}

//---------------------------------------------------------------------------

bool LPDbProfileMaps::read( LQuery pQuery, bool )
{
	pQuery.setSQL( "select * from profile_map order by profile_map_id" );
	return readData( pQuery );
}

//---------------------------------------------------------------------------
//	Create/update database entry; assumes profile_map_node is a view
//---------------------------------------------------------------------------

void LPDbProfileMap::saveRecord( LQuery query )
{
	const LPDbDescriptors & descriptors = LPDbDescriptors::records();
	if( !saved )
	{
		claimNextID( query );
		std::stringstream fields, values;
		fields << "profile_map_id, test_profile_id, analysis_cat_id, storage_cat_id, group_id";
		values << ":pmid, :tpid, :acid, :scid, :gby";
		for( std::map< int, int >::const_iterator ci = columns.begin(); ci != columns.end(); ++ ci )
		{
			fields << ", " << descriptors.get( ci -> first ).getMapField();
			values << ", " << ci -> second;
		}
		std::string command = "Insert into profile_map";
		query.setSQL( command + " (" + fields.str() + ") values (" + values.str() + ")" );
	}
	else
	{	std::stringstream changes;
		changes << "test_profile_id = :tpid, analysis_cat_id = :acid, storage_cat_id = :scid, group_id = :gby";
		for( Range< LPDbDescriptor > di = descriptors; di.isValid(); ++ di )
			if( di -> isInProfileMap() )
				changes << ", " << di -> getMapField() << "=" << findValueID( di -> getID() );
		std::string command = "Update profile_map";
		query.setSQL( command + " set " + changes.str() + " where profile_map_id = :pmid" );
	}

	query.setParam( "pmid", getID() );
	query.setParam( "tpid", testProfile );
	query.setParam( "acid", analysisCID );
	query.setParam( "scid", storageCID );
	query.setParam( "gby", groupBy );
	if( query.execSQL() ) {
		saved = true;
		LPDbProfileMaps::records().insert( *this );
	}
}

//---------------------------------------------------------------------------
//	Delete a record from the profile map - cache and database
//---------------------------------------------------------------------------

bool LPDbProfileMaps::remove( LQuery query, int pmid )
{
	query.setSQL( "delete from profile_map where profile_map_id = :mid" );
	query.setParam( "mid", pmid );
	bool removed = query.execSQL();
	erase( pmid );
	return removed;
}

//---------------------------------------------------------------------------
//	Return true if map includes given value ID, false if it does not
//---------------------------------------------------------------------------

bool LPDbProfileMap::includesValue( int valueID ) const
{
	for( std::map< int, int >::const_iterator ci = columns.begin(); ci != columns.end(); ++ ci )
		if( ci -> second == valueID )
			return true;
	return false;
}

//---------------------------------------------------------------------------
//	Return the value ID for the given descriptor, 0 if not found
//---------------------------------------------------------------------------

int LPDbProfileMap::findValueID( int descripID ) const
{
	std::map< int, int >::const_iterator ci = columns.find( descripID );
	return ci == columns.end() ? 0 : ci -> second;
}

//---------------------------------------------------------------------------
//	Add or modify a column in the profile map unless given value is 0
//---------------------------------------------------------------------------

void LPDbProfileMap::setValueID( int descriptor, int value )
{
	std::map< int, int >::iterator found = columns.find( descriptor );
	if( found != columns.end() )
		found -> second = value;
	else if( value != 0 )
		columns.insert( { descriptor, value } );
}

//---------------------------------------------------------------------------
//	check if the given descriptor values match this profile map
//---------------------------------------------------------------------------

bool LPDbProfileMap::matches( const ValueFields & dvpList ) const
{
	for( Range< ValueField > dvi = dvpList.getValueFields(); dvi.isValid(); ++ dvi )
	{
		const LPDbDescriptor * dp = dvi -> getDescriptor();
		if( dp != NULL && dp -> isInProfileMap() )
		{
			int required = findValueID( dp -> getID() );
			if( required != 0 )
			{
				const LPDbDescriptor::Value * dvp = dp -> findValueRange( dvi -> getRange() );
				if( dvp != NULL && dvp -> getID() != required )
					return false;
			}
		}
	}
	return true;
}

//---------------------------------------------------------------------------

