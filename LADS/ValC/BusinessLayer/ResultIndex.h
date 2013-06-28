#ifndef RESULTSINDEXH
#define RESULTSINDEXH

#include "CritSec.h"
#include "IntList.h"
#include <map>
#include "ResultDirectory.h"

namespace valc
{

class ResultIndex : public ResultDirectory
{
public:
    ResultIndex();
    void addIndexEntryForLocalResult( const TestResult* tr );
    void allocateResultToWorklistEntry( int resultID, int toWorklistID );
    const TestResult* findResult( int resultID ) const;
    const TestResult* findResultForWorklistEntry( int worklistID ) const;
    void listUnallocatedLocalResults( IntList& unallocatedResultIDs ) const;
private:
    typedef std::map< int, const TestResult* > ResultsKeyedOnID;
    ResultsKeyedOnID m_resultMap;
    typedef std::map< int, int > IDMap;
    IDMap m_mapWorklistIDToResultID;
    paulst::CritSec m_criticalSection;

    ResultIndex( const ResultIndex& );
    ResultIndex& operator=( const ResultIndex& );
};

};

#endif

