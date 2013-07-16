#include "StrUtil.h"

#include <cmath>

namespace paulst
{

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

bool notWhiteSpace( const char& c )
{
    return ! std::isspace( c );
}

bool strip_prefix( char* str, const char* prefix )
{
    // 1st check that str really does start with prefix...
    if ( str != std::strstr( str, prefix ) )
        return false;

    unsigned int strlenPrefix = std::strlen(prefix);

    char* dest = 0;

    for ( char* c = str; *c != '\0'; ++c )
    {
        dest = c - strlenPrefix;

        if ( dest >= str )
        {
            *dest = *c;
        }
    }

    *(++dest) = '\0';
    
    return true;
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


    
}

