#include "AcquireCriticalSection.h"
#include <boost/foreach.hpp>
#include "DBTransactionHandler.h"
#include "DBUpdateSchedule.h"
#include "DBUpdateTaskSyncBuddyDatabaseAndSampleRun.h"
#include "DBUpdateTaskLinkResultToWorklistEntry.h"

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

DBUpdateSchedule::DBUpdateSchedule()
{
}

DBUpdateSchedule::~DBUpdateSchedule()
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        BOOST_FOREACH( DBUpdateTask* t, m_updates )
        {
            delete t;
        }
    }
}

void DBUpdateSchedule::queueScheduledUpdates( DBTransactionHandler* th )
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        if ( ! m_newBuddyRuns.empty() )
        {
            th->queue( new DBUpdateTaskSyncBuddyDatabaseAndSampleRun( m_newBuddyRuns.begin(), m_newBuddyRuns.end() ) );
        }

        m_newBuddyRuns.clear();

        while (  m_updates.size() )
        {
            DBUpdateTask* t = m_updates.front();
            m_updates.pop_front();
            th->queue( t );
        }
    }
}


void DBUpdateSchedule::scheduleUpdate( int forBuddySampleID, const std::string& candidateNewSampleRunID )
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        if ( 0 == std::count_if( m_newBuddyRuns.begin(), m_newBuddyRuns.end(), SameBuddyID( forBuddySampleID ) ) )
        {
            m_newBuddyRuns.push_back( BuddyRun( forBuddySampleID, candidateNewSampleRunID ) );
        }
    }
}

void DBUpdateSchedule::scheduleUpdateLinkingResultToWorklistEntry( int resultID, int worklistEntry )
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        m_updates.push_back ( new DBUpdateTaskLinkResultToWorklistEntry( resultID, worklistEntry ) );
    }
}

}

