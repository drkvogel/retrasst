#include <algorithm>
#include <string.h>
#include "randutil.h"
#include "xbasic.h"
#include "xcgi.h"
#include "xdb.h"
#include "xquery.h"
#include "xexec.h"
#include "xencode.h"
#include "xmd5.h"
#include "randdefs.h"
#ifdef __BORLANDC__
#else
#include <sys/utsname.h>
#endif
//===========================================================================
#ifndef __BORLANDC__
const	std::string	RAND_UTIL::db_root = "erg_trials";
#else
const	std::string	RAND_UTIL::db_root = "dice_erg::erg_trials";
#endif
//===========================================================================
const	std::string	RAND_UTIL::privUserManager = "UM";
const	std::string	RAND_UTIL::privUserViewer  = "UV";
const	std::string	RAND_UTIL::privMenuManager = "MM";
const	std::string	RAND_UTIL::privMenuViewer  = "MV";
const	std::string	RAND_UTIL::privControlManager = "CM";
//===========================================================================
const	int	RAND_UTIL::permissable_consecutive_failures = 10;
//===========================================================================
#define	RANDUTIL_DATA	"ru_dsr"
//===========================================================================
RAND_UTIL::RAND_UTIL( XCGI *cg )
	:
	XERROR(),
	c( cg ),
	tdb( NULL ),
	id( -1 ),
#ifdef RANDLIVE
	is_live( true ),
	sys( "trials.ctsu.ox.ac.uk" ),
#else
	is_live( false ),
	sys( "dice.ctsu.ox.ac.uk" ),
#endif
	show_detail( false ),
	staff( NULL ),
	priv( NULL ),
	token( NULL )
{
	tstart = time( NULL );
	cstart = clock();
	if ( NULL == cg )
		{setInvalid( "Null XCGI" );
		}
	const std::string hex = cg->getParamDefault( MENU_PARAM_ROSETTA, "" );
	std::string	ser;
	if ( ! hex.empty() && XENCODE::hexToSz( hex, &ser ) )
		{webprm.serializeIn( ser );
		}
	if ( ! webprm.isValid() )
		{setInvalid( "Invalid webprm" );
		}
	const	std::string	d64 = cg->getParamDefault( RANDUTIL_DATA, "" );
	std::string	dsr;
	if ( ! d64.empty() && XENCODE::b64ToSz( d64, &dsr ) )
		{data.serializeIn( dsr );
		}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
RAND_UTIL::~RAND_UTIL( void )
{
	if ( NULL != tdb && tdb->isOpen() )
		{tdb->close();
		}
	XDELETE( tdb );
}
//---------------------------------------------------------------------------
std::string RAND_UTIL::determineCallerIP( void )
{
	if ( ip.empty() )
		{
#ifdef __BORLANDC__
		ip = "127.0.0.1";		// IPv4 LOCALHOST
#else
		ip = ROSETTA::compressString( c->getEnvVar("REMOTE_ADDR") );
#endif
		}
	return( ip );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string RAND_UTIL::getComputerName( void )
{
#ifdef __BORLANDC__
	return( "UNDETERMINED" );
#else
	struct  utsname u;
	if ( 0 != uname( &u ) )
		{return( "UNKNOWN" );
		}
	return( std::string( u.nodename ) );
#endif
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string RAND_UTIL::checkAlignment( void )
{
	const	std::string	cn = getComputerName();
	if ( cn == sys )
		{return( "" );	// GOOD
		}
	const	std::string	nota = std::string( isLive() ? "Live" : "Test" )
		+ " system running on " + cn + ", expected " + sys;
	return( nota );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool RAND_UTIL::isLive( void )
{
	return( is_live );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int RAND_UTIL::getId( void )
{
	return( -1 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string RAND_UTIL::getWebRoot( void ) const
{
	return( std::string( "https://" ) + sys + "/" );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XCGI *RAND_UTIL::getCGI( void ) const
{
	return( c );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool RAND_UTIL::showDetail( void )
{
/*
	if ( ! detail_known )
		{show_detail = c->paramExists( WEB_PARAM_SHOWDETAIL );
		detail_known = true;
		}
*/
	return( show_detail );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void RAND_UTIL::errorMsg( const std::string etxt )
{
	fflush( stdout );
	const	std::string	e = XCGI::makeWebSafe( etxt );
	printf( "\n<!--\nERROR %s\n-->", e.c_str() );
	fflush( stdout );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool RAND_UTIL::dbErr( const std::string caller, const int instance,
	const int ecount, const int ecode, const std::string etxt )
{
	fflush( stdout );
	const	std::string	e = XCGI::makeWebSafe( etxt );
	printf( "\n<!--\nDATABASE ERROR inst=%d, ecnt=%d, ecod=%d\n%s\n-->",
		instance, ecount, ecode, e.c_str() );
	fflush( stdout );
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define	IISYS_NAME	"II_SYSTEM"
#define	IISYS_VAL	"/dbsystem/II"
XDB *RAND_UTIL::openDB( const std::string root )
{
	char	*e = getenv( IISYS_NAME );      // BODGE FOR SUEXEC
	static	char	def[100] = IISYS_NAME "=" IISYS_VAL;
	if ( NULL == e || strlen(e) < 4 )
		{putenv( def );
		}
	std::string	name = root + ( ( is_live ) ? "_live" : "_test" );
	XDB	*xdb = new XDB( name );
	xdb->setErrorCallBack( dbErr );
	if ( ! xdb->isValid() )
		{setInvalid( "Invalid database" );
		XDELETE( xdb );
		}
	else if ( ! xdb->open() )
		{setInvalid( "Database failed to open" );
		XDELETE( xdb );
		}
	return( xdb );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XDB *RAND_UTIL::getTrialsDB( void )
{
	if ( tdb != NULL )
		{return( tdb );
		}
	tdb = openDB( db_root );
	return( tdb );
}
//---------------------------------------------------------------------------
bool RAND_UTIL::privelegedIP( void )
{
	const	std::string	caller_ip = determineCallerIP();
	if ( caller_ip.size() < 7 )
		{return( false );		// MAL-FORMED OR MISSING
		}
	if ( 0 == strncmp( caller_ip.c_str(), "10.", 3 ) )
		{return( true );		// PERMIT LOCAL ACCESS
		}
	if ( NULL == getTrialsDB() )
		{		// ERROR CONNECTING TO DB
		return( false );
		}
	if ( ip_priveleged.empty() )
		{
		XQUERY	qp( tdb,
			"SELECT ip_addr FROM ip_control WHERE status=0" );
		if ( qp.open() )
			{while ( qp.fetch() )
				{ip_priveleged.push_back(ROSETTA::compressString
					 ( qp.result.getString( 0 ) ) );
				}
			qp.close();
			}
		}
	std::vector<std::string>::const_iterator  pi = ip_priveleged.begin();
	while ( pi != ip_priveleged.end() )
		{if ( caller_ip == *pi )
			{return( true );
			}
		pi++;
		}
	return( false );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool RAND_UTIL::approvedIP( void )
{
	if ( privelegedIP() )
		{return( true );
		}
	if ( XCGI::methodString == c->getMethod()
			|| XCGI::methodFile == c->getMethod() )
		{printf(
		"\n<br>approvedIP() bypassed for non-cgi calling method.<br>" );
		return( true );
		}
	std::string	caller_ip = determineCallerIP();
	if ( caller_ip.empty() )
		{caller_ip = "null";
		}
	printf( "\nYour IP (%s) is not authorised for this function.",
			caller_ip.c_str() );
	return( false );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string RAND_UTIL::whoAmI( void ) const
{
	XDB	idb( "iidbdb" );
	if ( ! idb.open() )
		{return( "IIDBDB failed to open" );
		}
	XQUERY	qwho( &idb, "SELECT dbmsinfo('username')" );
	std::string	un = qwho.fetchString( "Fetch-failure in qwho");
	idb.close();
	return( un );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string RAND_UTIL::prettyInt( int n )
{			// FORMAT INTEGER WITH COMMAS TO AID READABILITY
	int	rem;
	char	thou_buf[10];
	bool	positive = ( n >= 0 );
	std::string	pretty;
	if ( ! positive )
		{n = -n;
		}
	do
		{rem = n % 1000;
		n /= 1000;
		sprintf( thou_buf, ( ( n > 0 ) ? ",%3.3d" : "%d" ), rem );
		pretty = std::string( thou_buf ) + pretty;
		}
		while ( n > 0 );
	if ( ! positive )
		{pretty = std::string( "-" ) + pretty;
		}
	return( pretty );
}
//---------------------------------------------------------------------------
void RAND_UTIL::htmlBeginDoc( const std::string title,
	const std::vector<std::string> style_sheets )
{
	printf( "<!DOCTYPE HTML PUBLIC"
		" \"-//W3C//DTD HTML 4.01 Transitional//EN\""
		" \"http://www.w3.org/TR/html4/loose.dtd\">"
		"\n<html lang=\"en\">\n<head>"
		"\n<META HTTP-EQUIV=\"Pragma\" CONTENT=\"no-cache\">"
		"\n<META HTTP-EQUIV=\"Expires\" CONTENT=\"-1\">" );
	if ( ! title.empty() )
		{const	std::string	tsafe = XCGI::makeWebSafe( title );
		printf( "\n<title>%s</title>", tsafe.c_str() );
		}
	std::vector<std::string>::const_iterator    si = style_sheets.begin();
	while ( si != style_sheets.end() )
		{printf(
		"\n<link rel=\"stylesheet\" href=\"%s\" type=\"text/css\">",
			si->c_str() );
		si++;
		}
	const   std::string     caller_ip = XCGI::makeWebSafe(
		determineCallerIP() );
	printf( "\n<!-- Caller IP %s -->", caller_ip.c_str() );
	printf( "\n</head>" );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void RAND_UTIL::htmlBeginDocControl( const std::string title )
{
	std::vector<std::string>	sty;
	sty.push_back( "rand.css" );
	htmlBeginDoc( title, sty );
}
//---------------------------------------------------------------------------
void RAND_UTIL::htmlBeginBody( void )
{
	std::string	wr = getWebRoot();
	XTIME	tnow;
	tnow.setNow();
	std::string	mn = tnow.getMonthAbbrev();
	printf( "\n<body onLoad=\"initValue()\" >\n<div class=\"upper\">"
        "\n<div class=\"univ_link\">"
"\n<a class=\"ctsu_link\" title=\"Link to Oxford University\" href=\"http://www.ox.ac.uk/\">"
"<img src=\"oxfordlogo.gif\"></a>"
	"\n</div>"
	"\n<div class=\"ctsu_title\">"
	"<h2 class=\"ctsu_title\">"
"<a class=\"ctsu_link\" title=\"Link to CTSU\" href=\"http://www.ctsu.ox.ac.uk/\">"
"Clinical Trial Service Unit and Epidemiological Studies Unit</a></h2>"
"\n<h3 class=\"ctsu_title\">"
"<a class=\"ctsu_link\" title=\"Link to CTSU Randomization\" href=\"%s\">"
"Randomization and Management Facility</a></h3>"
"\n<span class=\"tnow\">%d %s %4.4d, %2.2d:%2.2d:%2.2d</span>%s</div>"
"\n</div><div class=\"spacer\"></div>"
"\n<div class=\"main\">",
		wr.c_str(),
		tnow.getDay(), mn.c_str(), tnow.getYear(),
		tnow.getHour(), tnow.getMinute(), tnow.getSecond(),
		( isLive() ? ""
		: " <span style=\"color:#FFFF00\">TEST-SYSTEM</span>" ) );

}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void RAND_UTIL::htmlEndDoc( void )
{
	const	std::string	ca = checkAlignment();
	const	std::string	cs = getComputerName();
	if ( ! ca.empty() )
		{printf( "\n<hr>\n<span style=\"color:#FF0000\">"
			"WARNING : %s</span>", ca.c_str() );
		}
	printf( "\n<!-- (c) CTSU, Oxford University "
		__DATE__ " " __TIME__ " %s %s -->"
		"\n</body>\n</html>",
		( isLive() ? "L" : "T" ), cs.c_str() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void RAND_UTIL::htmlEndBody( void )
{
	printf( "\n</div>" );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void RAND_UTIL::showErrorHTML( const std::string msg, const std::string err )
{
	printf( "\n<!--ERROR:%s-->\n", err.c_str() );
	fflush( stdout );
/*
	const	std::string	html_msg = makeWebText( msg );
	printf( "\n<div class=\"box\">"
		"<span style=\"color: #FF0000; font-size: 120%;\" >"
		"%s</span></div>", html_msg.c_str() );
*/
	fflush( stdout );
}
//---------------------------------------------------------------------------
bool RAND_UTIL::hasToken( void )
{
	if ( NULL != token )
		{return( true );
		}
	const	std::string tk_hex = c->getParamDefault( "tk", "" );
	if ( tk_hex.empty() )
		{return( false );
		}
	std::string	tk_ser;
	if ( ! XENCODE::hexToSz( tk_hex, &tk_ser ) )
		{return( false );
		}
	if ( NULL == getTrialsDB() )
		{return( false );
		}
	token = new ROSETTA( tk_ser );
	if ( ! token->isValid() )
		{XDELETE( token );
		return( false );
		}
	XQUERY	qt( tdb, "SELECT status,dcreate,dmodify FROM sesh"
		" WHERE sesh_id = :si AND ip_addr = :ip AND auth = :un" );
	qt.setParamSource( token );
	ROSETTA	r;
	if ( ! qt.fetchSingle( &r ) )
		{XDELETE( token );
		return( false );
		}
	if ( r.getInt( "status" ) != 0 )
		{XDELETE( token );
		return( false );
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int RAND_UTIL::createToken( const std::string username )
{
	if ( hasToken() )
		{return( -1 );	// ONLY ONE PERMITTED
		}
	if ( NULL == getTrialsDB() )
		{return( -2 );
		}
	XQUERY	qs( tdb, "SELECT seq_sesh.nextval" );
	const	int	sesh_id = (int) qs.fetchLint( -1 );
	if ( sesh_id < 1 )
		{return( -3 );
		}
	token = new ROSETTA();
	token->setInt( "si", sesh_id );
	token->setString( "un", username );
	token->setString( "ip", determineCallerIP() );
	XEXEC	xt( tdb, "INSERT INTO sesh (sesh_id,ip_addr,auth)"
		"VALUES (:si,:ip,:un)" );
	xt.setParamSource( token );
	bool	ok = xt.exec();
	if ( ! ok )
		{XDELETE( token );
		}
	return( ok ? sesh_id : (-9) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool RAND_UTIL::invalidateToken( void )
{
	if ( ! hasToken() )
		{return( false );
		}
	XEXEC	xt( tdb, "UPDATE sesh SET status = 1"
		" WHERE sesh_id = :si AND status = 0" );
	xt.setParamSource( token );
	bool	ok = xt.exec();
	return( ok );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string RAND_UTIL::getTokenString( void )
{
	if ( ! hasToken() )
		{return( "" );
		}
	std::string	hex;
	XENCODE::szToHex( token->serializeOut(), &hex );
	return( hex );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool RAND_UTIL::outputFormToken( void )
{
	std::string	tk = getTokenString();
	if ( tk.empty() )
		{return( false );
		}
	printf( "<input type=\"hidden\" name=\"tk\" value=\"%s\">", tk.c_str());
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string RAND_UTIL::getTokenUsername( void )
{
	return( hasToken()
		? token->getStringDefault( "un", "" )
		: std::string( "" ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int RAND_UTIL::getTokenSessionID( void )
{
	return( hasToken() ? token->getIntDefault( "si", -1 ) : -1 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool RAND_UTIL::outputData( void ) const
{
	if ( ! data.isValid() )
		{return( false );
		}
	if ( data.count() < 1 )
		{return( true );
		}
	const	std::string	dsr = data.serializeOut();
	std::string	d64;
	if ( ! XENCODE::szToB64( dsr, &d64 ) )
		{return( false );
		}
	printf( "<input type=\"hidden\" name=\"%s\" value=\"%s\">",
		RANDUTIL_DATA, d64.c_str() );
	return( true );
}
//---------------------------------------------------------------------------
std::string RAND_UTIL::calcSecureMD5( const std::string unam,
	const std::string pswd )
{
	const	std::string	uap = unam + "&" + pswd;
	const	std::string	secure_md5 = XMD5::calcHex( uap.c_str() );
	return( secure_md5 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void RAND_UTIL::logFailure( XDB *tdb, const std::string un )
{
	XEXEC	xf( tdb, std::string("EXECUTE PROCEDURE login_failure")
		+ " ( usrnam = '" + XSQL::makeSafeString( un ) + "' )");
	xf.exec();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string RAND_UTIL::setStaffUser( const std::string uap )
{         			// UAP = USERNAME+"&"+PASSWORD, SEC_MD5=MD5(UAP)
	XDELETE( staff );
	XDELETE( priv );
	int	pos = 0;
	while ( uap[pos] != '&' )
		{if ( 0 == uap[pos] )
			{return( "Missing password, no &amp; present." );
			}
		pos++;
		}
	if ( pos < 1 || pos > (int) uap.size() - 2 )
		{return( "Mal-formed uap" );
		}
	std::string	username = std::string( uap, 0, pos );
	std::string	secure_md5 = XMD5::calcHex( uap.c_str() );
	if ( NULL == getTrialsDB() )
		{return( "Failed to connect to trials database" );
		}
	XQUERY	qs( tdb, "SELECT * FROM user"
		" WHERE username = :u" );
	qs.param.setString( "u", username );
	if ( ! qs.open() )
		{return( "Failed to open query" );
		}
	ROSETTA	r;
	bool	found = qs.fetch( &r );
	qs.close();
	if ( ! found )
		{return( "Username not found" );
		}
	if ( r.getInt( "status" ) != 0 )
		{return( "Username is inactive" );
		}
	const	std::string	current_sec_md5 = r.getString( "sec_md5" );
	const	LINT	nrf = r.getLintDefault( "nrecent_fail", 0 );
	if ( current_sec_md5 != secure_md5 )
		{
		logFailure( tdb, username );
		char	mbuf[80];
		sprintf( mbuf, "Password incorrect, %d attempts remaining",
			(int) ( permissable_consecutive_failures - nrf ) );
		return( mbuf );
		}
	if ( nrf > permissable_consecutive_failures )
		{if ( nrf < 10000000 )
			{logFailure( tdb, username );
			}
		return( "Too many previous failures - contact"
			" CTSU to have account reset" );
		}
	std::string	serpriv = r.getString( "priv" );
	staff = new ROSETTA();
	staff->setString( "un", username );
	staff->setString( "fn", r.getString( "fullname" ) );
	priv = new ROSETTA();
	priv->serializeIn( serpriv );
	if ( ! priv->isValid() )
		{XDELETE( priv );
		XDELETE( staff );
		return( std::string("Invalid priv object: ") + "REASON" );
		}
	XEXEC	xs( tdb, std::string("EXECUTE PROCEDURE login_success")
		+ " (usrnam = '" + XSQL::makeSafeString( username ) + "' )" );
	xs.exec();
	return( "" );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string RAND_UTIL::getStaffUsername( void )
{
	return( ( NULL != staff )
		? staff->getStringDefault( "un", "" )
		: std::string( "*MISSING*" ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string RAND_UTIL::getStaffFullname( void )
{
	return( ( NULL != staff )
		? staff->getStringDefault( "fn", "" )
		: std::string( "*MISSING*" ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool RAND_UTIL::hasPriv( const std::string pr )
{
	if ( NULL == priv )
		{if ( ! hasToken() || NULL == getTrialsDB() )
			{return( false );
			}
		XQUERY	qp( tdb, "SELECT priv FROM user WHERE username = :un" );
		qp.setParamSource( token );
		const	std::string	serpriv = qp.fetchString( "" );
		priv = new ROSETTA();
		priv->serializeIn( serpriv );
		if ( ! priv->isValid() )
			{XDELETE( priv );
			return( false );
			}
		}
	return ( ROSETTA::typeBool == priv->getType( pr )
		&& priv->getBool( pr ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string RAND_UTIL::permittedPW( const std::string pw )
{
	int	len = pw.size();
	if ( len < 8 )
		{return( "Passwords must be at least 8 characters" );
		}
	if ( len > 80 )
		{return( "Passwords cannot be more than 80 characters" );
		}
	int	i;
	int	upp = 0, low = 0, dig = 0, pun = 0;
	for ( i = 0; i < len; i++ )
		{if ( isupper( pw[i] ) )
			{upp = 1;
			}
		else if ( islower( pw[i] ) )
			{low = 1;
			}
		else if ( isdigit( pw[i] ) )
			{dig = 1;
			}
		else if ( ispunct( pw[i] ) )
			{pun = 1;
			}
		else
			{return( "Invalid character in password" );
			}
		}
	if ( upp + low + dig + pun < 3 )
		{return( "Password must contain at least 3 types from"
			" Upper-case, Lower-case, Numbers and Punctuation." );
		}
	return( "" );		// ACCEPTABLE PW
}
//---------------------------------------------------------------------------
bool RAND_UTIL::writeLog( XDB *db, const int severity, const std::string txt )
{
	return( false );
}
//===========================================================================

