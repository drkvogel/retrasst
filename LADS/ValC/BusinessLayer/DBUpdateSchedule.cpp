#include "AcquireCriticalSection.h"
#include <boost/foreach.hpp>
#include "DBTransactionHandler.h"
#include "DBUpdateSchedule.h"
#include "DBUpdateTaskInsertSampleRun.h"
#include "DBUpdateTaskLinkResultToWorklistEntry.h"
#include "DBUpdateTaskUpdateSampleRunID.h"
#include <set>

namespace valc
{

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
        if ( 0U == m_buddyDatabaseEntriesScheduledForUpdate.count( forBuddySampleID ) )
        {
            m_updates.push_front( new DBUpdateTaskInsertSampleRun  ( candidateNewSampleRunID, forBuddySampleID ) );
            m_updates.push_back ( new DBUpdateTaskUpdateSampleRunID( candidateNewSampleRunID, forBuddySampleID ) );
            m_buddyDatabaseEntriesScheduledForUpdate.insert( forBuddySampleID );
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

