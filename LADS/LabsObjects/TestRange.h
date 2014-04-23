//---------------------------------------------------------------------------

#ifndef TestRangeH
#define TestRangeH

#include <string>
#include <utility>

//---------------------------------------------------------------------------

class ResultValue
{
	double number;
	std::string text;

public:

	ResultValue() : number( 0 ), text( "0.00" )
	{}

	ResultValue( double dVal ) { setValue( dVal ); }
	ResultValue( const std::string & sVal ) { setValue( sVal ); }

	void setValue( double dVal );
	void setValue( const std::string & sVal );

	double asDouble() const { return number; }
	const std::string & asString() const { return text; }
	bool isValid() const { return number >= 0; }
};

//---------------------------------------------------------------------------

class ResultRange
{
	ResultValue low, high;

public:

	ResultRange()
	{}

	ResultRange( double min, double max ) : low( min ), high( max )
	{}

	ResultRange( const std::string & min, const std::string & max ) : low( min ), high( max )
	{}

	template< typename T > ResultRange & operator= ( const std::pair< T, T > & range ) {
		low = range.first;
		high = range.second;
		return *this;
	}

	std::pair< double, double > asDoubles() const {
		return std::pair< double, double >( low.asDouble(), high.asDouble() );
	}

	std::pair< std::string, std::string > asStrings() const {
		return std::pair< std::string, std::string >( low.asString(), high.asString() );
	}

	bool includes( ResultValue value ) const;
	bool isSet() const { return low.isValid() || high.isValid(); }
};

//---------------------------------------------------------------------------
#endif
