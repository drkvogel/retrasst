/*---------------------------------------------------------------------------
 *
 *  Modified April 2001 to allow for MySQL on repository and cluster
 *  Bug fix 2 May 2001 - only use SQL to get MySQL database name
 *  Modified 26 June 2001 - no need to keep CSV file; users do logging
 *  Modified 6 September 2001 to check disk space when program starts
 *  Modified 21 November 2001 - throw exception on error (nobody checks!)
 *  Modified 28 November 2001 - add get() to implement Shared singleton
 *  Renamed 12 December 2001 to follow Shared Classes naming convention
 *  Modified 17 October 2002 to include operator ID and add log-in
 *  Modified 11 November - use copy of passed query to update database
 *  Modified 4 December - allow alternative e-mail address for write
 *  Bug fix: 4 March 2003 - allocate new process ID for each new user
 *  16 December 2003, NG - get next CID from DbShared HasNextCID class
 *  13 February, NG/AM – replace control codes in message with spaces
 *
 *	25 May 2004, NG:	Renamed & moved to Shared/LabsDAO; removed
 *						SharedParams so TQuery destroyed before TDatabase
 *	28 May 2004, NG:	Look up project, analyser and operator ID each time
 *	7 July 2004, NG:	Added simple search-for-latest function
 *	28 September, NG:	Removed write() - use addRecord and/or sendEMail
 *	24 June 2005, NG:	Bug fix - check current project before getting name
 *  3 August 2005, NG:	don't send any e-mail if passed URL is "none"
 *	24 November, NG:	added default_email - linked in from user's code
 *  18 Jan 2008, NG:	Use TIdSMTP component for mail from Builder 2007
 *  28 April, NG:		added search() method to check when user ran a program
 *  10/03/2009, NG:		Don't throw exception when e-mail cannot be sent
 *  9 April 09, NG:		Bug fix: check process ID before adding record
 *  1 July 09, NG:		Encourage wider use of pre-defined message types
 *	11 June 2012, NG	Modified for C++Builder XE2
 * 	6 Jan 2014, NG:		No need to split details: 1000 characters in db 2.7
 *-------------------------------------------------------------------------*/

#include <vcl.h>

#include "LCDbAuditTrail.h"
#include "LQuery.h"
#include "LIMSParams.h"
#include "LCDbOperator.h"
#include "LCDbProject.h"
#include "LCDbAnalyser.h"
#include "XMLFile.h"
#include "LIMSDatabase.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//	Constructor - initialise variables but do not access database
//---------------------------------------------------------------------------

LCDbAuditTrail * LCDbAuditTrail::current = NULL;

LCDbAuditTrail::LCDbAuditTrail() : cQuery( LIMSDatabase::getCentralDb() )
{
	if( current != NULL ) {
		throw Exception( "Audit trail already created" );
	}

	mailer = new TIdSMTP( NULL );
	mailer -> Host = "mailgate";

	wchar_t name[ 100 ];
	DWORD size = sizeof( name );
	if( GetUserName( name, & size ) )
		userName = AnsiString( name, size-1 ).c_str();

	size = sizeof( name );
	if( GetComputerName( name, & size ) )
		computer = AnsiString( name, size ).c_str();

	std::string title = AnsiString( Application -> Title ).c_str();
	progName = title + " v" + LIMSParams::instance().getProgVersion();
	current = this;
}

//---------------------------------------------------------------------------
//	Destructor - closes audit trail if user didn't call Stop() first
//---------------------------------------------------------------------------

LCDbAuditTrail::~LCDbAuditTrail()
{
	current = NULL;

	if( getID() != 0 ) {
		addRecord( "** Program closed **", SHUTDOWN );
		setID( 0 );
	}

	if( mailer != NULL ) {
		delete mailer;
		mailer = NULL;
	}
}

//---------------------------------------------------------------------------
//	Allow others to write to the current audit trail if there is one
//---------------------------------------------------------------------------

LCDbAuditTrail & LCDbAuditTrail::getCurrent()
{
	if( current == NULL ) {
		throw Exception( "No audit trail yet created" );
	}
	return *current;
}

//---------------------------------------------------------------------------
//	Check an audit trail has been created and a process ID allocated
//---------------------------------------------------------------------------

bool LCDbAuditTrail::hasStarted()
{
	return current != NULL && current -> getID() != 0;
}

//---------------------------------------------------------------------------
//	Get current process ID; throw exception if audit trail not started
//---------------------------------------------------------------------------

int LCDbAuditTrail::getProcessID()
{
	if( hasStarted() ) {
		return current -> getID();
	}
	throw Exception( "Process ID not yet allocated" );
}

//---------------------------------------------------------------------------
//  Allocate new process ID and create the first audit trail record
//---------------------------------------------------------------------------

void LCDbAuditTrail::start( const std::string & message )
{
	MessageType mt = STARTUP;
	if( getID() != 0 && LCDbOperators::getCurrentID() != 0 ) {
		mt = USER_LOGIN;
	}
	int id = claimNextID( cQuery );
	const LCDbProject * central = LCDbProjects::records().findByID( 0 );
	if( id > 0 && central != NULL && central->getVersion().minor <= 7 && central->getVersion().phase<2 ) {
		setID( -id );
    }
	addRecord( message, mt );
}

//---------------------------------------------------------------------------
//	Mark log as complete for this process (next write generates new ID)
//---------------------------------------------------------------------------

void LCDbAuditTrail::stop( const std::string & message )
{
	if( getID() == 0 ) {
		throw Exception( "Audit trail already stopped" );
	}
	addRecord( message, SHUTDOWN );
	setID( 0 );
}

//---------------------------------------------------------------------------
//	Find out which database the audit trail is being written into
//---------------------------------------------------------------------------

std::string LCDbAuditTrail::getIngresInfo( const std::string & param )
{
	std::string value;
	cQuery.setSQL( "select dbmsinfo( :var )" );
	cQuery.setParam( "var", param );
	if( cQuery.open() == 1 ) {
		value = cQuery.readString( 0 );
	}
	return value.empty() ? param : value;
}

//---------------------------------------------------------------------------
//	Private class to split out consecutive lines from a long message
//---------------------------------------------------------------------------
/*
struct LCDbAuditTrail::LineSplitter
{
	const char * ch;
	LineSplitter( const char * start ) : ch( start )
	{}

	//	skip control characters; return true if more text found
	bool hasMore()
	{
		while( *ch != '\0' ) {
			if( *ch > ' ' ) {
				return true;
			}
			ch ++;
		}
		return false;
	}

	//	replace control characters with spaces; return next line
	std::string getNext()
	{
		std::string pending;
		for( ;; ) {
			while( *ch <= ' ' )	{
				if( *ch == '\0' || *ch == '\n' ) {
					return pending;
				}
				ch ++;
			}
			const char * start = ch;
			while( *ch > ' ' ) {
				ch ++;
			}
			std::string word( start, ch - start );
			if( pending.empty() ) {
				pending = word;
			} else {
				pending = pending + ' ' + word;
			}
		}
	}
};
*/
//---------------------------------------------------------------------------
//	Write the message to the audit trail in the central database
//---------------------------------------------------------------------------

bool LCDbAuditTrail::addRecord( const std::string & message, MessageType type )
{
	if( type == MAJOR_ERROR ) {
		sendEMail( message );
	}
	if( getID() == 0 ) {
		start( "Start-up error; starting audit trail" );
	}
	std::string dbName;
	int projID = LCDbProjects::getCurrentID();
	if( projID != LCDbProject::NONE_SELECTED ) {
		const LCDbProject * proj = LCDbProjects::records().findByID( projID );
		if( proj != NULL ) {
			dbName = proj -> getDbName();
		}
	}
	if( dbName.empty() ) {
		dbName = getIngresInfo( "database" );
	}
	cQuery.setSQL( "INSERT into c_audit_trail (message_date, process_cid, operator_cid,"
				   " message_type, machine_cid, database_name, program_name, details)"
				   " values ('now', :pID, :oID, :mType, :mID, :udbn, :uPName, :uMessage)" );
	cQuery.setParam( "pID", getID() );
	cQuery.setParam( "oID", LCDbOperators::getCurrentID() );
	cQuery.setParam( "mType", type );
	cQuery.setParam( "mID", LCDbAnalysers::getCurrentID() );
	cQuery.setParam( "udbn", dbName );
	cQuery.setParam( "uPName", progName );

/*	int records = 0;
	LineSplitter lines( message.c_str() );
	std::string part = lines.getNext();
	while( lines.hasMore() ) {
		// concatanate short lines; write extra record if no room
		std::string next = lines.getNext();
		if( part.length() + next.length() < MAX_MESSAGE ) {
			part = part + "; " + next;
		} else {
			cQuery.setParam( "uMessage", part );
			if( cQuery.execSQL() ) {
				records ++;
			}
			part = next;
		}
	}

	if( !part.empty() ) {
		cQuery.setParam( "uMessage", part );
		if( cQuery.execSQL() ) {
			records ++;
		}
	}
	*/
	cQuery.setParam( "uMessage", message );
	return cQuery.execSQL();
}

//---------------------------------------------------------------------------
//	Send an e-mail unless address is "none" - default given by constructor
//---------------------------------------------------------------------------

bool LCDbAuditTrail::sendEMail( const std::string & body, std::string address, std::string subject )
{
	if( address.length() < 3 ) {
		address = default_email;
	}
	if( address.empty() || address.compare( "none" ) == 0 ) {
		return false;
	}
	if( subject.empty() ) {
		subject = "Labs system alert";
	}
	try {
		std::auto_ptr< TIdMessage > message( new TIdMessage( NULL ) );
		message -> Recipients -> EMailAddresses = address.c_str();
		message -> From -> Address = (userName + "@" + computer).c_str();
		message -> Subject = subject.c_str();
		message -> Body -> Text = String( progName.c_str() ) + ", " + Now() + '\n';
		message -> Body -> Add( body.c_str() );
		mailer -> Connect();
		mailer -> Send( message.get() );
		mailer -> Disconnect();
		return true;
	}
	catch( ... ) {
		XMLFile::logError( "not-sent", body );
		return false;
	}
}

//---------------------------------------------------------------------------
//  Search audit trail for most recent entry for the current program
//---------------------------------------------------------------------------

TDateTime LCDbAuditTrail::findRecent( MessageType status )
{
	TDateTime when;
	std::string program = AnsiString( Application -> Title + "%" ).c_str();
	cQuery.setSQL( "select max(message_date) as last_time from c_audit_trail"
				  " where program_name like :pn and message_type = :mt" );
	cQuery.setParam( "pn", program );
	cQuery.setParam( "mt", status );
	if( cQuery.open() ) {
		when = cQuery.readDateTime( 0 );
	}
	return when;
}

//---------------------------------------------------------------------------
//  Search audit trail for recent entries for the current program
//---------------------------------------------------------------------------

std::vector< LCDbAuditTrail::Process > LCDbAuditTrail::read( const XTIME & first, const XTIME & last, int projID )
{
	const LCDbProject * proj = LCDbProjects::records().findByID( projID );
	std::string dbName;
	if( proj == NULL ) { dbName = "%"; } else { dbName = proj -> getDbName(); }
	std::string program = AnsiString( Application -> Title + "%" ).c_str();
	cQuery.setSQL( "select process_cid, operator_cid, machine_cid, program_name,"
					" min(message_date) as start_time, max(message_date) as end_time"
				  " from c_audit_trail where process_cid in"
				  " (select process_cid from c_audit_trail"
					  " where program_name like :pn and database_name like :db"
					  " and message_date between :df and :dl)"
				  " group by process_cid, program_name, operator_cid, machine_cid"
				  " order by process_cid" );
	cQuery.setParam( "df", first );
	cQuery.setParam( "dl", last );
	cQuery.setParam( "pn", program );
	cQuery.setParam( "db", dbName );
	std::vector< Process > found;
	for( cQuery.open(); !cQuery.eof(); cQuery.next() )
		found.push_back( Process( cQuery ) );
	return found;
}

//---------------------------------------------------------------------------

LCDbAuditTrail::Process::Process( const LQuery & data )
 : processID( data.readInt( "process_cid" ) ),
   operatorID( data.readInt( "operator_cid" ) ),
   program( data.readString( "program_name" ) ),
   buddyID( data.readInt( "machine_cid" ) ),
   start( data.readDateTime( "start_time" ) ),
   finish( data.readDateTime( "end_time" ) )
{}

//---------------------------------------------------------------------------
//  Search audit trail for operator logged in for the given process
//---------------------------------------------------------------------------

int LCDbAuditTrail::findUserID( int processID )
{
	static std::map< int, int > history;
	std::map< int, int >::const_iterator found = history.find( processID );
	if( found != history.end() ) {
		return found -> second;
	}
	cQuery.setSQL( "select distinct operator_cid from c_audit_trail where process_cid = :pid" );
	cQuery.setParam( "pid", processID );
	if( cQuery.open() != 1 ) {
		return 0;
    }
	std::pair< int, int > entry( processID, cQuery.readInt( 0 ) );
	history.insert( entry );
	return entry.second;
}

//---------------------------------------------------------------------------

