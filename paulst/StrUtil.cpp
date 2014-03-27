#include "StrUtil.h"

#include <algorithm>
#include <cmath>
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include "ExceptionUtil.h"
#include <fstream>
#include <boost/functional.hpp>
#include <iterator>
#include <sstream>
#include <SysUtils.hpp>
#include <windows.h>


std::ostream& operator<<( std::ostream& out, const UnicodeString& s )
{
    out << AnsiString( s );
    return out;
}

namespace paulst
{

std::ostream& operator<<( std::ostream& os, const FileSize& f )
{
    os << formatFileSize( f.size );
    return os;
}

void addTrailingPathSeparatorIfNone( std::string& path, char pathSeparator )
{
    if ( path.empty() || pathSeparator != *( path.rbegin() ) )
    {
        path.push_back( pathSeparator );
    }
}

bool Equals::operator()( const char& c )
{
    return c == target;
}

Equals equals( char c )
{
    Equals e;
    e.target = c;
    return e;
}

bool caseInsensitiveCompare( char a, char b )
{
    return std::toupper(a) == std::toupper(b);
}

int count( const std::string& within, const std::string& substring )
{
    int found = 0;

    std::size_t pos = -1;

    while( ( pos = within.find( substring, pos + 1 ) ) != std::string::npos ) ++found;

    return found;
}

std::string format( const char* format, ... )
{
    char buffer[2048];
    va_list args;
    va_start(args, format );
    std::vsprintf( buffer, format, args );
    va_end(args);
    return buffer;
}

std::string formatFileSize( __int64 s )
{
    char buffer[1024];

    double size = s;
    static const double K = std::pow(2.0,10);
    static const double M = std::pow(2.0,20);
    static const double G = std::pow(2.0,30);
    static const double T = std::pow(2.0,40);
    
    if ( size < K )
    {
        std::sprintf( buffer, "%d bytes", (int)size ); 
    }
    else if ( size < M )
    {
        std::sprintf( buffer, "%.1f KB", (size / K) );
    }
    else if ( size < G )
    {
        std::sprintf( buffer, "%.1f MB", (size / M) );
    }
    else if ( size < T )
    {
        std::sprintf( buffer, "%.1f GB", (size / G) );
    }
    else
    {
        std::sprintf( buffer, "%.1f TB", (size / T) );
    }
    
    return std::string(buffer);
}

bool ifind( const std::string& findWhat, const std::string& inWhat )
{
	return inWhat.end() != std::search( inWhat.begin(), inWhat.end(), findWhat.begin(), findWhat.end(), caseInsensitiveCompare );
}

bool isDigit( char c )
{
    return std::isdigit( c );
}

bool isInt( const std::string& s )
{
	const int numDigits = std::count_if( s.begin(), s.end(), isDigit );
	const int size = s.size();

	return numDigits && (
		(numDigits == size)
		||
		(
			( numDigits == ( size - 1 ) )
			&&
			( '-' == s.at(0) )
		)
		);
}

bool isSpace( char c )
{
    return std::isspace( c );
}




bool endsWith( const char* str, const char* substr )
{
    unsigned int substrLen = std::strlen( substr );
    unsigned int strLen    = std::strlen(   str  );

    if ( strLen >= substrLen )
    {
        const char* strEnd = str + strLen;
        return 0 == std::strcmp( strEnd - substrLen, substr );
    }
    else
    {
        return false;
    }
}

std::string loadContentsOf( const std::string& filePath )
{
    std::ostringstream s;
    std::ifstream file( filePath.c_str() );
    
    if ( file )
    {
        s << file.rdbuf();
    }

    return s.str();
}

bool notSpace( char c )
{
    return ! std::isspace( c );
}

std::string stdstring( const UnicodeString& s )
{
    AnsiString as( s.c_str() );
    return as.c_str();
}

bool strip_prefix( char* str, const char* prefix )
{
    // 1st check that str really does start with prefix...
    if ( str != std::strstr( str, prefix ) )
        return false;

    unsigned int strlenPrefix = std::strlen(prefix);

    char* dest = str;

    for ( char* c = str + strlenPrefix; *c != '\0'; ++c )
    {
        *dest = *c;
        ++dest;
    }

    *dest = '\0';
    
    return true;
}

void trim( std::string& s )
{
    while ( s.begin() != s.end() && isSpace( * (s.begin()) ) )
    {
        s.erase( s.begin() );
    }

    std::string::reverse_iterator   theEnd  ( s.rbegin() ), 
                                    theStart( s.rend()   ),
                                    lastNonWhitespaceChar;

    lastNonWhitespaceChar = std::find_if( theEnd, theStart, notSpace );

    int offTheEnd = std::distance( theEnd, lastNonWhitespaceChar );

    s.resize( s.size() - offTheEnd );
}

int toInt( const std::string& s )
{
    const int strLen = s.size();

    if( strLen == 0 )
    {
        throw Exception( "Cannot convert an emtpy string to an integer value" );
    }

    const char& firstChar = s.at(0);

    if ( ! ( 
        ( firstChar == '-' ) 
        ||
        ( firstChar == '+' ) 
        ||
        std::isdigit( firstChar )
        ) )
    {
        paulst::exception( "First charact must be '-', '+' or a digit: '%s'", s.c_str() );
    }

    for ( int i = 1; i < strLen; ++i )
    {
        if ( ! std::isdigit( s.at(i) ) )
        {
            paulst::exception( "Cannot convert to integer because encountered a non-digit character: '%s'", s.c_str() );
        }
    }

    return std::atoi( s.c_str() );
}


void dateParseException( const std::string& s ) { paulst::exception( "Failed to parse string '%s' as a date.", s.c_str() ); }


TDateTime toDate( const std::string& s )
{
    std::string segments[3];

    int i = 0;

    for ( std::string::const_iterator c = s.begin(); c != s.end(); ++c )
    {
        if ( *c == '/' )
        {
            if ( ++i > 2 )
            {
                dateParseException( s );
            }

            continue;
        }

        segments[i].push_back( *c );
    }


    if ( 
        ( segments[0].empty() || segments[0].size() > 2 ) 
        ||
        ( segments[1].empty() || segments[1].size() > 2 ) 
        ||
        ( segments[2].size() != 4 ) 
        )
    {
        dateParseException( s );
    }
        

    for ( i = 0; i < 3; ++i )
    {
        if ( std::count_if( segments[i].begin(), segments[i].end(), boost::not1( isDigit ) ) )
        {
            dateParseException( s );
        }

        // Erase leading zeros
        while ( segments[i].size() && ( '0' == *(segments[i].begin()) ) )
        {
            segments[i].erase( segments[i].begin() );
        }

        if ( segments[i].empty() )
        {
            dateParseException( s );
        }
    }

    int dayMonthYear[3] = { 0, 0, 0 };

    for ( i = 0; i < 3; ++i )
    {
        dayMonthYear[i] = toInt(segments[i]);
    }

    return TDateTime( dayMonthYear[2], dayMonthYear[1], dayMonthYear[0] );
}


TDateTime toDateTime( const std::string& s, const TDateTime& defaultVal )
{
    char yearMonth, monthDay, dayHour, hourMin, minSec, secMsec;
    unsigned short year, month, day, hour, min, sec, msec;
    year = month = day = hour = min = sec = msec = 0;

    std::istringstream in(s);

    in >> year >> yearMonth >> month >> monthDay >> day
        >> dayHour >> hour >> hourMin >> min >> minSec
        >> sec >> secMsec >> msec;

    TDateTime t = defaultVal;

    if ( 
        ( year   > 1899 ) &&
        ( year   < 9999 ) &&
        ( month  < 13 )   &&
        ( month  > 0  )   &&
        ( day    < 32 )   &&
        ( day    > 0 )    &&
        ( hour   < 24 )   &&
        ( min    < 60 )   &&
        ( minSec < 60 )   &&
        ( msec   < 1000 ) )
    {
        t = TDateTime(year, month, day, hour, min, sec, msec);
    }

    return t;
}

std::string toString( int i )
{
    std::ostringstream s;
    s << i;
    return s.str();
}

std::string toString( const TDateTime& dt, const std::string& format )
{
    char buffer[200];
    UnicodeString s = FormatDateTime( format.c_str(), dt );
    return stdstring( s );
}

bool validateInt( const std::string& s )
{
    int i;
    std::string copy(s);
    trim(copy);
    std::istringstream in(copy);
    in >> i;
    std::ostringstream out;
    out << i;
    return in && ( out.str() == copy );
}

bool validateReal( const std::string& str )
{
    std::string s(str);
    trim(s);
    int minusSigns    = std::count(    s.begin(), s.end(), '-' );
    int decimalPoints = std::count(    s.begin(), s.end(), '.' );
    int digits        = std::count_if( s.begin(), s.end(), isDigit );
    int whitespace    = std::count_if( s.begin(), s.end(), isSpace );

    if ( minusSigns > 1 || decimalPoints > 1 || digits == 0 || whitespace )
        return false;

    return  
        ( minusSigns == 0 || '-' == *(s.begin()) ) 
        &&
        (digits + decimalPoints + minusSigns == s.size() );
}
    
}

