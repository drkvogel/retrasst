#ifndef LOCALRUNITERATORH
#define LOCALRUNITERATORH

#include "API.h"
#include <boost/iterator/iterator_facade.hpp>
#include <boost/variant.hpp>

namespace valc
{

class VariantTypeCheck : public boost::static_visitor<>
{
public:
    int numLocalRuns, numBatchDelimiters;

    VariantTypeCheck() 
        :
        numLocalRuns(0),
        numBatchDelimiters(0)
    {
    }

    void operator()( const LocalRun& lr ) 
    {
        ++numLocalRuns;
    }

    void operator()( const BatchDelimiter& bd ) 
    {
        ++numBatchDelimiters;
    }

};

std::string getRunID( const LocalEntryIterator& i )
{
    const LocalRun& lr = boost::get<LocalRun>(*i);
    return lr.getRunID();
}

/*
An iterator over a sequence of LocalEntry instances, just picking out those that are LocalRuns.
*/
class LocalRunIterator : public boost::iterator_facade<
    LocalRunIterator,
    const LocalRun,
    boost::forward_traversal_tag
    >
{
public:
    LocalRunIterator() 
    {
    }

    LocalRunIterator( const LocalEntryIterator& begin, const LocalEntryIterator& end )
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
    LocalEntryIterator m_current, m_end;

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
            VariantTypeCheck vtc;

            boost::apply_visitor( vtc, *m_current );

            return vtc.numLocalRuns;
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

    const LocalRun& dereference() const
    {
        return boost::get<LocalRun>( *m_current );
    }
};

}

#endif

