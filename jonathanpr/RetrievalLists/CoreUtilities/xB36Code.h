//---------------------------------------------------------------------------

#ifndef xb36CodeH
#define xb36CodeH

#include <map>

//---------------------------------------------------------------------------

class xb36Code
{
	std::map< char, unsigned > decodeMap;
	char encodeMap[ 36 ];

	void fillMaps( char digit, unsigned value, char last );

public:

	xb36Code();

	unsigned __int64 decode( const std::string & alpha ) const;
	std::string encode( unsigned __int64 code ) const;
	int compare( const std::string & value, const std::pair< std::string, std::string > & range ) const;
};

//---------------------------------------------------------------------------

#endif
