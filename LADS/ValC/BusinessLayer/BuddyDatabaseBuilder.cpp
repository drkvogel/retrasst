#include "BuddyDatabaseBuilder.h"
#include "API.h"
#include <boost/lexical_cast.hpp>
#include "BuddyDatabase.h"
#include "DBUpdateSchedule.h"
#include "Projects.h"
#include "Require.h"
#include "ResultIndex.h"
#include "SampleRun.h"
#include <sstream>
#include "StringBuilder.h"
#include <SysUtils.hpp>
#include "TestResultImpl.h"

#include <Dialogs.hpp>

namespace valc
{

BuddyDatabaseBuilder::BuddyDatabaseBuilder( const Projects* p, ResultIndex* r, SampleRuns* sampleRuns, SampleRuns* candidateSampleRuns,
    const SampleRunIDResolutionService* s, DBUpdateSchedule* dbUpdateSchedule )
    :
    m_projects( p ),
    m_resultIndex( r ),
    m_sampleRuns( sampleRuns ),
    m_candidateSampleRuns( candidateSampleRuns ),
    m_sampleRunIDResolutionService( s ),
    m_dbUpdateSchedule( dbUpdateSchedule )
{
}

bool BuddyDatabaseBuilder::isQC() const
{
    return barcode.find( "QC" ) == 0;
}

bool BuddyDatabaseBuilder::accept( Cursor* c )
{
    reset();

    enum Cols { 
        COL_BUDDY_SAMPLE_ID, COL_BARCODE, COL_DATE_ANALYSED, COL_DATABASE_NAME, COL_ALPHA_SAMPLE_ID, COL_MACHINE_CID, // from buddy_database
        COL_BRF_BUDDY_RESULT_ID, COL_BRF_TEST_ID, COL_BRF_RES_VALUE, COL_BRF_ACTION_FLAG, COL_BRF_DATE_ANALYSED, // from buddy_result_float
        COL_BRF_RES_TEXT, COL_BRF_UPDATE_WHEN, COL_BRF_CBW_RECORD_NO,
        COL_SR_RUN_ID, COL_SR_IS_OPEN, COL_SR_CREATED_WHEN, COL_SR_CLOSED_WHEN, COL_SR_SEQUENCE_POSITION  }; // from sample_run

    c->read( COL_BUDDY_SAMPLE_ID    , buddySampleID );
    c->read( COL_BARCODE            , barcode       );
    c->read( COL_MACHINE_CID        , machineID     );
    c->read( COL_ALPHA_SAMPLE_ID    , alphaSampleID );
    c->read( COL_DATABASE_NAME      , databaseName  );

    if ( isQC() )
    {
        sampleDescriptor = barcode << "/" << machineID;
    }
    else
    {
        sampleDescriptor = std::string() << alphaSampleID << "/" << ( m_projects->findProjectIDForDatabase( databaseName ) );
    }

    // Negotiate the outer joins...

    if ( ! c->isNull( COL_BRF_BUDDY_RESULT_ID ) )
    {
        hasResult = true;
        c->read( COL_BRF_BUDDY_RESULT_ID, resID             );
        c->read( COL_BRF_TEST_ID        , resTestID         );
        c->read( COL_BRF_RES_VALUE      , resValue          );
        c->read( COL_BRF_ACTION_FLAG    , resActionFlag     );
        c->read( COL_BRF_DATE_ANALYSED  , resDateAnalysed   );
        c->read( COL_BRF_RES_TEXT       , resText           );
        c->read( COL_BRF_UPDATE_WHEN    , resUpdateWhen     );
        c->read( COL_BRF_CBW_RECORD_NO  , resWorklistID     );
    }

    if ( ! c->isNull( COL_SR_RUN_ID ) )
    {
        hasSampleRun = true;
        c->read( COL_SR_RUN_ID              , srID );
        c->read( COL_SR_IS_OPEN             , srIsOpen );
		c->read( COL_SR_CREATED_WHEN        , srCreatedWhen );
		if ( ! srIsOpen )
		{
			require( ! c->isNull( COL_SR_CLOSED_WHEN ) );
			c->read( COL_SR_CLOSED_WHEN, srClosedWhen );
		}
        c->read( COL_SR_SEQUENCE_POSITION   , srSequencePosition );
    }

    require( sampleDescriptor.size() );

    SampleRunID sampleRunID      = hasSampleRun ? SampleRunID(srID) : SampleRunID( sampleDescriptor, m_sampleRunIDResolutionService );
    SampleRuns* targetCollection = hasSampleRun ? m_sampleRuns : m_candidateSampleRuns;
    SampleRun sampleRun          = hasSampleRun ? 
                                        SampleRun( srID, sampleDescriptor, srIsOpen != 0, srCreatedWhen, srClosedWhen, srSequencePosition ) :
                                        SampleRun( sampleDescriptor, buddySampleID );

    targetCollection->push_back( sampleRun );

    if ( ! hasSampleRun )
    {
        m_dbUpdateSchedule->scheduleUpdate( buddySampleID, sampleRunID );
    }

    if ( hasResult )
    {
        result = new TestResultImpl( resActionFlag, sampleDescriptor, resDateAnalysed, machineID, resID, sampleRunID, resTestID, resValue );

        m_resultIndex->addIndexEntryForLocalResult( result );

        if ( resWorklistID )
        {
            m_resultIndex->allocateResultToWorklistEntry( resID, resWorklistID );
        }
    }
    
    return true;
}

void BuddyDatabaseBuilder::reset()
{
    resActionFlag = '\0';
    resDateAnalysed = resUpdateWhen = srClosedWhen = srCreatedWhen = 0.0;
    buddySampleID = machineID = resID = alphaSampleID = resTestID = resWorklistID = srSequencePosition = srID = srIsOpen = 0;
    barcode = databaseName = sampleDescriptor = resText = sampleRunID = "";
    resValue = 0.0;
    hasResult = hasSampleRun = false;
    result = 0;
}

}

