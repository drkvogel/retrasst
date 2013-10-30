//---------------------------------------------------------------------------

#ifndef StringUtilH
#define StringUtilH

#include <string>
#include <System.hpp>

//---------------------------------------------------------------------------

std::string bcsToStd( const String& s );
int compareIC( const std::string & str1, const std::string & str2, unsigned max = 0 );
std::string bdToStd( const TDate & dt );

//---------------------------------------------------------------------------

#endif
