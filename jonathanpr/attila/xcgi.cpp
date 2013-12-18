#ifndef XCGI_C_INCLUDED
#define XCGI_C_INCLUDED       1
/*===========================================================================*/
/*		UTILITIES FOR CONSTRUCTING CGI SCRIPTS			*/
/*===========================================================================*/
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "xtime.h"
#include "xcgi.h"
/*===========================================================================*/
#ifndef __BORLANDC__
#define	stricmp	strcasecmp
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int    	XCGI::debug = 0;
char const *XCGI::logname = "xcgi.log";
int	XCGI::max_param_num = 1000;		// CRASH PREVENTION LIMITS
int	XCGI::max_param_len = 3000;
int	XCGI::max_param_totlen = 50000;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static bool XCGI_ROSETTA_warning_handler( const ROSETTA *r, const int code,
	const std::string msg )
{
	printf( "\n<!-- ROSETTA WARNING %s code=%d msg=\"%s\" -->",
		( NULL == r ) ? "(NULL!)" : "",	code, msg.c_str() );
	fflush( stdout );
	return( true );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static bool XCGI_ROSETTA_error_handler( const ROSETTA *r, const int code,
	const std::string msg )
{
	printf( "\n<!-- ROSETTA ERROR %s code=%d msg=\"%s\" -->",
		( NULL == r ) ? "(NULL!)" : "",	code, msg.c_str() );
	printf( "\n<p>Sorry, an internal server error has occurred :-(<p>"
		"</html>" );
	fflush( stdout );
	exit( EXIT_SUCCESS );
	return( false );
}
/*===========================================================================*/
				/* INITIALIZE SYSTEM AND GET PARAMETERS */
XCGI::XCGI( int ac, char **av )
	:
	XERROR( "XCGI" ),
	method( methodError ),
	flog( NULL )
{	int	i, content_length;
	char	*pbuf;
	FILE	*fi = NULL;
	setvbuf( stdout, NULL, _IONBF, 0 );		// DISABLE BUFFERING
	if ( ! ROSETTA::hasWarningHandler() )		// REDUCE SILENT CRASHES
		{ROSETTA::setWarningHandler( XCGI_ROSETTA_warning_handler );
		}
	if ( ! ROSETTA::hasErrorHandler() )
		{ROSETTA::setErrorHandler( XCGI_ROSETTA_error_handler );
		}
	char	*mev = getenv( "REQUEST_METHOD" );

	if ( NULL == mev )
		{
		if ( ac > 2 && 0 == stricmp( "-qf", av[1] )
				&& NULL != ( fi = fopen(
				av[2], "r" ) ) )
			{
			method = methodFile;
			pbuf = (char *) malloc( 10002 );
			fgets( pbuf, 10000, fi );
			fclose( fi );
			pbuf[10000] = 0;
			fi = NULL;
			int	len = strlen( pbuf );
			while ( --len > 0 && ( '\n' == pbuf[len]
					|| '\r' == pbuf[len] ) )
				{pbuf[len] = 0;	// REMOVE TRAILING \n AND \r
				}
			}
		else if ( ac > 2 && 0 == strcmp( "-qs", av[1] ) )
			{
			method = methodString;
			pbuf = (char *) malloc( strlen( av[2] ) + 2 );
			strcpy( pbuf, av[2] );
			}
		else
			{printf( "\nUnsupported CGI method"
			"\nInteractive usage (2 types):"
		"\n\"-qf param\"  reads line from file 'param'"
		"\n\"-qs param\"  uses `param' as QUERY_STRING" );
			endFail(
	"Client not using a supported CGI method to request data." );
			exit( EXIT_SUCCESS );	// IMMEDIATE EXIT
			}
		splitParams( pbuf );
		free( pbuf );
		}
	else if ( 0 == stricmp( mev, "GET" ) )
		{
		method = methodGet;
		splitParams( getenv( "QUERY_STRING" ) );
		}
	else if ( 0 == stricmp( mev, "POST" ) )
		{
		method = methodPost;
		content_length = atoi( getenv( "CONTENT_LENGTH" ) );
		if ( content_length < 0 || content_length > max_param_totlen
			|| ( NULL == ( pbuf = (char *)
				malloc( content_length + 2 ) ) ) )
			{log(
			"total of %ld bytes recieved from user script",
				content_length );
			endFail("too much input transmitted by script" );
			}
		for ( i = 0; i < content_length; i++ )
			{pbuf[i] = (char) fgetc( stdin );
			}
		pbuf[ content_length ] = 0;
		splitParams( pbuf );
		free( pbuf );
		}
	else
		{log( "internal error, method = %ld", method );
		endFail( "server error" );
		}
	flush();
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
XCGI::~XCGI( void )
{
	if ( NULL != flog )
		{fclose( flog );
		flog = NULL;
		}
}
/*===========================================================================*/
void XCGI::flush( void ) const
{
	fflush( stdout );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* WRITE LOG OF INTERNAL MESSAGES */
bool XCGI::log( const char *txt, ... )
{
	bool	ok = true;
	static	int	log_count = 0;
	time_t	tnow;
	time( &tnow );
	try
		{va_list	args;			// VARIABLE ARGUMENT LIST
		if ( NULL == flog )
			{flog = fopen( logname, "w" );
			}
		if ( NULL == flog )
			{return( false ); 	// NO LOGGING POSSIBLE
			}
		fprintf( flog, "\n\nLog-item %d %s\n",
			++log_count, ctime( &tnow ) );
		fflush( flog );
		va_start( args, txt );
		vfprintf( flog, txt, args );
		va_end( args );		  	// TIDY ARGUMENT LIST
		fflush( flog );
		}
	catch ( ... )
		{ok = false;
		}
	return( ok );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SET INTERNAL DEBUG VALUE */
void XCGI::setDebug( const int dbg )
{
	debug = dbg;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int XCGI::getDebug( void ) const
{
	return( debug );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int XCGI::getMethod( void ) const
{
	return( method );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
std::string XCGI::getMethodName( void ) const
{
	if ( methodGet == method )
		{return( "GET" );
		}
	if ( methodPost == method )
		{return( "POST" );
		}
	if ( methodString == method )
		{return( "String" );
		}
	if ( methodFile == method )
		{return( "File" );
		}
	if ( methodUnknown == method )
		{return( "Unknown" );
		}
	if ( methodError == method )
		{return( "Error" );
		}
	return( "BUG" );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
std::string XCGI::getEnvVar( const std::string name )
{
	const	char	*tmp = getenv( name.c_str() );
	if ( NULL == tmp || 0 == *tmp )
		{return( "" );
		}
	return( std::string( tmp ) );
}
/*---------------------------------------------------------------------------*/
			/* READ FILE AND COPY VERBATIM TO OUTPUT STREAM */
bool XCGI::verbatim( const std::string vfname, const bool notify_fail )
{       int	c;
	flush();
	FILE	*fi = fopen( vfname.c_str(), "r" );
	if ( NULL == fi )
		{if ( notify_fail )
			{
			log( "xcgi_verbatim, unable to find file \"%s\"",
				vfname.c_str() );
			printf( "\n<p><hr><b>SERVER ERROR</b><br>" );
			printf( "<i>Unable to include document</i> " );
			printf( "\n<tt>%s</tt><hr><p>\n", vfname.c_str() );
			}
		return( false );
		}
	while ( EOF != ( c = fgetc( fi ) ) )
		{fputc( c, stdout );	/* READ THEN WRITE EACH CHARACTER */
		}
	flush();
	fclose( fi );
	return( true );
}
/*---------------------------------------------------------------------------*/
							/* EXIT NICELY */
void XCGI::end( void )
{       
	flush();
	if ( NULL != flog )
		{fclose( flog );
		flog = NULL;
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* TERMINATE DODGY REQUEST AND EXIT */
void XCGI::endFail( const std::string err_txt )
{
	setInvalid( err_txt );
	log( err_txt.c_str() );
	end();
}
/*---------------------------------------------------------------------------*/
		/* CONVERT 2-DIGIT HEXADECIMAL STRING TO NUMERIC VALUE */
int XCGI::unhex( const char *hx )
{       int	i, c;
	int	val = 0;
	for ( i = 0; i < 2; i++ )
		{c = hx[i];
		if ( '0' <= c && c <= '9' )
			{c -= '0';
			}
		else if ( 'a' <= c && c <= 'f' )
			{c -= ( 'a' - 10 );
			}
		else if ( 'A' <= c && c <= 'F' )
			{c -= ( 'A' - 10 );
			}
		else
			{log( "unhex: bad value \"%s\"", hx );
			return( (int) '?' );
			}
		val += ( i < 1 ) ? ( 16 * c ) : c;
		}
	return( val );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* DECODE URL-ENCODED STRING IN PLACE */
void XCGI::decodeURL( char *txt )
{       int	code;
	int	pos = 0, newpos = 0;
	while ( txt[pos] != 0 )
		{if ( '+' == txt[pos] )		/* CONVERT PLUS TO SPACE */
			{txt[newpos] = ' ';
			}
		else if ( '%' == txt[pos] )	/* UN-ESCAPE HEX CODE */
			{code = unhex( txt + pos + 1 );
			if ( code > 8 )
				{txt[newpos] = (char) code;
				}
			else
				{log(
			"xcgi_decode_URL: suspicious escape code %d ignored",
					code );
				txt[newpos] = '?';
				}
			pos += 2;
			}
		else				/* COPY NORMAL CHARACTER */
			{txt[newpos] = txt[pos];
			}
		pos++;
		newpos++;
		}
	txt[newpos] = 0;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* GET NEXT PARAMETER FROM STRING */
int XCGI::nextParam( const char *in, const char terminator, char **out )
{	int	nc = 0;
	if ( NULL == ( *out = (char *) malloc( 52 ) ) )
		{endFail( "memory error on server" );
		}
	while ( terminator != in[nc] && 0 != in[nc] )
		{(*out)[nc] = in[nc];
		if ( 0 == ++nc % 50 )
			{if ( NULL == ( *out = (char *) realloc( *out, nc + 52 ) ) )
				{endFail( "server out of memory" );
				}
			if ( nc > max_param_len )
				{log( "runway parameter ?   length > %d",
					nc );
				endFail(	"overlong parameter in user script" );
				}
			}
		}
	(*out)[ nc ] = 0;
	decodeURL( *out );
	return( nc );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* CONVERT PARAMETER STRING INTO SEPARATE ITEMS */
void XCGI::splitParams( const char *prm )
{	int	pos = 0;
	if ( debug > 0 )
		{log( "splitting\n\"%s\"", prm );
		}
	while ( 0 != prm[pos] )
		{char	*name, *value;
		pos += nextParam( prm + pos, '=', &name );
		if ( 0 != prm[pos] )
			{pos++;
			}
		pos += nextParam( prm + pos, '&', &value );
		if ( name != NULL && value != NULL )
			{param.setString( name, value );
			free( name );
			free( value );
			}
		else
			{endFail( "memory error extracting parameters" );
			return;
			}
		if ( 0 != prm[pos] )
			{pos++;
			}
		if ( param.count() > max_param_num )
			{log( "splitParams: at least %d parameters",
					param.count() );
			endFail( "script has sent too many parameters" );
			return;
			}
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
bool XCGI::paramExists( const std::string pname ) const
{
	return( param.isString( pname ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int XCGI::getParamAsIntDefault( const std::string pname, const int def ) const
{
	if ( param.isInt( pname ) )	// SHOULDN'T HAPPEN, BUT CODED ANYWAY
		{return( param.getInt( pname ) );
		}
	if ( ! param.isString( pname ) )
		{return( def );
		}
	std::string	v = param.getString( pname );
	const	char	*s = v.c_str();
	if ( '-' == *s ) 	// ENSURE ONLY VALID NON-EMPTY INTS
		{s++;
		}
	while ( 0 != *s )
		{if ( ! isdigit( *s ) )
			{return( def );
			}
		s++;
		}
	return( atoi( v.c_str() ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int XCGI::getParamAsInt( const std::string pname ) const
{
	return( getParamAsIntDefault( pname, invalidInt ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
std::string XCGI::getParamDefault( const std::string pname, const std::string def ) const
{
	return( param.isString( pname )	? param.getString( pname ) : def );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

std::string XCGI::setParamIfExistsElseDefault(const std::string pname, const std::string def ) const
{
	if (!paramExists(pname))
		return def;
	return getParamDefault(pname,"");
}

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
std::string XCGI::getParam( const std::string pname ) const
{
	return( getParamDefault( pname, "" ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* FIND DATA RECEIPT METHOD */
int XCGI::findMethod( void )
{
	return( methodUnknown );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void XCGI::writeHeaderCustom( const std::string mf )
{
	printf( "content-type: %s\n\n", mf.c_str() );
	log( "Mime format = \"%s\"", mf.c_str() );
	flush();
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* WRITE INITIAL MIME HEADER TO OUTPUT */
void XCGI::writeHeader( const int typ )
{
	char const *mime_fmt = "?";
	if ( typeHtml == typ  )
		{mime_fmt = "text/html";
		}		
	else if ( typeJpeg == typ )
		{mime_fmt = "text/jpeg";
		}
	else if ( typePlain == typ )
		{mime_fmt = "text/plain";
		}
	else if ( typePNG == typ )
		{mime_fmt = "text/png";
		}
	else if ( typeGif == typ )
		{mime_fmt = "image/gif";
		}
	else if ( typeTiff == typ )
		{mime_fmt = "image/tiff";
		}
	else if ( typeXmlText == typ )	// N.B. OTHER XML MIME-TYPES EXIST
		{mime_fmt = "text/xml";
		}
	else
		{mime_fmt = "text/plain";
		log( "unrecognized format type=%d", typ );
		}
	writeHeaderCustom( mime_fmt );
}
/*---------------------------------------------------------------------------*/
				/* SHOW ALL PARAMETERS AS COMMENTS IN OUTPUT */
void XCGI::paramShowall( void )
{	int	i, np;
	np = param.count();
	printf( "\n\n<!-- XCGI found %d parameters -->\n", np );
	for ( i = 0; i < np; i++ )
		{printf( "\n<!--  \"%s\"=\"%s\"  -->",
			param.getName( i ).c_str(),
			param.getString( i ).c_str() );
		}
	printf( "\n" );
}
/*---------------------------------------------------------------------------*/
int XCGI::escapeLength( const char *s, const int maxlen )
{			// RETURN LENGTH OF APPARENTLY VALID ESCAPE SEQUENCE
	int	nalpha = 0;
	while ( nalpha < maxlen && isalpha( s[nalpha] ) )
		{nalpha++;
		}
	return( ( nalpha > 0 && nalpha < maxlen && ';' == s[nalpha] )
		? ( nalpha + 1 ) : 0 );
}
/*---------------------------------------------------------------------------*/
	/* CONVERT `UNSAFE' HTML2.0 CHARACTERS TO HTML-FRIENDLY FORMS */
std::string XCGI::makeWebSafe( const std::string raw )
{
	int	i = 0;
	int	j, na;
	int	len = raw.size();
	const	char	*c = raw.c_str();
	std::string	clean;
	while ( i < len )
		{switch( c[i] )
			{
			case '<':
				clean += "&lt;";
				break;
			case '>':
				clean += "&gt;";
				break;
			case '&':
				na = escapeLength( c+i+1, len-i );
				if ( na > 0 )
					{for ( j = 0; j <= na; j++ )
						{clean += c[i+j];
						}
					i += na;
					}
				else
					{clean += "&amp;";
					}
				break;
			case '\"':
				clean += "&quot;";
				break;
			default:
				clean += (char) c[i];
				break;
			}
		i++;
		}
	return( clean );
}
/*===========================================================================*/
#endif                                                  	/* XCGI.CPP */
