#include "Require.h"
#include "ResultIndex.h"
#include "StringBuilder.h"
#include "WorklistEntryBuilder.h"
#include "WorklistEntryImpl.h"
#include "WorklistEntries.h"
#include "WorklistEntries.h"

namespace valc
{

WorklistEntryBuilder::WorklistEntryBuilder( WorklistEntries* worklistEntries, 
    ResultIndex* resultIndex, WorklistRelationsDataSource* worklistRelationsDataSource, const std::string& inclusionRule )
    : 
    m_worklistEntries( worklistEntries ),
    m_resultIndex( resultIndex ),
    m_worklistRelationsDataSource( worklistRelationsDataSource ),
    m_inclusionRule( inclusionRule )
{
}

bool WorklistEntryBuilder::isQC() const
{
    return barcode.find( "QC" ) == 0;
}

bool WorklistEntryBuilder::accept( Cursor* worklistCursor )
{
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

        worklistRelations = m_worklistRelationsDataSource->getRelations( recordNo );

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

        sampleDescriptor = isQC() ?  barcode << "/" << machineID : std::string() << sampleID << "/" << projectID;

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
            worklistRelations,
            m_worklistEntries,
            m_resultIndex ) );
    }
    while( false );

    return true;
}

void WorklistEntryBuilder::reset()
{
    recordNo = machineID = testID = groupID = categoryID = sampleID = projectID = profileID = tsSequence = buddyResultID = 0;
    barcode = profileName = sampleDescriptor = "";
    timeStamp = 0.0;
    status = '\0';
    diluent = 0.0;
    worklistRelations.resize( 0U );
}

}

