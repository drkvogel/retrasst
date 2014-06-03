/*---------------------------------------------------------------------------
 *
 *		22 July, NG:		Fixed changeDbName() to check for _ddb
 *  	3 August 2005:		Don't allow update external_name or db_name
 *  	10 March 2006:		Modified cache handling to allow two copies
 *  	1 April 08, NG: 	Use test_ldbc for test_ databases etc.
 *  	30 July 08, NG:		Remove claimDatabase() method - fails on Laptop
 *		4 Sep 08, NG:		Make database_minor_version optional
 *  	23 October, NG:		Added BOX_IMPORT flag for Kadoorie-like projects
 *		29 Oct 08, NG:      Modified lease handling to allow more than one
 *      24/03/09, NG:		Force user to select Live, Test or Mirror
 *		20 May 09, NG:		Refactor leasing; claim() frees old lease
 *      15 April 11, NG:	Check for _STAR when switching database
 *      7 October 2011:		Split live/test/mirror switching into DbFamily
 *		7 June 2012, NG:	C++Builder XE2 version
 *
 *-------------------------------------------------------------------------*/

#include <vcl.h>
#include <sstream>
#include "LIMSDatabase.h"
#include "LCDbProject.h"
#include "LQuery.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//	Private constructor used to populate cache when program starts
//---------------------------------------------------------------------------

LCDbProject::LCDbProject( const LQuery & query  )
 : LCDbID( query.readInt( "project_cid" ) ),
   LDbNames( query.readString( "external_name" ),
			 query.readString( "external_full" ) ),
   LDbValid( query.readDateTime( "valid_from" ),
			 query.readDateTime( "valid_to" ),
			 query.readInt( "status" ) ),
   database( query.readString( "db_name" ) ),
   contact( query.readString( "info_url" ) ),
   version( query.readInt( "database_version" ),
			query.readInt( "database_minor_version" ),
			query.readInt( "database_minor_phase" ) )
{
	if( query.fieldExists( "study_code" ) ) {
		code = query.readString( "study_code" );
		AnsiString asCode = code.c_str();
		code = asCode.Trim().UpperCase().c_str();
	}
	if( code.empty() || code == "." )
		code = getName().substr( 0, 2 );

   liveData = (database.substr(0,3)== "ldb");
   boxImport = (status != DELETED && (status & BOX_IMPORT) != 0);
}

//---------------------------------------------------------------------------
//  (Re)populate the cache from the current central database
//---------------------------------------------------------------------------

bool LCDbProjects::read( LQuery central, bool readAll )
{
	if( readAll )
		central.setSQL( "select * from c_project order by project_cid" );
	else
	{	central.setSQL( "select * from c_project where project_cid <> :cid"
					  " where status <> :sts and valid_from < 'now' "
					  " and (valid_to < valid_from or valid_to > 'now')"
					   " order by project_cid" );
		central.setParam( "cid", LCDbProject::CENTRAL_DB );
		central.setParam( "sts", LCDbProject::DELETED );
	}
	return readData( central );
}

//---------------------------------------------------------------------------
//	Create a new database entry for the current project; copy to cache
//---------------------------------------------------------------------------

bool LCDbProject::saveRecord( LQuery query )
{
	if( saved )
	{
		query.setSQL( "Update c_project set external_full = :fnam, info_url = :url,"
					 " study_code = :sco, valid_from = :from, valid_to = :to, status = :sts"
					 " where project_cid = :cid" );
	}
	else
	{	claimNextID( query );
		query.setSQL( "INSERT into c_project (project_cid, external_name, study_code,"
					" external_full, valid_from, valid_to, status, info_url)"
					" values( :cid, :pnam, :sco, :fnam, :from, :to, :sts, :ver, :pdb, :url )" );
		query.setParam( "pnam", getName() );
		query.setParam( "pdb", database );
	}

	std::pair< XTIME, XTIME > dates = getValidDates();
	query.setParam( "cid", getID() );
	query.setParam( "fnam", getDescription() );
	query.setParam( "sts", getFlags() );
	query.setParam( "sco", code );
	query.setParam( "from", dates.first );
	query.setParam( "to", dates.second );
	query.setParam( "url", contact );
	if( !query.execSQL() )
		return false;

	saved = true;
	LCDbProjects::records().insert( *this );
	return true;
}

//---------------------------------------------------------------------------

std::string LCDbProject::getVersionString() const
{
	std::stringstream out;
	out << version.major << '.' << version.minor << '.' << version.phase;
	return out.str();
}

//---------------------------------------------------------------------------

int LCDbProject::getFlags()
{
	if( !isActive() )
		status = DELETED;
	else
	{	status = isLive() ? IS_LIVE : 0;
		if( hasBoxes() )
			status |= BOX_IMPORT;
	}
	return status;
}

//---------------------------------------------------------------------------

bool LCDbProject::isInCurrentSystem() const {
	return LIMSDatabase::includes( database );
}

//---------------------------------------------------------------------------
//  Class used to search cache for name, description or database name
//---------------------------------------------------------------------------

class /* LCDbProjects:: */ PdbMatcher : public LDbNames::LCMatcher
{
public:
	PdbMatcher( const std::string & s ) : LCMatcher( s ) {}
	bool operator() ( const LCDbProject & other ) const {
		return lcValue == LDbNames::makeLower( other.getName() )
			|| lcValue == LDbNames::makeLower( other.getDescription() )
			|| lcValue == LDbNames::makeLower( other.getDbName() );
	}
};

//---------------------------------------------------------------------------
//	Search cache by name; copy if found; throw exception if cache clear.
//	No ID is allocated if not found, so caller can check using isStored()
//---------------------------------------------------------------------------

const LCDbProject * LCDbProjects::findByName( const std::string & nameOrDb ) const
{
	return findMatch( PdbMatcher( nameOrDb ) );
}

//---------------------------------------------------------------------------
//	Switch to using this project's database if a lease is available
//---------------------------------------------------------------------------

void LCDbProjects::setCurrent( const LCDbProject * proj )
{
	if( proj == NULL ) {
		currentID = LCDbProject::NONE_SELECTED;
	} else if( proj->isCentral() ) {
		String name = proj->getDbName().c_str();
		throw Exception( name + " is not a project database" );
	} else {
		currentID = proj->getID();
	}
}

//---------------------------------------------------------------------------

