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
    void listUnallocatedResults( IntList& unallocatedResultIDs ) const;
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

