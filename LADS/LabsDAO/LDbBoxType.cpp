/*---------------------------------------------------------------------------
 *
 *		19 July 2005		Set status and date range in v2.3 database
 *      17 March 2006		Read box types from ldbc or project as necessary
 *      14 Jun 08, NG:		Always use project database for box content
 *		20 Nov 12, NG:		Add number of analyses required before storage
 *      12 March 14, NG:	Prefer central database where possible (db2.7.2)
 *      16 May 2014, NG:	Always save to box_content and c_box_content
 *
 *--------------------------------------------------------------------------*/

#include <vcl.h>
#include <stdlib.h>
#include <sstream>
#include "LDbBoxType.h"
#include "LCDbProject.h"
#include "LDbBoxSize.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//  List the aliquots required for a box from the current project
//---------------------------------------------------------------------------

LPDbBoxType::LPDbBoxType( const LQuery & cQuery )
 : LCDbID( cQuery.readInt( "box_type_cid" ) ),
   LDbNames( cQuery.readString( "external_name" ), cQuery.readString( "description" ) ),
   status( cQuery.readInt( "status" ) ),
   uses( cQuery.readInt( "expected_use" ) ),
   sizeID( cQuery.readInt( "box_size_cid" ) ) {
	char field[ 20 ];
	for( int i = 1; i < 4; i ++ ) {
		std::sprintf( field, "aliquot_type%d", i );
		if( cQuery.fieldExists( field ) ) {
			int aliquot = cQuery.readInt( field );
			if( aliquot != 0 )
				content.push_back( aliquot );
		}
	}
	group = cQuery.fieldExists( "box_set_link" ) ? cQuery.readInt( "box_set_link" ) : 0;
	position = cQuery.fieldExists( "box_order" ) ? cQuery.readInt( "box_order" ) : -1;
	projectCID = cQuery.fieldExists( "project_cid" ) ? cQuery.readInt( "project_cid" ) : LCDbProjects::getCurrentID();
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
	if( !update( false, pQuery ) && !insert( false, pQuery ) ) {
		saved = false;
	} else if( !update( true, cQuery ) && !insert( true, cQuery ) ) {
		saved = false;
	} else {
		saved = true;
		LPDbBoxTypes::records().insert( *this );
	}
	return saved;
}

//---------------------------------------------------------------------------

bool LPDbBoxType::update( bool central, LQuery & query ) {
	if( central ) {
		query.setSQL( "Update c_box_content set status = :sts, time_stamp = 'now', project_cid = :proj,"
					" box_size_cid = :bs, aliquot_type1 = :at1, aliquot_type2 = :at2, aliquot_type3 = :at3"
					" where box_type_cid = :cid" );
		query.setParam( "proj", projectCID );
	} else {
		query.setSQL( "Update box_content set status = :sts, time_stamp = 'now',"
					" expected_use = :eu, box_size_cid = :bs, box_order = :ord, box_set_link = :lnk,"
					" aliquot_type1 = :at1, aliquot_type2 = :at2, aliquot_type3 = :at3"
					" where box_type_cid = :cid" );
		query.setParam( "eu", uses );
		query.setParam( "ord", position );
		query.setParam( "lnk", group );
	}
	query.setParam( "sts", status );
	query.setParam( "bs", sizeID );
	query.setParam( "cid", getID() );
	std::vector< int >::const_iterator alr = content.begin();
	query.setParam( "at1", alr == content.end() ? 0 : *alr ++ );
	query.setParam( "at2", alr == content.end() ? 0 : *alr ++ );
	query.setParam( "at3", alr == content.end() ? 0 : *alr ++ );
	return query.execSQL();
}

//---------------------------------------------------------------------------

bool LPDbBoxType::insert( bool central, LQuery & query ) {
	if( central ) {
		query.setSQL( "Insert into c_box_content (box_type_cid, external_name, description, status,"
					  " project_cid, box_size_cid, aliquot_type1, aliquot_type2, aliquot_type3)"
					  " values ( :cid, :nam, :desc, :sts, :proj, :bs, :at1, :at2, :at3 )" );
		query.setParam( "proj", projectCID );
	} else {
		query.setSQL( "Insert into box_content (box_type_cid, external_name, description, status,"
					  " expected_use, box_size_cid, aliquot_type1, aliquot_type2, aliquot_type3,"
					  " box_order, box_set_link)"
					  " values ( :cid, :nam, :desc, :sts, :eu, :bs, :at1, :at2, :at3, :ord, :lnk )" );
		query.setParam( "eu", uses );
		query.setParam( "ord", position );
		query.setParam( "lnk", group );
	}
	query.setParam( "cid", getID() );
	query.setParam( "nam", getName() );
	query.setParam( "desc", getDescription() );
	query.setParam( "sts", status );
	query.setParam( "bs", sizeID );
	std::vector< int >::const_iterator alr = content.begin();
	query.setParam( "at1", alr == content.end() ? 0 : *alr ++ );
	query.setParam( "at2", alr == content.end() ? 0 : *alr ++ );
	query.setParam( "at3", alr == content.end() ? 0 : *alr ++ );
	return query.execSQL();
}


//---------------------------------------------------------------------------
//	Find a type with a similar name in this or all projects
//---------------------------------------------------------------------------

class LPDbBoxTypes::NameMatcher : public LDbNames::LCMatcher
{
public:
	NameMatcher( const std::string & s ) : LCMatcher( s ) {}
	bool operator() ( const LPDbBoxType & other ) const	{
		return (other.getProjectCID() == 0
			 || other.getProjectCID() == LCDbProjects::getCurrentID())
			&& (lcValue == LDbNames::makeLower( other.getName() )
			 || lcValue == LDbNames::makeLower( other.getDescription() ));
	}
};

//---------------------------------------------------------------------------

const LPDbBoxType * LPDbBoxTypes::find( const std::string & name ) const {
	return findMatch( NameMatcher( name ) );
}

//---------------------------------------------------------------------------
//	Find how many cryovials fit into this type of box/rack
//---------------------------------------------------------------------------

short LPDbBoxType::getCapacity() const {
	const LCDbBoxSize * bs = LCDbBoxSizes::records().findByID( sizeID );
	if( bs == NULL ) {
		return -1;
	} else if( bs->getHole() < 0 ) {
		return bs->getLast();
	} else {
		return bs->getLast() - 1;
   }
}

//---------------------------------------------------------------------------

