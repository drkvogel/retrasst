#ifndef DBUPDATESCHEDULEH
#define DBUPDATESCHEDULEH

#include "CritSec.h"
#include <deque>
#include <set>
#include <string>

namespace valc
{

class DBUpdateTask;

class DBUpdateSchedule
{
public:
    DBUpdateSchedule();
    ~DBUpdateSchedule();
    bool noMoreUpdates() const;
    DBUpdateTask* front() const;
    void pop_front();
    void scheduleUpdate( int forBuddySampleID, const std::string& candidateNewSampleRunID );
    void scheduleUpdateLinkingResultToWorklistEntry( int resultID, int worklistEntry );
private:
    std::deque< DBUpdateTask* >         m_updates;
    paulst::CritSec m_cs;
    std::set< int > m_buddyDatabaseEntriesScheduledForUpdate;
};

};

#endif

