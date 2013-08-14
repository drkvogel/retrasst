#include <algorithm>
#include <boost/bind.hpp>
#include "BuddySampleIDKeyedOnSampleRunID.h"
#include "Require.h"
#include "SampleRunIDResolutionService.h"

namespace valc
{

BuddySampleIDKeyedOnSampleRunID::BuddySampleIDKeyedOnSampleRunID( const SampleRunIDResolutionService* s )
    :
    m_sampleRunIDResolutionService(s)
{
}

bool buddySampleIDEquals( int buddySampleID, const BuddySampleIDKeyedOnSampleRunID::Entry& e )
{
    return e.second == buddySampleID;
}

bool BuddySampleIDKeyedOnSampleRunID::addEntry( const std::string& sampleRunID, int buddySampleID )
{
    bool inserted = false;

    if ( std::find_if( m_runBuddyMap.begin(), m_runBuddyMap.end(), boost::bind( buddySampleIDEquals, buddySampleID, _1 ) ) 
            == m_runBuddyMap.end()   )
    {
        m_runBuddyMap.insert( std::make_pair( sampleRunID, buddySampleID ) );
        inserted = true;
    }

    return inserted;
}

void BuddySampleIDKeyedOnSampleRunID::findBuddySampleIDsFor( const std::string& sampleRunID, IntList& out ) const
{
    std::pair<RunBuddyMap::const_iterator,RunBuddyMap::const_iterator> range = m_runBuddyMap.equal_range( sampleRunID );
    
    for ( RunBuddyMap::const_iterator i = range.first; i != range.second; ++i )
    {
        out.push_back( i->second );
    }
}

void BuddySampleIDKeyedOnSampleRunID::getBuddySampleIDsFor( const std::string& sampleRunID, int* buffer, int& bufferSize ) const
{
    IntList buddySampleIDs;
    
    findBuddySampleIDsFor( sampleRunID, buddySampleIDs );

    if ( m_sampleRunIDResolutionService->existsAnAlternativeEquivalentFor( sampleRunID ) )
    {
        // Add any bsids associted with the real-existing sample-run-id to which sampleRunID is equivalent
        findBuddySampleIDsFor( m_sampleRunIDResolutionService->getAlternativeEquivalentFor( sampleRunID ), buddySampleIDs );
    }

    if ( buddySampleIDs.size() <= bufferSize )
    {
        std::copy( buddySampleIDs.begin(), buddySampleIDs.end(), buffer );
    }

    bufferSize = buddySampleIDs.size();

    require( bufferSize ); // A sample-run without any associated buddy_database entries? If happens, then bad programming somewhere!
}

}


