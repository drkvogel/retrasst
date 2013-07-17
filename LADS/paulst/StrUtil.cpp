#include "StrUtil.h"

#include <cmath>

#include <cstdlib>
#include <fstream>
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
    return std::atoi( s.c_str() );
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

