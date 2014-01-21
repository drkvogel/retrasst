#include "AbstractConnectionFactory.h"
#include "AnalysisActivitySnapshotImpl.h"
#include "ApplicationContext.h"
#include <boost/foreach.hpp>
#include "BuddyDatabase.h"
#include "ControlModel.h"
#include "DBUpdateSchedule.h"
#include "ExceptionUtil.h"
#include "Nullable.h"
#include "QueueBuilderParams.h"
#include "QueuedSamplesBuilderFunction.h"
#include "ResultAttributes.h"
#include "ResultDirectory.h"
#include "SampleRunIDResolutionService.h"
#include "SnapshotUpdateTaskQRerun.h"
#include "WorklistEntries.h"
#include "WorklistLinks.h"

namespace valc
{

AnalysisActivitySnapshotImpl::AnalysisActivitySnapshotImpl( 
    const BuddyDatabase*            bdb, 
    const ResultDirectory*          rd, 
    WorklistEntries*                wd, 
    WorklistLinks*                  wl,
    DBUpdateSchedule*               dbUpdateSchedule,
    SampleRunIDResolutionService*   sampleRunIDResolutionService,
    ApplicationContext*             appContext,
    int                             pendingUpdateWaitTimeoutSecs,
    SampleRunGroupIDGenerator*      sampleRunGroupIDGenerator,
    ControlModel*                   controlModel )
    : 
    m_buddyDatabase                 ( bdb ),
    m_log                           ( appContext->log),
    m_resultDirectory               ( rd ),
    m_worklistEntries               ( wd ),
    m_worklistLinks                 ( wl ),
    m_dbUpdateSchedule              ( dbUpdateSchedule ),
    m_sampleRunIDResolutionService  ( sampleRunIDResolutionService ),
    m_appContext                    ( appContext ),
    m_resultAttributes              ( appContext->resultAttributes ),
    m_updateHandle                  ( this ),
    m_dbTransactionHandler          (   
                                        appContext->connectionFactory->createConnection( 
                                            appContext->getProperty("DBUpdateThreadConnectionString"),
                                            appContext->getProperty("DBUpdateThreadSessionReadLockSetting") ),
                                        appContext->log,
                                        m_updateHandle,
                                        paulst::toInt(appContext->getProperty("DBUpdateThreadShutdownTimeoutSecs")),
                                        std::string("true") == appContext->getProperty("DBUpdateThreadCancelPendingUpdatesOnShutdown"),
                                        appContext->taskExceptionUserAdvisor,
                                        appContext->config 
                                     ),
    m_pendingUpdateWaitTimeoutSecs  ( pendingUpdateWaitTimeoutSecs ),
    m_snapshotUpdateThread          ( &m_dbTransactionHandler, m_updateHandle, appContext->log, appContext->taskExceptionUserAdvisor ),
    m_worklistRelativeImpl          ( wl ),
    m_sampleRunGroupModel           ( sampleRunGroupIDGenerator ),
    m_runIDC14n                     ( *sampleRunIDResolutionService ),
    m_controlModel                  ( controlModel )
{
    m_localRunImpl.setSampleRunGroupModel( &m_sampleRunGroupModel );

    // Group IDs are unstable while assignments are on-going.  (This is 
    // demonstrated by UnitTests/SampleRunGroupModelTest, test 13.)
    // So we build SampleRunGroupModel first, and then use it to get (reliable) group ID values.
    BOOST_FOREACH( const SampleRun& sr, *m_buddyDatabase )
    {
        m_sampleRunGroupModel.assignToGroup( sr.getID(), sr.isQC(), sr.getGroupID() );
    }

    paulst::Nullable<int> previousGroupID;

    BOOST_FOREACH( const SampleRun& sr, *m_buddyDatabase )
    {
        paulst::Nullable<int> groupID( m_sampleRunGroupModel.getGroupID( sr.getID() ) );

        if ( groupID != previousGroupID )
        {
            m_localEntries.push_back( BatchDelimiter() );
            previousGroupID = groupID;
        }

        LocalRun lr( sr.getSampleDescriptor(), sr.getID() );
        m_localRunImpl.introduce( lr, sr.isOpen() );
        m_localEntries.push_back( lr );
    }

    controlModel->setRunIDC14n( &m_runIDC14n );
    controlModel->setSampleRunGroupModel( &m_sampleRunGroupModel );

    QueuedSamplesBuilderFunction buildQueue( new QueueBuilderParams( bdb, wd, m_appContext->clusterIDs ) );
    buildQueue( &m_queuedSamples ); 
}

AnalysisActivitySnapshotImpl::~AnalysisActivitySnapshotImpl()
{
    delete m_buddyDatabase;
    delete m_resultDirectory;
    delete m_worklistEntries;
    delete m_worklistLinks;
    delete m_dbUpdateSchedule;
}

bool AnalysisActivitySnapshotImpl::compareSampleRunIDs( const std::string& oneRunID, const std::string& anotherRunID )    const
{
    return m_sampleRunIDResolutionService->compareSampleRunIDs( oneRunID, anotherRunID );
}

RuleResults AnalysisActivitySnapshotImpl::getRuleResults( int forResultID ) const
{
    return m_resultAttributes->getRuleResults( forResultID );
}

bool AnalysisActivitySnapshotImpl::hasRuleResults( int forResultID ) const
{
    return m_resultAttributes->hasRuleResults( forResultID );
}

BuddyDatabaseEntries AnalysisActivitySnapshotImpl::listBuddyDatabaseEntriesFor( const std::string& sampleRunID )   const
{
    return m_buddyDatabase->listBuddyDatabaseEntriesFor( sampleRunID );
}

void AnalysisActivitySnapshotImpl::runPendingDatabaseUpdates( bool block )
{
    m_dbUpdateSchedule->queueScheduledUpdates( &m_dbTransactionHandler );

    if ( block && ! m_dbTransactionHandler.waitForQueued( m_pendingUpdateWaitTimeoutSecs * 1000 ) )
    {
        paulst::exception( "Pending updates failed to run within the timeout limit of %d secs", m_pendingUpdateWaitTimeoutSecs );
    }
}

std::string AnalysisActivitySnapshotImpl::getTestName( int testID ) const
{
    TestNames::const_iterator i = m_appContext->testNames->find( testID );
    return i == m_appContext->testNames->end() ? std::string("Unknown test") : i->second;
}

LocalEntryIterator AnalysisActivitySnapshotImpl::localBegin() const
{
    return m_localEntries.begin();
}

LocalEntryIterator AnalysisActivitySnapshotImpl::localEnd()   const
{
    return m_localEntries.end();
}

QueuedSampleIterator AnalysisActivitySnapshotImpl::queueBegin() const
{
    return m_queuedSamples.begin();
}

QueuedSampleIterator AnalysisActivitySnapshotImpl::queueEnd()   const
{
    return m_queuedSamples.end();
}

Range<WorklistEntryIterator> AnalysisActivitySnapshotImpl::getWorklistEntries( const std::string& sampleDescriptor ) const
{
    return m_worklistEntries->equal_range( sampleDescriptor );
}

HANDLE AnalysisActivitySnapshotImpl::queueForRerun( int worklistID, const std::string& sampleRunID, const std::string& sampleDescriptor )
{
    SnapshotUpdateTask* sut = new SnapshotUpdateTaskQRerun( worklistID, sampleRunID, sampleDescriptor, m_appContext->user );
    
    HANDLE h = sut->getDoneSignal();

    m_snapshotUpdateThread.add( sut );

    return h;
}

void AnalysisActivitySnapshotImpl::setObserver( SnapshotObserver* so )
{
    m_snapshotUpdateThread.setSnapshotObserver( so );
}

WorklistRelative AnalysisActivitySnapshotImpl::viewRelatively( const WorklistEntry* e ) const
{
    return m_worklistRelativeImpl.wrap( e );
}

bool AnalysisActivitySnapshotImpl::waitForActionsPending( long millis )
{
    const bool noDBTransactionsInProgress = m_dbTransactionHandler.waitForQueued( millis );
    const bool noUpdatesPending           = m_snapshotUpdateThread.waitTillQuiet( millis );

    return noDBTransactionsInProgress && noUpdatesPending;
}

}

