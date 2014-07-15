#include "AbstractConnectionFactory.h"
#include "AnalysisActivitySnapshotImpl.h"
#include "ApplicationContext.h"
#include <boost/foreach.hpp>
#include "BuddyDatabase.h"
#include "ControlModel.h"
#include "DBTransactionHandler.h"
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
    m_dbTransactionHandler          ( appContext->dbTransactionHandler ),
    m_pendingUpdateWaitTimeoutSecs  ( pendingUpdateWaitTimeoutSecs ),
    m_snapshotUpdateThread          ( m_dbTransactionHandler, m_updateHandle, appContext->log, appContext->taskExceptionUserAdvisor ),
    m_worklistRelativeImpl          ( wl ),
    m_sampleRunGroupModel           ( sampleRunGroupIDGenerator ),
    m_controlModel                  ( controlModel )
{
    m_dbTransactionHandler->setSnapshotUpdateHandle( SnapshotUpdateHandle(this) );

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

        LocalRun lr( sr.getSampleDescriptor(), sr.getBarcode(), sr.getID() );
        m_localRunImpl.introduce( lr, sr.isOpen() );
        m_localEntries.push_back( lr );
    }

    controlModel->setSampleRunGroupModel( &m_sampleRunGroupModel );

    QueuedSamplesBuilderFunction buildQueue( new QueueBuilderParams( bdb, wd, m_appContext->clusterIDs ) );
    buildQueue( &m_queuedSamples ); 
}

AnalysisActivitySnapshotImpl::~AnalysisActivitySnapshotImpl()
{
    m_dbTransactionHandler->setSnapshotUpdateHandle( SnapshotUpdateHandle(NULL) );
    delete m_buddyDatabase;
    delete m_resultDirectory;
    delete m_worklistEntries;
    delete m_worklistLinks;
    delete m_dbUpdateSchedule;
}

RuleResults AnalysisActivitySnapshotImpl::getRuleResults( int forResultID ) const
{
    return m_resultAttributes->getRuleResults( forResultID );
}

bool AnalysisActivitySnapshotImpl::hasRuleResults( int forResultID ) const
{
    return m_resultAttributes->hasRuleResults( forResultID );
}

void AnalysisActivitySnapshotImpl::runPendingDatabaseUpdates( bool block )
{
    m_dbUpdateSchedule->runQueuedUpdates( m_dbTransactionHandler );

    if ( block && ! m_dbTransactionHandler->waitForQueued( m_pendingUpdateWaitTimeoutSecs * 1000 ) )
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

HANDLE AnalysisActivitySnapshotImpl::queueForRerun( int worklistID, const IDToken& sampleRunID, const std::string& sampleDescriptor,
    const std::string& barcode )
{
    SnapshotUpdateTask* sut = new SnapshotUpdateTaskQRerun( 
        worklistID, sampleRunID, sampleDescriptor, barcode, m_appContext->user, m_dbTransactionHandler );
    
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
    const bool noDBTransactionsInProgress = m_dbTransactionHandler->waitForQueued( millis );
    const bool noUpdatesPending           = m_snapshotUpdateThread.waitTillQuiet( millis );

    return noDBTransactionsInProgress && noUpdatesPending;
}

}

