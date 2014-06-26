#include "LocalRunIterator.h"

#include "Require.h"

namespace valcui
{

valc::IDToken getRunID( const valc::LocalEntryIterator& i )
{
    const valc::LocalRun& lr = boost::get<valc::LocalRun>(*i);
    return lr.getRunID();
}

LocalRunIterator::LocalRunIterator() 
{
}

LocalRunIterator::LocalRunIterator( const valc::LocalEntryIterator& begin, const valc::LocalEntryIterator& end )
    :
    m_current( begin ),
    m_end    ( end   )
{
    while ( ! atEnd() && ! atLocalRun() )
    {
        advanceToNext();
    }
}

void LocalRunIterator::advance( int n )
{
    for ( int nTimes = n; ( ! atEnd() ) && nTimes; --nTimes )
    {
        advanceToNext();
    }
}

void LocalRunIterator::advanceToNext()
{
    require( m_current != m_end );

    ++m_current;
}

bool LocalRunIterator::atEnd() const
{
    return m_current == m_end;
}

bool LocalRunIterator::atLocalRun() const
{
    if ( atEnd() )
    {
        return false;
    }
    else
    {
        return boost::get<valc::LocalRun>( &(*m_current) );
    }
}

void LocalRunIterator::increment() 
{
    advanceToNext();

    while ( ! atEnd() && ! atLocalRun() )
    {
        advanceToNext();
    }
}

bool LocalRunIterator::equal( const LocalRunIterator& other ) const
{
    return  
        ( this->atEnd() && other.atEnd() ) 
        || 
        ( 
            ( ! ( this->atEnd() || other.atEnd() ) )
            &&
            ( getRunID(m_current) == getRunID(other.m_current)  )
        );
}

const valc::LocalRun& LocalRunIterator::dereference() const
{
    return boost::get<valc::LocalRun>( *m_current );
}

}


