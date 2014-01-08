#ifndef ANALYSISACTIVITYSNAPSHOTIMPLH
#define ANALYSISACTIVITYSNAPSHOTIMPLH

#include "API.h"
#include <boost/scoped_ptr.hpp>
#include "DBTransactionHandler.h"
#include "LocalRunImpl.h"
#include "LoggingService.h"
#include <memory>
#include "RunIDC14n.h"
#include "SampleRunGroupModel.h"
#include "SnapshotUpdateHandle.h"
#include "SnapshotUpdateThread.h"
#include "WorklistRelativeImpl.h"

namespace valc
{

class ApplicationContext;
class BuddyDatabase;
class DBUpdateSchedule;
class ResultAttributes;
class ResultDirectory;
class SampleRunGroupIDGenerator;
class SampleRunIDResolutionService;
class WorklistEntries;
class WorklistLinks;


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
        WorklistEntries* wd, 
        WorklistLinks* wl,
        DBUpdateSchedule* dbUpdateSchedule,
        SampleRunIDResolutionService* sampleRunIDResolutionService,
        ApplicationContext* appContext,
        int pendingUpdateWaitTimeoutSecs,
        SampleRunGroupIDGenerator* sampleRunGroupIDGenerator );
    ~AnalysisActivitySnapshotImpl();
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
    HANDLE                          queueForRerun( int worklistID, const std::string& sampleRunID, const std::string& sampleDescriptor );
    void                            runPendingDatabaseUpdates( bool block );
    void                            setObserver( SnapshotObserver* obs );
    WorklistRelative                viewRelatively( const WorklistEntry* e ) const;
    bool                            waitForActionsPending( long millis );

    friend class SnapshotUpdateHandle;

private:
    ApplicationContext*             m_appContext;
    const BuddyDatabase*            m_buddyDatabase;
    const ResultDirectory*          m_resultDirectory;
    WorklistEntries*                m_worklistEntries;
    WorklistLinks*                  m_worklistLinks;
    DBUpdateSchedule*               m_dbUpdateSchedule;
    LocalEntries                    m_localEntries;
    QueuedSamples                   m_queuedSamples;
    paulst::LoggingService*         m_log;
    SampleRunIDResolutionService*   m_sampleRunIDResolutionService;
    ResultAttributes*               m_resultAttributes;
    const int                       m_pendingUpdateWaitTimeoutSecs;
    SnapshotUpdateHandle            m_updateHandle;
    DBTransactionHandler            m_dbTransactionHandler;
    SnapshotUpdateThread            m_snapshotUpdateThread;
    WorklistRelative::Impl          m_worklistRelativeImpl;
    LocalRun::Impl                  m_localRunImpl;
    SampleRunGroupModel             m_sampleRunGroupModel;
    RunIDC14n                       m_runIDC14n;
};

};

#endif

