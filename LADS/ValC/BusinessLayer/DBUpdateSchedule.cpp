#include <algorithm>
#include "DBUpdateSchedule.h"

namespace valc
{

DBUpdateSchedule::DBUpdateSchedule()
{
}

bool compare( const SampleRunID& r1, const SampleRunID& r2 )
{
    return r1.toString() < r2.toString();
}

bool equivalent( const SampleRunID& r1, const SampleRunID& r2 )
{
    return r1.toString() == r2.toString();
}

int DBUpdateSchedule::totalNewSampleRuns() const
{
    typedef std::vector< SampleRunID > RunIDs;

    RunIDs newRunIDs;

    for ( std::map< int, SampleRunID >::const_iterator i = m_sampleRunIDUpdates.begin(); i != m_sampleRunIDUpdates.end(); ++i )
    {
        const SampleRunID runID = i->second;
        
        if ( ! runID.existsOnDatabase() ) 
        {
            newRunIDs.push_back( runID );
        }
    }

    std::sort( newRunIDs.begin(), newRunIDs.end(), compare );

    RunIDs::iterator newEnd = std::unique( newRunIDs.begin(), newRunIDs.end(), equivalent );

    return std::distance( newRunIDs.begin(), newEnd );
}

int DBUpdateSchedule::totalUpdatesForSampleRunIDOnBuddyDatabase() const
{
    return m_sampleRunIDUpdates.size();
}

void DBUpdateSchedule::scheduleUpdate( int forBuddySampleID, const SampleRunID& sampleRunID )
{
    m_sampleRunIDUpdates.insert( std::make_pair( forBuddySampleID, sampleRunID ) );
}

}

