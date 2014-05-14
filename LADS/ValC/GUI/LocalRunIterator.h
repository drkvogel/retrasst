#ifndef LOCALRUNITERATORH
#define LOCALRUNITERATORH

#include "API.h"
#include <boost/iterator/iterator_facade.hpp>
#include <boost/variant.hpp>
#include "Require.h"

namespace valcui
{

std::string getRunID( const valc::LocalEntryIterator& i )
{
    const valc::LocalRun& lr = boost::get<valc::LocalRun>(*i);
    return lr.getRunID();
}

/*
An iterator over a sequence of LocalEntry instances, just picking out those that are LocalRuns.
*/
class LocalRunIterator : public boost::iterator_facade<
    LocalRunIterator,
    const valc::LocalRun,
    boost::forward_traversal_tag
    >
{
public:
    LocalRunIterator() 
    {
    }

    LocalRunIterator( const valc::LocalEntryIterator& begin, const valc::LocalEntryIterator& end )
        :
        m_current( begin ),
        m_end    ( end   )
    {
        while ( ! atEnd() && ! atLocalRun() )
        {
            advanceToNext();
        }
    }

private:
    valc::LocalEntryIterator m_current, m_end;

    friend class boost::iterator_core_access;

    void advance( int n )
    {
        for ( int nTimes = n; ( ! atEnd() ) && nTimes; --nTimes )
        {
            advanceToNext();
        }
    }

    void advanceToNext()
    {
        require( m_current != m_end );

        ++m_current;
    }

    bool atEnd() const
    {
        return m_current == m_end;
    }

    bool atLocalRun() const
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

    void increment() 
    {
        advanceToNext();

        while ( ! atEnd() && ! atLocalRun() )
        {
            advanceToNext();
        }
    }

    bool equal( const LocalRunIterator& other ) const
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

    const valc::LocalRun& dereference() const
    {
        return boost::get<valc::LocalRun>( *m_current );
    }
};

}

#endif

