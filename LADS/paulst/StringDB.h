#ifndef STRINGDBH
#define STRINGDBH

#include <memory>
#include <string>

#include "CSVIterator.h"
#include "Properties.h"
#include "Require.h"

namespace paulst
{

template<typename RowIter>
class Cursor
{
public:
    Cursor( RowIter begin, RowIter end ) : m_current(begin), m_end(end) 
    {
        if ( ! exhausted() )
        {
            m_properties.reset( new Properties( *m_current ) );
        }
    }

    bool exhausted() const
    {
        return m_current == m_end;
    }

    bool next()
    {
        require( ! exhausted() );
        ++m_current;
        m_properties.reset( exhausted() ? 0 : new Properties( *m_current ) ); 
        return ! exhausted();
    }

    std::string get( const std::string& columnName )
    {
        require( m_properties.get() );
        return m_properties->getValue( columnName );
    }

    std::string serializeCurrentRecord()
    {
        require( ! exhausted() );
        return *m_current;
    }


private:
    RowIter m_current, m_end;
    std::auto_ptr<Properties> m_properties;

    Cursor( const Cursor& c );
    Cursor& operator=(const Cursor& );
};

typedef Cursor< CSVIterator<'\n'> > LineByLineCursor;

class StringDB
{
public:
    StringDB( const std::string& csvAttributeValuePairs, const std::string& rowIDAttribute );
    void        appendData( const std::string& moreCSVAttributeValuePairs );
    std::string getData() const;
    void updateAttribute( const std::string& rowIdValue,
        const std::string& whichAttribute, const std::string& newValue );
    LineByLineCursor* newCursor();    
private:
    std::string m_data;
    std::string m_primaryKey;
    
    StringDB( const StringDB& );
    StringDB& operator=( const StringDB& );
};

};

#endif

