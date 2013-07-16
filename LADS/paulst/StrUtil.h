#ifndef STRUTILH
#define STRUTILH

#include <cctype>
#include <string>

#include <boost/algorithm/string/find.hpp>

#include "Require.h"

#include <System.hpp>

std::ostream& operator<<( std::ostream& out, const UnicodeString& s )
{
    out << AnsiString( s );
    return out;
}

namespace paulst
{

template<class Str>
struct SubString
{
    Str::const_iterator begin, end;
};

/*
Looks between start and end for the 1st instance of a substring sandwiched between startPattern
and endPattern. If such a substring is found (and an empty string is acceptable), then
iterators to the start and end of the substring are copied into 'found' and 'true' is 
returned. Otherwise false.
*/
template<class Str>
bool find( 
    Str::const_iterator start, 
    Str::const_iterator end, 
    const Str& startPattern, 
    const Str& endPattern, 
    SubString<Str>& found )
{
    typedef boost::iterator_range<Str::const_iterator> Range;

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


void addTrailingPathSeparatorIfNone( std::string& path, char pathSeparator );
// Does str end with substr?
bool endsWith( const char* str, const char* substr );
std::string formatFileSize( __int64 size );
bool notWhiteSpace( const char& c );
bool strip_prefix( char* str, const char* prefix );

struct FileSize
{
    __int64 size;
    FileSize( __int64 s ) : size(s) {}
};

std::ostream& operator<<( std::ostream& os, const FileSize& f )
{
    os << formatFileSize( f.size );
    return os;
}

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

    require( endOfLastLine > begin );

    CharacterIterator startOfLastLine = find_last( begin, endOfLastLine, equals('\n') );

    startOfLastLine = ( startOfLastLine == endOfLastLine /*not found*/) ? begin : startOfLastLine + 1;

    return std::string( startOfLastLine, endOfLastLine );
}    

template<typename CharacterIterator> char lastNonWhiteSpaceChar( CharacterIterator begin, CharacterIterator end )
{
    CharacterIterator i = find_last( begin, end, notWhiteSpace );

    return i == end ? '\0' : *i;
}
};


#endif

