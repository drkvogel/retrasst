
#include "StringUtil.h"
#include <memory>

//---------------------------------------------------------------------------
//	Convert from Borland String (inc. Unicode) to C++ standard (ASCII)
//---------------------------------------------------------------------------

std::string bcsToStd( const String & s ) {
	char * buffer = new char[ s.Length() + 2 ];
	int n = 0, i = 1;
	while( n < s.Length() ) {
		wchar_t ch = s[ i ++ ];
		if( ch < 0x20 ) {
			ch = ' ';
		} else if( ch >= 0x7f ) {
			ch = '_';
		}
		buffer[ n ++ ] = ch;
	}
	std::string result( buffer, n );
	delete[] buffer;
	return result;
}

//---------------------------------------------------------------------------
//	Convert from Borland date or date/time to a C++ string in UK format
//---------------------------------------------------------------------------

std::string bdToStd( const TDate & dt ) {
	return bcsToStd( dt.FormatString( "dd_mmm_yyyy" ) );
}

//---------------------------------------------------------------------------
//	Compare ASCII strings, ignoring case, up to max characters (if max>0)
//	Return 0 if equal, >0 if str1 is the greater, <0 if str2 is greater
//---------------------------------------------------------------------------

int compareIC( const std::string & str1, const std::string & str2, unsigned max )
{
	size_t len = std::min( str1.length(), str2.length() );
	if( max > 0 && max < len ) {
		len = max;
	}
	for( unsigned i = 0; i < len; i ++ ) {
		int ch1 = std::tolower( str1[ i ] );
		int ch2 = std::tolower( str2[ i ] );
		if( ch2 != ch1 ) {
			return ch1 - ch2;
		}
	}
	if( max > 0 && len == max ) {
		return 0;
	} else {
		return str1.length() - str2.length();
	}
}

//---------------------------------------------------------------------------

