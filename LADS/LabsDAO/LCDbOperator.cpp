//---------------------------------------------------------------------------
//
//	Version history:
//
//		7/6/05, NG:		Added nearExiry(): true if end date < 1 month away
//      18 July, NG:    Lock user out after three consecutive failed logins
//  	3 August 2005:	Don't allow update of external_name once created
//      20 May 08, NG:	Add permissions for users to run (parts of) programs
//
//---------------------------------------------------------------------------

#include <vcl.h>
#include "LCDbOperator.h"
#include "LQuery.h"
#include "xmd5.h"
#include "LCDbAnalyser.h"
#include "LCDbProject.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//  Read list of current staff; give preference to encrypted passwords
//---------------------------------------------------------------------------

LCDbOperator::LCDbOperator( const LQuery & query  )
 : LCDbID( query.readInt( "operator_cid" ) ),
   LDbNames( query.readString( "external_name" ),
			 query.readString( "external_full" ) ),
   LDbValid( query.readDateTime( "valid_from" ),
			 query.readDateTime( "valid_to" ),
			 query.readInt( "status" ) ),
   passCode( query.readString( "password_md5" ) )
{}

//---------------------------------------------------------------------------

bool LCDbOperators::read( LQuery cQuery, bool readAll )
{
	if( readAll )
		cQuery.setSQL( "select * from c_operator order by operator_cid" );
	else
	{	cQuery.setSQL( "select * from c_operator"
					  " where status <> :sts and valid_from < 'now' "
					  " and (valid_to < valid_from or valid_to > 'now')"
					  " order by operator_cid" );
		cQuery.setParam( "sts", LCDbOperator::DELETED );
	}
	if( !readData( cQuery ) )
		return false;

	if( readAll )
		cQuery.setSQL( "select * from c_permission order by operator_cid" );
	else
	{	cQuery.setSQL( "select * from c_permission where status <> :sts"
					  " order by operator_cid" );
		cQuery.setParam( "sts", LCDbOperator::DELETED );
	}

	Iterator ci = begin();
	for( cQuery.open(); !cQuery.eof(); cQuery.next() )
	{
		int oid = cQuery.readInt( "operator_cid" );
		while( ci != end() && ci -> getID() < oid )
			ci ++;

		if( ci != end() && ci -> getID() == oid )
			ci -> addPermission( cQuery );
	}
	return true;
}

//---------------------------------------------------------------------------

LCDbOperator::Priv::Priv( const LQuery & cQuery )
 : program( cQuery.readInt( "program_cid" ) ),
   project( cQuery.readInt( "project_cid" ) ),
   machine( cQuery.readInt( "machine_cid" ) ),
   page( cQuery.readInt( "page_number" ) ),
   status( cQuery.readInt( "status" ) )
{}

//---------------------------------------------------------------------------

void LCDbOperator::setPassword( const std::string & password ) {
	if( password.length() < 3 )
		throw Exception( "Password must be at least 3 characters" );
	passCode = encrypt( password );
}

//---------------------------------------------------------------------------

bool LCDbOperator::matchPassword( const std::string & password ) const {
	return passCode.empty() || compareIC( encrypt( password ), passCode );
}

//---------------------------------------------------------------------------

bool LCDbOperator::isActive() const {
	return LDbValid::isActive() && !hasLockedAccount();
}

//---------------------------------------------------------------------------

void LCDbOperator::addPermission( Priv allowed ) {
	permissions.push_back( allowed );
}

//---------------------------------------------------------------------------

std::string LCDbOperator::encrypt( const std::string & pass )
{
	char * lc = new char[ pass.length() + 2 ];
	unsigned n;
	for( n = 0; n < pass.length(); n ++ ) {
		lc[ n ] = tolower( pass[ n ] );
	}
	lc[ n ] = '\0';

	XMD5_DIGEST md5;
	std::string result;
	if( XMD5::calcSz( &md5, lc ) ) {
		result = md5.hex;
	} else {
		result = std::string( lc, n );
	}
	delete[] lc;
	return result;
}

//---------------------------------------------------------------------------

bool LCDbOperator::hasUsedPassword( LQuery query, const std::string & password ) const
{
	query.setSQL( "select count(*) from archive_c_operator"
				 " where operator_cid = :oid and password_md5 = :md5" );
	query.setParam( "oid", getID() );
	query.setParam( "md5", encrypt( password ) );
	return query.open() > 0 && query.readInt( 0 ) > 0;
}

//---------------------------------------------------------------------------

bool LCDbOperator::needsPassword() const
{
	return status == NEW_RECORD || status == NEW_PASSWORD || passCode.empty() || isNearExpiry();
}

//---------------------------------------------------------------------------
//  Check name and (encrypted) password (friend of LCDbOperator)
//---------------------------------------------------------------------------

class /* LCDbOperators:: */ PasswordMatcher : public std::unary_function< LCDbOperator, bool >
{
	std::string name, password;

public:

	PasswordMatcher( const std::string & user, const std::string & pass )
	 : name( user ), password( pass )
	{}

	operator std::string() const { return name; }

	bool operator() ( const LCDbOperator & other ) const
	{
		return (name.compare( other.getName() ) == 0
			 || name.compare( other.getDescription() ) == 0)
			 && other.matchPassword( password );
	}
};

//---------------------------------------------------------------------------
//	Match name and password; return pointer if OK; fail after 3 attempts
//---------------------------------------------------------------------------

const LCDbOperator * LCDbOperators::check( const std::string & name, const std::string & pass ) const
{
	static int failCount = 0;
	const LCDbOperator * found = findMatch( PasswordMatcher( name, pass ) );
	if( found != NULL )
		failCount = 0;
	else if( ++ failCount > 2 )
		throw Exception( String(name.c_str()) + " failed to log in - program locked" );

	return found;
}

//---------------------------------------------------------------------------

const LCDbOperator * LCDbOperators::findByName( const std::string & name ) const {
		return findMatch( LDbNames::LCMatcher( name ) );
}

//---------------------------------------------------------------------------
//	Create a new database entry for this operator and copy into cache
//---------------------------------------------------------------------------

bool LCDbOperator::saveRecord( LQuery query )
{
	if( saved )
	{
		query.setSQL( "Update c_operator"
					 " set external_full = :full, password_md5 = :pwmd5,"
					 " valid_from = :from, valid_to = :to, status = :status"
					 " where operator_cid = :cid" );
	}
	else
	{	setName( LCMatcher( getName() ) );
		claimNextID( query );
		query.setSQL( "INSERT into c_operator( operator_cid, external_name,"
					 " external_full, valid_from, valid_to, status, password_md5 )"
					 "values ( :cid, :name, :full, :from, :to, :status, :pwmd5 )" );
		query.setParam( "name", getName() );
	}

	query.setParam( "cid", getID() );
	query.setParam( "full", getDescription() );
	query.setParam( "pwmd5", passCode );
	std::pair< XTIME, XTIME > dates = getValidDates();
	query.setParam( "from", dates.first );
	query.setParam( "to", dates.second );
	query.setParam( "status", status );
	if( !query.execSQL() )
		return false;

	saved = true;
	LCDbOperators::records().insert( *this );
	return true;
}

//---------------------------------------------------------------------------
//	Does this user have permission to run the program here
//---------------------------------------------------------------------------

bool LCDbOperator::canRun( int project, int buddy, short page ) const
{
	if( buddy == 0 ) {
		buddy = LCDbAnalysers::getCurrentID();
	}

	for( Range< Priv > pi = permissions; pi.isValid(); ++ pi ) {
		if( (pi -> program == LCDbProjects::leaseID)
		 && (pi -> status != LCDbOperator::DELETED)
		 && (project == pi -> project || pi -> project == 0)
		 && (buddy == pi -> machine || buddy == 0 || pi -> machine == 0)
		 && (page == pi -> page || page == 0 || pi -> page == 0) )
			return true;
	}
	return false;
}

//---------------------------------------------------------------------------

