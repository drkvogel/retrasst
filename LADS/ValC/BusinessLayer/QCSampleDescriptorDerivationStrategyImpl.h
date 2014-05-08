#ifndef QCSAMPLEDESCRIPTORDERIVATIONSTRATEGYIMPLH
#define QCSAMPLEDESCRIPTORDERIVATIONSTRATEGYIMPLH

#include "QCSampleDescriptorDerivationStrategy.h"

namespace paulst
{
    class LoggingService;
}

namespace valc
{

class BuddyDatabaseEntryIndex;
class ExceptionalDataHandler;
class ResultDirectory;

class QCSampleDescriptorDerivationStrategyImpl : public QCSampleDescriptorDerivationStrategy
{
public:
    QCSampleDescriptorDerivationStrategyImpl(
        const ResultDirectory* resultDirectory,
        const BuddyDatabaseEntryIndex* buddyDatabaseEntries,
        ExceptionalDataHandler* exceptionalDataHandler );

    std::string deriveFromBuddyDatabaseEntry( 
        int                 buddySampleID, 
        const std::string&  barcode, 
        int                 machineID, 
        int                 resTestID,    // Might be zero if the buddy_database entry has no results associated with it.
        int                 resWorklistID // Can also be zero.
        );

    std::string deriveFromWorklistEntry( 
        int                 worklistID, 
        const std::string&  barcode, 
        int                 machineID, 
        int                 testID, 
        int                 buddyResultID ,
        char                status
        );

private:
    const ResultDirectory*          m_resultDirectory;
    const BuddyDatabaseEntryIndex*  m_buddyDatabaseEntries;
    ExceptionalDataHandler*         m_exceptionalDataHandler;
};

}

#endif

