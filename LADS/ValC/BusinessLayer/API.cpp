#include <algorithm>
#include "AllocateLocalResultsToWorklistEntries.h"
#include "AnalysisActivitySnapshotImpl.h"
#include "API.h"
#include <boost/lexical_cast.hpp>
#include "BuddyDatabase.h"
#include "ClusterIDs.h"
#include "Config.h"
#include "DBConnectionADO.h"
#include "DBUpdateSchedule.h"
#include "ExceptionalDataHandlerImpl.h"
#include <iterator>
#include "LoadBuddyDatabase.h"
#include "LoadClusterIDs.h"
#include "LoadNonLocalResults.h"
#include "LoadProjects.h"
#include "LoadReferencedWorklistEntries.h"
#include "LoadTestNames.h"
#include "LoadWorklistEntries.h"
#include "LoggingService.h"
#include <memory>
#include "Projects.h"
#include "Require.h"
#include "ResultIndex.h"
#include "SampleRunIDResolutionService.h"
#include "Task.h"
#include "TestNames.h"
#include "TestResultIteratorImpl.h"
#include "ThreadExceptionMsgs.h"
#include "Trace.h"
#include <vector>
#include "WorklistEntries.h"
#include "WorklistEntryIteratorImpl.h"


#define LOGPATH "C:\\temp\\valc.log"

namespace valc
{

paulstdb::DBConnection* DBConnectionFactory::createConnection( const std::string& connectionString,
        const std::string& sessionReadLockSetting )
{
    return new paulstdb::DBConnectionADO( connectionString, sessionReadLockSetting );
}

void wait( HANDLE* array, int howMany, ThreadExceptionMsgs* exceptionMsgs )
{
	DWORD waitResult = WaitForMultipleObjectsEx( howMany, array, true, INFINITE, false );

	require(
		( waitResult >= WAIT_OBJECT_0 			) &&
		( waitResult < WAIT_OBJECT_0 + howMany 	) );

	exceptionMsgs->throwFirst();
}

AnalysisActivitySnapshot* SnapshotFactory::load( int localMachineID, int user, paulstdb::DBConnection* con, paulst::LoggingService* log, 
    const std::string& configString, UserAdvisor* userAdvisor )
{
	ResultIndex*        resultIndex        = new ResultIndex();
	WorklistEntries*    worklistEntries    = new WorklistEntries();
	ClusterIDs*         clusterIDs         = new ClusterIDs();
	Projects*           projects           = new Projects();
	TestNames*          testNames          = new TestNames();
	BuddyDatabase*      buddyDatabase      = NULL;
    DBUpdateSchedule*   dbUpdateSchedule   = new DBUpdateSchedule();
    std::auto_ptr<SampleRunIDResolutionService> sampleRunIDResolutionService(new SampleRunIDResolutionService());
    paulst::Config      config(configString);
	ThreadExceptionMsgs threadExceptionMsgs;
    ExceptionalDataHandlerImpl exceptionalDataHandler(config.get("ExceptionalDataHandler"), userAdvisor, log);
	HANDLE hArray[3];

	ThreadTask<LoadProjects>   loadProjectsTask  ( new LoadProjects  ( projects,   log, con ), 					&threadExceptionMsgs );
	ThreadTask<LoadClusterIDs> loadClusterIDsTask( new LoadClusterIDs( clusterIDs, log, con, localMachineID ), 	&threadExceptionMsgs );
	ThreadTask<LoadTestNames>  loadTestNamesTask ( new LoadTestNames ( testNames,  log, con ), 					&threadExceptionMsgs );

	hArray[0] = loadProjectsTask  .start();
	hArray[1] = loadClusterIDsTask.start();
	hArray[2] = loadTestNamesTask .start();

	wait( hArray, 3, &threadExceptionMsgs );

	// Task for loading LOCAL analysis activity and LOCAL results
	ThreadTask<LoadBuddyDatabase> loadBuddyDatabaseTask(
		new LoadBuddyDatabase( localMachineID, con, log, resultIndex, projects, &buddyDatabase, dbUpdateSchedule, 
            sampleRunIDResolutionService.get(), config.get("LoadBuddyDatabase"), config.get("BuddyDatabaseInclusionRule"),
            &exceptionalDataHandler ),
		&threadExceptionMsgs );

    // Task for loading LOCAL and CLUSTER worklist entries
	ThreadTask<LoadWorklistEntries> loadWorklistEntriesTask(
		new LoadWorklistEntries( worklistEntries, con, log, resultIndex, config.get( "LoadWorklistEntries" ), 
            config.get( "LoadWorklistRelations" ), config.get("WorklistInclusionRule"), &exceptionalDataHandler ),
		&threadExceptionMsgs );

	hArray[0] = loadBuddyDatabaseTask  .start();
	hArray[1] = loadWorklistEntriesTask.start();

	wait( hArray, 2, &threadExceptionMsgs );

    // Task for allocating LOCAL results to worklist entries
	ThreadTask<AllocateLocalResultsToWorklistEntries> allocateLocalResultsToWorklistEntries(
		new AllocateLocalResultsToWorklistEntries( localMachineID, clusterIDs, log, worklistEntries, resultIndex, dbUpdateSchedule,
            &exceptionalDataHandler ),
		&threadExceptionMsgs );

	hArray[0] = allocateLocalResultsToWorklistEntries.start();

    wait( hArray, 1, &threadExceptionMsgs );

    // Task for loading hither-to unloaded worklist entries that are related to already-loaded worklist entries.
    // In other words, where there exists a row in worklist_relation for which only one of the referenced worklist entries has been loaded, 
    // load the other. 
	ThreadTask<LoadReferencedWorklistEntries> loadReferencedWorklistEntries(
		new LoadReferencedWorklistEntries(      con, 
                                                log, 
                                                worklistEntries, 
                                                resultIndex,
                                                config.get("RefTempTableName"),
                                                config.get("LoadReferencedWorklistEntries"),
                                                config.get("LoadReferencedWorklistRelations"),
                                                &exceptionalDataHandler ),
		&threadExceptionMsgs );

    // Task for loading non-local results for worklist entries loaded by LoadWorklistEntries (above).
    ThreadTask<LoadNonLocalResults> loadNonLocalResults( 
        new LoadNonLocalResults( projects, con, log, resultIndex, config.get("LoadNonLocalResults"), &exceptionalDataHandler ),
        &threadExceptionMsgs );

	hArray[0] = loadReferencedWorklistEntries        .start();
	hArray[1] = loadNonLocalResults                  .start();

	wait( hArray, 2, &threadExceptionMsgs );

    resultIndex->removeReferencesToResultsNotLoaded();

    return new AnalysisActivitySnapshotImpl( clusterIDs, projects, buddyDatabase, log, resultIndex, worklistEntries, testNames,
        dbUpdateSchedule, sampleRunIDResolutionService.release() );
}

DBUpdateExceptionHandlingPolicy::DBUpdateExceptionHandlingPolicy()
{
}

DBUpdateExceptionHandlingPolicy::~DBUpdateExceptionHandlingPolicy()
{
}

AnalysisActivitySnapshot::AnalysisActivitySnapshot()
{
}

AnalysisActivitySnapshot::~AnalysisActivitySnapshot()
{
}

BuddyDatabaseEntry::BuddyDatabaseEntry()
    : buddy_sample_id(0), alpha_sample_id(0)
{
}

BuddyDatabaseEntry::BuddyDatabaseEntry( int buddySampleID, int alphaSampleID, const std::string& bcode, const std::string& databaseName,
    const TDateTime& dateAnalysed )
    : buddy_sample_id( buddySampleID), alpha_sample_id( alphaSampleID ), barcode( bcode ), database_name( databaseName ),
    date_analysed( dateAnalysed )
{
}

BuddyDatabaseEntry::BuddyDatabaseEntry( const BuddyDatabaseEntry& other )
    :
    buddy_sample_id ( other.buddy_sample_id ),
    alpha_sample_id ( other.alpha_sample_id ),
    barcode         ( other.barcode ),
    database_name   ( other.database_name ),
    date_analysed   ( other.date_analysed )
{
}

BuddyDatabaseEntry& BuddyDatabaseEntry::operator=( const BuddyDatabaseEntry& other )
{
    buddy_sample_id = other.buddy_sample_id;
    alpha_sample_id = other.alpha_sample_id;
    barcode         = other.barcode;
    database_name   = other.database_name;
    date_analysed   = other.date_analysed;
    return *this;
}

LocalRun::LocalRun()
{
}

LocalRun::LocalRun( const LocalRun& o )
    :
    m_id                ( o.m_id ),
    m_sampleDescriptor  ( o.m_sampleDescriptor )
{
}

LocalRun::LocalRun( const std::string& sampleDescriptor, const std::string& id )
    :
    m_id                ( id ),
    m_sampleDescriptor  ( sampleDescriptor )
{
}

LocalRun& LocalRun::operator=( const LocalRun& o )
{
    m_id                = o.m_id;
    m_sampleDescriptor  = o.m_sampleDescriptor;
    return *this;
}

std::string LocalRun::getRunID() const
{
    return m_id;
}

std::string LocalRun::getSampleDescriptor() const
{
    return m_sampleDescriptor;
}

QueuedSample::QueuedSample()
{
}

QueuedSample::QueuedSample( const QueuedSample& o )
    :
    m_sampleDescriptor  ( o.m_sampleDescriptor )
{
}

QueuedSample::QueuedSample( const std::string& sampleDescriptor )
    :
    m_sampleDescriptor  ( sampleDescriptor )
{
}

QueuedSample& QueuedSample::operator=( const QueuedSample& o )
{
    m_sampleDescriptor  = o.m_sampleDescriptor;
    return *this;
}

std::string QueuedSample::getSampleDescriptor() const
{
    return m_sampleDescriptor;
}

WorklistEntry::WorklistEntry()
{
}

WorklistEntry::~WorklistEntry()
{
}

TestResult::TestResult()
{
}

TestResult::~TestResult()
{
}


WorklistEntryIterator::WorklistEntryIterator()
    : m_impl( new WorklistEntryIteratorImpl() )
{
}

WorklistEntryIterator::WorklistEntryIterator( WorklistEntryIteratorImpl* i )
    : m_impl( i )
{
}

WorklistEntryIterator::WorklistEntryIterator( const WorklistEntryIterator& other )
    : m_impl( new WorklistEntryIteratorImpl( *(other.m_impl) ) )
{
}

WorklistEntryIterator::~WorklistEntryIterator()
{
    delete m_impl;
}

void WorklistEntryIterator::increment()
{
    m_impl->increment();
}

bool WorklistEntryIterator::equal( WorklistEntryIterator const& other ) const
{
    return (*m_impl) == *(other.m_impl);
}

const WorklistEntry*&  WorklistEntryIterator::dereference() const
{
    return m_impl->dereference();
}

TestResultIterator::TestResultIterator()
    : m_impl( new TestResultIteratorImpl() )
{
}

TestResultIterator::TestResultIterator( TestResultIteratorImpl* i )
    : m_impl( i )
{
}

TestResultIterator::TestResultIterator( const TestResultIterator& other )
    : m_impl( new TestResultIteratorImpl( *(other.m_impl) ) )
{
}

TestResultIterator::~TestResultIterator()
{
    delete m_impl;
}

void TestResultIterator::increment()
{
    m_impl->increment();
}

bool TestResultIterator::equal( TestResultIterator const& other ) const
{
    return (*m_impl) == *(other.m_impl);
}

const TestResult*&  TestResultIterator::dereference() const
{
    return m_impl->dereference();
}

UserAdvisor::UserAdvisor()
{
}

UserAdvisor::~UserAdvisor()
{
}

}

