/* ---------------------------------------------------------------------------
 *
 *	An openAPI connection to one of the LIMS databases on vlab
 *
 *		8 June 2012		Initial version, based on Guru's Lab API code
 *
 --------------------------------------------------------------------------- */

#include <System.hpp>
#include <SysUtils.hpp>
#include <sstream>

#pragma hdrstop

#include "LIMSDatabase.h"
#include "xdb.h"
#include "rosetta.h"
#include "xerror.h"
#include "xexec.h"
#include "LCDbProject.h"
#include "LDbCacheBase.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------

LIMSDatabase::DbSystem LIMSDatabase::current = LIMSDatabase::UNKNOWN;
std::vector< std::string >LIMSDatabase::errors;

//---------------------------------------------------------------------------
//	find or create an entry for central, distributed or project database
//---------------------------------------------------------------------------

LIMSDatabase LIMSDatabase::getCentralDb( ) {
	return LIMSDatabase( "ldbc" );
}

//---------------------------------------------------------------------------

LIMSDatabase LIMSDatabase::getProjectDb( std::string dbName ) {
	if( dbName.empty( ) ) {
		int id = LCDbProjects::getCurrentID( );
		dbName = LCDbProjects::records( ).get( id ).getDbName( );
	}
	return LIMSDatabase( getRootName( dbName ) );
}

//---------------------------------------------------------------------------

LIMSDatabase LIMSDatabase::getDistributedDb( const std::string &dbName ) {
	return LIMSDatabase( getRootName( dbName ) + "_ddb/star" );
}

//---------------------------------------------------------------------------

LIMSDatabase::LIMSDatabase( const std::string &rootName )
 : name( rootName ), xdb( NULL ), dbs( UNKNOWN ) {
}

//---------------------------------------------------------------------------
//	Free up resources when we have finished using the database(s)
//---------------------------------------------------------------------------

LIMSDatabase::~LIMSDatabase( ) {
	if( isConnected( ) ) {
		xdb->close( );
	}
	delete xdb;
}

//---------------------------------------------------------------------------

LIMSDatabase::LIMSDatabase( const LIMSDatabase &other )
 : name( other.name ), xdb( NULL ), dbs( UNKNOWN ) {
}

//---------------------------------------------------------------------------

void LIMSDatabase::setCurrentSystem( DbSystem system ) {
	current = system;
	errors.clear( );
}

//---------------------------------------------------------------------------

bool LIMSDatabase::includes( const std::string & dbName ) {
	std::string expected = getPrefix( current ) + "ldb";
	return dbName.substr(0, expected.length()) == expected;
}

//---------------------------------------------------------------------------
//	Ignore test_ or t_ prefix; add them depending on current system
//---------------------------------------------------------------------------

std::string LIMSDatabase::getRootName( const std::string &dbName ) {
	if( dbName.substr(0,5) == "test_" ) {
		return dbName.substr( 5, dbName.length( ) - 5 );
	} else if( dbName.substr(0,2) == "t_" ) {
		return dbName.substr( 2, dbName.length( ) - 2 );
	} else {
		return dbName; 	// assume it's the live database
	}
}

//---------------------------------------------------------------------------

std::string LIMSDatabase::getPrefix( DbSystem system ) {
	switch( system ) {
		case LIVE_DATA:
			return "";
		case TEST_DATA:
			return "test_";
		case MIRROR_SYSTEM:
			return "t_";
		default:
			throw Exception( "Database system not selected" );
	}
}

//---------------------------------------------------------------------------

std::string LIMSDatabase::getDbName( ) const {
	DbSystem system = isConnected() ? dbs : current;
	return getPrefix( system ) + name;
}

//---------------------------------------------------------------------------
// 	Switch to current system and connect (using installation password)
//---------------------------------------------------------------------------

XDB *LIMSDatabase::connect( bool readLocks ) {
	if( isConnected() && dbs != current ) {
		xdb->close();
		delete xdb;
		xdb = NULL;
	}
	if( !isConnected() ) {
		std::string vnode = "vnode_vlab::" + getPrefix( current ) + name;
		xdb = new XDB( vnode );
		if( !xdb->open( ) ) {
			throw Exception( String( "Cannot open " ) + vnode.c_str( ) );
		}
		if( !readLocks ) {
			XEXEC lockMode( xdb, "set lockmode session where readlock=nolock" );
			if( !lockMode.exec( ) ) {
				throw Exception( "Error disabling read locks" );
			}
		}
		dbs = current;
		setErrorCallBacks( );
	}
	return xdb;
}

//---------------------------------------------------------------------------

bool LIMSDatabase::isConnected( ) const {
	return xdb != NULL && xdb -> isOpen();
}

//---------------------------------------------------------------------------
//	Record errors from Alan's code but don't throw until safe to do so
//---------------------------------------------------------------------------

void LIMSDatabase::xerrorCallback( const std::string object, const std::string error_txt ) {
	errors.push_back( object + " error: " + error_txt );
}

//---------------------------------------------------------------------------

bool LIMSDatabase::rosettaCallback( const ROSETTA *r, const int type, const std::string msg ) {
	errors.push_back( "Rosetta error: " + msg );
	return false;
}

//---------------------------------------------------------------------------

bool LIMSDatabase::xdbErrorCallback( const std::string object, const int instance,
		const int ecount, const int ecode, const std::string error_txt ) {
	errors.push_back( object + " error: " + error_txt );
	return false;
}

//---------------------------------------------------------------------------

void LIMSDatabase::setErrorCallBacks( ) {
	xdb->setErrorCallBack( xdbErrorCallback );
	XERROR::setErrorCallBack( xerrorCallback );
	ROSETTA::setErrorHandler( rosettaCallback );
}

//---------------------------------------------------------------------------
//	throw an exception after a query has been tried, reporting any errors
//---------------------------------------------------------------------------

void LIMSDatabase::confirm( bool success ) {
	std::string message;
	if( errors.empty() ) {
		if( !success ) {
			message = "Error executing query";
		}
	} else {
		std::vector< std::string >::iterator ei = errors.begin();
		message = *ei;
		while( ++ei != errors.end() ) {
			message = message + '\n' + *ei;
		}
		errors.clear();
	}
	if( !message.empty() ) {
		throw Exception( message.c_str() );
	}
}

//---------------------------------------------------------------------------

