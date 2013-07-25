#ifndef DBUPDATESCHEDULEH
#define DBUPDATESCHEDULEH

#include "API.h"
#include <map>
#include "SampleRunID.h"
#include <set>

namespace valc
{

class SampleRunIDResolutionService;

class DBUpdateSchedule : public DBUpdateStats
{
public:
    DBUpdateSchedule( );
    int totalNewSampleRuns()                        const;
    int totalUpdatesForSampleRunIDOnBuddyDatabase() const;
    void scheduleUpdate( int forBuddySampleID, const SampleRunID& sampleRunID );
private:
    std::map< int, SampleRunID > m_sampleRunIDUpdates;
};

};

#endif

