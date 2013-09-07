#include "CSVIterator.h"
#include "Require.h"
#include "StringDB.h"
#include "Trace.h"
#include "Util.h"

#include <algorithm>

#include <sstream>


namespace paulst
{

StringDB::StringDB( const std::string& csvAttributeValuesPairs, const std::string& rowIDAttribute )
    :
    m_data(csvAttributeValuesPairs),
    m_primaryKey(rowIDAttribute)
{
}

std::string StringDB::getData() const
{
    return m_data;
}

struct PrimaryKeyEquals
{
    std::string target;
    
    PrimaryKeyEquals( const std::string& keyName,
        const std::string& keyValue )
    {
		target = keyName << "=" << keyValue;
    }

    bool operator()( const std::string& row )
    {
        return std::string::npos != 
            row.find( target );
    }
};

struct AttributeNameEquals
{
    std::string target;
    
    AttributeNameEquals( const std::string& name )
    {
        std::ostringstream s;
        s << name << "=";
        target = s.str();
    }

    bool operator()( const std::string& row )
    {
        return std::string::npos != 
            row.find( target );
    }
};


void StringDB::updateAttribute( const std::string& rowIDValue,
    const std::string& whichAttribute, const std::string& newValue )
{
    std::string recSep(1, '\n');

    std::ostringstream buf;
    // Copy into 'buf' all the lines except the one 
    // where m_primaryKey == rowIDValue
    std::remove_copy_if( 
        CSVIterator<'\n'>( m_data ),
        CSVIterator<'\n'>(),
        std::ostream_iterator<std::string>( buf, recSep.c_str() ),
        PrimaryKeyEquals( m_primaryKey, rowIDValue ) );

    // Find the line where m_primaryKey == rowIDValue
    CSVIterator<'\n'> i = std::find_if( 
        CSVIterator<'\n'>( m_data ),
        CSVIterator<'\n'>(),
        PrimaryKeyEquals( m_primaryKey, rowIDValue ) );
    require( i != CSVIterator<'\n'>() );
    std::string lineToEdit = *i;

    // Copy over everything except the changed attribute

    std::remove_copy_if(
        CSVIterator<','>( lineToEdit ),
        CSVIterator<','>(),
        std::ostream_iterator<std::string>( buf, "," ),
        AttributeNameEquals( whichAttribute ) );

    buf << whichAttribute << "=" << newValue << "," << '\n';

    m_data = buf.str();
}

void StringDB::appendData( const std::string& moreCSVAttributeValuePairs )
{
    m_data += moreCSVAttributeValuePairs;
}

LineByLineCursor* StringDB::newCursor()
{
    return new LineByLineCursor( CSVIterator<'\n'>( m_data ), CSVIterator<'\n'>() );
}

}

