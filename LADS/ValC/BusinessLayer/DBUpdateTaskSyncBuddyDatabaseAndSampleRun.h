#ifndef DBUPDATETASKSYNCBUDDYDATABASEANDSAMPLERUNH
#define DBUPDATETASKSYNCBUDDYDATABASEANDSAMPLERUNH

#include <algorithm>
#include "BuddyRun.h"
#include <iterator>
#include "SampleRuns.h"
#include "SnapshotUpdateHandle.h"
#include "Task.h"
#include <vector>

namespace paulst
{
    class Config;
}

namespace paulstdb
{
    class DBConnection;
}

namespace valc
{

class DBTransactionResources;

class DBUpdateTaskSyncBuddyDatabaseAndSampleRun : public stef::Task
{
public:
    template<typename BuddyRunIter, typename SampleRunIter>
    DBUpdateTaskSyncBuddyDatabaseAndSampleRun( 
        BuddyRunIter buddyRunBegin, 
        BuddyRunIter buddyRunEnd,
        SampleRunIter sampleRunBegin, 
        SampleRunIter sampleRunEnd,
        const DBTransactionResources* dbtr )
        :
        m_dbTransactionResources( dbtr )
    {
        std::copy(  buddyRunBegin,  buddyRunEnd, std::back_inserter( m_newBuddyRuns ) );
        std::copy( sampleRunBegin, sampleRunEnd, std::back_inserter( m_newSampleRuns ) );
    }

protected:
    void doStuff();
private:
    const DBTransactionResources* const m_dbTransactionResources;
    std::vector<BuddyRun>               m_newBuddyRuns;
    SampleRuns                          m_newSampleRuns;

    const paulst::Config*   config() const;
    paulstdb::DBConnection* connection() const;
    int                     insertNewSampleRunEntry( const SampleRun& sr );
    SnapshotUpdateHandle    snapshotUpdateHandle() const;
    void                    updateBuddyDatabaseEntry( int buddySampleID, int sampleRunID  );
};

}

#endif

