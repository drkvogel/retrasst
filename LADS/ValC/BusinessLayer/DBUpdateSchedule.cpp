#include "AcquireCriticalSection.h"
#include <algorithm>
#include <boost/foreach.hpp>
#include "DBTransactionHandler.h"
#include "DBUpdateSchedule.h"
#include "DBUpdateTaskSyncBuddyDatabaseAndSampleRun.h"
#include "DBUpdateTaskLinkResultToWorklistEntry.h"
#include <iterator>
#include "Task.h"

namespace valc
{

struct SameBuddyID
{
    int targetID{};

    SameBuddyID( int i ) : targetID(i)
    {
    }

    bool operator()( const BuddyRun& br ) const
    {
        return br.buddySampleID == targetID;
    }
};

DBUpdateSchedule::DBUpdateSchedule( const DBTransactionResources* dbtr )
    :
    m_dbTransactionResources( dbtr )
{
}

DBUpdateSchedule::~DBUpdateSchedule()
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        BOOST_FOREACH( stef::Task* t, m_updates )
        {
            delete t;
        }
    }
}

void DBUpdateSchedule::runQueuedUpdates( DBTransactionHandler* th )
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        if ( ! m_newBuddyRuns.empty() || ! m_newSampleRuns.empty() )
        {
            th->queue( 
                new DBUpdateTaskSyncBuddyDatabaseAndSampleRun( 
                    m_newBuddyRuns.begin(), 
                    m_newBuddyRuns.end(),
                    m_newSampleRuns.begin(), 
                    m_newSampleRuns.end(),
                    m_dbTransactionResources ) );
        }

        m_newBuddyRuns.clear();
        m_newSampleRuns.clear();

        while (  m_updates.size() )
        {
            stef::Task* t = m_updates.front();
            m_updates.pop_front();
            th->queue( t );
        }
    }
}


void DBUpdateSchedule::queueBuddyDatabaseUpdate( int forBuddySampleID, const IDToken& sampleRunID )
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        if ( 0 == std::count_if( m_newBuddyRuns.begin(), m_newBuddyRuns.end(), SameBuddyID( forBuddySampleID ) ) )
        {
            m_newBuddyRuns.push_back( BuddyRun( forBuddySampleID, sampleRunID ) );
        }
    }
}

void DBUpdateSchedule::queueSampleRunInsertions( SampleRuns::const_iterator begin, SampleRuns::const_iterator end )
{
    m_newSampleRuns.clear();
    std::copy( begin, end, std::back_inserter( m_newSampleRuns ) );
}

void DBUpdateSchedule::scheduleUpdateLinkingResultToWorklistEntry( int resultID, int worklistEntry )
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        m_updates.push_back ( new DBUpdateTaskLinkResultToWorklistEntry( 
                                resultID, worklistEntry, m_dbTransactionResources ) );
    }
}

}

