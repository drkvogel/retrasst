#ifndef CSVIteratorH
#define CSVIteratorH

#include <boost/iterator/iterator_facade.hpp>
#include <memory>
#include <string>
#include "Util.h"

namespace paulst
{

template<char Separator>
class CSVIterator : public 
  boost::iterator_facade< CSVIterator<Separator>, std::string, boost::forward_traversal_tag>
{
public:
    explicit CSVIterator( const std::string& s )
        : m_s(s), m_pos(0), m_currentValue( new std::string() )
    {
        move(true);
    }

    CSVIterator()
        : m_pos ( std::string::npos ), m_currentValue( new std::string() )
    {
    }

    CSVIterator( const CSVIterator& other )
        : m_s(other.m_s), m_pos(other.m_pos), m_currentValue( new std::string( other.dereference() ) )
    {
    }

private:
    std::string m_s;
    std::auto_ptr<std::string> m_currentValue;
    std::size_t m_pos;

    friend class boost::iterator_core_access;

    void increment()
    {
        move();
    }

    bool equal( CSVIterator const& other ) const
    {
        return m_pos == other.m_pos;
    }

    bool exhausted() const
    {
        return m_pos == std::string::npos;
    }

    std::string& dereference() const
    {
        return *m_currentValue;
    }

    void move( bool beforeFirst = false )
    {
        m_currentValue->resize(0);

        if ( exhausted() )
            return;

        std::size_t startAt = beforeFirst? 0 : m_pos + 1;

        m_pos = m_s.find( Separator, startAt );

        if ( exhausted() )
            return;

        for ( ;startAt != m_pos; ++startAt )
            m_currentValue->push_back( m_s[startAt] );

        trim(*m_currentValue);
    }
};

};

#endif

