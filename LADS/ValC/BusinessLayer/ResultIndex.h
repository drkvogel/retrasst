#ifndef RESULTSINDEXH
#define RESULTSINDEXH

#include "CritSec.h"
#include "IntList.h"
#include <map>
#include "ResultDirectory.h"

namespace valc
{

typedef std::multimap< int, int > ResultIDsKeyedOnWorklistID; // A single worklist entry may map to more than one result

class ResultIndex : public ResultDirectory
{
public:

	typedef std::map< int, const TestResult* > ResultsKeyedOnID;
    typedef ResultIDsKeyedOnWorklistID::const_iterator const_iterator;

    ResultIndex();
    void addIndexEntryForLocalResult( const TestResult* tr );
    void allocateResultToWorklistEntry( int resultID, int toWorklistID );
    Range<TestResultIterator> equal_range( int worklistID ) const;
    const TestResult* findResult( int resultID ) const;
    void listUnallocatedLocalResults( IntList& unallocatedResultIDs ) const;
private:

    ResultsKeyedOnID m_resultMap;
    ResultIDsKeyedOnWorklistID m_mapWorklistIDToResultID;
    paulst::CritSec m_criticalSection;

    ResultIndex( const ResultIndex& );
    ResultIndex& operator=( const ResultIndex& );
};

};

#endif

