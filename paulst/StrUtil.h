#ifndef STRUTILH
#define STRUTILH

#include <algorithm>
#include <boost/algorithm/string/find.hpp>
#include <cctype>
#include <iomanip>
#include "Require.h"
#include <string>
#include <System.hpp>

std::ostream& operator<<( std::ostream& out, const UnicodeString& s );

namespace paulst
{

void        addTrailingPathSeparatorIfNone( std::string& path, char pathSeparator );
bool        endsWith( const char* str, const char* substr ); // Does str end with substr?
std::string formatFileSize( __int64 size );
bool        caseInsensitiveCompare( char a, char b );
int         count( const std::string& within, const std::string& substring );
std::string format( const char* format, ... );
std::string getErrorMessage( unsigned long windowsErrorCode );
bool        ifind( const std::string& findWhat, const std::string& inWhat );
bool        isDigit( char c );
bool        isSpace( char c );
std::string loadContentsOf( const std::string& filePath );
bool        notSpace( char c );
std::string stdstring( const UnicodeString& s );
bool        strip_prefix( char* str, const char* prefix );
void        trim( std::string& value );
TDateTime   toDateTime( const std::string& s, const TDateTime& defaultVal = TDateTime( 1900, 1, 1, 0, 0, 0, 0) );
int         toInt( const std::string& s );
std::string toString( int i );
std::string toString( const TDateTime& dt, const std::string& format = "yyyy/mm/dd:hh:nn:ss" );
bool        validateInt ( const std::string& s );
bool        validateReal( const std::string& s );


template<class Str>
struct SubString
{
    typename Str::const_iterator begin, end;
};

/*
Looks between start and end for the 1st instance of a substring sandwiched between startPattern
and endPattern. If such a substring is found (and an empty string is acceptable), then
iterators to the start and end of the substring are copied into 'found' and 'true' is 
returned. Otherwise false.
*/
template<class Str>
bool find( 
	typename Str::const_iterator start,
    typename Str::const_iterator end,
    const Str& startPattern, 
    const Str& endPattern, 
    SubString<Str>& found )
{
    typedef boost::iterator_range<typename Str::const_iterator> Range;

    Range inputSequence(start, end);

    Range startPatternInstance = boost::find_first(inputSequence , startPattern);

    if( startPatternInstance.empty() )
    {
        return false;
    }
    
    Range endPatternSearchSpace( startPatternInstance.end(), end );

    Range endPatternInstance = boost::find_first( endPatternSearchSpace, endPattern );

    if ( endPatternInstance.empty() )
    {
        return false;
    }

    found.begin = startPatternInstance.end();
    found.end   = endPatternInstance.begin();

    return true;
}

struct FileSize
{
    __int64 size;
    explicit FileSize( __int64 s ) : size(s) {}
};

//std::ostream& operator<<( std::ostream& os, const FileSize& f );

struct Equals
{
    char target;
    bool operator()( const char& c );
};

Equals equals( char c );


template<typename CharacterIterator, typename UnaryFunc> 
CharacterIterator find_last( CharacterIterator begin, CharacterIterator end, UnaryFunc matchCondition )
{
    CharacterIterator result = end;

    for ( CharacterIterator i = end; i > begin; )
    {
        --i;
        if ( matchCondition(*i) )
        {
            result = i;
            break;
        }
    }

    return result;
}
 
/*
    From the sequence begin->end, returns the last sequence of characters that has a terminating '\n'.

    Returns an empty string for both of the following scenarios:
    1) there is no newline in the sequence begin->end.
    2) the last newline is at the start of the sequence.
    3) the last newline in the sequence is immediately preceded by another newline
*/
template<typename CharacterIterator> 
std::string last_line( CharacterIterator begin, CharacterIterator end )
{
    CharacterIterator endOfLastLine = find_last( begin, end, equals('\n') );
    
    if ( ( endOfLastLine == end /*not found*/ ) || endOfLastLine == begin /*found at start*/) return "";

    //require( endOfLastLine > begin );

    CharacterIterator startOfLastLine = find_last( begin, endOfLastLine, equals('\n') );

    startOfLastLine = ( startOfLastLine == endOfLastLine /*not found*/) ? begin : startOfLastLine + 1;

    return std::string( startOfLastLine, endOfLastLine );
}    

template<typename CharacterIterator> char lastNonWhiteSpaceChar( CharacterIterator begin, CharacterIterator end )
{
    CharacterIterator i = find_last( begin, end, notSpace );

    return i == end ? '\0' : *i;
}

template<typename IfTrue, typename IfFalse> 
void conditionBasedOutput( bool condition, IfTrue outputIfTrue, IfFalse outputIfFalse, std::ostream& out )
{
    if ( condition )
    {
        out << outputIfTrue;
    }
    else
    {
        out << outputIfFalse;
    }
}

};

#endif

