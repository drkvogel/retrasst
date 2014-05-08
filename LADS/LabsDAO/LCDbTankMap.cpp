//---------------------------------------------------------------------------
#include <vcl.h>
#include <sstream>
#include "LCDbTankMap.h"
#include "LQuery.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

LCDbTankMap::LCDbTankMap( const LQuery & query )
 : LCDbID( query.readInt("record_cid") )
		, LDbValid( query.readDateTime("valid_from")
					,query.readDateTime("valid_to")
					,query.readInt("status"))
		,tankCID( query.readInt("tank_cid"))
		,storageCID( query.readInt("storage_cid"))
		,layoutCID( query.readInt("rack_layout_cid"))
		,locationCID( query.readInt("location_cid"))
		,position( query.readInt("position"))
{
	if( query.fieldExists( "storage_type" ) ) {
		storeTypeCID = query.readInt( "storage_type" );
	}
	if( query.fieldExists( "shelf_number" ) ) {
		shelf = query.readInt( "shelf_number" );
	}
}

//---------------------------------------------------------------------------
//	Create or update a record in c_object_name for this object
//---------------------------------------------------------------------------

bool LCDbTankMap::saveRecord( LQuery query )
{
	if( saved ) {
		query.setSQL( "update c_tank_map"
					" set tank_cid = :pop, storage_cid = :stor, location_cid = :site, position = :pos,"
					" status = :stat, valid_from = :vf, valid_to = :vt,"
					" storage_type = :st, rack_layout_cid = :lay, shelf_number = :sno"
					" where record_cid = :rid" );
		std::pair< XTIME, XTIME > dates = getValidDates();
		query.setParam( "vf", dates.first );
		query.setParam( "vt", dates.second );
	} else {
		claimNextID( query );
		query.setSQL( "INSERT into c_tank_map ( record_cid, tank_cid, storage_cid, location_cid, position,"
						" valid_from, status, valid_to, rack_layout_cid, storage_type, shelf_number )"
					" values ( :rid, :pop, :stor, :site, :pos, 'now', :stat, '', :lay, :st, :sno )" );
	}
	query.setParam( "rid", getID() );
	query.setParam( "pop", tankCID );
	query.setParam( "stor", storageCID );
	query.setParam( "site", locationCID );
	query.setParam( "pos", position );
	query.setParam( "stat", status );
	query.setParam( "lay", layoutCID );
	query.setParam( "st", storeTypeCID );
	query.setParam( "sno", shelf );
	if( query.execSQL() ) {
		saved = true;
		LCDbTankMaps::records().insert( *this );
		return true;
	} else {
		return false;
	}
}

//---------------------------------------------------------------------------

bool LCDbTankMaps::read( LQuery central, bool readAll )
{
	if( readAll )
		central.setSQL( "select * from c_tank_map order by record_cid" );
	else
	{	central.setSQL( "select * from c_tank_map where status <> :sts"
					   " order by record_cid" );
		central.setParam( "sts", LCDbTankMap::DELETED );
	}
	return readData( central );
}

//---------------------------------------------------------------------------

class LCDbTankMaps::Matcher : public std::unary_function< LCDbTankMap, bool >
{
	const int tankCID;

public:

	operator std::string() const
	{
		std::stringstream out;
		out << "Population " << tankCID; //  << ", store " << storageCID;
		return out.str();
	}

	Matcher( const int ptankCID ) : tankCID( ptankCID )
	{}

	bool operator()( const LCDbTankMap & other ) const
	{
		return tankCID == other.getTankCID() && other.isActive();
	}
};

//---------------------------------------------------------------------------

const LCDbTankMap * LCDbTankMaps::findCurrent( int pTankCID ) const
{
	return findMatch( Matcher( pTankCID ) );
}

//---------------------------------------------------------------------------


