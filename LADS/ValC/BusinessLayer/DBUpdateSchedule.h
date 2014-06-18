#ifndef DBUPDATESCHEDULEH
#define DBUPDATESCHEDULEH

#include "BuddyRun.h"
#include "CritSec.h"
#include <deque>
#include "SampleRuns.h"
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
    void queueBuddyDatabaseUpdate( int forBuddySampleID, const IDToken& sampleRunID );
    void queueSampleRunInsertions( SampleRuns::const_iterator from, SampleRuns::const_iterator to );
    void runQueuedUpdates( DBTransactionHandler* th );
    void scheduleUpdateLinkingResultToWorklistEntry( int resultID, int worklistEntry );
private:
    std::deque< DBUpdateTask* > m_updates;
    paulst::CritSec             m_cs;
    std::vector< BuddyRun >     m_newBuddyRuns;
    SampleRuns                  m_newSampleRuns;
};

}

#endif

