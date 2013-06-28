//---------------------------------------------------------------------------

#include "vcl.h"
#include "TestRange.h"
#include "StringUtil.h"

#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------

void ResultValue::setValue( const std::string & val )
{
	String trimmed = String(val.c_str()).Trim();
	if( trimmed.IsEmpty() )
	{
		number = 0.0;
		text = "0.00";
	}
	else try
	{
		number = trimmed.ToDouble();
		text = bcsToStd( trimmed );
	}
	catch( ... )
	{
		number = -999.99;
	}
}

//---------------------------------------------------------------------------

void ResultValue::setValue( double val )
{
	number = val;
	if( val < 0 )
		text = "###";
	else
		text = bcsToStd(FormatFloat( "###0.00", val ));
}

//---------------------------------------------------------------------------

bool ResultRange::includes( ResultValue value ) const
{
	double minimum = low.asDouble();	// 0 if no lower limit
	if( value.asDouble() < minimum )
		return false;

	double maximum = high.asDouble();
	return (maximum == 0 || value.asDouble() <= maximum);
}

//---------------------------------------------------------------------------

