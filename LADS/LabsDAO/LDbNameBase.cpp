
#include <string>
#include "LDbNameBase.h"

//---------------------------------------------------------------------------

std::string LDbNames::LCMatcher::makeLower( const std::string & s )
{
	char nameBuff[ MAX_LENGTH + 1 ];
	int i = 0;
	for( const char * c = s.c_str(); *c != '\0' && i < MAX_LENGTH; i ++, c ++ )
	{
		if( *c == ' ' || *c == '-' )
			nameBuff[ i ] = '_';
		else
			nameBuff[ i ] = char( std::tolower( *c ) );
	}
	nameBuff[ i ] = '\0';
	return nameBuff;
}

//---------------------------------------------------------------------------

bool LDbNames::LCMatcher::operator() ( const LDbNames & other ) const {
	return lcValue == makeLower( other.getName() )
		|| makeLower( value ) == makeLower( other.getDescription() );
}

//---------------------------------------------------------------------------

