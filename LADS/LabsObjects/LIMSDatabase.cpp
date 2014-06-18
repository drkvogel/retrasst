/* ---------------------------------------------------------------------------
 *
 *	An openAPI connection to one of the LIMS databases on vlab
 *
 *		8 June 2012		Initial version, based on Guru's Lab API code
 *      11 Sept 2013	Find database name from LCDbProjects if needed
 *      21 May 14, NG:	Switch vnodes between vlab and vlabdev
 *
 --------------------------------------------------------------------------- */

#include <System.hpp>
#include <SysUtils.hpp>

#include "LIMSDatabase.h"
#include "xdb.h"
#include "rosetta.h"
#include "xerror.h"
#include "xexec.h"
#include "LCDbProject.h"
#include "LDbCacheBase.h"
#include "LIMSParams.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

LIMSDatabase::DbSystem LIMSDatabase::current = LIMSDatabase::UNKNOWN;
std::vector< std::string >LIMSDatabase::errors;

//---------------------------------------------------------------------------

LIMSDatabase::DbSystem LIMSDatabase::getCurrentSystem() {
	if( current == LIMSDatabase::UNKNOWN ) {
		current = LIMSParams::instance().getDbSystem();
	}
	return current;
}

//---------------------------------------------------------------------------

std::string LIMSDatabase::getDescription( DbSystem system ) {
	switch( system ) {
		case VLAB_LIVE:
			return "Live data (vlab::ldb*)";
		case VLAB_TEST:
			return "Test data for software testing (vlab::test_ldb*)";
		case LABDEV_MIRROR:
			return "Mirror system for database testing (vlabdev::ldb*)";
		case LABDEV_DEV:
			return "Software development (vlabdev::t_ldb*)";
		case LABDEV_TEST:
			return "Database development testing (vlabdev::test_ldb*)";
		default:
			return "Database system not selected";
	}
}

//---------------------------------------------------------------------------
//	find or create an entry for central, distributed or project database
//---------------------------------------------------------------------------

LIMSDatabase LIMSDatabase::getCentralDb( ) {
	return LIMSDatabase( "ldbc" );
}

//---------------------------------------------------------------------------

LIMSDatabase LIMSDatabase::getProjectDb( int projID ) {
	if( projID == 0 || projID == -1 ) {
		projID = LCDbProjects::getCurrentID( );
	}
	return getProjectDb( LCDbProjects::records().get( projID ).getDbName() );
}

//---------------------------------------------------------------------------

LIMSDatabase LIMSDatabase::getProjectDb( const std::string & dbName, bool distributed ) {
	std::string root = getRootName( dbName );
	return LIMSDatabase( distributed ? root + "_ddb/star" : root );
}

//---------------------------------------------------------------------------

LIMSDatabase::LIMSDatabase( const std::string &rootName )
 : name( rootName ), xdb( NULL ), owner( UNKNOWN ) {
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
 : name( other.name ), xdb( NULL ), owner( other.owner ) {
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
//	Ignore test_ or t_ prefix (add them depending on current system)
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
		case VLAB_LIVE:
		case LABDEV_MIRROR:
			return "";
		case VLAB_TEST:
		case LABDEV_TEST:
			return "test_";
		case LABDEV_DEV:
			return "t_";
		default:
			throw Exception( "Database system not selected" );
	}
}

//---------------------------------------------------------------------------
//	chose the apropriate vnode for the given database
//---------------------------------------------------------------------------

std::string LIMSDatabase::getVNode( DbSystem system ) {
	switch( system ) {
		case VLAB_LIVE:
		case VLAB_TEST:
			return "vnode_vlab_64";

		case LABDEV_MIRROR:
		case LABDEV_TEST:
		case LABDEV_DEV:
			return "vnode_labdev_64";

		default:
			throw Exception( "Database system not selected" );
	}
}

//---------------------------------------------------------------------------

std::string LIMSDatabase::getDbName( ) const {
	DbSystem dbs = isConnected() ? owner : current;
	return getVNode( dbs ) + "::" + getPrefix( dbs ) + name;
}

//---------------------------------------------------------------------------

std::string LIMSDatabase::getConnectionName( const std::string & dbName ) {
	return getVNode( current ) + "::" + getPrefix( current ) + getRootName( dbName );
}

//---------------------------------------------------------------------------
// 	Switch to current system and connect (using installation password)
//---------------------------------------------------------------------------

XDB *LIMSDatabase::connect( bool readLocks ) {
	if( isConnected() && owner != current ) {
		xdb->close();
		delete xdb;
		xdb = NULL;
	}
	if( !isConnected() ) {
		std::string dbName = getVNode( current ) + "::" + getPrefix( current ) + name;
		xdb = new XDB( dbName );
		if( !xdb->open( ) ) {
			throw Exception( String( "Cannot open " ) + dbName.c_str( ) );
		}
		if( !readLocks ) {
			XEXEC lockMode( xdb, "set lockmode session where readlock=nolock" );
			if( !lockMode.exec( ) ) {
				throw Exception( "Error disabling read locks" );
			}
		}
		owner = current;
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

