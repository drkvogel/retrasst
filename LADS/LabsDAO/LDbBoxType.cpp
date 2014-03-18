/*---------------------------------------------------------------------------
 *
 *		19 July 2005		Set status and date range in v2.3 database
 *      17 March 2006		Read box types from ldbc or project as necessary
 *      14 Jun 08, NG:		Always use project database for box content
 *		20 Nov 12, NG:		Add number of analyses required before storage
 *      12 March 14, NG:	Prefer central database where possible (db2.7.2)
 *
 *--------------------------------------------------------------------------*/

#include <vcl.h>
#include <stdlib.h>
#include <sstream>
#include "LQuery.h"
#include "LDbBoxType.h"
#include "LCDbProject.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//  List the aliquots required for a box from the current project
//---------------------------------------------------------------------------

LPDbBoxType::LPDbBoxType( const LQuery & query )
 : LPDbID( query.readInt( "box_type_cid" ) ),
   LDbNames( query.readString( "external_name" ),
			 query.readString( "description" ) ),
   status( query.readInt( "status" ) ),
   uses( query.readInt( "expected_use" ) ),
   sizeID( query.readInt( "box_size_cid" ) ),
   group( query.readInt( "box_set_link" ) ),
   position( query.readInt( "box_order" ) )
{
	char field[ 16 ];
	for( int i = 1; i < 4; i ++ )
	{
		std::sprintf( field, "aliquot_type%d", i );
		if( query.fieldExists( field ) )
		{
			int aliquot = query.readInt( field );
			if( aliquot != 0 )
				content.push_back( aliquot );
		}
	}
}

//---------------------------------------------------------------------------
//  Read list types for current project, if it exist, otherwise central
//---------------------------------------------------------------------------

bool LPDbBoxTypes::read( LQuery query, bool readAll ) {
	std::stringstream sql;
	sql << "select * from c_box_content where project_cid in (" << LCDbProjects::getCurrentID() << ",0)";
	if( !readAll ) {
		sql << " and status <> " << LPDbBoxType::DELETED;
	}
	sql << " order by box_type_cid";
	query.setSQL( sql.str() );
	return readData( query );
}

//---------------------------------------------------------------------------
//	Change the aliquot types in this type of box
//---------------------------------------------------------------------------

void LPDbBoxType::setAliquots( Aliquots aliquotTypes )
{
	for( content.clear(); aliquotTypes.isValid(); ++ aliquotTypes )
		content.push_back( *aliquotTypes );
}

//---------------------------------------------------------------------------

bool LPDbBoxType::hasAliquot( int atid ) const
{
	for( Aliquots ali = content; ali.isValid(); ++ ali ) {
		if( *ali == atid ) {
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------
//	Create or update the given box content record; copy into the cache
//---------------------------------------------------------------------------

bool LPDbBoxType::saveRecord( LQuery query )
{
	if( saved ) {
		query.setSQL( "Update c_box_content set external_name = :nam, description = :desc, status = :sts,"
					  " expected_use = :eu, box_size_cid = :bs, box_order = :ord, box_set_link = :lnk,"
					  " aliquot_type1 = :at1, aliquot_type2 = :at2, aliquot_type3 = :at3"
					  " where box_type_cid = :cid" );
	} else {
		while( needsNewID() ) {
			claimNextID( query );
		}
		query.setSQL( "Insert into c_box_content (box_type_cid, external_name, description, status, expected_use,"
					  " box_size_cid, box_order, aliquot_type1, aliquot_type2, aliquot_type3, box_set_link)"
					  " values ( :cid, :nam, :desc, :sts, :eu, :bs, :ord, :at1, :at2, :at3, :lnk )" );
	}

	query.setParam( "nam", getName() );
	query.setParam( "desc", getDescription() );
	query.setParam( "sts", status );
	query.setParam( "eu", uses );
	query.setParam( "bs", sizeID );
	query.setParam( "ord", position );
	query.setParam( "lnk", group );
	query.setParam( "cid", getID() );
	std::vector< int >::const_iterator alr = content.begin();
	query.setParam( "at1", alr == content.end() ? 0 : *alr ++ );
	query.setParam( "at2", alr == content.end() ? 0 : *alr ++ );
	query.setParam( "at3", alr == content.end() ? 0 : *alr ++ );
	if( query.execSQL() ) {
		saved = true;
		LPDbBoxTypes::records().insert( *this );
		return true;
	}
	else return false;
}

//---------------------------------------------------------------------------
//	check the box type ID is valid and another project doesn't use it
//---------------------------------------------------------------------------

bool LPDbBoxType::needsNewID() const
{
	if( getID() != 0 ) {
		LQuery cq( LIMSDatabase::getCentralDb() );
		cq.setSQL( "select count(*) from c_box_content where box_type_cid in (:pid, :nid)" );
		cq.setParam( "pid", getID() );
		cq.setParam( "nid", -getID() );
		if( cq.open() && cq.readInt( 0 ) == 0 ) {
			return false;
		}
	}
	return true;
}

//---------------------------------------------------------------------------

