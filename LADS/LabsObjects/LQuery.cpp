/* ---------------------------------------------------------------------------
 *
 *	LQuery - SQL logging for TQuery::Open() and ExecSQL()
 *
 *  Bug fix 24/1/2001 - don't just check file size at start-up
 *  Modified 20/6/2001 to write to the file asynchronously
 *  Modified 27/6/2001 to separate out the XML LogFile class
 *  Modifed 28 November to provide Open and ExecSQL methods
 *  6/12/2001 - Enclosed XMLFile in structure to ensure cleanup
 *
 *	14 May 2004, NG:    Moved .cpp & .h to Shared/LabUtils
 *  18 June, NG/AM:     Checked in to CVS/GeneralClasses/CoreUtilities
 *  12 October 2004:	Passed member pointer directly to getResult()
 *  2 December 2004:	Add methods to set parameters and read fields
 *  8 March 2005:		Allow assignment unless query is active
 *  13 October, NG:		Use strings for date/times to avoid ODBC problems
 *	8 November 2005:	Only use strings for MySQL/Ingres, not Paradox
 *	14 May 2009, NG:	Record dsn as well as database used
 *	2 June 2009, NG:	Record slow queries even when debug is off
 *	17 Dec 2010, NG:	Create new query if required, linked to a TDatabase
 *
 *	8 June 2012:		Re-written to use LIMSDatabase instead of the BDE
 *	26/06/12, NG:		Log record count when query closes (unknown until then)
 *	21/8/12, NG:		Add copy constructor that doesn't copy any results
 *  15 Jan 2013, NG:    Allow for null fields (using Alan's latest XSQL)
 *	28/2/2013, NG:		Use XTIME in place of TDateTime to prevent implicit
 *						conversion from Borland String parameters
 *	18/4/2013, NG:		Use '' for date/times when values invalid/not set
 --------------------------------------------------------------------------- */

#include <System.hpp>
#include <SysUtils.hpp>
#include <sstream>

#include "LQuery.h"
#include "XMLFile.h"
#include "LIMSDatabase.h"
#include "xquery.h"
#include "xexec.h"
#include "StringUtil.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

#ifdef _DEBUG
LQuery::LogLevel LQuery::logLevel = LQuery::SELECTS;
#else
LQuery::LogLevel LQuery::logLevel = LQuery::UPDATES;
#endif

//---------------------------------------------------------------------------

XMLFile &LQuery::getLog( ) {
	static XMLFile history( "sql" );
	return history;
}

//---------------------------------------------------------------------------

LQuery::LQuery( const LIMSDatabase & data )
 : db( data ), cursor( NULL ), logging( false ), rows( 0 ) {
}

//---------------------------------------------------------------------------

LQuery::LQuery( const LQuery & other )
 : db( other.db ), sql( other.sql ), parameters( other.parameters ), cursor( NULL ), logging( false ), rows( 0 ) {
}

//---------------------------------------------------------------------------

LQuery::~LQuery( ) {
	dropCursor( );
}

//---------------------------------------------------------------------------
//	Close previous query if necessary and get ready to prepare a new one
//---------------------------------------------------------------------------

void LQuery::setSQL( const std::string &query ) {
	dropCursor( );
	parameters.clear( );
	sql = query;
}

//---------------------------------------------------------------------------

void LQuery::setParam( const std::string &pName, int value ) {
	parameters.setInt( pName, value );
}

//---------------------------------------------------------------------------

void LQuery::setParam( const std::string &pName, double value ) {
	parameters.setReal( pName, value );
}

//---------------------------------------------------------------------------

void LQuery::setParam( const std::string &pName, const std::string &value ) {
	parameters.setString( pName, value );
}

//---------------------------------------------------------------------------

// Ingres, C, XTIME and the labs databases understand recent dates
static const XDATE EPOCH_START( 1980, 1, 1 ), EPOCH_END( 2037, 12, 31 );

void LQuery::setParam( const std::string &pName, const XDATE & value ) {
	if( value.isValid() && value > EPOCH_START && value < EPOCH_END ) {
		parameters.setDate( pName, value );
	} else {
		parameters.setString( pName, "" );
	}
}

//---------------------------------------------------------------------------

void LQuery::setParam( const std::string &pName, const XTIME & value ) {
	if( value.isValid() && value > EPOCH_START && value < EPOCH_END ) {
		parameters.setTime( pName, value );
	} else {
		parameters.setString( pName, "" );
	}
}

//---------------------------------------------------------------------------
//	Close an existing query and report the number of rows read
//---------------------------------------------------------------------------

void LQuery::dropCursor( ) {
	if( cursor != NULL ) {
		if( logging ) {
			logCount( rows );
		}
		if( cursor->isOpen( ) ) {
			cursor->close( );
		}
		delete cursor;
		cursor = NULL;
	}
}

//---------------------------------------------------------------------------
//	BDE-like functions to execute a query, logging the result if necessary
//---------------------------------------------------------------------------

bool LQuery::open( ) {
	return call( &LQuery::doOpen, SELECTS );
}

//---------------------------------------------------------------------------

bool LQuery::doOpen( ) {
	cursor = new XQUERY( db.connect( ), sql );
	cursor -> setAcceptNull( true );
	cursor -> setParamSource( &parameters );
	rows = 0;
	db.confirm( cursor -> open( ) );
	return doNext( );
}

//---------------------------------------------------------------------------

bool LQuery::execSQL( ) {
	return call( &LQuery::doExec, UPDATES );
}

//---------------------------------------------------------------------------

bool LQuery::doExec( ) {
	XEXEC query( db.connect( ), sql );
	query.setParamSource( &parameters );
	db.confirm( query.exec( ) );
	rows = query.getNRows( );
	if( logging ) {
		logCount( rows );
	}
	return rows > 0;
}

//---------------------------------------------------------------------------

bool LQuery::next( ) {
	return call( &LQuery::doNext, SELECTS );
}

//---------------------------------------------------------------------------

bool LQuery::doNext( ) {
	if( cursor != NULL && cursor->isOpen() ) {
		cursor->result.clear( );
		bool hasData = cursor -> fetch();
		db.confirm( true );
		if( hasData ) {
			rows ++;
			return true;
		}
	}
	dropCursor();
	return false;
}

//---------------------------------------------------------------------------

bool LQuery::eof( ) {
	if( cursor != NULL && cursor->isOpen() ) {
		return cursor->result.isEmpty() && !next();
	} else {
		return true;
	}
}

//---------------------------------------------------------------------------
//	Log the current query if required, execute it and log the result
//---------------------------------------------------------------------------

bool LQuery::call( Operation function, LogLevel log ) {
	if( logLevel >= log && !logging ) {
		//logQuery( );
		logging = true;
	}
	try {
		return ( this->*function )( );
	}
	catch( Exception &ex ) {
		logError( bcsToStd( ex.Message ) );
		throw;
	}
	catch( std::string &ex ) {
		logError( ex );
		throw;
	}
	catch( std::exception &ex ) {
		logError( ex.what( ) );
		throw;
	}
	catch( ... ) {
		logError( "Unrecognised exception" );
		throw;
	}
}

//---------------------------------------------------------------------------
//	List the SQL and any parameters if the logging level is high enough
//---------------------------------------------------------------------------

void LQuery::logQuery( ) {
	XMLFile &log = getLog( );
	log.start( "query" );
	log.addAttribute( "database", db.getDbName( ) );
	log.addAttribute( "when", Now( ) );
	log.addText( sql + '\n' );
	for( int i = 0; i < parameters.count( ); i++ ) {
		log.start( "param" );
		log.addAttribute( "name", parameters.getName( i ) );
		log.addText( parameters.getString( i ) );
		log.endTag( );
	}
}

//---------------------------------------------------------------------------
//  Record result in the log file if already logging or after an error
//---------------------------------------------------------------------------

void LQuery::logError( const std::string &message ) {
	if( !logging ) {
		logQuery( );
	}
	//logResult( "error", message );
}

//---------------------------------------------------------------------------

void LQuery::logCount( int records ) {
	std::stringstream out;
	out << records;
	//logResult( "records", out.str( ) );
}

//---------------------------------------------------------------------------

void LQuery::logResult( const char *tag, const std::string &result ) {

	XMLFile &log = getLog( );
	log.start( tag );
	log.addAttribute( "when", Now( ) );
	log.addText( result );
	log.endTag( );
	logging = false;
	log.endTag( );
}

//---------------------------------------------------------------------------
//  Return data of the selected type from the given field, if available
//---------------------------------------------------------------------------

const ROSETTA &LQuery::getRecord( ) const {
	if( cursor == NULL ) {
		throw Exception( "Error: no result data available" );
	} else {
		return cursor->result;
	}
}

//---------------------------------------------------------------------------

bool LQuery::fieldExists( const std::string &field ) const {
	if( cursor == NULL ) {
		return false;
	}
	const ROSETTA & record = cursor -> result;
	if( !record.exists( field ) ) {
		return false;
	}
	return !record.hasTag( field, XSQL::null );
}

//---------------------------------------------------------------------------

int LQuery::readInt( int index ) const {
	return getRecord( ).getInt( index );
}

//---------------------------------------------------------------------------

int LQuery::readInt( const std::string &field ) const {
	return getRecord( ).getInt( field );
}

//---------------------------------------------------------------------------

double LQuery::readDouble( int index ) const {
	return getRecord( ).getReal( index );
}

//---------------------------------------------------------------------------

double LQuery::readDouble( const std::string &field ) const {
	return getRecord( ).getReal( field );
}

//---------------------------------------------------------------------------

std::string LQuery::readString( int index ) const {
	return getRecord( ).getString( index );
}

//---------------------------------------------------------------------------

std::string LQuery::readString( const std::string &field ) const {
	return getRecord( ).getString( field );
}

//---------------------------------------------------------------------------

TDateTime LQuery::readDateTime( int index ) const {
	const ROSETTA & record = getRecord( );
	if( record.isTime( index ) ) {
		return record.getTime( index ).outputTDateTime();
	} else if( record.isDate( index ) ) {
		return record.getDate( index ).outputTDateTime();
	} else {
		return 0;
	}
}

//---------------------------------------------------------------------------

TDateTime LQuery::readDateTime( const std::string &field ) const {
	const ROSETTA & record = getRecord( );
	if( record.isTime( field ) ) {
		return record.getTime( field ).outputTDateTime();
	} else if( record.isDate( field ) ) {
		return record.getDate( field ).outputTDateTime();
	} else {
		return 0;
	}
}

//---------------------------------------------------------------------------
