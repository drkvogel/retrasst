#include "Cursor.h"
#include "ExceptionalDataHandler.h"
#include "QCSampleDescriptorDerivationStrategy.h"
#include "Require.h"
#include "ResultIndex.h"
#include "StringBuilder.h"
#include "WorklistEntryBuilder.h"
#include "WorklistEntryImpl.h"
#include "WorklistEntries.h"

namespace valc
{

WorklistEntryBuilder::WorklistEntryBuilder( WorklistEntries* worklistEntries, 
    ResultIndex* resultIndex, const std::string& inclusionRule,
    ExceptionalDataHandler* exceptionalDataHandler,
    QCSampleDescriptorDerivationStrategy* qcsdds )
    : 
    m_worklistEntries( worklistEntries ),
    m_resultIndex( resultIndex ),
    m_inclusionRule( inclusionRule ),
    m_exceptionalDataHandler( exceptionalDataHandler ),
    m_QCSampleDescriptorDerivationStrategy( qcsdds )
{
}

bool WorklistEntryBuilder::isQC() const
{
    return barcode.find( "QC" ) == 0;
}

bool WorklistEntryBuilder::accept( paulstdb::Cursor* worklistCursor )
{
    bool carryOn = true;

    reset();

    do
    {
        worklistCursor->read( COL_WL_RECORD_NO,         recordNo );
        worklistCursor->read( COL_WL_MACHINE_CID,       machineID );
        worklistCursor->read( COL_WL_BARCODE,           barcode );
        worklistCursor->read( COL_WL_TEST_CID,          testID );
        worklistCursor->read( COL_WL_GROUP_ID,          groupID );
        worklistCursor->read( COL_WL_CATEGORY_ID,       categoryID );
        worklistCursor->read( COL_WL_SAMPLE_ID,         sampleID );
        worklistCursor->read( COL_WL_PROJECT_CID,       projectID );
        worklistCursor->read( COL_WL_PROFILE_ID,        profileID );
        worklistCursor->read( COL_WL_PROFILE_NAME,      profileName );
        worklistCursor->read( COL_WL_TIME_STAMP,        timeStamp );
        worklistCursor->read( COL_WL_TS_SEQUENCE,       tsSequence );
        worklistCursor->read( COL_WL_STATUS,            status );
        worklistCursor->read( COL_WL_DILUENT,           diluent );
        worklistCursor->read( COL_WL_BUDDY_RESULT_ID,   buddyResultID );

        m_inclusionRule.init();
        m_inclusionRule.addParam( barcode );
        m_inclusionRule.addParam( categoryID );
        m_inclusionRule.addParam( projectID );
        m_inclusionRule.addParam( profileID );
        m_inclusionRule.addParam( profileName );
        m_inclusionRule.addParam( std::string(1, status) );
        m_inclusionRule.run();
        const bool acceptable = m_inclusionRule.getBooleanResult();
        if ( ! acceptable )
        {
            break;
        }

        if ( buddyResultID )
        {
            m_resultIndex->allocateResultToWorklistEntry( buddyResultID, recordNo );
        }

        if ( isQC() )
        {
            if ( m_QCSampleDescriptorDerivationStrategy )
            {
                sampleDescriptor = m_QCSampleDescriptorDerivationStrategy->deriveFromWorklistEntry( 
                    recordNo, barcode, machineID, testID, buddyResultID, status );
            }

            if ( sampleDescriptor.empty() )
            {
                break;
            }
        }
        else
        {
            // zero values for sampleID or projectID are unacceptable
            if ( ( sampleID == 0 ) || ( projectID == 0 ) )
            {
                carryOn = m_exceptionalDataHandler->notifyWorklistEntryIgnored( recordNo,
                    "If the barcode doesn't start with 'QC', then neither sample_id nor project_cid can be zero." ); 
                break;
            }

            sampleDescriptor = std::string() << sampleID << "/" << projectID;
        }

        m_worklistEntries->add( new WorklistEntryImpl(
            recordNo,
            sampleDescriptor,
            machineID,
            barcode,
            testID,
            groupID,
            categoryID,
            sampleID,
            projectID,
            profileID,
            profileName,
            timeStamp,
            tsSequence,
            status,
            diluent,
            buddyResultID,
            m_worklistEntries,
            m_resultIndex ) );
    }
    while( false );

    return carryOn;
}

void WorklistEntryBuilder::reset()
{
    recordNo = machineID = testID = groupID = categoryID = sampleID = projectID = profileID = tsSequence = buddyResultID = 0;
    barcode = profileName = sampleDescriptor = "";
    timeStamp = 0.0;
    status = '\0';
    diluent = 0.0;
}

}

