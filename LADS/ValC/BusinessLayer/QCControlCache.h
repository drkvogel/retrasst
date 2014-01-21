#ifndef QCCONTROLCACHEH
#define QCCONTROLCACHEH

#include "API.h"
#include <string>
#include <vector>

namespace valc
{

class RunIDC14n;
class UncontrolledResult;

class QCControlCache
{
public:
    QCControlCache();
    /*
        Add a controlled QC result to the cache.
    */
    void add( const UncontrolledResult& qcResult, const RuleResults& qcQuality );

    void clear();

    /*
        Search the cache for controlled QC results which:
            a) belong to one of the QC-runs specified by 'runIDs', and
            b) are for the specified test
    */
    void searchForMatchingQCControls( const std::vector< std::string >& runIDs, int testID, std::vector< QCControl >& out ) const;

    /*
        The runID values of the UncontrolledResult instances (refer to the 'add' method above) may not be in their 
        canonical form.

        For more info, refer to documentation in RunIDC14n.h and in SampleRunIDResolutionService.h
    */
    void setRunIDC14n( RunIDC14n* r );

private:
    RunIDC14n* m_runIDC14n;

    struct CacheEntry
    {
        int         resultID;
        int         testID;
        std::string runID;
        ResultCode  resultCode;
    };

    std::vector< CacheEntry > m_entries;


    QCControlCache( const QCControlCache& );
    QCControlCache& operator=( const QCControlCache& );
};

}

#endif

