#ifndef DBUPDATETASKSYNCBUDDYDATABASEANDSAMPLERUNH
#define DBUPDATETASKSYNCBUDDYDATABASEANDSAMPLERUNH

#include <algorithm>
#include "BuddyRun.h"
#include "DBUpdateTask.h"
#include <iterator>
#include "SampleRuns.h"
#include <vector>

namespace valc
{

class DBUpdateTaskSyncBuddyDatabaseAndSampleRun : public DBUpdateTask
{
public:
    template<typename BuddyRunIter, typename SampleRunIter>
    DBUpdateTaskSyncBuddyDatabaseAndSampleRun( 
        BuddyRunIter buddyRunBegin, BuddyRunIter buddyRunEnd,
        SampleRunIter sampleRunBegin, SampleRunIter sampleRunEnd )
    {
        std::copy(  buddyRunBegin,  buddyRunEnd, std::back_inserter( m_newBuddyRuns ) );
        std::copy( sampleRunBegin, sampleRunEnd, std::back_inserter( m_newSampleRuns ) );
    }

protected:
    std::string describeUpdate() const;
    void        updateDatabase();
private:
    std::vector<BuddyRun>   m_newBuddyRuns;
    SampleRuns              m_newSampleRuns;

    int insertNewSampleRunEntry( const SampleRun& sr );
    void updateBuddyDatabaseEntry( int buddySampleID, int sampleRunID  );
};

}

#endif

