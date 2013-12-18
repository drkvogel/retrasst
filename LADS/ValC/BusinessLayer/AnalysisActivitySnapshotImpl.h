#ifndef ANALYSISACTIVITYSNAPSHOTIMPLH
#define ANALYSISACTIVITYSNAPSHOTIMPLH

#include "API.h"
#include <boost/scoped_ptr.hpp>
#include "LoggingService.h"
#include <memory>
#include "SnapshotUpdateHandle.h"
#include "SnapshotUpdateThread.h"

namespace valc
{

class ApplicationContext;
class BuddyDatabase;
class DBTransactionHandler;
class DBUpdateSchedule;
class ResultAttributes;
class ResultDirectory;
class SampleRunIDResolutionService;
class WorklistEntries;


/*  An ordered sequence of LocalEntry instances, each of which wraps a SampleRun and knows how to obtain the WorklistEntries associated 
    with that Sample.
    Plus:
    An ordered sequence of QueuedSample instances, each of which knows how to obtain the WorklistEntries associated with that Sample.
*/
class AnalysisActivitySnapshotImpl : public AnalysisActivitySnapshot
{
public:
    AnalysisActivitySnapshotImpl( const BuddyDatabase* bdb, 
        const ResultDirectory* rd, 
        const WorklistEntries* wd, DBUpdateSchedule* dbUpdateSchedule,
        SampleRunIDResolutionService* sampleRunIDResolutionService,
        ApplicationContext* appContext,
        int pendingUpdateWaitTimeoutSecs );
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
    HANDLE                          queueForRerun( int worklistID, const std::string& sampleRunID );
    void                            runPendingDatabaseUpdates( bool block );
    void                            setObserver( SnapshotObserver* obs );
    bool                            waitForActionsPending( long millis );

    friend class SnapshotUpdateHandle;

private:
    typedef boost::scoped_ptr<const BuddyDatabase>          BuddyDatabasePtr;
    typedef boost::scoped_ptr<const ResultDirectory>        ResultDirectoryPtr;
    typedef boost::scoped_ptr<const WorklistEntries>        WorklistEntriesPtr;
    typedef boost::scoped_ptr<DBUpdateSchedule>             DBUpdateSchedulePtr;
    typedef boost::scoped_ptr<SampleRunIDResolutionService> SampleRunIDResolutionServicePtr;
    ApplicationContext*             m_appContext;
    DBTransactionHandler*           m_dbTransactionHandler;
    BuddyDatabasePtr                m_buddyDatabase;
    ResultDirectoryPtr              m_resultDirectory;
    WorklistEntriesPtr              m_worklistEntries;
    DBUpdateSchedulePtr             m_dbUpdateSchedule;
    LocalEntries                    m_localEntries;
    QueuedSamples                   m_queuedSamples;
    paulst::LoggingService*         m_log;
    SampleRunIDResolutionServicePtr m_sampleRunIDResolutionService;
    ResultAttributes*               m_resultAttributes;
    const int                       m_pendingUpdateWaitTimeoutSecs;
    SnapshotUpdateHandle            m_updateHandle;
    SnapshotUpdateThread            m_snapshotUpdateThread;
};

};

#endif

