#include <algorithm>
#include "API.h"
#include <boost/foreach.hpp>
#include "QCControlCache.h"
#include "Require.h"
#include "UncontrolledResult.h"

namespace valc
{

QCControlCache::QCControlCache()
{
}

void QCControlCache::add( const UncontrolledResult& ur, const IDToken& runID, const RuleResults& rr )
{
    CacheEntry e;

    e.resultID   = ur.resultID;
    e.testID     = ur.testID;
    e.runID      = runID;
    e.resultCode = rr.getSummaryResultCode();

    m_entries.push_back( e );
}

void QCControlCache::clear()
{
    m_entries.clear();
}

void QCControlCache::searchForMatchingQCControls( const IDTokenSequence& qcRunIDs, int testID, std::vector< QCControl >& out ) const
{
    for ( const CacheEntry& cacheEntry : m_entries )
    {
        if ( ( testID == cacheEntry.testID ) && qcRunIDs.contains( cacheEntry.runID ) )
        {
            out.push_back( QCControl( cacheEntry.resultID, cacheEntry.resultCode ) );
        }
    }
}

}

