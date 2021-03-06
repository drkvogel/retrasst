#include <algorithm>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdlib>
#include <iterator>
#include "Require.h"
#include "StringBackedCursor.h"
#include "StrUtil.h"
#include <SysUtils.hpp>

namespace paulstdb
{

StringBackedCursor::StringBackedCursor( const std::string& s, FieldParsingStrategy fps )
    :
    m_currentRecord( s ),
    m_fieldParsingStrategy( fps )
{
    if ( ! endOfRecordSet() )
    {
        parseCurrentRow();
    }
}

void StringBackedCursor::close()
{
}

std::string StringBackedCursor::colValue( int col ) const
{
    require( static_cast<unsigned int>(col) < m_rowValues.size() );
    std::string value = m_rowValues.at(col);
    paulst::trim(value);
    return value;
}

bool StringBackedCursor::isNull( int col )
{
    return colValue(col).empty();
}

bool StringBackedCursor::endOfRecordSet() const
{
    return m_currentRecord == m_endOfRecords;
}

void StringBackedCursor::next()
{
    ++m_currentRecord;
    if ( ! endOfRecordSet() )
    {
        parseCurrentRow();
    }
}

void StringBackedCursor::parseCurrentRow()
{
	m_rowValues.clear();
    m_fieldParsingStrategy( *m_currentRecord, m_rowValues );
}

void StringBackedCursor::read( int col, bool&        outVal )
{
    outVal = boost::lexical_cast<short>( colValue(col) ) ? true : false;
}

void StringBackedCursor::read( int col, char&        outVal )
{
    std::string s = colValue(col);
    require( s.size() == 1 );
    outVal = s[0];
}

void StringBackedCursor::read( int col, float&       outVal )
{
	outVal = static_cast<float>( std::atof( colValue(col).c_str() ) );
}

void StringBackedCursor::read( int col, double&       outVal )
{
	outVal = static_cast<double>( std::atof( colValue(col).c_str() ) );
}

void StringBackedCursor::read( int col, int&         outVal )
{
	std::string s = colValue(col);
	outVal = paulst::toInt(s);
}

void StringBackedCursor::read( int col, std::string& outVal )
{
    outVal = colValue(col);
}

unsigned short extractInt( const std::string& s, int startPos, int length )
{
	std::string substr = s.substr( startPos, length );
	return static_cast<short>( paulst::toInt( substr ) );
}
void StringBackedCursor::read( int col, TDateTime&   outVal )
{
	unsigned short day, month, year, hour, minute;
	// expected format: 21/06/2013 06:43
	std::string s = colValue(col);
	try
	{
		day     = extractInt( s,  0, 2 );
		month   = extractInt( s,  3, 2);
		year    = extractInt( s,  6, 4);
		hour    = extractInt( s, 11, 2);
		minute  = extractInt( s, 14, 2);
		outVal = TDateTime( year, month, day, hour, minute, 0, 0 );
	}
	catch( ... )
	{
		std::string msg = std::string()
			<< "Failed to convert value of '" << s << "' for column "
			<< col << " to TDateTime. "
			<< "Current row is: " << *m_currentRecord;

		throw Exception( UnicodeString( msg.c_str() ) );
    }
}

void parseCSV( const std::string& record, RowValues& out )
{
    typedef paulst::CSVIterator<','> CSVIter;
	std::copy( CSVIter(record), CSVIter(), std::back_inserter( out ) );
}

}

