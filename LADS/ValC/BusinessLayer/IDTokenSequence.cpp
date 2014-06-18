#include <algorithm>
#include <boost/bind.hpp>
#include <functional>
#include "IDTokenSequence.h"

namespace valc
{

IDTokenSequence::IDTokenSequence()
{
}

IDTokenSequence::IDTokenSequence( const IDTokenSequence& other )
    :
    m_sequence( other.m_sequence )
{
}

IDTokenSequence& IDTokenSequence::operator=( const IDTokenSequence& other )
{
    m_sequence = other.m_sequence;
    return *this;
}

void IDTokenSequence::push_back( const IDToken& idt )
{
    m_sequence.push_back( idt );
}

void IDTokenSequence::clear()
{
    m_sequence.clear();
}

bool IDTokenSequence::contains( const IDToken& idt ) const
{
    return m_sequence.end() != std::find_if(
        m_sequence.begin(), m_sequence.end(),
        boost::bind( std::equal_to<IDToken>(), _1, idt ) );
}

IDTokenSequence::const_iterator IDTokenSequence::begin() const  { return m_sequence.begin(); }

IDTokenSequence::iterator       IDTokenSequence::begin()        { return m_sequence.begin(); }

IDTokenSequence::const_iterator IDTokenSequence::end() const    { return m_sequence.end(); }

IDTokenSequence::iterator       IDTokenSequence::end()          { return m_sequence.end(); }


void IDTokenSequence::erase( IDTokenSequence::iterator i )
{
    m_sequence.erase(i);
}

IDTokenSequence::iterator IDTokenSequence::find( const IDToken& t )          
{ 
    return std::find_if( m_sequence.begin(), m_sequence.end(), boost::bind( std::equal_to<IDToken>(), _1, t ) );
}

IDTokenSequence::const_iterator IDTokenSequence::find( const IDToken& t ) const
{ 
    return std::find_if( m_sequence.begin(), m_sequence.end(), boost::bind( std::equal_to<IDToken>(), _1, t ) );
}

}

