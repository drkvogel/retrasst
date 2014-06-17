#ifndef DBUPDATESCHEDULEH
#define DBUPDATESCHEDULEH

#include "BuddyRun.h"
#include "CritSec.h"
#include <deque>
#include <string>
#include <vector>

namespace valc
{

class DBTransactionHandler;
class DBUpdateTask;


class DBUpdateSchedule 
{
public:
    DBUpdateSchedule();
    ~DBUpdateSchedule();
    void queueScheduledUpdates( DBTransactionHandler* th );
    void scheduleUpdate( int forBuddySampleID, const std::string& candidateNewSampleRunID );
    void scheduleUpdateLinkingResultToWorklistEntry( int resultID, int worklistEntry );
private:
    std::deque< DBUpdateTask* > m_updates;
    paulst::CritSec             m_cs;
    std::vector< BuddyRun >     m_newBuddyRuns;
};

}

#endif

