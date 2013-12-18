#include <algorithm>
#include "AllocateLocalResultsToWorklistEntries.h"
#include "AnalysisActivitySnapshotImpl.h"
#include "API.h"
#include "ApplicationContext.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include "BuddyDatabase.h"
#include "BuddyDatabaseEntryIndex.h"
#include "ClusterIDs.h"
#include "Config.h"
#include "ConnectionFactoryWithLogging.h"
#include "Cursor.h"
#include "DBConnectionFactory.h"
#include "DBQueryTask.h"
#include "DBTransactionHandler.h"
#include "DBUpdateSchedule.h"
#include "ExceptionalDataHandlerImpl.h"
#include "ExceptionUtil.h"
#include <iterator>
#include "LoadBuddyDatabase.h"
#include "LoadNonLocalResults.h"
#include "LoadReferencedWorklistEntries.h"
#include "LoadWorklistEntries.h"
#include "LoggingService.h"
#include <memory>
#include "MockConnectionFactory.h"
#include "Projects.h"
#include "QCGates.h"
#include "QCSampleDescriptorDerivationStrategyImpl.h"
#include "Require.h"
#include "ResultAttributes.h"
#include "ResultIndex.h"
#include "RuleEngineContainer.h"
#include "SampleRunIDResolutionService.h"
#include "StrUtil.h"
#include "TaskExceptionUserAdvisor.h"
#include "TestNames.h"
#include "TestResultIteratorImpl.h"
#include "ThreadPool.h"
#include "Trace.h"
#include <vector>
#include "WorklistEntries.h"
#include "WorklistEntryIteratorImpl.h"


namespace valc
{

void addProject( paulstdb::Cursor* c, Projects* p )
{
    const int           projectID       = paulstdb::read<int>           ( *c, 0 );
    const std::string   externalName    = paulstdb::read<std::string>   ( *c, 1 );
    const std::string   dbName          = paulstdb::read<std::string>   ( *c, 2 );

    p->add( projectID, externalName, dbName );
}
 
void addCluster( paulstdb::Cursor* c, ClusterIDs* clusterIDs )
{
    const int clusterID = paulstdb::read<int>( *c, 0 );

    clusterIDs->insert( clusterID );
}

void addTestName( paulstdb::Cursor* c, TestNames* tn )
{
    const int           testID       = paulstdb::read<int>        ( *c, 0 );
    const std::string   externalName = paulstdb::read<std::string>( *c, 1 );

    tn->insert( std::make_pair( testID, externalName ) );
}

ApplicationContext* applicationContext = NULL;

void InitialiseApplicationContext( int localMachineID, int user, const std::string& config, paulst::LoggingService* log,
    UserAdvisor* userAdvisor )
{
    if ( applicationContext )
    {
        throw Exception( L"Application context already initialised." );
    }
    else
    {
		ApplicationContext* ac = new ApplicationContext();
        ac->config = new paulst::Config( config );
        std::string connectionFactoryType = ac->getProperty("ConnectionFactoryType");
        paulst::trim( connectionFactoryType );
        if ( connectionFactoryType == "ADO" )
        {
            ac->connectionFactory = new paulstdb::DBConnectionFactory();
        }
        else if ( connectionFactoryType == "Mock" )
        {
            ac->connectionFactory = new valc::MockConnectionFactory();
        }
        else
        {
            throw Exception( L"ConnectionFactoryType (in config) is not valid.  Valid values are 'ADO' or 'Mock'." );
        }
        if ( "true" == ac->getProperty("LogAllDatabaseStatements") )
        {
            ac->connectionFactory = new paulstdb::ConnectionFactoryWithLogging( ac->connectionFactory, log );
        }
        ac->localMachineID                  = localMachineID;
        ac->user                            = user;
        ac->userAdvisor                     = userAdvisor;
        ac->taskExceptionUserAdvisor        = new TaskExceptionUserAdvisor( userAdvisor, log );
        ac->log                             = log;
        ac->sampleRunIDResolutionService    = new SampleRunIDResolutionService();
        ac->initialisationQueries           = new stef::ThreadPool(0, 1);
        ac->initialisationQueries->addDefaultTaskExceptionHandler( ac->taskExceptionUserAdvisor );
        ac->databaseUpdateThread            = new DBTransactionHandler( 
                                                    ac->connectionFactory->createConnection( 
                                                        ac->getProperty("DBUpdateThreadConnectionString"),
                                                        ac->getProperty("DBUpdateThreadSessionReadLockSetting") ),
                                                    log,
                                                    ac->sampleRunIDResolutionService,
                                                    paulst::toInt("DBUpdateThreadShutdownTimeoutSecs"),
                                                    std::string("true") == ac->getProperty("DBUpdateThreadCancelPendingUpdatesOnShutdown"),
                                                    ac->taskExceptionUserAdvisor,
                                                    ac->config  );
        ac->resultAttributes                = new ResultAttributes();
        ac->clusterIDs                      = new ClusterIDs();
        ac->projects                        = new Projects();
        ac->testNames                       = new TestNames();
        ac->qcGates                         = new QCGates(  
                                                    ac->initialisationQueries,
                                                    ac->config,
                                                    ac->connectionFactory,
                                                    ac->log,
                                                    ac->localMachineID );
        ac->ruleEngineContainer             = new RuleEngineContainer(  
                                                    ac->initialisationQueries,
                                                    ac->config,
                                                    ac->connectionFactory,
                                                    ac->log,
                                                    ac->resultAttributes,
                                                    ac->qcGates,
                                                    ac->taskExceptionUserAdvisor );
        ac->initialisationQueries->addTask(
            new DBQueryTask( "Projects", ac->connectionFactory, ac->config, boost::bind( addProject, _1, ac->projects ) ) );
        ac->initialisationQueries->addTask(
            new DBQueryTask( "ClusterIDs", ac->connectionFactory, ac->config, boost::bind( addCluster, _1, ac->clusterIDs ) ) );
        ac->initialisationQueries->addTask(
            new DBQueryTask( "TestNames", ac->connectionFactory, ac->config, boost::bind( addTestName, _1, ac->testNames ) ) );
        
		applicationContext = ac;
    }
}

void DeleteApplicationContext()
{
    try
    {
        delete applicationContext;
    }
    catch( ... )
    {
    }
    applicationContext = NULL;
}

void wait( stef::ThreadPool* taskRunner, long timeoutMillis )
{
    if ( ! taskRunner->waitTillQuiet( timeoutMillis ) )
    {
        throw Exception( "Initialisation task timed out." );
    }

    std::string lastError = taskRunner->getLastError();

    if ( ! lastError.empty() )
    {
        throw Exception( lastError.c_str() );
    }
}

SnapshotPtr Load() 
{
    if ( applicationContext == NULL )
    {
        throw Exception( L"Application context has not been initialised." );
    }

    if ( applicationContext->snapshot )
    {
        throw Exception( L"Snapshot already loaded." );
    }

    const int initialisationWaitTimeout = paulst::toInt(applicationContext->getProperty("InitialisationTimeoutSecs")) * 1000;

    if ( ! applicationContext->initialisationQueries->waitTillQuiet( initialisationWaitTimeout ) )
    {
        throw Exception( L"Initialisation timeout. Try increasing value of InitialisationTimeoutSecs in config." );
    }

    std::string lastError = applicationContext->initialisationQueries->getLastError();

    if ( ! lastError.empty() )
    {
        paulst::exception( "Initialisation error. \%s", lastError.c_str() );
    }

    // Don't want rules using cached stats.
    applicationContext->ruleEngineContainer->clearRulesCache();

    applicationContext->sampleRunIDResolutionService->clear();
    
    std::auto_ptr<paulstdb::DBConnection> dbConnection( applicationContext->connectionFactory->createConnection(
        applicationContext->getProperty("ForceReloadConnectionString"),
        applicationContext->getProperty("ForceReloadSessionReadLockSetting") ) );

    paulstdb::DBConnection* con            = dbConnection.get();
    paulst::LoggingService* log            = applicationContext->log;
    int                 localMachineID     = applicationContext->localMachineID;
	ResultIndex*        resultIndex        = new ResultIndex();
	WorklistEntries*    worklistEntries    = new WorklistEntries();
	BuddyDatabase*      buddyDatabase      = NULL;
    DBUpdateSchedule*   dbUpdateSchedule   = new DBUpdateSchedule();
    ExceptionalDataHandlerImpl exceptionalDataHandler(
        applicationContext->getProperty("ExceptionalDataHandler"), applicationContext->userAdvisor, log);
    BuddyDatabaseEntryIndex* buddyDatabaseEntryIndex = new BuddyDatabaseEntryIndex();
    boost::scoped_ptr<QCSampleDescriptorDerivationStrategy> QCSampleDescriptorDerivationStrategy(
        new QCSampleDescriptorDerivationStrategyImpl( resultIndex, buddyDatabaseEntryIndex, applicationContext->log ) );

    // Clear previously obtained RuleResults
    applicationContext->resultAttributes->clearRuleResults();

    stef::ThreadPool* taskRunner = new stef::ThreadPool(1,5);
    taskRunner->addDefaultTaskExceptionHandler( applicationContext->taskExceptionUserAdvisor );

	// Task for loading LOCAL analysis activity and LOCAL results
    taskRunner->addTask( new LoadBuddyDatabase( 
        localMachineID, 
        con, 
        log, 
        resultIndex, 
        applicationContext->projects, 
        &buddyDatabase, 
        dbUpdateSchedule, 
        applicationContext->sampleRunIDResolutionService, applicationContext->getProperty("LoadBuddyDatabase"), 
        applicationContext->getProperty("BuddyDatabaseInclusionRule"),
        &exceptionalDataHandler,
        applicationContext->ruleEngineContainer,
        QCSampleDescriptorDerivationStrategy.get(),
        buddyDatabaseEntryIndex ) );

	wait( taskRunner, initialisationWaitTimeout );

    // Task for loading LOCAL and CLUSTER worklist entries
    taskRunner->addTask( new LoadWorklistEntries( 
        worklistEntries, 
        con, 
        log, 
        resultIndex, 
        applicationContext->getProperty( "LoadWorklistEntries" ), 
        applicationContext->getProperty("WorklistInclusionRule"), 
        &exceptionalDataHandler,
        QCSampleDescriptorDerivationStrategy.get() ) );

	wait( taskRunner, initialisationWaitTimeout );

    // Task for allocating LOCAL results to worklist entries
    taskRunner->addTask( new AllocateLocalResultsToWorklistEntries( 
                localMachineID, applicationContext->clusterIDs, log, worklistEntries, resultIndex, dbUpdateSchedule, &exceptionalDataHandler ) );

    wait( taskRunner, initialisationWaitTimeout );

    // Task for loading hither-to unloaded worklist entries that are related to already-loaded worklist entries.
    // In other words, where there exists a row in worklist_relation for which only one of the referenced worklist entries has been loaded, 
    // load the other. 
    taskRunner->addTask( new LoadReferencedWorklistEntries(      
        con, 
        log, 
        worklistEntries, 
        resultIndex,
        applicationContext->getProperty("RefTempTableName"),
        applicationContext->getProperty("LoadReferencedWorklistEntries"),
        &exceptionalDataHandler ) );

    // Task for loading non-local results for worklist entries loaded by LoadWorklistEntries (above).
    taskRunner->addTask( new LoadNonLocalResults( 
        applicationContext->projects, 
        con, 
        log, 
        resultIndex, 
        applicationContext->getProperty("LoadNonLocalResults"), 
        &exceptionalDataHandler ) );

	wait( taskRunner, initialisationWaitTimeout );

    taskRunner->shutdown( initialisationWaitTimeout, false );

    resultIndex->removeReferencesToResultsNotLoaded();

    const int ruleEngineWaitTimeoutSecs = paulst::toInt(applicationContext->getProperty("RuleEngineTimeoutSecs"));

    if ( ! applicationContext->ruleEngineContainer->waitForQueued( ruleEngineWaitTimeoutSecs * 1000 ) )
    {
        paulst::exception( "RuleEngine wait timeout after %d secs", ruleEngineWaitTimeoutSecs );
    }

    applicationContext->snapshot = new AnalysisActivitySnapshotImpl( 
        buddyDatabase, 
        resultIndex, 
        worklistEntries, 
        dbUpdateSchedule, 
        applicationContext->sampleRunIDResolutionService, 
        applicationContext,
        paulst::toInt( applicationContext->getProperty("PendingUpdateWaitTimeoutSecs") ) );

    return SnapshotPtr( applicationContext->snapshot );
}

void Unload( SnapshotPtr s )
{
    if ( applicationContext->snapshot )
    {
        delete applicationContext->snapshot;
        applicationContext->snapshot = NULL;
    }
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


RuleDescriptor::RuleDescriptor( int recordID, int ruleID, const std::string& uniqueName, const std::string& desc )
    :
    m_recordID  ( recordID  ),
    m_ruleID    ( ruleID    ),
    m_uniqueName( uniqueName),
    m_desc      ( desc      )
{
}

RuleDescriptor::RuleDescriptor()
    :
    m_recordID  ( 0 ),
    m_ruleID    ( 0 )
{
}

RuleDescriptor::RuleDescriptor( const RuleDescriptor& rd )
    :
    m_recordID  ( rd.m_recordID  ),
    m_ruleID    ( rd.m_ruleID    ),
    m_uniqueName( rd.m_uniqueName),
    m_desc      ( rd.m_desc      )
{
}

RuleDescriptor& RuleDescriptor::operator=( const RuleDescriptor& rd )
{
    m_recordID   = rd.m_recordID;
    m_ruleID     = rd.m_ruleID;
    m_uniqueName = rd.m_uniqueName;
    m_desc       = rd.m_desc;
    return *this;
}

int RuleDescriptor::getRecordID() const
{
    return m_recordID;
}

int RuleDescriptor::getRuleID() const
{
    return m_ruleID;
}

std::string RuleDescriptor::getUniqueName() const
{
    return m_uniqueName;
}

std::string RuleDescriptor::getDesc() const
{
    return m_desc;
}


RuleResults::RuleResults()
    : m_summaryResultCode(0)
{
}

RuleResults::RuleResults( const RuleDescriptor& rd, RuleResults::const_iterator begin, RuleResults::const_iterator end, int summaryResultCode,
    const std::string& summaryMsg, const std::vector<std::string>& extraValues ) 
    :
    m_ruleDescriptor( rd ),
    m_summaryResultCode(summaryResultCode),
    m_summaryMsg(summaryMsg),
    m_results( begin, end ),
    m_extraValues( extraValues )
{
}

RuleResults::RuleResults( const RuleResults& other )
    :
    m_ruleDescriptor    ( other.m_ruleDescriptor ),
    m_summaryResultCode ( other.m_summaryResultCode ),
    m_summaryMsg        ( other.m_summaryMsg ),
    m_results           ( other.m_results ),
    m_extraValues       ( other.m_extraValues )
{
}

RuleResults& RuleResults::operator=( const RuleResults& other )
{
    m_ruleDescriptor    = other.m_ruleDescriptor;
    m_summaryResultCode = other.m_summaryResultCode;
    m_summaryMsg        = other.m_summaryMsg;
    m_results           = other.m_results;
    m_extraValues       = other.m_extraValues;
    return *this;
}

RuleDescriptor RuleResults::getRuleDescriptor() const
{
    return m_ruleDescriptor;
}

int RuleResults::getSummaryResultCode() const
{
    return m_summaryResultCode;
}

std::string RuleResults::getExtraValue( int index ) const
{
    require( ( index < numExtraValues() ) && ( index >= 0 ) );
    return m_extraValues.at( index );
}

std::string RuleResults::getSummaryMsg() const
{
    return m_summaryMsg;
}

RuleResults::const_iterator RuleResults::begin() const
{
    return m_results.begin();
}

RuleResults::const_iterator RuleResults::end() const
{
    return m_results.end();
}

int RuleResults::numExtraValues() const
{
    return m_extraValues.size();
}

SnapshotObserver::SnapshotObserver()
{
}

SnapshotObserver::~SnapshotObserver()
{
}

}

