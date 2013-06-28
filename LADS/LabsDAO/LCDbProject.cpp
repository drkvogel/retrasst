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
#include "LCDbProject.h"
// #include "lease.h"
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
   contact( query.readString( "info_url" ) )
{
	if( query.fieldExists( "database_version" ) )
		version.first = query.readInt( "database_version" );
	else
		version.first = 2;

	if( query.fieldExists( "database_minor_version" ) )
		version.second = query.readInt( "database_minor_version" );
	else
		version.second = 5;

	if( query.fieldExists( "database_minor_phase" ) )
		;

	if( query.fieldExists( "study_code" ) )
		code = query.readString( "study_code" );
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
					   " and status <> :sts and 'now' between valid_from and valid_to"
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
	out << version.first << '.' << version.second;
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
//	Select the current project (taking a new lease if necessary)
//---------------------------------------------------------------------------
/*
void LCDbProject::claim( TDatabase* dbCentral, TDatabase* dbProj ) const
{
	LCDbProjects & cache = LCDbProjects::records();
	if( cache.getCurrentID() != getID() )
	{
		cache.endLease( cache.getCurrentID() );
		std::string message = cache.claimLease( dbCentral, getID() );
		if( !message.empty() )
			throw Exception( message.c_str() );
		cache.setCurrent( *this );
	}

	DbFamily().changeDbName( dbProj, getDbName() );
}

//---------------------------------------------------------------------------
//  Free up lease, assuming the database is currently being claimed
//---------------------------------------------------------------------------

void LCDbProject::release() const
{
	LCDbProjects::records().endLease( getID() );
}

//---------------------------------------------------------------------------
//	return the name of the Ingres database for the given system
//---------------------------------------------------------------------------

// DbFamily::DbSystem DbFamily::currentSystem = DbFamily::UNKNOWN;

//---------------------------------------------------------------------------

std::string DbFamily::getDbPrefix() const
{
	switch( currentSystem )
	{
		case LIVE_DATA:
			return "ldb";

		case TEST_DATA:
			return "test_ldb";

		case MIRROR_SYSTEM:
			return "t_ldb";
	}
	throw Exception( "Database system not yet set" );
}

//---------------------------------------------------------------------------

std::string DbFamily::getName() const
{
	switch( currentSystem )
	{
		case LIVE_DATA:
			return "LIVE";

		case TEST_DATA:
			return "TEST";

		case MIRROR_SYSTEM:
			return "MIRROR";
	}
	throw Exception( "Database system not yet set" );
}

//---------------------------------------------------------------------------
//	select Test, Live or Mirror system - call before selecting project
//---------------------------------------------------------------------------

void DbFamily::setDbSystem( TDatabase* dbComponent, DbSystem system, bool readLocks )
{
	currentSystem = system;
	changeDbName( dbComponent, getCentralDb() );

	std::string lockMode, bdeMode;
	if( readLocks )
	{
		lockMode = "shared";
		bdeMode = "SHARED AUTOCOMMIT";
	}
	else
	{	lockMode = "nolock";
		bdeMode = "NOT SHARED";
	}

	dbComponent -> Close();
	dbComponent -> Params -> Values[ "SQLPASSTHRU MODE" ] = bdeMode.c_str();

	LQuery central( bcsToStd(dbComponent -> DatabaseName) );
	central.setSQL( "set lockmode session where readlock=" + lockMode );
	central.execSQL();

	XMLFile & log = central.getLog();
	log.start( "db-change" );
	log.addAttribute( "time", Now() );
	log.addAttribute( "alias", dbComponent -> AliasName );
	log.addAttribute( "new-db", getCentralDb() );
	log.endTag();
}

//---------------------------------------------------------------------------
//	check that given database belongs to the current database system
//---------------------------------------------------------------------------

void DbFamily::checkSystem( const std::string & pdb ) const
{
	std::string cdbPrefix = getDbPrefix();
	std::string pdbPrefix = pdb.substr( 0, cdbPrefix.length() );
	if( cdbPrefix.compare( pdbPrefix ) != 0 )
		throw Exception( String(pdb.c_str()) + " does not belong to " + getCentralDb().c_str() );
}

//---------------------------------------------------------------------------
//  Take out lease on the selected project in test, live or mirror system
//---------------------------------------------------------------------------

std::string LCDbProjects::claimLease( TDatabase* dbComponent, int projID )
{
	if( projID != LCDbProject::CENTRAL_DB )
	{
		const LCDbProject & proj = get( projID );
		if( !proj.isActive() )
			throw Exception( String(proj.getDbName().c_str()) + " is not an active project" );
	}

	if( leaseManager == NULL )
	{
		std::string alias = bcsToStd(dbComponent -> AliasName);
		std::string name = bcsToStd(Application -> Title);
		std::string central = "DATABASE NAME=" + DbFamily().getCentralDb();
		leaseManager = new TLeaseManager( alias, name, central );
	}

	std::string result;
	if( leaseManager -> Activate( projID, leaseID, LEASE_TIME_DEFAULT, result ) != 0 )
	{
		if( result.empty() )
			result = "Cannot claim lease on project database";
		else
			result = "Waiting for " + result;
	}
	return result;
}

//---------------------------------------------------------------------------
//	free up the lease for the given project; release if it was current
//---------------------------------------------------------------------------

void LCDbProjects::endLease( int projID )
{
	if( leaseManager != NULL )
		leaseManager -> Deactivate( projID, leaseID );
	if( currentID == projID )
		clearCurrentID();
}

//---------------------------------------------------------------------------
//  Free up all leases created by this program (even if forgotten)
//---------------------------------------------------------------------------

void LCDbProjects::releaseDatabases()
{
	if( leaseManager != NULL )
	{
		leaseManager -> DeactivateAll();
		delete leaseManager;
		leaseManager = NULL;
	}

	LDbNextID::clearAll();
	clearCurrentID();
	Session -> Close();
}

//---------------------------------------------------------------------------
//  Make this project current; point given component to its database
//---------------------------------------------------------------------------

void DbFamily::changeDbName( TDatabase* dbComponent, const std::string & db ) const
{
	String alias = dbComponent -> AliasName;
	String oldDb = dbComponent -> Params -> Values[ "DATABASE NAME" ];
	String newDb = db.c_str();

	bool ddb;
	if( oldDb.IsEmpty() )
		ddb = alias.LowerCase().Pos( "_star" ) != 0;
	else
		ddb = oldDb.LowerCase().Pos( "_ddb" ) != 0;

	if( ddb && newDb.LowerCase().Pos( "_ddb" ) == 0 )
		newDb += "_ddb";

	if( newDb.CompareIC( oldDb ) != 0 )
	{
		dbComponent -> Close();
		dbComponent -> Params -> Values[ "DATABASE NAME" ] = newDb;
	}
}
*/
//---------------------------------------------------------------------------
//  Class used to search cache for name, description or database name
//---------------------------------------------------------------------------

class /* LCDbProjects:: */ PdbMatcher : public std::unary_function< LCDbProject, bool >
{
	const std::string value;

public:

	PdbMatcher( const std::string & s ) : value( s )
	{}

	operator std::string() const { return value; }

	bool operator() ( const LCDbProject & other ) const
	{
		return value.compare( other.getName() ) == 0
			|| value.compare( other.getDescription() ) == 0
			|| value.compare( other.getDbName() ) == 0;
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

void LCDbProjects::setCurrent( const LCDbProject & proj )
{
	String name = proj.getDbName().c_str();
	if( proj.isCentral() )
		throw Exception( name + " is not a project database" );

	LDbNextID::clearAll();
	currentID = proj.getID();
}

//---------------------------------------------------------------------------



