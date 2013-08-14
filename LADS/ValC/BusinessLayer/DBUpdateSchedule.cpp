#include "AcquireCriticalSection.h"
#include <boost/foreach.hpp>
#include "DBUpdateSchedule.h"
#include "DBUpdateTaskInsertSampleRun.h"
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

DBUpdateTask* DBUpdateSchedule::front() const
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        return m_updates.front();
    }
}

bool DBUpdateSchedule::noMoreUpdates() const
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        return m_updates.empty();
    }
}

void DBUpdateSchedule::pop_front()
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        m_updates.pop_front();
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

}

