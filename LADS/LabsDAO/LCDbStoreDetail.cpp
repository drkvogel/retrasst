/*---------------------------------------------------------------------------
 *
 *		24/5/10, NG:	implemented saveRecord() for use in StoreMan
 *      15/3/11, NG:	find by serial number rather than description
 *
---------------------------------------------------------------------------*/

#include <vcl.h>
#include "LCDbStoreDetail.h"
#include "LQuery.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

LCDbStorageDetail::LCDbStorageDetail( const LQuery & query )
 : LCDbID( query.readInt("storage_cid"))
	,storageType( query.readInt("storage_type"))
	,locationCID( query.readInt("location_cid"))
	,position( query.readInt("position"))
	,serialNumber( query.readString( "serial_number"))
	,physDesc( query.readString( "physical_description"))
	,status( query.readInt("status"))
	,timeStamp( query.readDateTime("time_stamp"))
{}

//---------------------------------------------------------------------------

bool LCDbStorageDetails::read( LQuery central, bool readAll )
{
	if( readAll )
		central.setSQL( "select * from c_storage_details"
					   " order by storage_cid" );
	else
	{	central.setSQL( "select * from c_storage_details where status <> :sts"
					   " order by storage_cid" );
		central.setParam( "sts", LDbValid::DELETED );
	}
	return readData( central );
}

//---------------------------------------------------------------------------

class LCDbStorageDetails::Matcher : public std::unary_function< LCDbStorageDetail, bool >
{
	const std::string serial;

public:

	operator std::string() const { return serial; }

	Matcher( const std::string & pSerial ) : serial( pSerial )
	{}

	bool operator()( const LCDbStorageDetail & other ) const
	{
		return serial.compare( other.getSerialNumber() ) == 0;
	}
};

//---------------------------------------------------------------------------

const LCDbStorageDetail * LCDbStorageDetails::find( const std::string & pSerial ) const
{
	return findMatch( Matcher( pSerial ) );
}

//---------------------------------------------------------------------------
//	Create/update database entry for this (physical) storage tank
//---------------------------------------------------------------------------

bool LCDbStorageDetail::saveRecord( LQuery query )
{
	if( saved )
	{
		query.setSQL( "Update c_storage_details set storage_type = :sty,"
					 " location_cid = :lid, position = :pos, serial_number = :sno,"
					 " status = :sts, physical_description = :phys, time_stamp = :dts"
					 " where storage_cid = :cid" );
	}
	else
	{	claimNextID( query );
		query.setSQL( "Insert into c_storage_details (storage_cid, storage_type, location_cid,"
					"  position, serial_number, status, time_stamp, physical_description)"
					" values ( :cid, :sty, :lid, :pos, :sno, :sts, :dts, :phys )" );
	}

	query.setParam( "cid", getID() );
	query.setParam( "sty", storageType );
	query.setParam( "phys", physDesc );
	query.setParam( "lid", locationCID );
	query.setParam( "pos", position );
	query.setParam( "sno", serialNumber );
	query.setParam( "sts", status );
	query.setParam( "dts", XTIME(timeStamp) );
	if( !query.execSQL() )
		return false;

	saved = true;
	LCDbStorageDetails::records().insert( *this );
	return true;
}

//---------------------------------------------------------------------------

