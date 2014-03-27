#ifndef XENCODE_C_DEFINED
#define XENCODE_C_DEFINED	1
/*===========================================================================*
 *
 * 	BASE-64 & HEX ENCODE AND DECODE (described in RFC 2045 section 6.8)
 *
 *	Fixed 17 Feb 2004, NG to use standard padding for last few characters
 *	see also http://www.elcel.com/products/opentop/index.html
 *
 *===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "xencode.h"
/*---------------------------------------------------------------------------*/
typedef	struct
	{
	char	c;
	unsigned	char	hi;	// lo * 16
	unsigned	char	lo;
	}
	HEX_MAP;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static const HEX_MAP hmap[23] =
{
	{'0', 0,  0}, {'1', 16, 1}, {'2', 32, 2}, {'3', 48, 3}, {'4', 64, 4},
	{'5', 80, 5}, {'6', 96, 6}, {'7',112, 7}, {'8',128, 8}, {'9',144, 9},
	{'A',160,10}, {'B',176,11}, {'C',192,12},
	{'D',208,13}, {'E',224,14}, {'F',240,15},
	{'a',160,10}, {'b',176,11}, {'c',192,12},
	{'d',208,13}, {'e',224,14}, {'f',240,15},
	{0, 0, 0 }
 };
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string	XENCODE::last_error = "";
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XENCODE::b64_chunking = false; /* SET TRUE FOR FULL RFC1421 COMPLIANCE */
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const char XENCODE::trans[65] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
/*---------------------------------------------------------------------------*/
std::string XENCODE::getLastError( void )
{
	return( last_error );
}
/*---------------------------------------------------------------------------*/
		/* DETERMINE IF CHUNKING SHOULD BE APPLIED DURING ENCODE */
void XENCODE::b64ChunkingSet( const bool do_chunking )
{
	b64_chunking = do_chunking;
}
/*---------------------------------------------------------------------------*/
#define	xBase64_MASK 	0x3F
#define	xBase64_ADD(x)	(*b64)[(*len_b64)++] = (x)
#define	xBase64_ENC(x)	xBase64_ADD( XENCODE::trans[x] )
						/* BINARY ARRAY -> BASE64 */
bool XENCODE::binToB64( const unsigned char *dat, const int len_dat,
	char **b64, int *len_b64 )
{
	if ( len_dat < 0 )
		{last_error = "binToB64, invalid len_dat < 0";
		return( false );
		}
	if ( NULL == dat )
		{last_error = "binToB64, NULL input *dat";
		return( false );
		}
	int	byte = 0, chunk = 0;
	unsigned long	quantum = 0;
	*len_b64 = 0;
	*b64 = (char *) malloc(	( ( len_dat * 4 ) / 3 )
		+ ( b64_chunking ? ( len_dat / 50 ) : 0 ) + 10 );
	if ( NULL == *b64 )
		{last_error = "binToB64, failed to malloc *b64";
		return( false );
		}
	for ( int i = 0; i < len_dat; i++ )
		{
		quantum = ( quantum << 8 ) | (unsigned char) *dat++;
		if ( 3 == ++byte )
			{
			xBase64_ENC( quantum >> 18 );
			xBase64_ENC( (quantum >> 12) & xBase64_MASK );
			xBase64_ENC( (quantum >> 6) & xBase64_MASK );
			xBase64_ENC( quantum & xBase64_MASK );
			quantum = 0;
			byte = 0;
			if ( 1 == b64_chunking && ++chunk >= 19 )
				{/* RFC1421 SPLITS OUTPUT INTO LINES OF 76 */
				xBase64_ADD( '\n' );
				chunk = 0;
				}
			}
		}
	if ( 1 == byte ) 			// TAKE CARE OF EXTRA BITS
		{
		quantum <<= 16;
		xBase64_ENC( quantum >> 18 );
		xBase64_ENC( (quantum >> 12) & xBase64_MASK );
		xBase64_ADD( '=' );
		xBase64_ADD( '=' );
		}
	else if ( 2 == byte )
		{
		quantum <<= 8;
		xBase64_ENC( quantum >> 18 );
		xBase64_ENC( (quantum >> 12) & xBase64_MASK );
		xBase64_ENC( (quantum >> 6) & xBase64_MASK );
		xBase64_ADD( '=' );
		}
	(*b64)[*len_b64] = '\0';
	return( true );
}
#undef	xBase64_MASK
#undef	xBase64_ADD
#undef	xBase64_ENC
/*---------------------------------------------------------------------------*/
bool XENCODE::binToB64( const unsigned char *dat, const int len_dat,
	std::string *b64 )
{
	int 	len_b64;
	char 	*ch_b64 = NULL;
	if ( ! binToB64( dat, len_dat, &ch_b64, &len_b64 ) || NULL == ch_b64 )
		{return( false );
		}
	*b64 = std::string( ch_b64, len_b64 );
	free( ch_b64 );
	return( true );
}
/*---------------------------------------------------------------------------*/

bool XENCODE::szToB64( const std::string sz, std::string *b64 )
{
	if ( NULL == b64 )
		{return( false );
		}
	int	len_b64 = 0;
	char	*dat_b64 = NULL;
	if ( ! binToB64( (const unsigned char *) sz.c_str(), (int) sz.size(),
			&dat_b64, &len_b64 ) || NULL == dat_b64 )
		{return( false );
		}
	*b64 = std::string( dat_b64, len_b64 );
	free( dat_b64 );
	return( true );
}
/*---------------------------------------------------------------------------*/
#define	xBase64_OUT(x)	(*dat)[(*len_dat)++] = (unsigned char) (x)
					/* DECODE BASE64 INTO BINARY ARRAY */
bool XENCODE::b64ToBin( const char *b64, const int len_b64,
	unsigned char **dat, int *len_dat )
{
	if ( len_b64 < 0 )
		{*dat = NULL;
		last_error = "b64ToBin, invalid len_b64 < 0";
		return( false );
		}
	if ( NULL == b64 )
		{*dat = NULL;
		last_error = "b64ToBin, NULL input *dat";
		return( false );
		}
	int	byte = 0;
	unsigned char 	input;
	unsigned long	quantum = 0;
	*len_dat = 0;
	*dat = (unsigned char *) malloc( ( ( 3 * len_b64 ) / 4 ) + 5 );
	if ( NULL == *dat )
		{last_error = "b64ToBin, failed to malloc *dat";
		return( false );
		}
	int	len = len_b64;
	bool	ok = true;
	while ( len > 0 && ( '\n' == b64[len-1] || '\r' == b64[len-1] ) )
		{len--;		// IGNORE TERMINATING END-OF-LINE CHARS
		}
	while ( len-- > 0 )
		{
		input = *b64++;
		if ( '\n' == input || '\r' == input )
			{continue;   // IGNORE EMBEDDED END-OF-LINE SEPARATORS
			}
		if( input == '=' )
			{			// END OF DATA MARKER REACHED
			if ( 3 == byte )
				{quantum <<= 6;
				xBase64_OUT( quantum >> 16 );
				xBase64_OUT( (quantum >> 8) & 0xff );
				ok = ( 0 == len );
				if ( ! ok )
					{last_error = "b64ToBin, invalid byte3 end";
					}
				return( ok );
				}
			else if ( 2 == byte )
				{quantum <<= 12;
				xBase64_OUT( quantum >> 16 );
				ok = ( 1 == len && '=' == *b64 );
				if ( ! ok )
					{last_error = "b64ToBin, invalid byte2 end";
					}
				return( ok );
				}
			else
				{free( *dat );
				*dat = NULL;
				last_error = "b64ToBin, invalid end";
				return( false ); 	// INVALID ENDING
				}
			}
		else
			{
			if( input >= 'A' && input <= 'Z' )
				{input += (unsigned char ) ( 0 - 'A' );
				}
			else if( input >= 'a' && input <= 'z' )
				{input += (unsigned char ) ( 26 - 'a' );
				}
			else if( input >= '0' && input <= '9' )
				{input += (unsigned char ) ( 52 - '0' );
				}
			else if( input == '+' )
				{input = 62;
				}
			else if( input == '/' )
				{input = 63;
				}
			else			// NOTE FAILURE AND RETURN
				{free( *dat );
				*dat = NULL;
				last_error = "b64ToBin, non-base64 input";
				return( false );
				}
			quantum = ( quantum << 6 ) | input;
			if( 4 == ++byte )
				{
				xBase64_OUT( quantum >> 16 );
				xBase64_OUT( (quantum >> 8) & 0xff );
				xBase64_OUT( quantum & 0xff );
				quantum = 0;
				byte = 0;
				}
			}
		}
	ok = ( 0 == byte );
	if ( ! ok )
		{last_error = "b64ToBin, finished with non-zero byte";
		}
	return( ok );
}
#undef	xBase64_OUT
/*---------------------------------------------------------------------------*/
bool XENCODE::b64ToBin( const char *b64,
	unsigned char **dat, int *len_dat )
{
	return( ( NULL != b64 )
		? b64ToBin( b64, strlen(b64), dat, len_dat ) : false );
}
/*---------------------------------------------------------------------------*/
bool XENCODE::b64ToBin( const std::string b64, unsigned char **dat,
	int *len_dat )
{
	return( b64ToBin( (char *)b64.c_str(), (int) b64.size(), dat, len_dat ) );
}
/*---------------------------------------------------------------------------*/
bool XENCODE::b64ToSz( const std::string b64, std::string *sz )
{
	if ( NULL == sz )
		{return( false );
		}
	int	len_sz = -1;
	unsigned char	*dat_sz = NULL;
	bool	ok = b64ToBin( b64, &dat_sz, &len_sz );
	if ( ! ok || NULL == dat_sz )
		{return( false );
		}
	int	i;
	for ( i = 0; i < len_sz; i++ )	// ENSURE OUTPUT IS A VALID STRING
		{if ( '\0' == dat_sz[i] )
			{return( false );
			}
		}
	*sz = std::string( (char *) dat_sz, len_sz );
	free( dat_sz );
	return( true );
}
/*---------------------------------------------------------------------------*/
bool XENCODE::b2h( const unsigned char b, char *h2 )
{
	*h2     = hmap[ b / 16 ].c;
	*(h2+1) = hmap[ b % 16 ].c;
	return( true );
}
/*---------------------------------------------------------------------------*/
bool XENCODE::binToHex( const unsigned char *dat, const int len_dat,
	char **hex, int *len_hex )
{
	if ( len_dat < 0 )
		{*hex = NULL;
		last_error = "binToHex, invalid len_dat < 0";
		return( false );
		}
	if ( NULL == dat )
		{*hex = NULL;
		last_error = "binToHex, NULL input *hex";
		return( false );
		}
	*len_hex = 2 * len_dat;
	*hex = (char *) malloc( *len_hex + 1 );
	if ( NULL == *hex )
		{last_error = "binToHex, failed to malloc *hex";
		return( false );
		}
	int	i;
	int	j = 0;
	for ( i = 0; i < len_dat; i++ )
		{
		(*hex)[j++] = hmap[ dat[i] / 16 ].c;
		(*hex)[j++] = hmap[ dat[i] % 16 ].c;
		}
	(*hex)[j] = 0;
	return( true );
}
/*---------------------------------------------------------------------------*/
bool XENCODE::binToHex( const unsigned char *dat, const int len_dat,
	std::string *hex )
{
	int 	len_hex;
	char 	*ch_hex = NULL;
	if ( ! binToHex( dat, len_dat, &ch_hex, &len_hex ) || NULL == ch_hex )
		{return( false );
		}
	*hex = std::string( ch_hex, len_hex );
	free( ch_hex );
	return( true );
}
/*---------------------------------------------------------------------------*/
bool XENCODE::szToHex( const std::string sz, std::string *hex )
{
	if ( NULL == hex )
		{return( false );
		}
	int	len_hex = 0;
	char	*dat_hex = NULL;
	if ( ! binToHex( (const unsigned char *) sz.c_str(), (int) sz.size(),
			&dat_hex, &len_hex ) || NULL == dat_hex )
		{return( false );
		}
	*hex = std::string( dat_hex, len_hex );
	free( dat_hex );
	return( true );
}
/*---------------------------------------------------------------------------*/
	/* CONVERT 2 CHARACTER HEX INTO 1 CHARACTER BINARY */
bool XENCODE::h2b( const char *h2, unsigned char *bin )
{
	const	HEX_MAP	*upper = hmap, *lower = hmap;
	while ( upper->c != *h2 )
		{if ( ! upper->c ) // END OF ARRAY, NON-HEX CHARACTER
			{last_error = "h2b, non-hex input";
			return( false );
			}
		upper++;
		}
	h2++;
	while ( lower->c != *h2 )
		{if ( ! lower->c )
			{last_error = "h2b, non-hex input";
			return( false );
			}
		lower++;
		}
	*bin = (unsigned char) ( upper->hi + lower->lo );
	return( true );
}
/*---------------------------------------------------------------------------*/
bool XENCODE::hexToBin( const char *hex, const int len_hex,
	unsigned char **dat, int *len_dat )
{
	if ( NULL == hex || len_hex < 0 || len_hex % 2 != 0 )
		{*dat = NULL;
		last_error = "hexToBin, invalid input";
		return( false );
		}
	*len_dat = len_hex / 2;
	*dat = (unsigned char *) malloc( (*len_dat) + 1 );
	if ( NULL == *dat )
		{return( false );
		}
	int	i;
	for ( i = 0; i < *len_dat; i++ )
		{if ( ! h2b( hex, (*dat)+i ) )
			{free( *dat );
			return( false );
			}
		hex += 2;
		}
	return( true );
}
/*---------------------------------------------------------------------------*/
bool XENCODE::hexToBin( const char *hex, unsigned char **dat, int *len_dat )
{
	return( ( NULL != hex )
		? hexToBin( hex, strlen(hex), dat, len_dat ) : false );
}
/*---------------------------------------------------------------------------*/
bool XENCODE::hexToBin( const std::string &hex, unsigned char **dat,
	int *len_dat )
{
	return( hexToBin( hex.c_str(), hex.size(), dat, len_dat ) );
}
/*---------------------------------------------------------------------------*/
bool XENCODE::hexToSz( const std::string hex, std::string *sz )
{
	if ( NULL == sz )
		{return( false );
		}
	int	len_sz = -1;
	unsigned char	*dat_sz = NULL;
	bool	ok = hexToBin( hex, &dat_sz, &len_sz );
	if ( ! ok || NULL == dat_sz )
		{return( false );
		}
	int	i;
	for ( i = 0; i < len_sz; i++ )	// ENSURE OUTPUT IS A VALID STRING
		{if ( '\0' == dat_sz[i] )
			{return( false );
			}
		}
	*sz = std::string( (char *) dat_sz, len_sz );
	free( dat_sz );
	return( true );
}
/*---------------------------------------------------------------------------*/
bool XENCODE::binToUrl( const char *dat, const int len_dat,
	std::string *url )
{
	*url = "";
	if ( len_dat < 0 )
		{last_error = "binToUrl, invalid len_dat < 0";
		return( false );
		}
	if ( NULL == dat )
		{last_error = "binToUrl, NULL input *dat";
		return( false );
		}
	char	*u = (char *) malloc( ( len_dat * 3 ) + 1 );
	if ( NULL == u )
		{last_error = "binToUrl, failed to malloc";
		return( false );
		}
	int	i;
	int	j = 0;
	unsigned char	ch;
	for ( i = 0; i < len_dat; i++ )
		{ch = (unsigned char) dat[i];
		if ( ' ' == ch )
			{u[j++] = '+';
			}
		else if ( isalnum( ch ) )
			{u[j++] = (char) ch;	// COPY ALPHANUMERIC CHARACTERS
			}
		else
			{			// HEX-ENCODING
			u[j++] = '%';
			u[j++] = hmap[ ch / 16 ].c;
			u[j++] = hmap[ ch % 16 ].c;
			}
		}
	u[j] = 0;
	*url = u;
	free( u );
	return( true );
}
/*---------------------------------------------------------------------------*/
bool XENCODE::isHex( const std::string ihx )
{						// IS A STRING VALID HEX
	int	len = ihx.size();
	if ( 0 != len % 2 )
		{return( false );
		}
	const	char	*h = ihx.c_str();
	unsigned char	bin;
	while ( ( len -= 2 ) >= 0 )
		{if ( ! h2b( h + len, &bin ) )
			{return( false );
			}
		}
	return( true );
}
/*===========================================================================*/
#endif

