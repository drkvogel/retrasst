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

namespace valc
{

AnalysisActivitySnapshotImpl::AnalysisActivitySnapshotImpl( 
    const BuddyDatabase* bdb, 
    const ResultDirectory* rd, const WorklistEntries* wd, DBUpdateSchedule* dbUpdateSchedule,
    SampleRunIDResolutionService* sampleRunIDResolutionService,
    ApplicationContext* appContext,
    int pendingUpdateWaitTimeoutSecs )
    : 
    m_buddyDatabase     ( bdb ),
    m_log               ( appContext->log),
    m_resultDirectory   ( rd ),
    m_worklistEntries ( wd ),
    m_dbUpdateSchedule  ( dbUpdateSchedule ),
    m_sampleRunIDResolutionService( sampleRunIDResolutionService ),
    m_appContext( appContext ),
    m_resultAttributes( appContext->resultAttributes ),
    m_dbTransactionHandler( appContext->databaseUpdateThread ),
    m_pendingUpdateWaitTimeoutSecs( pendingUpdateWaitTimeoutSecs ),
    m_updateHandle( this ),
    m_snapshotUpdateThread( appContext->databaseUpdateThread, m_updateHandle, appContext->log, appContext->taskExceptionUserAdvisor )
{
    BOOST_FOREACH( const SampleRun& sr, *m_buddyDatabase )
    {
        m_localEntries.push_back( LocalRun( sr.getSampleDescriptor(), sr.getID() ) );
    }

    QueuedSamplesBuilderFunction buildQueue( new QueueBuilderParams( bdb, wd, m_appContext->clusterIDs ) );
    buildQueue( &m_queuedSamples ); 
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

HANDLE AnalysisActivitySnapshotImpl::queueForRerun( int worklistID, const std::string& sampleRunID )
{
    SnapshotUpdateTask* sut = new SnapshotUpdateTaskQRerun( worklistID, sampleRunID );
    
    HANDLE h = sut->getDoneSignal();

    m_snapshotUpdateThread.add( sut );

    return h;
}

void AnalysisActivitySnapshotImpl::setObserver( SnapshotObserver* so )
{
    m_snapshotUpdateThread.setSnapshotObserver( so );
}

bool AnalysisActivitySnapshotImpl::waitForActionsPending( long millis )
{
    const bool noDBTransactionsInProgress = m_dbTransactionHandler->waitForQueued( millis );
    const bool noUpdatesPending           = m_snapshotUpdateThread. waitTillQuiet( millis );

    return noDBTransactionsInProgress && noUpdatesPending;
}

}

