#ifndef ALLOCATELOCALRESULTSTOWORKLISTENTRIESH
#define ALLOCATELOCALRESULTSTOWORKLISTENTRIESH

#include "ClusterIDs.h"
#include "IntList.h"
#include "WorklistDirectory.h"

namespace paulst
{
    class LoggingService;
};

namespace valc
{

class DBUpdateSchedule;
class ExceptionalDataHandler;
class ResultIndex;
class TestResult;
class WorklistEntries;
 
/*  Queries resultIndex for local results that remain unassociated with any worklist entry. For each, attempts to find a matching worklist entry.
    In order to be considered a match, the worklist entry must:
        - have the same sample descriptor
        - not already have a value for BuddyResultID
        - be for the same test
        - be for the same machine or a cluster in which the machine participates
        - have an 'eligible' status (see below)
    If a single matching worklist entry is found, then information describing the match is passed to resultIndex 
    (method 'allocateResultToWorklistEntry').
    Otherwise, a match failure is logged.
*/
class AllocateLocalResultsToWorklistEntries : public WorklistDirectory::Func
{
public:
    AllocateLocalResultsToWorklistEntries( int localMachineID, const ClusterIDs* clusterIDs, paulst::LoggingService* log, WorklistEntries* worklistEntries, ResultIndex* resultIndex, DBUpdateSchedule* dbUpdateSchedule, ExceptionalDataHandler* exceptionalDataHandler );
    void execute();
    void execute( const WorklistEntry* wle );
    int releaseReturnValue();
private:
    paulst::LoggingService* m_log;
    WorklistEntries* m_worklistEntries;
    ResultIndex* m_resultIndex;
    IntList m_matchingWorklistEntries;
    const TestResult* m_result;
    const ClusterIDs* m_clusterIDs;
    const int m_localMachineID;
    DBUpdateSchedule* m_dbUpdateSchedule;
    ExceptionalDataHandler* m_exceptionalDataHandler;

    bool isWorklistEntryStatusEligibleForAllocatingToResult( char status )
    {
        switch ( status )
        {
        case 'P':
        case 'Q':
        case 'S':
        case 'W':
        case 'C':
        case 'X':
            return true;
        }

        return false;
    }
};

};

#endif

