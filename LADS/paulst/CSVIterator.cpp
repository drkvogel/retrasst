#include "CSVIterator.h"
#include "Util.h"

#include <algorithm>

namespace paulst
{

CSVIterator::CSVIterator( const std::string& s, bool exhausted, char separator )
 : m_s(s), m_separator(separator)
{
    m_pos = exhausted ? std::string::npos : m_s.find(m_separator);
}

CSVIterator::CSVIterator() : m_pos(std::string::npos) {}

CSVIterator::CSVIterator( const CSVIterator& i )
{
    *this = i;
}

CSVIterator& CSVIterator::operator=( const CSVIterator& i )
{
    m_s = i.m_s;
    m_pos = i.m_pos;
    m_separator = i.m_separator;
    return *this;
}

std::string CSVIterator::operator*() const
{
    if ( m_pos == std::string::npos )
    {
        return "";
    }

    std::size_t pos = m_pos;
    std::string s;

    while ( pos && m_s[--pos] != m_separator )
    {
        s.push_back( m_s[pos] );
    }

    std::reverse( s.begin(), s.end() );

    trim(s);
    
    return s;
}

CSVIterator& CSVIterator::operator++() // pre-increment
{
    m_pos = m_pos == std::string::npos ? std::string::npos :
        m_s.find( m_separator, m_pos + 1 );
    return *this;
}

CSVIterator CSVIterator::operator++(int) // post-increment
{
    CSVIterator copy(*this);
    ++(*this);
    return copy;
}

bool CSVIterator::operator==( const CSVIterator& other ) const
{
    return 0 == m_s.compare( other.m_s ) &&
        m_pos == other.m_pos;
}

bool CSVIterator::operator!=( const CSVIterator& other ) const
{
    return !(*this == other);
}

};

