#include <algorithm>
#include "API.h"
#include <boost/foreach.hpp>
#include "QCControlCache.h"
#include "Require.h"
#include "RunIDC14n.h"
#include "UncontrolledResult.h"

namespace valc
{

QCControlCache::QCControlCache()
    :
    m_runIDC14n(0)
{
}

void QCControlCache::add( const UncontrolledResult& ur, const RuleResults& rr )
{
    CacheEntry e;

    e.resultID   = ur.resultID;
    e.testID     = ur.testID;
    e.runID      = ur.runID;
    e.resultCode = rr.getSummaryResultCode();

    m_entries.push_back( e );
}

void QCControlCache::clear()
{
    m_runIDC14n = 0;
    m_entries.clear();
}

void QCControlCache::searchForMatchingQCControls( const std::vector< std::string >& runIDs, int testID, std::vector< QCControl >& out ) const
{
    require( m_runIDC14n );

    BOOST_FOREACH( const CacheEntry& e, m_entries )
    {
        if ( 
             ( testID == e.testID ) 
             &&
             ( runIDs.end() != std::find( runIDs.begin(), runIDs.end(), m_runIDC14n->toCanonicalForm(e.runID) ) )
            )
        {
            out.push_back( QCControl( e.resultID, e.resultCode ) );
        }
    }
}

void QCControlCache::setRunIDC14n( RunIDC14n* r )
{
    m_runIDC14n = r;
}

}

