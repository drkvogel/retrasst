#include "BuddyDatabaseBuilder.h"
#include "API.h"
#include <boost/lexical_cast.hpp>
#include "BuddyDatabase.h"
#include "BuddyDatabaseEntryIndex.h"
#include "BuddySampleIDKeyedOnSampleRunID.h"
#include "Cursor.h"
#include "DBUpdateSchedule.h"
#include "ExceptionalDataHandler.h"
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

BuddyDatabaseBuilder::BuddyDatabaseBuilder( 
    const Projects*                     p, 
    ResultIndex*                        r, 
    SampleRuns*                         sampleRuns, 
    SampleRuns*                         candidateSampleRuns,
    const SampleRunIDResolutionService* s, 
    DBUpdateSchedule*                   dbUpdateSchedule,
    BuddySampleIDKeyedOnSampleRunID*    buddySampleIDKeyedOnSampleRunID,
    BuddyDatabaseEntryIndex*            buddyDatabaseEntryIndex,
    const std::string&                  inclusionRule,
    ExceptionalDataHandler*             exceptionalDataHandler
 )
    :
    m_projects                          ( p ),
    m_resultIndex                       ( r ),
    m_sampleRuns                        ( sampleRuns ),
    m_candidateSampleRuns               ( candidateSampleRuns ),
    m_sampleRunIDResolutionService      ( s ),
    m_dbUpdateSchedule                  ( dbUpdateSchedule ),
    m_buddySampleIDKeyedOnSampleRunID   ( buddySampleIDKeyedOnSampleRunID ),
    m_buddyDatabaseEntryIndex           ( buddyDatabaseEntryIndex ),
    m_inclusionRule                     ( inclusionRule ),
    m_exceptionalDataHandler            ( exceptionalDataHandler )
{
}

bool BuddyDatabaseBuilder::isQC() const
{
    return barcode.find( "QC" ) == 0;
}

bool BuddyDatabaseBuilder::accept( paulstdb::Cursor* c )
{
    bool carryOn = true;

    do
    {
        reset();

        enum Cols { 
            COL_BUDDY_SAMPLE_ID, COL_BARCODE, COL_DATE_ANALYSED, COL_DATABASE_NAME, COL_ALPHA_SAMPLE_ID, COL_MACHINE_CID, // from buddy_database
            COL_BRF_BUDDY_RESULT_ID, COL_BRF_TEST_ID, COL_BRF_RES_VALUE, COL_BRF_ACTION_FLAG, COL_BRF_DATE_ANALYSED, // from buddy_result_float
            COL_BRF_RES_TEXT, COL_BRF_UPDATE_WHEN, COL_BRF_CBW_RECORD_NO,
            COL_SR_RUN_ID, COL_SR_IS_OPEN, COL_SR_CREATED_WHEN, COL_SR_CLOSED_WHEN, COL_SR_SEQUENCE_POSITION,
            COL_SR_FAO_LEVEL_ONE  }; // from sample_run

        c->read( COL_BUDDY_SAMPLE_ID    , buddySampleID );
        c->read( COL_BARCODE            , barcode       );
        c->read( COL_DATE_ANALYSED      , dateAnalysed  );
        c->read( COL_MACHINE_CID        , machineID     );
        c->read( COL_ALPHA_SAMPLE_ID    , alphaSampleID );
        c->read( COL_DATABASE_NAME      , databaseName  );

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
            c->read( COL_SR_FAO_LEVEL_ONE       , srFAOLevelOne      );
        }
        else
        {
            srIsOpen = true;
            srSequencePosition = buddySampleID;
            srCreatedWhen = Now();
            srFAOLevelOne = 'y';
        }

        m_inclusionRule.init();
        m_inclusionRule.addParam( barcode );
        m_inclusionRule.addParam( databaseName );
        m_inclusionRule.addParam( hasResult ? std::string(1, resActionFlag) : std::string("NO_RESULT") );
        m_inclusionRule.addParam( std::string( 1, srFAOLevelOne ) );
        
        m_inclusionRule.run();

        const bool acceptable = m_inclusionRule.getBooleanResult();

        if ( ! acceptable )
        {
            break;
        }

        if ( isQC() )
        {
            sampleDescriptor = barcode << "/" << machineID;
        }
        else
        {
            int projectID = 0;

            if ( m_projects->canFindProjectIDForDatabase( databaseName ) )
            {
                projectID = m_projects->findProjectIDForDatabase( databaseName );
            }
            else if ( m_exceptionalDataHandler )
            {
                if ( m_exceptionalDataHandler->canProvideProjectIDFor( barcode ) )
                {
                    projectID = m_exceptionalDataHandler->getProjectIDFor( barcode );
                }
            }

            if ( 0 == projectID )
            {
                carryOn = m_exceptionalDataHandler->notifyBuddyDatabaseEntryIgnored( buddySampleID, "No Project ID" );
                break;
            }

            sampleDescriptor = std::string() << alphaSampleID << "/" << projectID;
        }

        std::string sampleRunID      = hasSampleRun ? paulst::toString(srID) : sampleDescriptor;
        SampleRuns* targetCollection = hasSampleRun ? m_sampleRuns : m_candidateSampleRuns;
        SampleRun   sampleRun( sampleRunID, sampleDescriptor, srIsOpen != 0, srCreatedWhen, srClosedWhen, srSequencePosition );

        m_buddyDatabaseEntryIndex->add( buddySampleID, alphaSampleID, barcode, databaseName, dateAnalysed );

        targetCollection->push_back( sampleRun );

        m_buddySampleIDKeyedOnSampleRunID->addEntry( sampleRunID, buddySampleID );

        if ( ! hasSampleRun )
        {
            m_dbUpdateSchedule->scheduleUpdate( buddySampleID, sampleRunID );
        }

        if ( hasResult )
        {
            result = new TestResultImpl( resActionFlag, sampleDescriptor, resDateAnalysed, machineID, resID, sampleRunID, resTestID, resValue );

            m_resultIndex->addIndexEntryForResult( result );

            if ( resWorklistID )
            {
                m_resultIndex->allocateResultToWorklistEntry( resID, resWorklistID );
            }
        }
    }
    while ( false );

    return carryOn;
}

void BuddyDatabaseBuilder::reset()
{
    resActionFlag = srFAOLevelOne = '?';
    resDateAnalysed = resUpdateWhen = srClosedWhen = srCreatedWhen = 0.0;
    buddySampleID = machineID = resID = alphaSampleID = resTestID = resWorklistID = srSequencePosition = srID = srIsOpen = 0;
    barcode = databaseName = sampleDescriptor = resText = sampleRunID = "";
    resValue = 0.0;
    hasResult = hasSampleRun = false;
    result = 0;
}

}

