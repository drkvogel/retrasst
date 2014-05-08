
#include <string>
#include "LDbNameBase.h"

//---------------------------------------------------------------------------

std::string LDbNames::makeLower( const std::string & s )
{
	static const MAX_LENGTH = 30;
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

int LDbNames::compareIC( const std::string & s1, const std::string & s2 ) {
	return makeLower( s1 ).compare( makeLower( s2 ) ) == 0;
}

//---------------------------------------------------------------------------

bool LDbNames::LCMatcher::operator() ( const LDbNames & other ) const {
	return lcValue == makeLower( other.getName() ) || lcValue == makeLower( other.getDescription() );
}

//---------------------------------------------------------------------------

