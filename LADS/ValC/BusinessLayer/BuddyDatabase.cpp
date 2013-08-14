#include <algorithm>
#include <boost/scoped_array.hpp>
#include "BuddyDatabase.h"
#include "BuddyDatabaseEntryIndex.h"
#include "BuddySampleIDKeyedOnSampleRunID.h"
#include "Require.h"

namespace valc
{

BuddyDatabase::BuddyDatabase( 
    int                                     localMachineID, 
    const SampleRuns*                       sr,
    const BuddySampleIDKeyedOnSampleRunID*  bsidKeyedOnRunID, 
    const BuddyDatabaseEntryIndex*          buddyDatabaseEntries )
    : 
    m_localMachineID                    ( localMachineID ), 
    m_sampleRuns                        ( sr ),
    m_buddySampleIDKeyedOnSampleRunID   ( bsidKeyedOnRunID ),
    m_buddyDatabaseEntryIndex           ( buddyDatabaseEntries )
{
}

BuddyDatabase::const_iterator BuddyDatabase::begin() const
{
    return m_sampleRuns->begin();
}

BuddyDatabase::const_iterator BuddyDatabase::end() const
{
    return m_sampleRuns->end();
}

int BuddyDatabase::getLocalMachineID() const
{
    return m_localMachineID;
}

BuddyDatabaseEntries BuddyDatabase::listBuddyDatabaseEntriesFor( const std::string& sampleRunID ) const
{
    int numIDs = 10;
    boost::scoped_array<int> buffer( new int[numIDs] );
    m_buddySampleIDKeyedOnSampleRunID->getBuddySampleIDsFor( sampleRunID, buffer.get(), numIDs );
    if ( numIDs > 10 )
    {
        buffer.reset( new int[numIDs] );
        const int adjustedBufferSize = numIDs;
        m_buddySampleIDKeyedOnSampleRunID->getBuddySampleIDsFor( sampleRunID, buffer.get(), numIDs );
        require( adjustedBufferSize == numIDs ); // Should be big enough now 
    }

    BuddyDatabaseEntries entries;

    for ( int i = 0; i < numIDs; ++i )
    {
        const int buddySampleID = buffer[i];
        entries.push_back( *(m_buddyDatabaseEntryIndex->get( buddySampleID )) );
    }

    return entries;
}

}

