#include "AnalysisActivitySnapshotImpl.h"
#include "ApplicationContext.h"
#include <boost/foreach.hpp>
#include "BuddyDatabase.h"
#include "DBUpdateConsumer.h"
#include "DBUpdateSchedule.h"
#include "Projects.h"
#include "QueueBuilderParams.h"
#include "QueuedSamplesBuilderFunction.h"
#include "ResultDirectory.h"
#include "SampleRunIDResolutionService.h"
#include "WorklistDirectory.h"

namespace valc
{

AnalysisActivitySnapshotImpl::AnalysisActivitySnapshotImpl( 
    const ClusterIDs* clusterIDs, const Projects* p, const BuddyDatabase* bdb, 
    const ResultDirectory* rd, const WorklistDirectory* wd, const TestNames* tns, DBUpdateSchedule* dbUpdateSchedule,
    SampleRunIDResolutionService* sampleRunIDResolutionService,
    ApplicationContext* appContext )
    : 
    m_buddyDatabase     ( bdb ),
    m_clusterIDs        ( clusterIDs ),
    m_log               ( appContext->log),
    m_projects          ( p ),
    m_resultDirectory   ( rd ),
    m_worklistDirectory ( wd ),
    m_testNames         ( tns ),
    m_dbUpdateSchedule  ( dbUpdateSchedule ),
    m_sampleRunIDResolutionService( sampleRunIDResolutionService ),
    m_appContext( appContext )
{
    BOOST_FOREACH( const SampleRun& sr, *m_buddyDatabase )
    {
        m_localEntries.push_back( LocalRun( sr.getSampleDescriptor(), sr.getID() ) );
    }

    QueuedSamplesBuilderFunction buildQueue( new QueueBuilderParams( bdb, wd, clusterIDs ) );
    buildQueue( &m_queuedSamples ); 
}

bool AnalysisActivitySnapshotImpl::compareSampleRunIDs( const std::string& oneRunID, const std::string& anotherRunID )    const
{
    return m_sampleRunIDResolutionService->compareSampleRunIDs( oneRunID, anotherRunID );
}


BuddyDatabaseEntries AnalysisActivitySnapshotImpl::listBuddyDatabaseEntriesFor( const std::string& sampleRunID )   const
{
    return m_buddyDatabase->listBuddyDatabaseEntriesFor( sampleRunID );
}

void AnalysisActivitySnapshotImpl::runPendingDatabaseUpdates( DBUpdateExceptionHandlingPolicy* exceptionCallback, 
    bool block )
{
    m_dbUpdateConsumer.reset( 
        new DBUpdateConsumer( m_appContext, m_sampleRunIDResolutionService.get(), m_dbUpdateSchedule.get(), exceptionCallback ) );

    if ( block )
    {
        m_dbUpdateConsumer->waitFor();
    }
}

std::string AnalysisActivitySnapshotImpl::getTestName( int testID ) const
{
    TestNames::const_iterator i = m_testNames->find( testID );
    return i == m_testNames->end() ? std::string("Unknown test") : i->second;
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
    return m_worklistDirectory->equal_range( sampleDescriptor );
}

}

