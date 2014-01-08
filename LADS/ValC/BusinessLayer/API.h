#ifndef APIH
#define APIH

#include <System.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/variant.hpp>
#include "IntList.h"
#include <map>
#include <string>
#include "StringBuilder.h"
#include <SysUtils.hpp>
#include <vector>

namespace paulst
{
    class LoggingService;
    class Properties;
};

namespace valc
{

// Forward declarations
class AnalysisActivitySnapshot;
class UserAdvisor;

/*
    An opaque pointer to a loaded Snapshot. A Snapshot is a model of sample analysis activity for a particular analyser.

    Refer to method 'Load' (below) regarding the loading of a Snapshot instance.
*/
class SnapshotPtr
{
public:
    SnapshotPtr( AnalysisActivitySnapshot* p = NULL )
        : m_ptr(p)
    {
    }

    SnapshotPtr( const SnapshotPtr& p )
        :
        m_ptr(p.m_ptr)
    {
    }

    SnapshotPtr& operator=( const SnapshotPtr& p )
    {
        m_ptr = p.m_ptr;
        return *this;
    }

    operator bool() const { return m_ptr != NULL; }

    AnalysisActivitySnapshot* operator->() 
    {
        return m_ptr;
    }

    const AnalysisActivitySnapshot* operator->() const
    {
        return m_ptr;
    }

private:
    AnalysisActivitySnapshot* m_ptr;
};

/*
    Implementations of UserAdvisor serve to advise 
    the user of issues encountered by the BusinessLayer.

    Note that implementations of 'advise' should be able 
    to cope with being called concurrently on different 
    threads and should be non-blocking.

    One possibility is to implement UserAdvisor as a Queue.
    Implementations of the 'advise' method would simply 
    add the message to the end of the queue.
*/
class UserAdvisor
{
public:
    UserAdvisor();
    virtual ~UserAdvisor();
    virtual void advise( const std::string& warning ) = 0;
private:
    UserAdvisor( const UserAdvisor& );
    UserAdvisor& operator=( const UserAdvisor& );
};


/*
    The application context must be initialised before a Snapshot can be loaded.

    It is an error to attempt to initialise the application context if it has already 
    been initialised.

    Parameters:

        localMachineID  - the ID of the local analyser
        user            - the ID of the user
        configString    - configuration. Load config.txt into a string. e.g. (in paulst/StrUtil.h) loadContentsOf("config.txt")
        log             - a logging service
        userAdvisor     - callback allowing the BusinessLayer to issue warnings to the UI

    Example of how to create an instance of LoggingService:

        paulst::LoggingService* log = new paulst::LoggingService( new paulst::ConsoleWriter() );
*/
void InitialiseApplicationContext( 
    int                                     localMachineID, 
    int                                     user, 
    const std::string&                      config, /* Refer to paulst::Config re. format of this string */
    paulst::LoggingService*                 log,
    UserAdvisor*                            userAdvisor ); 

/*
    Loads a Snapshot.

    Loading of Snapshots is one-at-a-time.  To reload, Unload and then Load.
*/
SnapshotPtr Load(); 

/*
    Unloads a loaded Snapshot.

    The SnapshotPtr is no longer safe to use.
*/
void Unload( SnapshotPtr s );

/*
    Applications should call this method to release system resources prior to termination. 
*/
void DeleteApplicationContext();



/*
    A pair of iterators describing the start and end of a sequence of items (of any sort).

    Is a subclass of std::pair. 
    The 'first'  member variable holds the iterator describing the start.
    The 'second' member variable holds the iterator describing the end.
*/
template<class Iter>
struct Range : public std::pair<Iter, Iter>
{
    Range() 
    {
    }

    Range( Iter start, Iter end ) 
        : std::pair<Iter,Iter>( start, end ) 
    {
    }

    Range( const Range& other ) 
    {
        first  = other.first;
        second = other.second;
    }

    Range& operator=( const Range& other )
    {
        first  = other.first;
        second = other.second;
        return *this;
    }
};


/*
    Represents a sample-run, i.e. an instance of a sample 
    having been (or, in the middle of being) run on the local
    analyser.
*/
class LocalRun
{
public:

    struct Impl;
    friend Impl;

    LocalRun();
    LocalRun( const LocalRun& );
    LocalRun( const std::string& sampleDescriptor, const std::string& id );
    LocalRun& operator=( const LocalRun& r );
    /*
        Returns the ID of the group/batch with which this run is associated.
    */
    int getGroupID() const;
    /*
        Returns an identifier for the sample being run.

        To obtain worklist entries for this sample, refer to 
        AnalysisActivitySnapshot::getWorklistEntries ( const std::string& )   
    */
    std::string getSampleDescriptor() const;
    /*
        Returns an identifier for this sample-run.
    */
    std::string getRunID() const;

    /*
        Is this run open, or has it been closed off?
    */
    bool isOpen() const;
private:
    std::string m_id, m_sampleDescriptor;
    Impl* m_impl;
};

/*
    Marks the boundary of a batch of samples.

    Thus, for example, one might expect to encounter 
    a BatchDelimiter in the following cases:
        a) following a group of QC samples and 
            before the first Unknown sample.
        b) following a group of Unknown samples and 
            before a following QC sample
*/
class BatchDelimiter
{
};

/*
    A LocalEntry is EITHER a LocalRun or a BatchDelimiter. It is expected to 
    have a representation in the 'top half' of the ValC user interface.
*/
typedef boost::variant< LocalRun, BatchDelimiter >              LocalEntry;
/*
    LocalEntries is a sequence of LocalEntry instances.
*/
typedef std::vector< LocalEntry >                               LocalEntries;
typedef LocalEntries::const_iterator                            LocalEntryIterator;

/*
    Represents a sample that is in the 'todo' list. It is expected to
    have a representation in the 'bottom half' of the ValC user interface.
*/
class QueuedSample
{
public:
    QueuedSample();
    QueuedSample( const QueuedSample& );
    QueuedSample( const std::string& sampleDescriptor );
    QueuedSample& operator=( const QueuedSample& q );
    /*
        Returns an identifier for the sample that is queued.

        To obtain worklist entries for this sample, refer to 
        AnalysisActivitySnapshot::getWorklistEntries ( const std::string& )   
    */
    std::string getSampleDescriptor() const;
private:
    std::string m_sampleDescriptor;
};

typedef std::vector< QueuedSample >     QueuedSamples;
typedef QueuedSamples::const_iterator   QueuedSampleIterator;

class WorklistEntry;
class WorklistEntryIteratorImpl;

/*
    Given a sequence of WorklistEntry instances, WorklistEntryIterator 
    is a handle to one such instance.

    Dereferencing a WorklistEntryIterator 
    yields the following datatype: const WorklistEntry*
    Do NOT delete the WorklistEntry instances exposed in this way.
    The exposted instances of WorklistEntry have the same lifetime as 
    the AnalysisActivitySnapshot from which they are obtained.

    Increment a WorklistEntryIterator in order to 
    access the next WorklistEntry in the sequence.
*/
class WorklistEntryIterator : public 
  boost::iterator_facade< WorklistEntryIterator, const WorklistEntry*, boost::forward_traversal_tag>
{
public:
    WorklistEntryIterator();
    WorklistEntryIterator( WorklistEntryIteratorImpl* );
    WorklistEntryIterator( const WorklistEntryIterator& other );
    ~WorklistEntryIterator();
private:
    WorklistEntryIteratorImpl *m_impl;

    friend class boost::iterator_core_access;

    void                    increment();
    bool                    equal( WorklistEntryIterator const& other ) const;
    const WorklistEntry*&   dereference() const;
};


/*
    Describes a row in the buddy_database table.
*/
struct BuddyDatabaseEntry
{
    int buddy_sample_id, alpha_sample_id;
    std::string barcode, database_name;
    TDateTime date_analysed;
    
    BuddyDatabaseEntry();
    BuddyDatabaseEntry( int buddySampleID, int alphaSampleID, const std::string& bcode, const std::string& databaseName,
        const TDateTime& dateAnalysed );
    BuddyDatabaseEntry( const BuddyDatabaseEntry& );
    BuddyDatabaseEntry& operator=( const BuddyDatabaseEntry& );
};

typedef std::vector<BuddyDatabaseEntry> BuddyDatabaseEntries;

/*
    A TestResult can be subjected to Rules in order to assess their reliability/validity.

    RuleResult describes the outcome for a single Rule.
*/
struct RuleResult
{
    /* Regarding legitimate values for resultCode, refer to RuleResults below. */
    int         resultCode;
    /* The name of the rule */
    std::string rule;
    std::string msg;
};

/*
    Identifies a row in the qc_rule table.
*/
class RuleDescriptor
{
public:
    RuleDescriptor( int recordID, int ruleID, const std::string& uniqueName, const std::string& desc );
    RuleDescriptor();
    RuleDescriptor( const RuleDescriptor& rd );
    RuleDescriptor& operator=( const RuleDescriptor& rd );
    int         getRecordID()   const;
    int         getRuleID()     const;
    std::string getUniqueName() const;
    std::string getDesc()       const;
private:
    int m_recordID, m_ruleID;
    std::string m_uniqueName, m_desc;
};

/*
    A TestResult can be subjected to Rules in order to assess their reliability/validity.

    RuleResults encapsulates the outcome of the application of such Rules for a particular TestResult.

    Because an individual TestResult can have multiple Rules applied to it, RuleResults is an aggregation of RuleResult instances.
*/
class RuleResults
{
public:
    const static int RESULT_CODE_FAIL               = 0;
    const static int RESULT_CODE_PASS               = 1;
    const static int RESULT_CODE_BORDERLINE         = 2;
    const static int RESULT_CODE_ERROR              = 3;
    const static int RESULT_CODE_NO_RULES_APPLIED   = -1;

    typedef std::vector<RuleResult> RuleResultCollection;
    typedef RuleResultCollection::const_iterator const_iterator;

    RuleResults();
    RuleResults( const RuleDescriptor& rd, const_iterator begin, const_iterator end, int summaryResultCode, const std::string& summaryMsg,
        const std::vector< std::string >& extraValues = std::vector< std::string >() );
    RuleResults( const RuleResults& );
    RuleResults& operator=( const RuleResults& );
    /* Returns an iterator to the first RuleResult in the sequence.  */
    const_iterator  begin()                const;
    /* Returns an iterator marking the end of the RuleResult sequence.  */
    const_iterator  end()                  const;
    std::string     getExtraValue(int idx) const; // zero-based index
    RuleDescriptor  getRuleDescriptor()    const;
    /* Returns one of the RESULT_CODE constants listed above. */
    int             getSummaryResultCode() const;
    std::string     getSummaryMsg()        const;
    int             numExtraValues()       const;
private:
    int                         m_summaryResultCode;
    std::string                 m_summaryMsg;
    RuleResultCollection        m_results;
    RuleDescriptor              m_ruleDescriptor;
    std::vector<std::string>    m_extraValues;
};

/*
    Parent-child relationships can exist between worklist entries.
    For example, a rerun is a 'child' of the original worklist entry.

    WorklistRelative provides access to such data. Instances of WorklistRelative should be sourced 
    from AnalysisActivitySnapshot, via the method 'viewRelatively'.

    An instance of WorklistRelative always describes a worklist entry.  However, that 
    worklist entry may not have been loaded from the database.  Examples of when a worklist entry may not have been loaded:
        a) The worklist entry has status 'T' ('transmitted to project')
        b) The worklist entry is associated with a different cluster
    Use 'isBoundToWorklistEntryInstance' to test whether or not the worklist entry has been loaded.
    If this returns 'true', then it is safe to use '->' to access methods on the WorklistEntry interface.
*/
class WorklistRelative
{
public:

    struct Impl;
    friend Impl;

    WorklistRelative( int id = 0, const WorklistEntry* entry = 0, char howCreated = '\0' );
    WorklistRelative( const WorklistRelative& );
    WorklistRelative&               operator=( const WorklistRelative& );
    bool                            isBoundToWorklistEntryInstance()    const;
    const WorklistEntry*            operator->()                        const;
    bool                            hasChildren()                       const;
    bool                            hasParent()                         const;
    std::vector<WorklistRelative>   getChildren()                       const;
    int                             getID()                             const;
    WorklistRelative                getParent()                         const;
    char                            getRelation()                       const;
private:
    const WorklistEntry* m_worklistEntry;
    char m_howCreated;// triggered? rerun?
    int  m_id;

    const Impl* m_impl;
};


class SnapshotObserver;

/*  
    AnalysisActivitySnapshot is the model of which the ValC
    user interface is a representation.

    'localBegin' and 'localEnd' describe on ordered sequence of 
    LocalEntry instances for representation in the 'top half' of
    the ValC user interface. From the perspective of the user 
    interface, the order of the sequence is top-to-bottom.
    Thus, top of the 'top half' should be the LocalEntry 
    obtained by dereferencing the iterator returned by 
    'localBegin'.

    'queueBegin' and 'queueEnd' describe on ordered sequence of 
    QueuedSample instances for representation in the 'bottom half' of
    the ValC user interface. From the perspective of the user 
    interface, the order of the sequence is top-to-bottom.
    Thus, top of the 'bottom half' should be the QueuedSample 
    obtained by dereferencing the iterator returned by 
    'queueBegin'.

    Note that AnalysisActivitySnapshot OWNS the sequences of instances 
    to which it gives access. Thus it is not advisable to use 
    instances of objects that have been obtained from an instance of 
    AnalysisActivitySnapshot once the latter has been destroyed.
    To give a concrete example:
        The following sequence will cause trouble:
            a) Obtain an instance of AnalysisActivitySnapshot.
            b) Obtain a LocalEntryIterator by calling 'localBegin' on the 
                instance obtained in step (a).
            c) Delete the instance of AnalysisActivitySnapshot
                that was obtained in step (a).
            d) Dereference the LocalEntryIterator obtained in 
                step (b).
        Following (c), the iterator obtained at (b) ceases to 
        be valid.
*/
class AnalysisActivitySnapshot
{
public:
    AnalysisActivitySnapshot();
    virtual ~AnalysisActivitySnapshot();
    /*
        When a Snapshot gets Loaded, one or more database updates may get scheduled.
        Call this method to allow those updates to proceed.
        Parameters:
            - whether the method should block until all updates have been performed, or allow 
                updates to continue in the background
    */
    virtual void runPendingDatabaseUpdates( bool block ) = 0;
    /*
        How to test that a TestResult is associated with a particular LocalRun? 
        The only way to find out is to use this method, supplying the runID of the LocalRun
        and the sampleRunID of the TestResult. ie
            compareSampleRunIDs( myTestResult.getSampleRunID(), myLocalRun.getRunID() );
    */
    virtual bool compareSampleRunIDs( const std::string& oneRunID, const std::string& anotherRunID )        const = 0;
    virtual LocalEntryIterator              localBegin()                                                    const = 0;
    virtual LocalEntryIterator              localEnd()                                                      const = 0;
    virtual QueuedSampleIterator            queueBegin()                                                    const = 0;
    virtual QueuedSampleIterator            queueEnd()                                                      const = 0;

    /*
        Returns a 'wrapper' that supports queries regarding how a worklist entry was 
        created, whether it has a parent, whether it has children, etc.
    */
    virtual WorklistRelative                viewRelatively( const WorklistEntry* e )                        const = 0;

    /* Returns the RuleResults for the specified result.  See hasRuleResults below. */
    virtual RuleResults                     getRuleResults( int resultID )                                  const = 0;

    /*
        Returns a pair of iterators that describe the set 
        of worklist entries that are associated with 
        the specified sample.

        Note that the return value cannot be used with confidence if there are pending asynchronous 
        updates to the list of worklist entries - for example if the insertion of a rerun is in progress.
    */
    virtual Range<WorklistEntryIterator>    getWorklistEntries( const std::string& sampleDescriptor )       const = 0;
    /*
        Returns the (short) name for the specified test.
        Implementatin uses a cache and is therefore efficient and cheap.
    */
    virtual std::string                     getTestName( int testID )                                       const = 0;
    
    /* Returns true if Rules have been applied to the specified result.  */
    virtual bool                            hasRuleResults( int resultID )                                  const = 0;
    
    /*
        A diagnostic method for identifying the rows in buddy_database that are associated with 
        the specified sample-run.

        The original motivation for this method was that sometimes a LocalRun has no worklist entries 
        associated with it.  In such cases, the interface can only show a blank entry for the LocalRun.
        In such cases, it would be useful to know more.
    */
    virtual BuddyDatabaseEntries            listBuddyDatabaseEntriesFor( const std::string& sampleRunID )   const = 0;

    /*
        Initiate a rerun of the specified worklist entry.
        The return value is a handle which can be used to test when the rerun request has been processed (WaitForSingleObject).
        The handle should be closed when it is no longer needed (using the Windows API function CloseHandle).

        Changes made to the snaphost object model are broadcast via SnapshotObserver.
    */
    virtual HANDLE                       queueForRerun( int worklistID, const std::string& sampleRunID, const std::string& sampleDescriptor ) = 0;

    /*
        Use this method to specifiy a callback interface on which to receive notifications of updates to the snapshot.
    */
    virtual void                            setObserver( SnapshotObserver* obs ) = 0;

    /*
        Call this method before Unloading the Snapshot.  

        There are methods on the Snapshot interface which cause processes to run in the background. 
        For example, 'runPendingDatabaseUpdates' can create a background process for updating the database 
        (unless a value of 'true' is supplied for the 'block' parameter). 
        Another example is 'queueForRerun'.  The process that updates the database and the in-memory model (i.e. this Snapshot) 
        runs in the background.

        Use the 'millis' parameter to specify how long you are prepared to wait for background processes to complete.

        A return value of 'true' indicates that all background processes have run to completion.

        If a value of 'false' is obtained, then there are two options:
            1) wait again
            2) continue with the Unload, in which case some of the background processes may get cancelled.
    */
    virtual bool                            waitForActionsPending(long millis) = 0;

private:
    AnalysisActivitySnapshot( const AnalysisActivitySnapshot& );
    AnalysisActivitySnapshot& operator=( const AnalysisActivitySnapshot& );
};

class BuddyDatabase;
class Projects;
class ResultIndex;

class TestResult;
class TestResultIteratorImpl;

/*
    Given a sequence of TestResult instances, TestResultIterator 
    is a handle to one such instance.

    Dereferencing a TestResultIterator 
    yields the following datatype: const TestResult*
    Do NOT delete the TestResult instances exposed in this way.
    The exposted instances of TestResult have the same lifetime as 
    the AnalysisActivitySnapshot from which they are obtained.

    Increment a TestResultIterator in order to 
    access the next TestResult in the sequence.
*/
class TestResultIterator : public 
  boost::iterator_facade< TestResultIterator, const TestResult*, boost::forward_traversal_tag>
{
public:
    TestResultIterator();
    TestResultIterator( TestResultIteratorImpl* );
    TestResultIterator( const TestResultIterator& other );
    ~TestResultIterator();
private:
    TestResultIteratorImpl *m_impl;

    friend class boost::iterator_core_access;

    void                 increment();
    bool                 equal( TestResultIterator const& other ) const;
    const TestResult*&   dereference() const;
};


/*
    A WorklistEntry represents a row in the database table c_buddy_worklist.
    Most of the properties of a WorklistEntry correspond to 
    similarly named columns on this table.
*/
class WorklistEntry
{
public:
    WorklistEntry();
    virtual ~WorklistEntry();
    /*
        c_buddy_worklist.barcode
    */
    virtual std::string                 getBarcode()                const = 0;
    /*
        c_buddy_worklist.buddy_result_id
    */
    virtual int                         getBuddyResultID()          const = 0;
    /*
        c_buddy_worklist.category_id
    */
    virtual int                         getCategoryID()             const = 0;
    /*
        c_buddy_worklist.diluent
    */
    virtual float                       getDiluent()                const = 0;
    /*
        c_buddy_worklist.group_id
    */
    virtual int                         getGroupID()                const = 0;
    /*
        c_buddy_worklist.record_no
    */
    virtual int                         getID()                     const = 0;
    /*
        c_buddy_worklist.machine_cid
    */
    virtual int                         getMachineID()              const = 0;
    /*
        c_buddy_worklist.profile_id
    */
    virtual int                         getProfileID()              const = 0;
    /*
        c_buddy_worklist.profile_name
    */
    virtual std::string                 getProfileName()            const = 0;
    /*
        c_buddy_worklist.project_cid
    */
    virtual int                         getProjectID()              const = 0;
    /*
        Returns a pair of iterators that describe a sequence of TestResults.

        For more info, refer to documentation for Range and for TestResultIterator.
    */
    virtual Range<TestResultIterator>   getTestResults()            const = 0;
    /*
        Returns a string value which uniquely identifies the sample 
        to which this worklist entry concerns.
    */
    virtual std::string                 getSampleDescriptor()       const = 0;// implements Business Rules 2 & 3
    /*
        c_buddy_worklist.sample_id
    */
    virtual int                         getSampleID()               const = 0;
    /*
        c_buddy_worklist.test_cid
    */
    virtual int                         getTestID()                 const = 0;
    /*
        c_buddy_worklist.time_stamp
    */
    virtual TDateTime                   getTimeStamp()              const = 0;
    /*
        c_buddy_worklist.ts_sequence
    */
    virtual int                         getTSSequence()             const = 0;
    /*
        c_buddy_worklist.status
    */
    virtual char                        getStatus()                 const = 0;
private:
    WorklistEntry( const WorklistEntry& );
    WorklistEntry& operator=( const WorklistEntry& );
};


/*
    A TestResult represents a row in the database table buddy_result_float.
    Most of the properties of a TestResult correspond to 
    similarly named columns on this table.

    Note that ValC is selective in its inclusion of rows from buddy_result_float,
    only including rows with an action_flag value of zero or 'X' (Business Rule 1).
*/
class TestResult
{
public:
    TestResult();
    virtual ~TestResult();
    /*
        buddy_result_float.action_flag
    */
    virtual char        getActionFlag       () const = 0;
    /*
        buddy_result_float.date_analysed
    */
    virtual TDateTime   getDateAnalysed     () const = 0;
    /*
        Returns a string value which uniquely identifies the sample 
        to which this test result concerns.
    */
    virtual std::string getSampleDescriptor () const = 0;// implements Business Rules 2 & 3
    /*
        buddy_result_float.machine_cid
    */
    virtual int         getMachineID        () const = 0;
    /*
        buddy_result_float.buddy_result_id
    */
    virtual int         getID               () const = 0;
    /*
        buddy_result_float.res_text
    */
    virtual std::string getResultText       () const = 0;
    /*
        buddy_result_float.res_value
    */
    virtual float       getResultValue      () const = 0;
    /*
        Returns an identifier for the sample-run on which this 
        result was obtained.
    */
    virtual std::string getSampleRunID      () const = 0;
    /*
        buddy_result_float.test_id
    */
    virtual int         getTestID           () const = 0;
private:
    TestResult( const TestResult& );
    TestResult& operator=( const TestResult& );
};


/*
    Used in order to notify the UI of changes that have been 
    made to the snapshot object model.
*/
class SnapshotObserver
{
public:
    SnapshotObserver();
    virtual ~SnapshotObserver();
    virtual void notifyWorklistEntryChanged ( const WorklistEntry* we ) = 0;
    virtual void notifyNewWorklistEntry     ( const WorklistEntry* we ) = 0;
    virtual void notifySampleAddedToQueue   ( const std::string& sampleDescriptor ) = 0;
    virtual void notifySampleRunClosedOff   ( const std::string& runID ) = 0;
    virtual void notifyUpdateFailed         ( const char* errorMsg ) = 0;
};


}
#endif

