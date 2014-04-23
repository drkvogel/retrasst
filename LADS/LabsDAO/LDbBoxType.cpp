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
#include "LDbBoxType.h"
#include "LCDbProject.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//  List the aliquots required for a box from the current project
//---------------------------------------------------------------------------

LPDbBoxType::LPDbBoxType( const LQuery & cQuery )
 : LCDbID( cQuery.readInt( "box_type_cid" ) ),
   LDbNames( cQuery.readString( "external_name" ),
			 cQuery.readString( "description" ) ),
   status( cQuery.readInt( "status" ) ),
   uses( cQuery.readInt( "expected_use" ) ),
   sizeID( cQuery.readInt( "box_size_cid" ) ),
   group( cQuery.readInt( "box_set_link" ) ),
   position( cQuery.readInt( "box_order" ) )
{
	char field[ 16 ];
	for( int i = 1; i < 4; i ++ )
	{
		std::sprintf( field, "aliquot_type%d", i );
		if( cQuery.fieldExists( field ) )
		{
			int aliquot = cQuery.readInt( field );
			if( aliquot != 0 )
				content.push_back( aliquot );
		}
	}
	if( cQuery.fieldExists( "project_cid" ) ) {
		projectCID = cQuery.readInt( "project_cid" );
	} else {
		projectCID = LCDbProjects::getCurrentID();
	}
}

//---------------------------------------------------------------------------
//  Read list types for current project, if it exist, otherwise central
//---------------------------------------------------------------------------

bool LPDbBoxTypes::read( LQuery cQuery, bool readAll ) {
	std::stringstream sql;
	sql << "select * from c_box_content";
	if( !readAll ) {
		sql << " where status <> " << LPDbBoxType::DELETED;
	}
	sql << " order by box_type_cid";
	cQuery.setSQL( sql.str() );
	return readData( cQuery );
}

//---------------------------------------------------------------------------
//	Change the aliquot types in this type of box
//---------------------------------------------------------------------------

void LPDbBoxType::setAliquots( const std::vector< int > & aliquotTypes ) {
	content.clear();
	for( int at : aliquotTypes ) {
		content.push_back( at );
	}
}

//---------------------------------------------------------------------------

bool LPDbBoxType::hasAliquot( int atid ) const {
	for( int at : content ) {
		if( at == atid ) {
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------
//	Create or update the given box content record; copy into the cache
//---------------------------------------------------------------------------

bool LPDbBoxType::saveRecord( LQuery pQuery, LQuery cQuery ) {
	if( getID() == 0 ) {
		claimNextID( cQuery );
	}
	if( save( "c_box_content", cQuery ) && save( "box_content", pQuery ) ) {
		saved = true;
		LPDbBoxTypes::records().insert( *this );
		return true;
	}
	else return false;
}

//---------------------------------------------------------------------------

bool LPDbBoxType::save( const std::string & table, LQuery & query ) {
	if( saved ) {
		query.setSQL( "Update " + table + " set external_name = :nam, description = :desc, status = :sts,"
					  " expected_use = :eu, box_size_cid = :bs, box_order = :ord, box_set_link = :lnk,"
					  " aliquot_type1 = :at1, aliquot_type2 = :at2, aliquot_type3 = :at3"
					  " where box_type_cid = :cid" );
		query.setParam( "eu", uses );
	} else {
		query.setSQL( "Insert into " + table + " (box_type_cid, external_name, description, status,"
					  " box_size_cid, box_order, aliquot_type1, aliquot_type2, aliquot_type3, box_set_link)"
					  " values ( :cid, :nam, :desc, :sts, :eu, :bs, :ord, :at1, :at2, :at3, :lnk )" );
	}
	query.setParam( "nam", getName() );
	query.setParam( "desc", getDescription() );
	query.setParam( "sts", status );
	query.setParam( "bs", sizeID );
	query.setParam( "ord", position );
	query.setParam( "lnk", group );
	query.setParam( "cid", getID() );
	std::vector< int >::const_iterator alr = content.begin();
	query.setParam( "at1", alr == content.end() ? 0 : *alr ++ );
	query.setParam( "at2", alr == content.end() ? 0 : *alr ++ );
	query.setParam( "at3", alr == content.end() ? 0 : *alr ++ );
	return query.execSQL();
}

//---------------------------------------------------------------------------

