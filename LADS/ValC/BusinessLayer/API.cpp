#include <algorithm>
#include "AnalysisActivitySnapshotImpl.h"
#include "API.h"
#include <boost/lexical_cast.hpp>
#include "BuddyDatabase.h"
#include "ClusterIDs.h"
#include "ConsoleWriter.h"
#include "DBConnectionADO.h"
#include "AllocateLocalResultsToWorklistEntries.h"
#include <iterator>
#include "LoadBuddyDatabase.h"
#include "LoadClusterIDs.h"
#include "LoadProjects.h"
#include "LoadReferencedWorklistEntries.h"
#include "LoadTestNames.h"
#include "LoadWorklistEntries.h"
#include "LoggingService.h"
#include <memory>
#include "Projects.h"
#include "Require.h"
#include "ResultIndex.h"
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


Properties::Properties()
{
}

void Properties::setProperty( const std::string& name, const std::string& value )
{
    m_propertyValues.insert( std::make_pair(name, value) );
}

std::string Properties::getProperty( const std::string& name ) const
{
    Map::const_iterator i = m_propertyValues.find( name );
    return i == m_propertyValues.end() ? std::string() : i->second;
}

DBConnection* DBConnectionFactory::createConnection( const Properties& p )
{
    return new DBConnectionADO( p );
}

void wait( HANDLE* array, int howMany, ThreadExceptionMsgs* exceptionMsgs )
{
	DWORD waitResult = WaitForMultipleObjectsEx( howMany, array, true, INFINITE, false );

	require(
		( waitResult >= WAIT_OBJECT_0 			) &&
		( waitResult < WAIT_OBJECT_0 + howMany 	) );

	exceptionMsgs->throwFirst();
}

AnalysisActivitySnapshot* SnapshotFactory::load( int localMachineID, int user, DBConnection* con )
{
	ResultIndex*     resultIndex     = new ResultIndex();
	WorklistEntries* worklistEntries = new WorklistEntries();
	ClusterIDs*      clusterIDs      = new ClusterIDs();
	Projects*        projects        = new Projects();
	TestNames*       testNames       = new TestNames();
	BuddyDatabase*   buddyDatabase   = NULL;
	ThreadExceptionMsgs threadExceptionMsgs;
	HANDLE hArray[3];
	std::auto_ptr<paulst::LoggingService> log( new paulst::LoggingService( new paulst::ConsoleWriter() ) );

	ThreadTask<LoadProjects>   loadProjectsTask  ( new LoadProjects  ( projects,   log.get(), con ), 					&threadExceptionMsgs );
	ThreadTask<LoadClusterIDs> loadClusterIDsTask( new LoadClusterIDs( clusterIDs, log.get(), con, localMachineID ), 	&threadExceptionMsgs );
	ThreadTask<LoadTestNames>  loadTestNamesTask ( new LoadTestNames ( testNames,  log.get(), con ), 					&threadExceptionMsgs );

	hArray[0] = loadProjectsTask  .start();
	hArray[1] = loadClusterIDsTask.start();
	hArray[2] = loadTestNamesTask .start();

	wait( hArray, 3, &threadExceptionMsgs );

	// Task for loading LOCAL analysis activity and LOCAL results
	ThreadTask<LoadBuddyDatabase> loadBuddyDatabaseTask(
		new LoadBuddyDatabase( localMachineID, con, log.get(), resultIndex, projects, &buddyDatabase ),
		&threadExceptionMsgs );

    // Task for loading LOCAL and CLUSTER worklist entries
	ThreadTask<LoadWorklistEntries> loadWorklistEntriesTask(
		new LoadWorklistEntries( clusterIDs, worklistEntries, localMachineID, con, log.get(), resultIndex ),
		&threadExceptionMsgs );

	hArray[0] = loadBuddyDatabaseTask  .start();
	hArray[1] = loadWorklistEntriesTask.start();

	wait( hArray, 2, &threadExceptionMsgs );

    // Task for loading hither-to unloaded worklist entries that are related to already-loaded worklist entries.
    // In other words, where there exists a row in worklist_relation for which only one of the referenced worklist entries has been loaded, 
    // load the other. 
	ThreadTask<LoadReferencedWorklistEntries> loadReferencedWorklistEntries(
		new LoadReferencedWorklistEntries( clusterIDs, localMachineID, con, log.get(), worklistEntries, resultIndex ),
		&threadExceptionMsgs );

    // Task for allocating LOCAL results to worklist entries
	ThreadTask<AllocateLocalResultsToWorklistEntries> allocateLocalResultsToWorklistEntries(
		new AllocateLocalResultsToWorklistEntries( localMachineID, clusterIDs, log.get(), worklistEntries, resultIndex ),
		&threadExceptionMsgs );

	hArray[0] = loadReferencedWorklistEntries        .start();
	hArray[1] = allocateLocalResultsToWorklistEntries.start();

	wait( hArray, 2, &threadExceptionMsgs );

    return new AnalysisActivitySnapshotImpl( clusterIDs, projects, buddyDatabase, log.release(), resultIndex, worklistEntries, testNames );
}

Cursor::Cursor()
{
}

Cursor::~Cursor()
{
}

DBConnection::DBConnection()
{
}

DBConnection::~DBConnection()
{
}

AnalysisActivitySnapshot::AnalysisActivitySnapshot()
{
}

AnalysisActivitySnapshot::~AnalysisActivitySnapshot()
{
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

}

