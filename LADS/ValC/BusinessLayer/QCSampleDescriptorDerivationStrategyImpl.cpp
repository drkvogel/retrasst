#include "BuddyDatabaseEntryIndex.h"
#include "ExceptionalDataHandler.h"
#include "LoggingService.h"
#include "QCSampleDescriptorDerivationStrategyImpl.h"
#include "ResultDirectory.h"
#include "StrUtil.h"

namespace valc
{

class LookupAssistant : public BuddyDatabaseEntryIndex::Visitor
{
public:
    LookupAssistant( const std::string& barcode, int testID)
        :
        m_candidateBuddySampleID(0),
        m_barcode( barcode ),
        m_testID( testID )
    {
    }

    int getBuddySampleID() const
    {
        return m_candidateBuddySampleID;
    }

    void visit( const BuddyDatabaseEntry* bde, const BuddyDatabaseEntryIndex::TestIDs& testsForWhichResults )
    {
        // Find the latest entry (i.e. highest value for buddy_sample_id) which matches on barcode and
        // doesn't already have a result for the specified test.
        if (    ( bde->barcode == m_barcode                         ) &&
                ( bde->buddy_sample_id > m_candidateBuddySampleID   ) && 
                ( 0U == testsForWhichResults.count( m_testID )      )        )
        {
            m_candidateBuddySampleID = bde->buddy_sample_id;
        }
    }
private:
    int m_candidateBuddySampleID;
    const std::string m_barcode;
    const int m_testID;
};

QCSampleDescriptorDerivationStrategyImpl::QCSampleDescriptorDerivationStrategyImpl(
    const ResultDirectory* resultDirectory,
    const BuddyDatabaseEntryIndex* buddyDatabaseEntries,
    ExceptionalDataHandler* exceptionalDataHandler )
    :
    m_resultDirectory( resultDirectory ),
    m_buddyDatabaseEntries( buddyDatabaseEntries ),
    m_exceptionalDataHandler( exceptionalDataHandler )
{
}

std::string makeSampleDescriptor( int buddySampleID )
{
    return paulst::format( "QC buddy_sample_id %d", buddySampleID );
}

/*
    Returns buddySampleID as a string value.
*/
std::string QCSampleDescriptorDerivationStrategyImpl::deriveFromBuddyDatabaseEntry( 
    int                 buddySampleID, 
    const std::string&  barcode, 
    int                 machineID, 
    int                 resTestID,    // Might be zero if the buddy_database entry has no results associated with it.
    int                 resWorklistID // Can also be zero.
    )
{
    return makeSampleDescriptor( buddySampleID );
}

std::string QCSampleDescriptorDerivationStrategyImpl::deriveFromWorklistEntry( 
    int                 worklistID, 
    const std::string&  barcode, 
    int                 machineID, 
    int                 testID, 
    int                 buddyResultID ,
    char                status
    )
{
    int buddySampleID = 0;

    // Is there a result that is linked to this worklist entry?
    int worklistEntryResultID = buddyResultID;

    if ( ! worklistEntryResultID )
    {
        Range<TestResultIterator> results = m_resultDirectory->equal_range( worklistID );

        if ( results.first != results.second )
        {
            const TestResult* r = *(results.first);
            worklistEntryResultID = r->getID();
        }
    }

    if ( worklistEntryResultID )
    {
        buddySampleID = m_buddyDatabaseEntries->lookupBuddySampleIDForResult( worklistEntryResultID );
    }
    else if ( status == 'Q' || status == 'P' )
    {
        LookupAssistant la( barcode, testID );
        m_buddyDatabaseEntries->accept( &la );
        buddySampleID = la.getBuddySampleID();
    //     Find the latest buddy_database entry for this barcode which doesn't already have an associated result for the test.
    }

    if ( ! buddySampleID )
    {
        m_exceptionalDataHandler->notifyWorklistEntryIgnored( worklistID,
            paulst::format(
            "Failed to find any suitable buddy_database entry with which "
            "QC worklist entry \%d (\%s) might be associated." 
            "Without a buddy_database entry, a sample-descriptor for the worklist entry cannot be derived.",
            worklistID, barcode.c_str() ) );
    }

    return buddySampleID ? makeSampleDescriptor(buddySampleID) : std::string();
}

}

