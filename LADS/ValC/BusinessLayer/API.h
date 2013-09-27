#ifndef APIH
#define APIH

#include "AbstractConnectionFactory.h"
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

namespace paulstdb
{
    class DBConnection;
};

namespace valc
{

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
    Factory method for obtaining a new DBConnection instance.

    It is the caller's responsibility to delete the returned instance 
    when they are done with it.

    Example value for connectionString:
        dsn=paulst_brat_64;db=paulst_test
    In the preceding example, 'paulst_test' is the name of the database and 
    'paulst_brat_64' is the name of a 64bit ODBC datasource.

    Example value for sessionReadLockSetting:
        set lockmode session where readlock = nolock
*/
class DBConnectionFactory : public paulstdb::AbstractConnectionFactory
{
public:
    DBConnectionFactory();
    paulstdb::DBConnection* createConnection( const std::string& connectionString, const std::string& sessionReadLockSetting = "" );
};

/*
    Represents a sample-run, i.e. an instance of a sample 
    having been (or, in the middle of being) run on the local
    analyser.
*/
class LocalRun
{
public:
    LocalRun();
    LocalRun( const LocalRun& );
    LocalRun( const std::string& sampleDescriptor, const std::string& id );
    LocalRun& operator=( const LocalRun& r );
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
private:
    std::string m_id, m_sampleDescriptor;
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
    API users must implement this policy and pass it as a parameter in order to
    'enableDatabaseUpdates' on AnalysisActivitySnapshot.
*/
class DBUpdateExceptionHandlingPolicy
{
public:
    DBUpdateExceptionHandlingPolicy();
    virtual ~DBUpdateExceptionHandlingPolicy();
    /*
        Implement this method to explain that there has been a database update failure.
        Note that callbacks on this method are asynchronous, from a different thread.
        Implementations should not block the thread. e.g. consider using 'PostMessage'
        or some other asynchronous technique in order not to tie up the calling thread.
    */
    virtual void handleException( const std::string& msg ) = 0;
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

struct RuleResult
{
    int         resultCode;
    std::string rule;
    std::string msg;
};

class RuleResults
{
public:
    typedef std::vector<RuleResult> RuleResultCollection;
    typedef RuleResultCollection::const_iterator const_iterator;

    RuleResults();
    RuleResults( const_iterator begin, const_iterator end, int summaryResultCode, const std::string& summaryMsg );
    RuleResults( const RuleResults& );
    RuleResults& operator=( const RuleResults& );
    const_iterator  begin() const;
    const_iterator  end() const;
    int             getSummaryResultCode() const;
    std::string     getSummaryMsg() const;
private:
    int                  m_summaryResultCode;
    std::string          m_summaryMsg;
    RuleResultCollection m_results;
};

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
        A thread updates the database following the loading of a snapshot.
        This thread doesn't run automatically, but only when this method is called.
        Parameters:
            - the connection on which updates should be performed
                (might be the same as the connection used to load the snapshot).
            - an implementation of DBUpdateExceptionHandlingPolicy
                (e.g. to show a message to the user). Note that callbacks on
                this policy will be asynchronous, from a background thread.
            - whether the method should block until all updates have been performed, or allow 
                updates to continue in the background
    */
    virtual void runPendingDatabaseUpdates( paulstdb::DBConnection* c, DBUpdateExceptionHandlingPolicy* p, bool block ) = 0;
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
        Returns a pair of iterators that describe the set 
        of worklist entries that are associated with 
        the specified sample.
    */
    virtual Range<WorklistEntryIterator>    getWorklistEntries( const std::string& sampleDescriptor )       const = 0;
    /*
        Returns the (short) name for the specified test.
        Implementatin uses a cache and is therefore efficient and cheap.
    */
    virtual std::string                     getTestName( int testID )                                       const = 0;
    /*
        A diagnostic method for identifying the rows in buddy_database that are associated with 
        the specified sample-run.

        The original motivation for this method was that sometimes a LocalRun has no worklist entries 
        associated with it.  In such cases, the interface can only show a blank entry for the LocalRun.
        In such cases, it would be useful to know more.
    */
    virtual BuddyDatabaseEntries            listBuddyDatabaseEntriesFor( const std::string& sampleRunID )   const = 0;
private:
    AnalysisActivitySnapshot( const AnalysisActivitySnapshot& );
    AnalysisActivitySnapshot& operator=( const AnalysisActivitySnapshot& );
};


class BuddyDatabase;
class Projects;
class ResultIndex;

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
    Factory method for obtaining an instance of AnalysisActivitySnapshot.

    The caller must delete the returned instance when they no longer need it.
*/
class SnapshotFactory
{
public:
    /*
    Parameters:
        localMachineID the ID of the local analyser
        user            the ID of the user
        c               connection for querying the central database (for worklist entries, results, sample-runs...)
        log             a logging service
        configString    configuration. Load config.txt into a string. e.g. (in paulst/StrUtil.h) loadContentsOf("config.txt")
        userAdvisor     an implementation of UserAdvisor, to receive warnings for display to the user. Can be NULL.

    Use DBConnectionFactory to create an instance of DBConnection. Deleting connection before obtaining an instance of
    AnalysisActivitySnapshot will result in unpredictable behaviour.

    Deleting the log instance before deleting the obtained instance of AnalysisActivitySnapshot will result in 
    unpredictable behaviour.

    Example of how to create an instance of LoggingService:

        paulst::LoggingService* log = new paulst::LoggingService( new paulst::ConsoleWriter() );

    */
    static AnalysisActivitySnapshot* load( int localMachineID, int user, paulstdb::DBConnection* c, paulst::LoggingService* log,
    const std::string& configString, UserAdvisor* userAdvisor );
private:
    SnapshotFactory();
};

/*
    Parent-child relationships can exist between worklist entries.
    For example, a rerun is a 'child' of the original worklist entry.

    WorklistEntry has methods to support access to this data, 
    i.e. 'getParent' and 'getChildren'. These methods return 
    instances of RelatedEntry.

    RelatedEntry describes a WorklistEntry that is related 
    to the WorklistEntry from which it was obtained. It also 
    describes the reason for the existence of the relationship.
*/
struct RelatedEntry
{
    const WorklistEntry* related;
    char howRelated;// triggered? rerun?
};

typedef std::vector< RelatedEntry > RelatedEntries;

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
        Refer to documentation on RelatedEntry
    */
    virtual RelatedEntries              getChildren()               const = 0;
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
        Returns a list of the IDs of worklist entries 
        that are related to this one. For info re. 
        what 'related' means in this context,
        refer to documentation on RelatedEntry.
    */
    virtual IntList                     getIDsOfRelatedEntries()    const = 0;
    /*
        c_buddy_worklist.machine_cid
    */
    virtual int                         getMachineID()              const = 0;
    /*
        Refer to documentation on RelatedEntry.
    */
    virtual RelatedEntry                getParent()                 const = 0;
    /*
        c_buddy_worklist.profile_id
    */
    virtual int                         getProfileID()              const = 0;
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
    virtual bool                        hasChildren()               const = 0;
    virtual bool                        hasParent()                 const = 0;
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

};
#endif

