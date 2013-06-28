/*---------------------------------------------------------------------------
 *
 *	12 July 2005, NG:	Always store name in lower case, no spaces
 *  20 June 2008, NG:	Match dash and underscore (e.g. for machine names)
 *	6 June 2012, NG:	Modified for C++Builder XE2
 *
 *-------------------------------------------------------------------------*/

#ifndef LDbNameBaseH
#define LDbNameBaseH

#include <functional>

//---------------------------------------------------------------------------

class LDbNames
{
	std::string name, description;

protected:

	LDbNames() : name( "unknown" ), description( "Unknown" )
	{}

	LDbNames( const std::string & externalName, const std::string & externalFull )
	 : name( externalName ), description( externalFull )
	{}

public:

	class LCMatcher : public std::unary_function< LDbNames, bool >
	{
		const std::string value, lcValue;
		static const short MAX_LENGTH = 30;

		static std::string makeLower( const std::string & s );

	public:

		LCMatcher( const std::string & s ) : value( s ), lcValue( makeLower( s ) )
		{}

		const std::string & getLCValue() const { return lcValue; }
		operator std::string() const { return lcValue; }
//		int compare( const std::string & s ) const;
		bool operator() ( const LDbNames & other ) const;
	};

	void setName( const std::string & nm ) { name = nm; }
	const std::string & getName() const { return name; }
	void setDescription( const std::string & nm ) { description = nm; }
	const std::string & getDescription() const { return description; }
};

//---------------------------------------------------------------------------

#endif

