#ifndef DBUPDATETASKSYNCBUDDYDATABASEANDSAMPLERUNH
#define DBUPDATETASKSYNCBUDDYDATABASEANDSAMPLERUNH

#include <algorithm>
#include "BuddyRun.h"
#include "DBUpdateTask.h"
#include <iterator>
#include <vector>

namespace valc
{

class DBUpdateTaskSyncBuddyDatabaseAndSampleRun : public DBUpdateTask
{
public:
    template<typename BuddyRunIter>
    DBUpdateTaskSyncBuddyDatabaseAndSampleRun( BuddyRunIter begin, BuddyRunIter end )
    {
        std::copy( begin, end, std::back_inserter( m_newBuddyRuns ) );
    }

protected:
    std::string describeUpdate() const;
    void        updateDatabase();
private:
    std::vector< BuddyRun > m_newBuddyRuns;

    int insertNewSampleRunEntry( const BuddyRun& br );
    void updateBuddyDataseEntry( int buddySampleID, int sampleRunID  );
};

}

#endif

