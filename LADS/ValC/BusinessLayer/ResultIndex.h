#ifndef RESULTSINDEXH
#define RESULTSINDEXH

#include "CritSec.h"
#include "IntList.h"
#include <map>
#include "ResultDirectory.h"

namespace paulst
{
    class LoggingService;
};

namespace valc
{

typedef std::multimap< int, int > ResultIDsKeyedOnWorklistID; // A single worklist entry may map to more than one result

class ResultIndex : public ResultDirectory
{
public:

	typedef std::map< int, const TestResult* > ResultsKeyedOnID;
    typedef ResultIDsKeyedOnWorklistID::const_iterator const_iterator;

    ResultIndex();
    void addIndexEntryForResult( const TestResult* tr );
    void allocateResultToWorklistEntry( int resultID, int toWorklistID );
    Range<TestResultIterator> equal_range( int worklistID ) const;
    const TestResult* findResult( int resultID ) const;
    /*
        List the IDs of instances of TestResult that have been added (addIndexEntryForResult) but
        which have not been allocated to a worklist entry (allocateResultToWorklistEntry).
    */
    void listUnallocatedResults( IntList& unallocatedResultIDs ) const;
    /*
        Housekeeping. There may be worklist->result associations (built by allocateResultToWorklistEntry)
        which refer to results for which no instance has been added (via addIndexEntryForResult).
        While such associations remain, then there is the risk that the range of results obtained 
        via 'equal_range' may include results that are unobtainable (because never added). 
        This method removes such associations, so that, consequently, a TestResult instance is 
        guaranteed to exist for each item in the range obtained from 'equal_range'.
    */
    void removeReferencesToResultsNotLoaded( paulst::LoggingService* log = 0 );
private:

    ResultsKeyedOnID m_resultMap;
    ResultIDsKeyedOnWorklistID m_mapWorklistIDToResultID;
    paulst::CritSec m_criticalSection;

    ResultIndex( const ResultIndex& );
    ResultIndex& operator=( const ResultIndex& );
};

};

#endif

