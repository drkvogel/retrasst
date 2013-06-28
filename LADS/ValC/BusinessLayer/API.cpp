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
#include "LoadWorklistEntries.h"
#include "LoggingService.h"
#include <memory>
#include "Projects.h"
#include "Require.h"
#include "ResultIndex.h"
#include "Task.h"
#include "Trace.h"
#include <vector>
#include "WorklistEntries.h"


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

AnalysisActivitySnapshot* SnapshotFactory::load( int localMachineID, int user, DBConnection* con )
{
    ResultIndex*     resultIndex     = new ResultIndex();
    WorklistEntries* worklistEntries = new WorklistEntries();
    ClusterIDs*      clusterIDs      = new ClusterIDs();
    Projects*        projects        = new Projects();

    std::auto_ptr<paulst::LoggingService> log( new paulst::LoggingService( new paulst::ConsoleWriter() ) );

    Task<LoadProjects,   int> loadProjectsTask  ( new LoadProjects  ( projects,   log.get(), con ) );
    Task<LoadClusterIDs, int> loadClusterIDsTask( new LoadClusterIDs( clusterIDs, log.get(), con, localMachineID ) );

    loadProjectsTask.start();
    loadClusterIDsTask.start();
    loadProjectsTask.waitFor();

    // Task for loading LOCAL analysis activity and LOCAL results
    Task<LoadBuddyDatabase, BuddyDatabase*> loadBuddyDatabaseTask( 
        new LoadBuddyDatabase( localMachineID, con, log.get(), resultIndex, projects ) );

    loadClusterIDsTask.waitFor();

    // Task for loading LOCAL and CLUSTER worklist entries
    Task<LoadWorklistEntries, int> loadWorklistEntriesTask( 
        new LoadWorklistEntries( clusterIDs, worklistEntries, localMachineID, con, log.get(), resultIndex ) );

    loadBuddyDatabaseTask  .start();
    loadWorklistEntriesTask.start();

    BuddyDatabase* buddyDatabase = loadBuddyDatabaseTask.waitFor();
    loadWorklistEntriesTask.waitFor();

    // Task for loading hither-to unloaded worklist entries that are related to already-loaded worklist entries.
    // In other words, where there exists a row in worklist_relation for which only one of the referenced worklist entries has been loaded, 
    // load the other. 
    Task<LoadReferencedWorklistEntries, int> loadReferencedWorklistEntries( 
        new LoadReferencedWorklistEntries( clusterIDs, localMachineID, con, log.get(), worklistEntries, resultIndex ) );

    // Task for allocating LOCAL results to worklist entries
    Task<AllocateLocalResultsToWorklistEntries, int> allocateLocalResultsToWorklistEntries(
        new AllocateLocalResultsToWorklistEntries( localMachineID, clusterIDs, log.get(), worklistEntries, resultIndex ) );

    loadReferencedWorklistEntries.start();
    allocateLocalResultsToWorklistEntries.start();
    loadReferencedWorklistEntries.waitFor();
    allocateLocalResultsToWorklistEntries.waitFor();

    return new AnalysisActivitySnapshotImpl( clusterIDs, projects, buddyDatabase, log.release(), resultIndex, worklistEntries );
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

}

