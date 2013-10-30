//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "xb36Code.h"

//---------------------------------------------------------------------------

xb36Code::xb36Code()
{
	fillMaps( '0', 0, '9' );
	fillMaps( 'a', 10, 'z' );
	fillMaps( 'A', 10, 'Z' );
}

//---------------------------------------------------------------------------

void xb36Code::fillMaps( char digit, unsigned value, char last )
{
	do
	{	decodeMap[ digit ] = value;
		encodeMap[ value ] = digit;
		value ++, digit ++;
	} while( digit <= last );
}

//---------------------------------------------------------------------------

unsigned __int64 xb36Code::decode( const std::string & alpha ) const
{
	unsigned __int64 code = 0;
	for( int pos = 0; pos < alpha.length(); pos++ )
	{
		char ch = alpha[ pos ];
		if( ch != ' ' && ch != '_' && ch != ',' )
		{
			std::map< char, unsigned >::const_iterator itc = decodeMap.find( ch );
			if( itc == decodeMap.end() || code >= 512409557603000000ui64 )
				throw Exception( String(alpha.c_str()) + " cannot be converted from base 36" );
			code = (code * 36) + itc -> second;
		}
	}
	return code;
}

//---------------------------------------------------------------------------

std::string xb36Code::encode( unsigned __int64 code ) const
{
	if( code < 36 ) {
		char ch = encodeMap[ unsigned( code ) ];
		return std::string( 1, ch );
	} else {
		char ch = encodeMap[ unsigned( code % 36 ) ];
		return encode( code / 36 ).append( 1, ch );
	}
}

//---------------------------------------------------------------------------
//	check if value is in range; return 0 if it is, -1 if below, 1 if above
//---------------------------------------------------------------------------

int xb36Code::compare( const std::string & value, const std::pair< std::string, std::string > & range ) const
{
	if( range.first.compare(range.second)==0 )
		return range.first.empty() ? 0 : value.compare( range.first );

	unsigned __int64 val = decode( value );
	if( !range.first.empty() && val < decode( range.first ) )
		return -1;

	if( !range.second.empty() && val > decode( range.second ) )
		return 1;

	return 0;
}

//---------------------------------------------------------------------------

