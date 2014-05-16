#ifndef LOCALRUNITERATORH
#define LOCALRUNITERATORH

#include "API.h"
#include <boost/iterator/iterator_facade.hpp>
#include <boost/variant.hpp>

namespace valcui
{

std::string getRunID( const valc::LocalEntryIterator& i );

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
    LocalRunIterator();
    LocalRunIterator( const valc::LocalEntryIterator& begin, const valc::LocalEntryIterator& end );

private:
    valc::LocalEntryIterator m_current, m_end;

    friend class boost::iterator_core_access;

    void advance( int n );
    void advanceToNext();
    bool atEnd() const;
    bool atLocalRun() const;
    void increment();
    bool equal( const LocalRunIterator& other ) const;
    const valc::LocalRun& dereference() const;
};

}

#endif

