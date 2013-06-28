#ifndef ANALYSISACTIVITYSNAPSHOTIMPLH
#define ANALYSISACTIVITYSNAPSHOTIMPLH

#include "API.h"
#include <boost/scoped_ptr.hpp>
#include "ClusterIDs.h"
#include "LoggingService.h"

namespace valc
{

class BuddyDatabase;
class Projects;
class ResultDirectory;
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
        paulst::LoggingService* log, const ResultDirectory* rd, 
        const WorklistDirectory* wd );
    LocalEntryIterator   localBegin() const;
    LocalEntryIterator   localEnd()   const;
    QueuedSampleIterator queueBegin() const;
    QueuedSampleIterator queueEnd()   const;
    SampleWorklistEntries getWorklistEntries( const std::string& sampleDescriptor ) const;
private:
    typedef boost::scoped_ptr<const Projects>           ProjectsPtr;
    typedef boost::scoped_ptr<const BuddyDatabase>      BuddyDatabasePtr;
    typedef boost::scoped_ptr<const ResultDirectory>    ResultDirectoryPtr;
    typedef boost::scoped_ptr<const WorklistDirectory>  WorklistDirectoryPtr;
    typedef boost::scoped_ptr<const ClusterIDs>         ClusterIDsPtr;
    BuddyDatabasePtr        m_buddyDatabase;
    ClusterIDsPtr           m_clusterIDs;
    ProjectsPtr             m_projects;
    ResultDirectoryPtr      m_resultDirectory;
    WorklistDirectoryPtr    m_worklistDirectory;
    LocalEntries            m_localEntries;
    QueuedSamples           m_queuedSamples;
    boost::scoped_ptr< paulst::LoggingService > m_log;
};

};

#endif

