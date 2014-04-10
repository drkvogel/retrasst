#include "xbinob.h"
#include "xencode.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale>
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef __BORLANDC__
#include <mem.h>
#endif
//===========================================================================
XBV::XBV( void )
	:
	ref_count( 1 ),
	nbytes( 0 ),
	value( NULL )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XBV::XBV( int nb, unsigned char *dat )
	:
	ref_count( 1 ),
	nbytes( nb ),
	value( dat )            // TAKE OWNER SHIP OF INPUT MEMORY
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XBV::XBV( XBV *p )
	:
	ref_count( 1 ),
	nbytes( p->nbytes ),
	value( NULL )
{
	value = (unsigned char *) malloc( nbytes );
	memcpy( value, p->value, nbytes );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XBV::~XBV( void )
{
	nbytes = 0;
	if ( NULL != value )
		{free( value );
		value = NULL;
		}
}
//---------------------------------------------------------------------------
XBV *XBV::acquire( XBV *p )
{
	if ( NULL == p )
		{return( NULL );		// SHOULD NEVER HAPPEN
		}
	p->ref_count++;
	return( p );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XBV::relinquish( XBV **p )
{
	if ( NULL == *p )
		{return;
		}
	if ( 0 == --((*p)->ref_count) )
		{delete *p;
		}
	*p = NULL;	// OBJECT SUPPLYING POINTER IS FINISHED WITH IT
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XBV::size( void ) const
{
	return( nbytes );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const unsigned char *XBV::data( void ) const
{
	return( value );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBV::append( const int input_size, const unsigned char *data )
{		// NOTE: SHOULD BE MADE UNIQUE PRIOR TO DOING THIS
	if ( input_size < 0 || NULL == data )
		{return( false );
		}
	if ( 0 == input_size )		// NOTHING TO DO
		{return( true );
		}
	int	new_size = nbytes + input_size;
	void	*ptr = realloc( value, new_size + 1 );
	if ( NULL == ptr )
		{return( false );
		}
	value = (unsigned char *) ptr;
	memcpy( value+nbytes, data, input_size );
	nbytes = new_size;
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XBV::getRefCount( void ) const
{
	return( ref_count );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBV::equivalent( const XBV *pa, const XBV *pb )
{
	if ( pa == pb )			// POINTERS TO SAME OBJECT OR BOTH NULL
		{return( true );
		}
	if ( NULL == pa || NULL == pb)	// ONLY ONE IS NULL (FROM ABOVE TEST)
		{return( false );
		}
	if ( pa->nbytes != pb->nbytes )
		{return( false );
		}
	int	i;
	for ( i = 0; i < pa->nbytes; i++ )
		{if ( pa->value[i] != pb->value[i] )
			{return( false );
			}
		}
	return( true );
}
//===========================================================================
XBINOB::XBINOB( void )
	:
	XERROR( "XBINOB" )
{
	payload = new XBV();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XBINOB::XBINOB( const int input_size, const unsigned char *data )
	:
	XERROR( "XBINOB" ),
	payload( NULL )
{
	insert( input_size, data );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XBINOB::XBINOB( const XBINOB &b )
	:
	XERROR( "XBINOB" ),
	payload( NULL )
{
	if ( ! b.isValid() )
		{setInvalid( "copy of invalid object" );
		return;
		}
	payload = XBV::acquire( b.payload );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XBINOB::~XBINOB( void )
{
	XBV::relinquish( &payload );
}
//---------------------------------------------------------------------------
void XBINOB::clear( void )
{
	XBV::relinquish( &payload );
	resetXError();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
				/* SET CONTENTS WHEN DATA IS INVALID */
void XBINOB::setInvalid( const std::string emsg ) const
{
	XERROR::setInvalid( emsg );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XBINOB::size( void ) const
{
	return( ( NULL != payload ) ? payload->size() : 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBINOB::empty( void ) const
{
	return( ( NULL != payload ) ? ( 0 == payload->size() ) : true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBINOB::acquire( const int input_size, unsigned char *data )
{			// DANGEROUS - TAKE OWNERSHIP OF EXTERNAL BUFFER
	XBV	*ptmp = payload;
	payload = new XBV( input_size, data );
	XBV::relinquish( &ptmp );
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBINOB::insert( const int input_size, const unsigned char *data )
{
	if ( input_size < 0 )
		{setInvalid( "bad insert, negative input_size" );
		return( false );
		}
	if ( input_size > 0 && NULL == data )
		{setInvalid( "bad insert, positive size for NULL data" );
		return( false );
		}
	if ( 0 == input_size )
		{return( acquire( 0, NULL ) );
		}
	unsigned char *dbuf = (unsigned char *) malloc( input_size + 1 );
	if ( NULL == dbuf )
		{setInvalid( "malloc failure during insert" );
		return( false );
		}
	memcpy( dbuf, data, input_size );
	return( acquire( input_size, dbuf ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBINOB::insert( const XBINOB &b )
{
	XBV	*ptmp = XBV::acquire( b.payload );
	bool	ok = insert( ptmp->size(), ptmp->data() );
	XBV::relinquish( &ptmp );
	return( ok );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBINOB::insertHex( const std::string hex )
{
	clear();
	int	sz = hex.size();
	if ( sz % 2 != 0 )
		{setInvalid( "invalid length for Hex input" );
		}
	int	nb = 0;
	unsigned char *v = NULL;
	if ( XENCODE::hexToBin( hex, &v, &nb ) )
		{acquire( nb, v );
		}
	else
		{setInvalid(
			"insertHex, internal failure in XENCODE::hexToBin" );
		return( false );
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBINOB::insertBase64( const std::string b64 )
{
	clear();
	int	nb = 0;
	unsigned char *v = NULL;
	if ( XENCODE::b64ToBin( b64, &v, &nb ) )
		{acquire( nb, v );
		}
	else
		{setInvalid(
		"insertBase64, internal failure in XENCODE::b64ToBin" );
		return( false );
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBINOB::insertFile( FILE *f )
{	// FILE POINTER MUST ALREADY BE OPENED, ROUTINE DOES NOT CLOSE FILE
	if ( NULL == f )
		{setInvalid( "insertFile, NULL file pointer" );
		return( false );
		}
	int	c;
	int	nbytes = 0;
	const	int	block_size = 100000;			// 100K
	int	size = block_size;
	unsigned char *value = (unsigned char *) malloc( size + 5 );
	if ( NULL == value )
		{setInvalid( "malloc failure" );
		return( false );
		}
	try
		{while ( ( c = fgetc( f ) ) != EOF )
			{value[nbytes++] = (char) c;
			if ( nbytes > size )
				{size += block_size;
				value = (unsigned char *) realloc( value,
					size + 5 );
				if ( NULL == value )
					{setInvalid( "realloc failure" );
					return( false );
					}
				}
			}				// TRIM EXCESS USAGE
		value = (unsigned char *) realloc( value, nbytes + 1 );
		if ( NULL == value )
			{setInvalid( "realloc failure" );
			return( false );
			}
		}
	catch ( ... )
		{setInvalid( "insertFile, unhandled exception" );
		return( false );
		}
	acquire( nbytes, value );
	return( isValid() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBINOB::append( const int input_size, const unsigned char *data )
{
	if ( ! isValid() )
		{return( false );
		}
	if ( input_size < 0 || NULL == data )
		{setInvalid( "Invalid input data" );
		return( false );
		}
	if ( 0 == input_size )
		{return( true );
		}
	if ( payload->getRefCount() > 1 )
		{
		XBV	*ptmp = payload;
		payload = new XBV( payload );	// MAKE UNIQUE COPY
		XBV::relinquish( &ptmp );
		}
	if ( ! payload->append( input_size, data ) )
		{setInvalid( "append failure" );
		}
	return( isValid() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBINOB::append( const XBINOB &b )
{
	bool	same = ( b == *this );
	XBV	*ptmp = same
		? new XBV( b.payload )
		: XBV::acquire( b.payload );
	bool	ok = append( ptmp->size(), ptmp->data() );
	XBV::relinquish( &ptmp );
	return( ok );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBINOB::extract( int *output_size, unsigned char **data ) const
{
	*output_size = -1;			// INITIALISE AS INVALID
	*data = NULL;
	if ( ! isValid() )		     	// INVALID CONTENTS
		{return( false );
		}
	XBV	*ptmp = XBV::acquire( payload );
	if ( 0 == ptmp->size() )			// EMPTY CASE
		{*output_size = 0;
		return( true );
		}
	*data = (unsigned char *) malloc( ptmp->size() );
	if ( NULL == *data )
		{return( false );
		}
	memcpy( *data, ptmp->data(), ptmp->size() );
	*output_size = ptmp->size();
	XBV::relinquish( &ptmp );
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const unsigned char *XBINOB::data( void ) const
{
	return( ( NULL != payload ) ? payload->data() : NULL );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string XBINOB::extractHex( void ) const
{
	if ( ! isValid() )
		{setCaveat( "extractHex, not possible with invalid data" );
		return( "!" );	// NOT HEX, SO CALLER SHOULD CHECK VALIDITY FIRST
		}
	if ( 0 == payload->size() )
		{return( "" );
		}
	int	len;
	char	*hx; 			// ENSURE VALUE CONSTANT DURING OUTPUT
	XBV	*ptmp = XBV::acquire( payload );
	bool	ok = XENCODE::binToHex( ptmp->data(), ptmp->size(), &hx, &len );
	XBV::relinquish( &ptmp );
	if ( ! ok )
		{setInvalid( "extractHex, internal failure in binToHex" );
		return( "!" );
		}
	std::string	s( hx );
	free( hx );
	return( s );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string XBINOB::extractBase64( void ) const
{
	if ( ! isValid() )
		{setCaveat( "extractBase64, not possible with invalid data" );
		return( "!" );	// NOT HEX, SO CALLER SHOULD CHECK VALIDITY FIRST
		}
	if ( 0 == payload->size() )
		{return( "" );
		}
	int	len_out;
	char	*out = NULL;		// ENSURE VALUE CONSTANT DURING OUTPUT
	XBV	*ptmp = XBV::acquire( payload );
	bool	ok = XENCODE::binToB64( ptmp->data(), ptmp->size(), &out,
		&len_out );
	XBV::relinquish( &ptmp );
	if ( ! ok )
		{setInvalid(
		"extractBase64, internal failure in XENCODE::binToB64");
		return( "" );
		}
	std::string	b( out );
	free( out );
	return( b );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBINOB::extractFile( FILE *f ) const
{	// FILE POINTER MUST ALREADY BE OPENED, ROUTINE DOES NOT CLOSE FILE
	if ( NULL == f )
		{return( false );
		}
	bool	ok = true;		// ENSURE VALUE CONSTANT DURING OUTPUT
	XBV	*ptmp = XBV::acquire( payload );
	int	nbytes = ptmp->size();
	const	unsigned	char *dat = ptmp->data();
	int	nb = 0;
	try
		{while ( nb < nbytes )
			{if ( EOF == fputc( dat[nb++], f ) )
				{ok = false;
				break;
				}
			}
		}
	catch ( ... )
		{ok = false;
		}
	XBV::relinquish( &ptmp );
	return( ok );
}
//---------------------------------------------------------------------------
XBINOB &XBINOB::operator=( const XBINOB &b )
{
	if ( b.payload == payload )
		{return( *this );       // ALREADY IDENTICAL
		}
	if ( ! b.isValid())
		{setInvalid( "copy of invalid object" );
		XBV::relinquish( &payload );
		return( *this );
		}
	XBV	*ptmp = payload;
	payload = XBV::acquire( b.payload );
	XBV::relinquish( &ptmp );
	return( *this );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBINOB::isEqual( const XBINOB &b ) const
{                              // TWO INVALID OBJECTS WILL (PROBABLY) BE EQUAL
	return( XBV::equivalent( payload, b.payload ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBINOB::operator==( const XBINOB &b ) const
{
	return( isEqual( b ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XBINOB::operator!=( const XBINOB &b ) const
{
	return( ! isEqual( b ) );
}
//===========================================================================


