#include "Util.h"

#include <SysUtils.hpp>

#include <windows.h>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <sstream>

namespace paulst
{

#ifdef BUILDER_2007
#else
std::string getErrorMessage( unsigned long windowsErrorCode )
{
    LPVOID lpvMessageBuffer;

    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_HMODULE|FORMAT_MESSAGE_FROM_SYSTEM,
        GetModuleHandleW(L"wininet.dll"), windowsErrorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (wchar_t*)&lpvMessageBuffer, 0, NULL);

    UnicodeString us( (wchar_t*)lpvMessageBuffer );

    LocalFree(lpvMessageBuffer);

    return stdstring( us.c_str() );
}
#endif

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

int toInt( const std::string& s )
{
    int i = 0;
    std::istringstream in(s);
    in >> i;
    return i;
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

#ifdef BUILDER_2007
#else
std::string stdstring( const UnicodeString& s )
{
    std::wstring ws(s.c_str());
    char* buffer = new char[ws.size() + 1];
    std::wcstombs( buffer, ws.c_str(), ws.size() + 1 );
    std::string str(buffer);
    delete []buffer;
    return str;
}

std::string toString( const TDateTime& dt, const std::string& format )
{
    char buffer[200];
    std::wstring ws = FormatDateTime( format.c_str(), dt ).c_str();
    std::wcstombs( buffer, ws.c_str(), 200 );
    return buffer;
}
#endif

bool notSpace( char c )
{
    return ! std::isspace( c );
}

void trim( std::string& s )
{
    std::string::iterator start = std::find_if( s.begin(), s.end(), notSpace );
    if ( start == s.end() )
    {
        s = "";
    }
    else
    {
        s = std::string( start, s.end() );
        std::string::reverse_iterator end = std::find_if( s.rbegin(),s.rend(),
            notSpace );
        int offTheEnd = std::distance( s.rbegin(), end );
        s.resize( s.size() - offTheEnd );
    }
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
    int digits        = std::count_if( s.begin(), s.end(), std::isdigit );
    int whitespace    = std::count_if( s.begin(), s.end(), std::isspace );

    if ( minusSigns > 1 || decimalPoints > 1 || digits == 0 || whitespace )
        return false;

    return  
        ( minusSigns == 0 || '-' == *(s.begin()) ) 
        &&
        (digits + decimalPoints + minusSigns == s.size() );
}

}

