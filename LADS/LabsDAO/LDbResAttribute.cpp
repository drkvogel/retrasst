
#include <vcl.h>
#include "LDbResAttribute.h"
#include "LCDbAuditTrail.h"
#include "LCDbObject.h"
#include "LCDbAnalyser.h"

#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------

LDbResAttribute::LDbResAttribute( const String & otName, Type data, const String & val, TDateTime timeStamp )
 : value( val ), dataType( data ), when( timeStamp )
{
	const LCDbObject * found = LCDbObjects::records().find( otName, LCDbObject::RESULT_ATTRIBUTE );
	if( found == NULL )
		throw Exception( "Result attribute " + otName + " not defined" );
	fieldType = found -> getID();
}

//---------------------------------------------------------------------------
//	store this attribute, linked to the given ID on this cluster
//---------------------------------------------------------------------------

bool LDbResAttribute::createRecord( LQuery cluster, int brid ) const
{
	cluster.setSQL( "INSERT into buddy_res_attribute"
					" (Buddy_Result_ID, machine_cid, string_value, data_type,"
					"  time_stamp, result_type, process_cid, delete_status)"
					" values ( :brid, :mid, :sval, :dty, :dts, :rty, :pid, 0 )" );
	cluster.setParam( "brid", brid );
	cluster.setParam( "mid", LCDbAnalysers::getCurrentID() );
	cluster.setParam( "sval", value );
	cluster.setParam( "dts", when );
	cluster.setParam( "rty", fieldType );
	cluster.setParam( "dty", dataType );
	cluster.setParam( "pid", LCDbAuditTrail::getProcessID() );
	return cluster.execSQL() == 1;
}

//---------------------------------------------------------------------------
//	read an attribute from the database, ready to be copied to project
//---------------------------------------------------------------------------

LDbResAttribute::LDbResAttribute( const LQuery & buddyData )
 : fieldType( buddyData.readInt( "result_type" ) ),
   value( buddyData.readString( "string_value" ) ),
   when( buddyData.readDateTime( "time_stamp" ) ),
   dataType( buddyData.readInt( "data_type" ) )
{}

//---------------------------------------------------------------------------

bool LDbAttributes::checkResult( LQuery query, int objID )
{
	details.clear();
	query.setSQL( "select * from buddy_res_attribute"
				 " where Buddy_Result_ID = :oid and machine_cid = :mid"
				 " order by time_stamp" );
	query.setParam( "oid", objID );
	query.setParam( "mid", LCDbAnalysers::getCurrentID() );
	for( query.open(); !query.eof(); query.next() )
		details.push_back( query );
	return !details.empty();
}

//---------------------------------------------------------------------------
//	Flag attributes for deletion from cluster after they've been copied
//---------------------------------------------------------------------------

bool LDbAttributes::mark( LQuery query, int objID )
{
	query.setSQL( "update buddy_res_attribute set time_stamp = :td, delete_status = 2"
				 " where Buddy_Result_ID = :oid and machine_cid = :mid" );
	query.setParam( "td", Now() );
	query.setParam( "oid", objID );
	query.setParam( "mid", LCDbAnalysers::getCurrentID() );
	return query.execSQL() > 0;
}

//---------------------------------------------------------------------------
//	Delete attributes before analyses and results can be deleted
//---------------------------------------------------------------------------

unsigned LDbAttributes::deleteOld( LQuery cluster, TDateTime flagged, TDateTime oldest )
{
	cluster.setSQL( "delete from buddy_res_attribute where time_stamp < :old"
					" or (delete_status = 2 and time_stamp < :fdt)" );
	cluster.setParam( "old", oldest );
	cluster.setParam( "fdt", flagged );
	return cluster.execSQL();
}

//---------------------------------------------------------------------------
//	create records in the central database to match those in local
//---------------------------------------------------------------------------

int LDbAttributes::addReagent( LQuery qCentral )
{
	int reagent = 0;
	const LCDbObjects & names = LCDbObjects::records();
	static LCDbID cidSource;
	for( Range< LDbResAttribute > rai = details; rai.isValid(); ++ rai )
	{
		String barcode, lot;
		const LCDbObject * field = names.findByID( rai -> getType() );
		if( field != NULL )
		{
			if( field -> getName().AnsiCompareIC( "Reagent_ID" ) == 0 )
				barcode = rai -> getValue();
			else if( field -> getName().AnsiCompareIC( "Reagent_Lot" ) == 0 )
				lot = rai -> getValue();
		}

		if( !barcode.IsEmpty() || !lot.IsEmpty() )
		{
			if( reagent == 0 )
			{
				cidSource.claimNextID( qCentral, 2 );
				reagent = cidSource.getID();
			}
			cidSource.claimNextID( qCentral, 1 );
			int wedge = cidSource.getID();

			qCentral.setSQL( "insert into c_reagent_lot"
				" (wedge_cid, serial_number, lot_number, reagent_cid, machine_cid, first_result)"
				" values( :wid, :sno, :lno, :rid, :mid, :ts )" );
			qCentral.setParam( "wid", wedge );
			qCentral.setParam( "sno", barcode );
			qCentral.setParam( "lno", lot );
			qCentral.setParam( "rid", reagent );
			qCentral.setParam( "mid", LCDbAnalysers::getCurrentID() );
			qCentral.setParam( "ts", rai -> getTimeStamp() );
			qCentral.execSQL();
		}
	}
	return reagent;
}

//---------------------------------------------------------------------------
//	attempt to resuse existing reagent records if they already exist
//---------------------------------------------------------------------------

int LDbAttributes::checkReagent( LQuery qCentral )
{
	const LCDbObjects & names = LCDbObjects::records();
	std::set< String > required, found;
	for( Range< LDbResAttribute > rai = details; rai.isValid(); ++ rai )
	{
		const LCDbObject * field = names.findByID( rai -> getType() );
		if( field != NULL )
		{
			if( field -> getName().AnsiCompareIC( "Reagent_ID" ) == 0
			 || field -> getName().AnsiCompareIC( "Reagent_Lot" ) == 0 )
			{
				required.insert( rai -> getValue().Trim() );
			}
		}
	}

	int previous = 0;
	qCentral.setSQL( "select * from c_reagent_lot order by reagent_cid" );
	for( qCentral.open(); !qCentral.eof(); qCentral.next() )
	{
		int reagent = qCentral.readInt( "reagent_cid" );
		String value = qCentral.readString( "serial_number" );
		if( value.IsEmpty() )
			value = qCentral.readString( "lot_number" );
		if( reagent == previous )
			found.insert( value );
		else if( found == required )
			return previous;
		else
		{	found.clear();
			found.insert( value );
			previous = reagent;
		}
	}
	return found == required ? previous : 0;
}

//---------------------------------------------------------------------------

