#ifndef APIH
#define APIH

#include <System.hpp>
#include <boost/variant.hpp>
#include "IntList.h"
#include <map>
#include <string>
#include <vector>

namespace paulst
{
    class LoggingService;
};

namespace valc
{

class Cursor
{
public:
    Cursor();
    virtual                 ~Cursor();
    virtual operator bool() const { return ! endOfRecordSet(); }
    virtual void            close()              = 0;
    virtual bool            isNull   ( int col ) = 0;
    virtual bool            endOfRecordSet() const = 0;
    virtual void            next()               = 0;
    virtual void            read( int col, bool&        outVal ) = 0;
    virtual void            read( int col, char&        outVal ) = 0;
    virtual void            read( int col, float&       outVal ) = 0;
    virtual void            read( int col, int&         outVal ) = 0;
    virtual void            read( int col, std::string& outVal ) = 0;
    virtual void            read( int col, TDateTime&   outVal ) = 0;
private:
    Cursor( const Cursor& );
    Cursor& operator=( const Cursor& );
};

class DBConnection
{ 
public:
    DBConnection();
    virtual ~DBConnection();
    virtual void close() = 0;
    virtual Cursor* executeQuery( const std::string& sql ) = 0;
    virtual void    executeStmt ( const std::string& sql ) = 0;
private:
    DBConnection( const DBConnection& );
    DBConnection& operator=( const DBConnection& );
};

class Properties
{
public:
    Properties();
    void setProperty( const std::string& name, const std::string& value );
    std::string getProperty( const std::string& name ) const;
private:
    typedef std::map< std::string, std::string > Map;
    Map m_propertyValues;

    Properties( const Properties& );
    Properties& operator=( const Properties& );
};

class DBConnectionFactory
{
public:
    static DBConnection* createConnection( const Properties& p );
private:
    DBConnectionFactory();
};


class WorklistEntry;


typedef std::vector< const WorklistEntry* > SampleWorklistEntries;

class LocalRun
{
public:
    LocalRun();
    LocalRun( const LocalRun& );
    LocalRun( const std::string& sampleDescriptor, const std::string& id );
    LocalRun& operator=( const LocalRun& r );
    std::string getSampleDescriptor() const;
private:
    std::string m_id, m_sampleDescriptor;
};

class BatchDelimiter
{
};

typedef boost::variant< LocalRun, BatchDelimiter >              LocalEntry;
typedef std::vector< LocalEntry >                               LocalEntries;
typedef LocalEntries::const_iterator                            LocalEntryIterator;

class QueuedSample
{
public:
    QueuedSample();
    QueuedSample( const QueuedSample& );
    QueuedSample( const std::string& sampleDescriptor );
    QueuedSample& operator=( const QueuedSample& q );
    std::string getSampleDescriptor() const;
private:
    std::string m_sampleDescriptor;
};

typedef std::vector< QueuedSample >     QueuedSamples;
typedef QueuedSamples::const_iterator   QueuedSampleIterator;

/*  An ordered sequence of LocalEntry instances.  */
class AnalysisActivitySnapshot
{
public:
    AnalysisActivitySnapshot();
    virtual ~AnalysisActivitySnapshot();
    virtual LocalEntryIterator   localBegin() const = 0;
    virtual LocalEntryIterator   localEnd()   const = 0;
    virtual QueuedSampleIterator queueBegin() const = 0;
    virtual QueuedSampleIterator queueEnd()   const = 0;
    virtual SampleWorklistEntries getWorklistEntries( const std::string& sampleDescriptor ) const = 0;
private:
    AnalysisActivitySnapshot( const AnalysisActivitySnapshot& );
    AnalysisActivitySnapshot& operator=( const AnalysisActivitySnapshot& );
};


class BuddyDatabase;
class Projects;
class ResultIndex;

class SnapshotFactory
{
public:
    static AnalysisActivitySnapshot* load( int localMachineID, int user, DBConnection* c );
private:
    SnapshotFactory();
};

class WorklistEntry;

struct RelatedEntry
{
    const WorklistEntry* related;
    char howRelated;// triggered? rerun?
};

typedef std::vector< RelatedEntry > RelatedEntries;

class TestResult;

class WorklistEntry
{
public:
    WorklistEntry();
    virtual ~WorklistEntry();
    virtual std::string         getBarcode()        const = 0;
    virtual int                 getBuddyResultID()  const = 0;
    virtual int                 getCategoryID()     const = 0;
    virtual RelatedEntries      getChildren()       const = 0;
    virtual float               getDiluent()        const = 0;
    virtual int                 getGroupID()        const = 0;
    virtual int                 getID()             const = 0;
    virtual IntList             getIDsOfRelatedEntries() const = 0;
    virtual int                 getMachineID()      const = 0;
    virtual RelatedEntry        getParent()         const = 0;
    virtual int                 getProfileID()      const = 0;
    virtual int                 getProjectID()      const = 0;
    virtual const TestResult*   getTestResult()     const = 0;
    virtual std::string         getSampleDescriptor () const = 0;// implements Business Rules 2 & 3
    virtual int                 getSampleID()       const = 0;
    virtual int                 getTestID()         const = 0;
    virtual TDateTime           getTimeStamp()      const = 0;
    virtual int                 getTSSequence()     const = 0;
    virtual char                getStatus()         const = 0;
    virtual bool                hasChildren()       const = 0;
    virtual bool                hasParent()         const = 0;
private:
    WorklistEntry( const WorklistEntry& );
    WorklistEntry& operator=( const WorklistEntry& );
};


class TestResult
{
public:
    TestResult();
    virtual ~TestResult();
    virtual char        getActionFlag       () const = 0;
    virtual TDateTime   getDateAnalysed     () const = 0;
    virtual std::string getSampleDescriptor () const = 0;// implements Business Rules 2 & 3
    virtual int         getMachineID        () const = 0;
    virtual int         getID               () const = 0;
    virtual float       getResultValue      () const = 0;
    virtual std::string getSampleRunID      () const = 0;
    virtual int         getTestID           () const = 0;
private:
    TestResult( const TestResult& );
    TestResult& operator=( const TestResult& );
};

};
#endif

