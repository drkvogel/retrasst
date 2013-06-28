#include <algorithm>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <iterator>
#include "Require.h"
#include "StringBackedCursor.h"
#include <SysUtils.hpp>

namespace valc
{

StringBackedCursor::StringBackedCursor( const std::string& s )
    :
    m_currentRecord( s )
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
    require( col < m_rowValues.size() );
    std::string value = m_rowValues.at(col);
    boost::trim(value);
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
    typedef paulst::CSVIterator<','> CSVIter;
    std::copy( CSVIter(*m_currentRecord), CSVIter(), std::back_inserter( m_rowValues ) );
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
    outVal = boost::lexical_cast<float>( colValue(col) );
}

void StringBackedCursor::read( int col, int&         outVal )
{
    outVal = boost::lexical_cast<int>( colValue(col) );
}

void StringBackedCursor::read( int col, std::string& outVal )
{
    outVal = colValue(col);
}

unsigned short extractInt( const std::string& s, int startPos, int length )
{
	std::string substr = s.substr( startPos, length );
	return boost::lexical_cast<unsigned short>( substr );
}
void StringBackedCursor::read( int col, TDateTime&   outVal )
{
	unsigned short day, month, year, hour, minute;
	// expected format: 21/06/2013 06:43
	std::string s = colValue(col);
	day     = extractInt( s,  0, 2 );
	month   = extractInt( s,  3, 2);
	year    = extractInt( s,  6, 4);
	hour    = extractInt( s, 11, 2);
	minute  = extractInt( s, 14, 2);
	
	outVal = TDateTime( year, month, day, hour, minute, 0, 0 );
}

}

