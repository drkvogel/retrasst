#ifndef QCCONTROLCACHEH
#define QCCONTROLCACHEH

#include "API.h"
#include "IDTokenSequence.h"
#include <string>
#include <vector>

namespace valc
{

class UncontrolledResult;

class QCControlCache
{
public:
    QCControlCache();
    /*
        Add a controlled QC result to the cache.
    */
    void add( const UncontrolledResult& qcResult, const IDToken& runID, const RuleResults& qcQuality );

    void clear();

    /*
        Search the cache for controlled QC results which:
            a) belong to one of the QC-runs specified by 'runIDs', and
            b) are for the specified test
    */
    void searchForMatchingQCControls( const IDTokenSequence& runIDs, int testID, std::vector< QCControl >& out ) const;

private:

    struct CacheEntry
    {
        int         resultID;
        int         testID;
        IDToken     runID;
        ResultCode  resultCode;
    };

    std::vector< CacheEntry > m_entries;

    QCControlCache( const QCControlCache& );
    QCControlCache& operator=( const QCControlCache& );
};

}

#endif

