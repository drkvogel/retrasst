#ifndef ANALYSISACTIVITYSNAPSHOTIMPLH
#define ANALYSISACTIVITYSNAPSHOTIMPLH

#include "API.h"
#include <boost/scoped_ptr.hpp>
#include "ClusterIDs.h"
#include "LoggingService.h"
#include <memory>
#include "TestNames.h"


namespace valc
{

class ApplicationContext;
class BuddyDatabase;
class DBUpdateConsumer;
class DBUpdateSchedule;
class Projects;
class ResultAttributes;
class ResultDirectory;
class SampleRunIDResolutionService;
class WorklistDirectory;

/*  An ordered sequence of LocalEntry instances, each of which wraps a SampleRun and knows how to obtain the WorklistEntries associated 
    with that Sample.
    Plus:
    An ordered sequence of QueuedSample instances, each of which knows how to obtain the WorklistEntries associated with that Sample.
*/
class AnalysisActivitySnapshotImpl : public AnalysisActivitySnapshot
{
public:
    AnalysisActivitySnapshotImpl( const ClusterIDs* clusterIDs, const Projects* p, const BuddyDatabase* bdb, 
        const ResultDirectory* rd, 
        const WorklistDirectory* wd, const TestNames* tns, DBUpdateSchedule* dbUpdateSchedule,
        SampleRunIDResolutionService* sampleRunIDResolutionService,
        ApplicationContext* appContext );
    bool                            compareSampleRunIDs( const std::string& oneRunID, const std::string& anotherRunID )    const;
    RuleResults                     getRuleResults( int forResultID ) const;
    bool                            hasRuleResults( int forResultID ) const;
    LocalEntryIterator              localBegin() const;
    LocalEntryIterator              localEnd()   const;
    QueuedSampleIterator            queueBegin() const;
    QueuedSampleIterator            queueEnd()   const;
    std::string                     getTestName( int testID ) const;
    Range<WorklistEntryIterator>    getWorklistEntries( const std::string& sampleDescriptor ) const;
    BuddyDatabaseEntries            listBuddyDatabaseEntriesFor( const std::string& sampleRunID )   const;
    void                            runPendingDatabaseUpdates( DBUpdateExceptionHandlingPolicy* p, bool block );
private:
    typedef boost::scoped_ptr<const Projects>               ProjectsPtr;
    typedef boost::scoped_ptr<const BuddyDatabase>          BuddyDatabasePtr;
    typedef boost::scoped_ptr<const ResultDirectory>        ResultDirectoryPtr;
    typedef boost::scoped_ptr<const WorklistDirectory>      WorklistDirectoryPtr;
    typedef boost::scoped_ptr<const ClusterIDs>             ClusterIDsPtr;
    typedef boost::scoped_ptr<const TestNames>              TestNamesPtr;
    typedef boost::scoped_ptr<DBUpdateSchedule>             DBUpdateSchedulePtr;
    typedef boost::scoped_ptr<SampleRunIDResolutionService> SampleRunIDResolutionServicePtr;
    ApplicationContext*             m_appContext;
    std::auto_ptr<DBUpdateConsumer> m_dbUpdateConsumer;
    BuddyDatabasePtr                m_buddyDatabase;
    ClusterIDsPtr                   m_clusterIDs;
    ProjectsPtr                     m_projects;
    ResultDirectoryPtr              m_resultDirectory;
    WorklistDirectoryPtr            m_worklistDirectory;
    TestNamesPtr                    m_testNames;
    DBUpdateSchedulePtr             m_dbUpdateSchedule;
    LocalEntries                    m_localEntries;
    QueuedSamples                   m_queuedSamples;
    paulst::LoggingService*         m_log;
    SampleRunIDResolutionServicePtr m_sampleRunIDResolutionService;
    ResultAttributes*               m_resultAttributes;
};

};

#endif

