#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <math.h>
#include "rosebase.h"
#include "rosetta.h"
//===========================================================================
#define	AND		'&'
#define	BAR		'|'
#define	BSLASH		'\\'
#define	COLON		':'
#define	COMMA		','
#define	DOT		'.'
#define	EOS		'\0'
#define	EQUALS		'='
#define	FSLASH		'/'
#define	HASH		'#'
#define	LRB		'('
#define	LSB		'['
#define	MINUS		'-'
#define	PERCENT		'%'
#define	PLUS		'+'
#define	RRB		')'
#define	RSB		']'
#define	SPACE		' '
#define	UNDERSCORE	'_'
#define	ZERO		'0'
//===========================================================================
ROSE_BASE::ROSE_BASE( const ROSETTA *owner, const std::string name )
	:
	owner_rosetta( owner ),
	id( name ),
	tags( NULL ),
	valid( true )
{
	if ( id.empty() )
		{error( ROSETTA::ProblemForbiddenByRule,
			"name cannot be empty/blank" );
		valid = false;
		}
	if ( ! isAllowableName( id ) )
		{valid = false;
		error( ROSETTA::ProblemForbiddenByRule,
			"names must be alphanumeric+underscores only" );
		}
	if ( ! owner_rosetta->isCaseSensitive() )
		{ROSETTA::makeLowerCase( &id );
		}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_BASE::~ROSE_BASE( void )
{
	delete tags;
	tags = NULL;
}
//---------------------------------------------------------------------------
bool ROSE_BASE::warning( const int ecode, const std::string emsg ) const
{
	std::string	etxt = std::string("ROSE_BASE exception for object \"")
		+ id + std::string( "\" " ) + emsg;
	return( owner_rosetta->warning( ecode, etxt ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::error( const int ecode, const std::string emsg ) const
{
	std::string	etxt = std::string("ROSE_BASE exception for object \"")
		+ id + std::string( "\" " ) + emsg;
	return( owner_rosetta->error( ecode, etxt ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::isAllowableName( const std::string nm )
{
	const	char	*c = nm.c_str();
	if ( 0 == *c )
		{return( false );	// CANNOT BE EMPTY
		}
	while ( 0 != *c )
		{if ( ! isalnum( *c ) && *c != UNDERSCORE )
			{return( false );
			}
		c++;
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::isValid( void ) const
{	return( valid );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		/* RETURN -VE IF OWN-NAME IS LESS THAN ARGUMENT-NAME */
int ROSE_BASE::compareName( const ROSE_BASE *b ) const
{	return( strcmp( id.c_str(), b->id.c_str() ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::lessThan( const ROSE_BASE *b ) const
{	return( compareName( b ) < 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::greaterThan( const ROSE_BASE *b ) const
{	return( compareName( b ) > 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ROSE_BASE::sort( void )
{	// DO NOTHING FOR OBJECTS WITHOUT SUB-OBJECTS
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_BASE::getName( void ) const
{	return( id );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::rename( const std::string new_name )
{
	if ( new_name.empty()  )
		{error( ROSETTA::ProblemForbiddenByRule,
			"new_name cannot be empty/blank" );
		valid = false;
		return( false );
		}
	if ( ! isAllowableName( new_name ) )
		{valid = false;
		error( ROSETTA::ProblemForbiddenByRule,
			"names must be alphanumeric+underscores only" );
		return( false );
		}
	id = new_name;
	if ( ! owner_rosetta->isCaseSensitive() )
		{ROSETTA::makeLowerCase( &id );
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_BASE::serializeOut( void ) const
{
	char	buf[5];
	sprintf( buf, "%c#", (const char) type() );
	std::string	descript = buf + id + "=";
	return( descript + serializeOutValue() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const std::set<std::string> *ROSE_BASE::tagPtr( void ) const
{
	return( tags );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::attachTag( const std::string tag_name )
{
	if ( NULL == tags )
		{tags = new std::set<std::string>;
		if ( NULL == tags )
			{return( false );
			}
		}
	tags->insert( tag_name );
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::attachTagSet( const std::set<std::string> *tagset )
{
	if ( NULL == tagset )
		{return( false );
		}
	std::set<std::string>::const_iterator	ti = tagset->begin();
	while ( ti != tagset->end() )
		{attachTag( *ti );
		ti++;
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::removeTag( const std::string tag_name )
{
	if ( NULL != tags )
		{tags->erase( tag_name );
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::hasTag( const std::string tag_name ) const
{
	return( ( NULL != tags ) && ( tags->count( tag_name ) > 0 ) );
}
//---------------------------------------------------------------------------
std::string ROSE_BASE::exportXMLValue( void ) const
{
	return( serializeOutValue() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_BASE::exportXML( void ) const
{
	return( exportFlatXML( "" ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_BASE::exportFlatXML( const std::string prefix ) const
{
	char	tbuf[10];
	sprintf( tbuf, "%c", (char) type() );
	std::string	descript = std::string("\n<v n=\"")
		+ prefix + getName() + "\" t=\"" + tbuf + "\">"
		+ exportXMLValue() + "</v>";
	return( descript );
}
//---------------------------------------------------------------------------
void ROSE_BASE::errorSerializeIn( const char *in ) const
{
	std::string	et = std::string( "serializeIn, field \"")
		+ id + std::string("\" contains invalid value \"")
		+ std::string(in) + std::string( "\"" );
	owner_rosetta->error( ROSETTA::ProblemInvalidValue, et );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::setFlag( void )
{	error( ROSETTA::ProblemUndesiredType, "is not a Flag" );
	return( false );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::setInt( const int value )
{       char	ebuf[80];
	sprintf( ebuf, "cannot accept integer (%d) value", value );
	error( ROSETTA::ProblemUndesiredType, ebuf );
	return( false );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::setLint( const LINT value )
{       char	ebuf[80];
	sprintf( ebuf, "cannot accept LINT (%Ld) value", value );
	error( ROSETTA::ProblemUndesiredType, ebuf );
	return( false );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::operator=( const int value )
{	return( setInt( value ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::setBool( const bool value )
{	char	ebuf[80];
	sprintf( ebuf, "cannot accept bool (%s) value",
		( value ? "True" : "False" ) );
	error( ROSETTA::ProblemUndesiredType, ebuf );
	return( false );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::operator=( const bool value )
{	return( setBool( value ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::setReal( const double value )
{	char	ebuf[80];
	sprintf( ebuf, "cannot accept real (%lg) value", value );
	error( ROSETTA::ProblemUndesiredType, ebuf );
	return( false );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::operator=( const double value )
{	return( setReal( value ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::setString( const std::string value )
{	error( ROSETTA::ProblemUndesiredType, "cannot accept string value" );
	return( false );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::operator=( const std::string value )
{	return( setString( value ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::setTime( const XTIME value )
{	error( ROSETTA::ProblemUndesiredType, "cannot accept Time value" );
	return( false );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::operator=( const XTIME value )
{	return( setTime( value ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::setRosetta( const ROSETTA * value )
{	char	ebuf[80];
	sprintf( ebuf, "cannot accept ROSETTA (%s) value",
		( NULL == value ? "NULL" : "non-NULL" ) );
	error( ROSETTA::ProblemUndesiredType, ebuf );
	return( false );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::setRosetta( const ROSETTA &value )
{	return( setRosetta( &value ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::operator=( const ROSETTA &value )
{	return( setRosetta( &value ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int ROSE_BASE::getInt( void ) const
{	error( ROSETTA::ProblemUndesiredType, "cannot return integer value" );
	return( ROSETTA::errorInt );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
LINT ROSE_BASE::getLint( void ) const
{	error( ROSETTA::ProblemUndesiredType, "cannot return LINT value" );
	return( ROSETTA::errorLint );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BASE::getBool( void ) const
{	error( ROSETTA::ProblemUndesiredType, "cannot return bool value" );
	return( ROSETTA::errorBool );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
double ROSE_BASE::getReal( void ) const
{	error( ROSETTA::ProblemUndesiredType, "cannot return real value" );
	return( ROSETTA::errorReal );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XDATE ROSE_BASE::getDate( void ) const
{	error( ROSETTA::ProblemUndesiredType, "cannot return Date value" );
	return( ROSETTA::errorDate );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XTIME ROSE_BASE::getTime( void ) const
{	error( ROSETTA::ProblemUndesiredType, "cannot return Time value" );
	return( ROSETTA::errorTime );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XBINOB ROSE_BASE::getBinob( void ) const
{	error( ROSETTA::ProblemUndesiredType, "cannot return Binob value" );
	return( ROSETTA::errorBinob );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSETTA * ROSE_BASE::getRosetta( void ) const
{	error( ROSETTA::ProblemUndesiredType, "cannot return ROSETTA value" );
	return( (ROSETTA *) ROSETTA::errorRosetta );
}
//---------------------------------------------------------------------------
		/* CONVERT 2-DIGIT HEXADECIMAL STRING TO NUMERIC VALUE */
int ROSE_BASE::unhex( const char *hx ) const
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
			{error( ROSETTA::ProblemLethalBug,
				"ROSE_BASE::unhex, invalid non-hex char-code" );
			return( (int) -1 );
			}
		val += ( i < 1 ) ? ( 16 * c ) : c;
		}
	return( val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
				/* DECODE URL-ENCODED STRING IN PLACE */
bool ROSE_BASE::decodeURL( char *txt ) const
{
	int	code;
	int	pos = 0, newpos = 0;
	bool	ok = true;
	while ( txt[pos] != EOS )
		{if ( PLUS == txt[pos] )    	// CONVERT PLUS TO SPACE
			{txt[newpos] = SPACE;
			}
		else if ( PERCENT == txt[pos] )	// UN-ESCAPE HEX CODE
			{code = unhex( txt + pos + 1 );
			if ( code > 8 )
				{txt[newpos] = (char) code;
				}
			else
				{ok = warning( ROSETTA::ProblemSuspiciousString,
				"decode_URL: suspicious escape code ignored" );
				txt[newpos] = '?';
				}
			pos += 2;
			}
		else				// COPY NORMAL CHARACTER
			{txt[newpos] = txt[pos];
			if ( ! isPlainCharacter( txt[pos] ) )
				{ok = warning( ROSETTA::ProblemUncodedChar,
				"decode_URL: unexpected unencoded character" );
				}
			}
		pos++;
		newpos++;
		}
	txt[newpos] = EOS;
	return( ok );
}
//---------------------------------------------------------------------------
bool ROSE_BASE::isPlainCharacter( const char c )
{ 		// CAN CHARACTER BE USED IN URL-ENCODED STRING WITHOUT ENCODING
	return( isalnum( c ) || DOT == c || COMMA == c || UNDERSCORE == c
		|| MINUS == c || COLON == c || FSLASH == c
		|| LRB == c || RRB == c ); // MADE SINGLE CHARS PLAIN 6 OCT 2008
}
//---------------------------------------------------------------------------
		/* CONVERT CHARACTER VALUE TO 2-DIGIT HEXADECIMAL STRING */
void ROSE_BASE::tohex( const char c, char *hx ) const
{
	static	const	char	*hex_char = "0123456789ABCDEFx";
	int	ic = ( ( (int) ( (unsigned char) c ) ) + 256 ) % 256;
	hx[0] = hex_char[ ic / 16 ];
	hx[1] = hex_char[ ic % 16 ];
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
							/* URL-ENCODE STRING */
void ROSE_BASE::encodeURL( const char *in, char *out ) const
{       int	pos = 0, newpos = 0;
	char	c;
	while ( ( c = in[pos] ) != EOS )
		{if ( SPACE == c )			// CONVERT PLUS TO SPACE
			{out[newpos] = PLUS;
			}
		else if ( isPlainCharacter( c ) )	// SIMPLE COPY
			{out[newpos] = c;
			}
		else			       		// ESCAPED HEX CODE
			{out[newpos] = PERCENT;
			tohex( c, out + newpos + 1);
			newpos += 2;
			}
		pos++;
		newpos++;
		}
	out[newpos] = EOS;
}
//===========================================================================
ROSE_FLAG::ROSE_FLAG( const ROSETTA *owner, const std::string name )
	:
	ROSE_BASE( owner, name )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_FLAG::~ROSE_FLAG( void )
{
}
//---------------------------------------------------------------------------
int ROSE_FLAG::type( void ) const
{
	return( ROSETTA::typeFlag );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_BASE *ROSE_FLAG::copy( const ROSETTA *owner ) const
{
	ROSE_FLAG *c = new ROSE_FLAG( owner, id );
	c->attachTagSet( tagPtr() );
	return( c );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_FLAG::equalValue( const ROSE_BASE *b ) const
{
	if ( b->type() != type() )
		{return( false );
		}
	return( true );				// ALL FLAGS HAVE `NULL' VALUE
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_FLAG::getString( void ) const
{	return( "" );				// REQUIRED FOR SIGNATURES
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_FLAG::serializeOutValue( void ) const
{
	return( "" );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_FLAG::serializeInValue( const char *in )
{
	if ( 0 != *in )
		{valid = false;
		errorSerializeIn( in );
		}
	return( valid );
}
//===========================================================================
template <class T>
ROSE_COMMON<T>::ROSE_COMMON( const ROSETTA *owner, const std::string name,
	const T value )
	:
	ROSE_BASE( owner, name ),
	val( value )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template <class T>
ROSE_COMMON<T>::~ROSE_COMMON( void )
{
}
//---------------------------------------------------------------------------
template <class T>
void ROSE_COMMON<T>::set_native( const T value )
{	val = value;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template <class T>
T ROSE_COMMON<T>::get_native( void ) const
{	return( val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template <class T>
bool ROSE_COMMON<T>::equalValue( const ROSE_BASE *b ) const
{	if ( b->type() != type() )
		{return( false );
		}
	return( ((ROSE_COMMON<T> *)b)->get_native() == val );
}
//===========================================================================
ROSE_INT::ROSE_INT( const ROSETTA *owner, const std::string name, const int value )
	:
	ROSE_COMMON<int>( owner, name, value )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_INT::~ROSE_INT( void )
{
}
//---------------------------------------------------------------------------
int ROSE_INT::type( void ) const
{
	return( ROSETTA::typeInt );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_INT::setInt( const int value )
{	val = value;
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int ROSE_INT::getInt( void ) const
{	return( val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
LINT ROSE_INT::getLint( void ) const
{	return( (LINT) val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
double ROSE_INT::getReal( void ) const
{	return( (double) val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_INT::getString( void ) const
{
	char	ibuf[30];
	sprintf( ibuf, "%d", val );
	return( std::string( ibuf ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_BASE *ROSE_INT::copy( const ROSETTA *owner ) const
{
	ROSE_INT *c = new ROSE_INT( owner, id, val );
	c->attachTagSet( tagPtr() );
	return( c );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_INT::serializeOutValue( void ) const
{
	char	ibuf[30];
	sprintf( ibuf, "%d", val );
	return( std::string( ibuf ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_INT::serializeInValue( const char *in )
{
	const	char	*pin = ( MINUS != *in ) ? in : (in+1);
	while ( *pin != EOS )
		{if ( ! isdigit( *pin ) )
			{valid = false;
			errorSerializeIn( in );
			break;
			}
		pin++;
		}
	val = atoi( in );
	return( valid );
}
//===========================================================================
ROSE_LINT::ROSE_LINT( const ROSETTA *owner, const std::string name,
	const LINT value )
	:
	ROSE_COMMON<LINT>( owner, name, value )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_LINT::~ROSE_LINT( void )
{
}
//---------------------------------------------------------------------------
int ROSE_LINT::type( void ) const
{
	return( ROSETTA::typeLint );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_LINT::setLint( const LINT value )
{	val = value;
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
LINT ROSE_LINT::getLint( void ) const
{	return( val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
double ROSE_LINT::getReal( void ) const
{	return( (double) val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_LINT::getString( void ) const
{
	char	ibuf[40];
	sprintf( ibuf, "%Ld", val );
	return( std::string( ibuf ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_BASE *ROSE_LINT::copy( const ROSETTA *owner ) const
{
	ROSE_LINT *c = new ROSE_LINT( owner, id, val );
	c->attachTagSet( tagPtr() );
	return( c );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_LINT::serializeOutValue( void ) const
{
	char	ibuf[30];
	sprintf( ibuf, "%Ld", val );
	return( std::string( ibuf ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_LINT::serializeInValue( const char *in )
{
	const	char	*pin = ( MINUS != *in ) ? in : (in+1);
	while ( *pin != EOS )
		{if ( ! isdigit( *pin ) )
			{valid = false;
			errorSerializeIn( in );
			break;
			}
		pin++;
		}
#ifdef __BORLANDC__
	val = _atoi64( in );
#else
	val = atoll( in );
#endif
	return( valid );
}
//===========================================================================
ROSE_BOOL::ROSE_BOOL( const ROSETTA *owner, const std::string name,
	const bool value )
	:
	ROSE_COMMON<bool>( owner, name, value )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_BOOL::~ROSE_BOOL( void )
{
}
//---------------------------------------------------------------------------
int ROSE_BOOL::type( void ) const
{
	return( ROSETTA::typeBool );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BOOL::setBool( const bool value )
{	val = value;
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BOOL::getBool( void ) const
{	return( val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int ROSE_BOOL::getInt( void ) const
{	return( val ? 1 : 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_BOOL::getString( void ) const
{
	return( val ? std::string("T") : std::string( "F" ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_BASE *ROSE_BOOL::copy( const ROSETTA *owner ) const
{
	ROSE_BOOL *c = new ROSE_BOOL( owner, id, val );
	c->attachTagSet( tagPtr() );
	return( c );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_BOOL::serializeOutValue( void ) const
{
	return( val ? "T" : "F" );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_BOOL::serializeInValue( const char *in )
{
	if ( 0 == strcmp( "T", in ) )
		{val = true;
		}
	else if ( 0 == strcmp( "F", in ) )
		{val = false;
		}
	else
		{val = false;		// ASSIGN ARBITRARILY FOR DEFINITE-NESS
		valid = false;
		errorSerializeIn( in );
		}
	return( valid );
}
//===========================================================================
ROSE_REAL::ROSE_REAL( const ROSETTA *owner, const std::string name,
	const double value )
	:
	ROSE_COMMON<double>( owner, name, value )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_REAL::~ROSE_REAL( void )
{
}
//---------------------------------------------------------------------------
int ROSE_REAL::type( void ) const
{
	return( ROSETTA::typeReal );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_REAL::setReal( double value )
{	val = value;
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
double ROSE_REAL::getReal( void ) const
{	return( val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_REAL::getString( void ) const
{
	char	rbuf[50];
	sprintf( rbuf, "%lg", val );
	return( std::string( rbuf ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_BASE *ROSE_REAL::copy( const ROSETTA *owner ) const
{
	ROSE_REAL *c = new ROSE_REAL( owner, id, val );
	c->attachTagSet( tagPtr() );
	return( c );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_REAL::serializeOutValue( void ) const
{
	char	rbuf[50];
	sprintf( rbuf, "%lg", val );
	return( std::string( rbuf ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_REAL::serializeInValue( const char *in )
{
	val = atof( in );
	valid = ( fabs(val) != HUGE_VAL );	// CHECK FOR OVERFLOW
	if ( ! valid )
		{errorSerializeIn( in );
		}
	return( valid );
}
//===========================================================================
ROSE_STRING::ROSE_STRING( const ROSETTA *owner, const std::string name,
	const std::string value )
	:
	ROSE_COMMON<std::string>( owner, name, value )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_STRING::~ROSE_STRING( void )
{
}
//---------------------------------------------------------------------------
int ROSE_STRING::type( void ) const
{
	return( ROSETTA::typeString );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_STRING::setString( const std::string value )
{	val = value;
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_STRING::getString( void ) const
{
	return( val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const std::string *ROSE_STRING::pointerString( void ) const
{
	return( &val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_BASE *ROSE_STRING::copy( const ROSETTA *owner ) const
{
	ROSE_STRING *c = new ROSE_STRING( owner, id, val );
	c->attachTagSet( tagPtr() );
	return( c );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_STRING::serializeOutValue( void ) const
{
	char	*out = (char *) malloc( ( val.size() * 3 ) + 1 );
	if ( NULL == out )
		{return( ROSETTA::errorString );
		}
	encodeURL( val.c_str(), out );
	std::string	encoded = out;
	free( out );
	return( encoded );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_STRING::exportXMLValue( void ) const
{
	char	*out = (char *) malloc( ( val.size() * 6 ) + 1 );
	char	*pos = out;
	const 	char *x = val.c_str();
	while ( *x != 0 )
		{switch ( *x )
			{
			case '&':
				strcpy( pos, "&amp;" );
				pos += 5;
				break;
			case '"':
				strcpy( pos, "&quot;" );
				pos += 6;
				break;
			case '\'':
				strcpy( pos, "&apos;" );
				pos += 6;
				break;
			case '<':
				strcpy( pos, "&lt;" );
				pos += 4;
				break;
			case '>':
				strcpy( pos, "&gt;" );
				pos += 4;
				break;
			default:
				*pos++ = *x;
				break;
			}
		x++;
		}
	*pos = 0;
	std::string	escaped = out;
	free( out );
	return( escaped );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_STRING::serializeInValue( const char *in )
{
	val = in;
	return( valid );
}
//===========================================================================
ROSE_XDATE::ROSE_XDATE( const ROSETTA *owner, const std::string name,
	const XDATE value )
	:
	ROSE_COMMON<XDATE>( owner, name, value )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_XDATE::~ROSE_XDATE( void )
{
}
//---------------------------------------------------------------------------
int ROSE_XDATE::type( void ) const
{
	return( ROSETTA::typeDate );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_XDATE::setDate( const XDATE value )
{	val = value;
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XDATE ROSE_XDATE::getDate( void ) const
{	return( val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XTIME ROSE_XDATE::getTime( void ) const
{	return( val.asXTIME() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_XDATE::getString( void ) const
{
	return( val.iso() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_BASE *ROSE_XDATE::copy( const ROSETTA *owner ) const
{
	ROSE_XDATE *c = new ROSE_XDATE( owner, id, val );
	c->attachTagSet( tagPtr() );
	return( c );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_XDATE::serializeOutValue( void ) const
{
	return( val.iso() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_XDATE::serializeInValue( const char *in )
{
	val.set( in );
	valid = val.isValid();
	if ( ! valid )
		{errorSerializeIn( in );
		}
	return( valid );
}
//===========================================================================
ROSE_XTIME::ROSE_XTIME( const ROSETTA *owner, const std::string name,
	const XTIME value )
	:
	ROSE_COMMON<XTIME>( owner, name, value )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_XTIME::~ROSE_XTIME( void )
{
}
//---------------------------------------------------------------------------
bool ROSE_XTIME::allow_deserialize_invalid = false;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ROSE_XTIME::setAllowDeserializeInvalid( const bool adi )
{
	allow_deserialize_invalid = adi;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_XTIME::getAllowDeserializeInvalid( void )
{
	return( allow_deserialize_invalid );
}
//---------------------------------------------------------------------------
int ROSE_XTIME::type( void ) const
{
	return( ROSETTA::typeTime );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_XTIME::setTime( const XTIME value )
{	val = value;
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XTIME ROSE_XTIME::getTime( void ) const
{	return( val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XDATE ROSE_XTIME::getDate( void ) const
{	return( val.asXDATE() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_XTIME::getString( void ) const
{
	return( val.iso() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_BASE *ROSE_XTIME::copy( const ROSETTA *owner ) const
{
	ROSE_XTIME *c = new ROSE_XTIME( owner, id, val );
	c->attachTagSet( tagPtr() );
	return( c );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_XTIME::serializeOutValue( void ) const
{
	return( val.iso() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_XTIME::serializeInValue( const char *in )
{
	val.set( in );
	valid = val.isValid();
	if ( ! valid )
		{if ( allow_deserialize_invalid && NULL != in
			&& 0 == strcmp( XTIME::invalid_serialization, in ) )
			{valid = true;
			val.set( -1, -1, -1, -1, -1, -1 );
			}
		}
	return( valid );
}
//===========================================================================
ROSE_XBINOB::ROSE_XBINOB( const ROSETTA *owner, const std::string name,
	const XBINOB value )
	:
	ROSE_COMMON<XBINOB>( owner, name, value )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_XBINOB::~ROSE_XBINOB( void )
{
}
//---------------------------------------------------------------------------
int ROSE_XBINOB::type( void ) const
{
	return( ROSETTA::typeBinob );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_XBINOB::setBinob( const XBINOB value )
{	val = value;
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XBINOB ROSE_XBINOB::getBinob( void ) const
{	return( val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_XBINOB::getString( void ) const
{
	return( val.extractBase64() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_BASE *ROSE_XBINOB::copy( const ROSETTA *owner ) const
{
	ROSE_XBINOB *c = new ROSE_XBINOB( owner, id, val );
	c->attachTagSet( tagPtr() );
	return( c );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_XBINOB::serializeOutValue( void ) const
{
	return( val.extractBase64() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_XBINOB::serializeInValue( const char *in )
{
	val.insertBase64( std::string(in) );
	valid = val.isValid();
	if ( ! valid )
		{error( ROSETTA::ProblemInvalidValue, "serializeIn, bad XBINOB" );
		}
	return( valid );
}
//===========================================================================
ROSE_ROSETTA::ROSE_ROSETTA( const ROSETTA *owner, const std::string name,
	const ROSETTA * value )
	:
	ROSE_COMMON<ROSETTA *>( owner, name, NULL )
{
	val = new ROSETTA( value );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		/* PRIVATE CONSTRUCTOR, FOR USE BY attachSubRosetta ONLY */
ROSE_ROSETTA::ROSE_ROSETTA( const ROSETTA *owner, const std::string name )
	:
	ROSE_COMMON<ROSETTA *>( owner, name, NULL )
{			// VALUE MUST BE ATTACHED BY ROSETTA::attachSubRosetta
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_ROSETTA::~ROSE_ROSETTA( void )
{
	delete val;
}
//---------------------------------------------------------------------------
int ROSE_ROSETTA::type( void ) const
{
	return( ROSETTA::typeRosetta );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_ROSETTA::setRosetta( const ROSETTA * value )
{
	delete val;		// REMOVE OLD OBJECT
	val = new ROSETTA( value );
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_ROSETTA::getString( void ) const
{
	return( val->serializeOut() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSETTA *ROSE_ROSETTA::getRosetta( void ) const
{
	return( new ROSETTA( val ) );	// PASS BACK A CONSTRUCTED COPY
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ROSE_ROSETTA::sort( void )
{	if ( ! val->isSorted() )
		{val->sort();
		}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSE_BASE *ROSE_ROSETTA::copy( const ROSETTA *owner ) const
{
	ROSE_ROSETTA *c = new ROSE_ROSETTA( owner, id, val );
	c->attachTagSet( tagPtr() );
	return( c );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_ROSETTA::serializeOutValue( void ) const
{
	return( val->serializeOut() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_ROSETTA::exportXML( void ) const
{
	return( val->exportXML( getName() ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSE_ROSETTA::exportFlatXML( const std::string prefix ) const
{
	return( val->exportFlatXML( prefix + getName() ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_ROSETTA::serializeInValue( const char *in )
{
	val->clear();
	val->serializeIn( in );
	valid &= val->isValid();
	return( valid );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSE_ROSETTA::equalValue( const ROSE_BASE *b ) const
{
	if ( b->type() != ROSETTA::typeRosetta )
		{return( false );
		}	// TEMPLATE ROUTINE JUST COMPARES POINTER ADDRESSES
	return( val->isEqual( ((ROSE_ROSETTA *)b)->val ) );
}
//===========================================================================





