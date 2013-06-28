#include <algorithm>
#include "BuddyDatabase.h"

namespace valc
{

BuddyDatabase::BuddyDatabase( int localMachineID, const SampleRuns* sr, const SampleRunIDResolutionService* s )
    : 
    m_localMachineID( localMachineID ), 
    m_sampleRuns( sr ),
    m_sampleRunIDResolutionService( s )
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

