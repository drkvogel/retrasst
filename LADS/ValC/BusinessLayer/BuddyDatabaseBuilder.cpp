#include <algorithm>
#include "API.h"
#include <boost/lexical_cast.hpp>
#include "BuddyDatabase.h"
#include "BuddyDatabaseBuilder.h"
#include "BuddyDatabaseEntryIndex.h"
#include "Cursor.h"
#include "DBUpdateSchedule.h"
#include "ExceptionalDataHandler.h"
#include "LoggingService.h"
#include "Projects.h"
#include "QCSampleDescriptorDerivationStrategy.h"
#include "Require.h"
#include "ResultIndex.h"
#include "RuleEngineContainer.h"
#include "SampleRun.h"
#include <sstream>
#include "StringBuilder.h"
#include "StrUtil.h"
#include <SysUtils.hpp>
#include "TestResultImpl.h"

#include <Dialogs.hpp>

namespace valc
{

struct SameRunID
{
    IDToken m_targetID;

    SameRunID( const IDToken& target )
        :
        m_targetID( target )
    {
    }

    bool operator()( const SampleRun& sr )
    {
        return sr.getID() == m_targetID;
    }
};

bool contains( const SampleRuns* sampleRunCollection, const IDToken& sampleRunID )
{
    return sampleRunCollection->end() != 
        std::find_if( sampleRunCollection->begin(), sampleRunCollection->end(), SameRunID( sampleRunID ) );
}

BuddyDatabaseBuilder::BuddyDatabaseBuilder( 
    const Projects*                     p, 
    ResultIndex*                        r, 
    SampleRuns*                         sampleRuns, 
    SampleRuns*                         candidateSampleRuns,
    const SampleRunIDResolutionService* s, 
    DBUpdateSchedule*                   dbUpdateSchedule,
    BuddyDatabaseEntryIndex*            buddyDatabaseEntryIndex,
    const std::string&                  inclusionRule,
    ExceptionalDataHandler*             exceptionalDataHandler,
    RuleEngineContainer*                ruleEngine,
    paulst::LoggingService*             log,
    QCSampleDescriptorDerivationStrategy* qcsdds,
    const ControlModel*                 controlModel
 )
    :
    m_projects                          ( p ),
    m_resultIndex                       ( r ),
    m_sampleRuns                        ( sampleRuns ),
    m_candidateSampleRuns               ( candidateSampleRuns ),
    m_sampleRunIDResolutionService      ( s ),
    m_dbUpdateSchedule                  ( dbUpdateSchedule ),
    m_buddyDatabaseEntryIndex           ( buddyDatabaseEntryIndex ),
    m_inclusionRule                     ( inclusionRule ),
    m_exceptionalDataHandler            ( exceptionalDataHandler ),
    m_ruleEngine                        ( ruleEngine ),
    m_log                               ( log ),
    m_QCSampleDescriptorDerivationStrategy( qcsdds ),
    m_controlModel                      ( controlModel )
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
            COL_SR_FAO_LEVEL_ONE, COL_SR_GROUP_ID  }; // from sample_run

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
            if ( ! c->isNull( COL_SR_GROUP_ID ) )
            {
                srGroupID = paulstdb::read<int>(*c, COL_SR_GROUP_ID );
            }
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
        m_inclusionRule.addParam( std::string(1, resActionFlag) );
        m_inclusionRule.addParam( std::string( 1, srFAOLevelOne ) );
        
        m_inclusionRule.run();

        const bool acceptable = m_inclusionRule.getBooleanResult();

        if ( ! acceptable )
        {
            break;
        }

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

        if ( isQC() )
        {
            sampleDescriptor = m_QCSampleDescriptorDerivationStrategy->deriveFromBuddyDatabaseEntry( 
                buddySampleID, barcode, machineID, resTestID, resWorklistID ); 
        }
        else
        {
            sampleDescriptor = paulst::format( "\%d/\%d", alphaSampleID, projectID );
        }

        m_buddyDatabaseEntryIndex->add( buddySampleID, alphaSampleID, barcode, databaseName, dateAnalysed );

        IDToken sampleRunID( hasSampleRun ? paulst::toString(srID) : sampleDescriptor, m_sampleRunIDResolutionService );

        SampleRuns* targetSampleRunCollection = hasSampleRun ? m_sampleRuns : m_candidateSampleRuns;

        // Only add a sample-run if the collection doesn't already include it.
        // As regards additions to the collection of candidate sample-runs, note that:
        //     a) the identifier used for the sample-run is simply the sample-descriptor
        //     b) the order of the query result-set is by buddy_sample_id (ascending).
        //     c) buddy_sample_id values get used as sequence_position values in sample-run instances. 
        // Consequently, given a scenario in which there are multiple rows which refer 
        // to the same sample and which are not associated with an existing sample-run,
        // then the sample-run instance constructed will be based on the first of 
        // these rows, i.e. the one with the lowest value for buddy_sample_id.
        if ( ! contains( targetSampleRunCollection, sampleRunID ) )
        {
            targetSampleRunCollection->push_back( 
                SampleRun(
                    sampleRunID,
                    sampleDescriptor, 
                    srIsOpen != 0, 
                    srCreatedWhen, 
                    srClosedWhen, 
                    srSequencePosition, 
                    srGroupID, 
                    isQC() ) );
        }

        if ( ! hasSampleRun )
        {
            m_dbUpdateSchedule->queueBuddyDatabaseUpdate( buddySampleID, sampleRunID );
        }

        if ( hasResult )
        {
            result = new TestResultImpl( resActionFlag, sampleDescriptor, resDateAnalysed, machineID, resID, sampleRunID, resTestID, resValue,
                            resText, isQC() ? 0 : m_controlModel );

            m_resultIndex->addIndexEntryForResult( result );
            m_buddyDatabaseEntryIndex->supplementEntryWithResultInfo( buddySampleID, resID, resTestID );

            if ( resWorklistID )
            {
                m_resultIndex->allocateResultToWorklistEntry( resID, resWorklistID );
            }

            UncontrolledResult r;
            r.testID        = resTestID;
            r.resultID      = resID;
            r.machineID     = machineID;
            r.resultValue   = resValue;
            r.resultText    = resText;
            r.barcode       = barcode;
            r.projectID     = projectID;
            r.dateAnalysed  = resDateAnalysed;
            r.actionFlag    = resActionFlag;

            if ( m_ruleEngine->queue( r, sampleRunID ) )
            {
                m_log->logFormatted( "Queued result \%d with Rule Engine.", r.resultID );
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
    srGroupID = paulst::Nullable<int>();
}

}

