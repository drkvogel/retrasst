#include "AnalysisActivitySnapshotImpl.h"
#include "ApplicationContext.h"
#include <boost/foreach.hpp>
#include "BuddyDatabase.h"
#include "DBTransactionHandler.h"
#include "DBUpdateSchedule.h"
#include "ExceptionUtil.h"
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
    int                             pendingUpdateWaitTimeoutSecs )
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
    m_dbTransactionHandler          ( appContext->databaseUpdateThread ),
    m_pendingUpdateWaitTimeoutSecs  ( pendingUpdateWaitTimeoutSecs ),
    m_updateHandle                  ( this ),
    m_snapshotUpdateThread  ( appContext->databaseUpdateThread, m_updateHandle, appContext->log, appContext->taskExceptionUserAdvisor ),
    m_worklistRelativeImpl          ( wl )
{
    BOOST_FOREACH( const SampleRun& sr, *m_buddyDatabase )
    {
        LocalRun lr( sr.getSampleDescriptor(), sr.getID() );
        m_localRunImpl.introduce( lr, sr.isOpen() );
        m_localEntries.push_back( lr );
    }

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
    m_dbUpdateSchedule->queueScheduledUpdates( m_dbTransactionHandler );

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
    const bool noDBTransactionsInProgress = m_dbTransactionHandler->waitForQueued( millis );
    const bool noUpdatesPending           = m_snapshotUpdateThread. waitTillQuiet( millis );

    return noDBTransactionsInProgress && noUpdatesPending;
}

}

