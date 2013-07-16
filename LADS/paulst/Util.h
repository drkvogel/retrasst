#ifndef UtilH
#define UtilH

#include <iomanip>
#include <string>
#include <System.hpp>

namespace paulst
{

#ifdef BUILDER_2007
#else
std::string getErrorMessage( unsigned long windowsErrorCode );
std::string stdstring( const UnicodeString& s );
std::string toString( const TDateTime& dt, const std::string& format = "yyyy/mm/dd:hh:nn:ss" );
#endif
std::string loadContentsOf( const std::string& filePath );
int toInt( const std::string& s );
TDateTime toDateTime( const std::string& s, 
    const TDateTime& defaultVal = TDateTime( 1900, 1, 1, 0, 0, 0, 0) );
std::string toString( int i );
void trim( std::string& s );
bool validateInt ( const std::string& s );
bool validateReal( const std::string& s );

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

