#include "Require.h"
#include "ResultIndex.h"
#include "StringBuilder.h"
#include "WorklistEntryBuilder.h"
#include "WorklistEntryImpl.h"

namespace valc
{

WorklistEntryBuilder::WorklistEntryBuilder( const WorklistDirectory* worklistDirectory, 
    ResultIndex* resultIndex, WorklistRelationsDataSource* worklistRelationsDataSource )
    : 
    m_worklistDirectory( worklistDirectory ),
    m_resultIndex( resultIndex ),
    m_worklistRelationsDataSource( worklistRelationsDataSource )
{
}

bool WorklistEntryBuilder::isQC() const
{
    return barcode.find( "QC" ) == 0;
}

const WorklistEntry* WorklistEntryBuilder::newInstance( Cursor* worklistCursor )
{
    reset();

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

    if ( buddyResultID )
    {
        m_resultIndex->allocateResultToWorklistEntry( buddyResultID, recordNo );
    }

    sampleDescriptor = isQC() ?  barcode << "/" << machineID : std::string() << sampleID << "/" << projectID;

    require( sampleDescriptor.size() );

    return new WorklistEntryImpl(
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
        m_worklistDirectory,
        m_resultIndex );
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

