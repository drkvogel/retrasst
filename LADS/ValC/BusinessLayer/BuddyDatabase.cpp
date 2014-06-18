#include <algorithm>
#include <boost/scoped_array.hpp>
#include "BuddyDatabase.h"
#include "BuddyDatabaseEntryIndex.h"
#include "Require.h"

namespace valc
{

BuddyDatabase::BuddyDatabase( 
    int                                     localMachineID, 
    const SampleRuns*                       sr,
    const BuddyDatabaseEntryIndex*          buddyDatabaseEntries )
    : 
    m_localMachineID                    ( localMachineID ), 
    m_sampleRuns                        ( sr ),
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


}

