#include "AnalysisActivitySnapshotImpl.h"
#include <boost/foreach.hpp>
#include "BuddyDatabase.h"
#include "Projects.h"
#include "QueueBuilderParams.h"
#include "QueuedSamplesBuilderFunction.h"
#include "ResultDirectory.h"
#include "WorklistDirectory.h"

namespace valc
{

AnalysisActivitySnapshotImpl::AnalysisActivitySnapshotImpl( 
    const ClusterIDs* clusterIDs, const Projects* p, const BuddyDatabase* bdb, paulst::LoggingService* log, 
    const ResultDirectory* rd, const WorklistDirectory* wd )
    : 
    m_buddyDatabase     ( bdb ),
    m_clusterIDs        ( clusterIDs ),
    m_log               ( log),
    m_projects          ( p ),
    m_resultDirectory   ( rd ),
    m_worklistDirectory ( wd )
{
    BOOST_FOREACH( const SampleRun& sr, *m_buddyDatabase )
    {
        m_localEntries.push_back( LocalRun( sr.getSampleDescriptor(), sr.getID() ) );
    }

    QueuedSamplesBuilderFunction buildQueue( new QueueBuilderParams( bdb, wd, clusterIDs ) );
    buildQueue( &m_queuedSamples ); 
}

LocalEntryIterator AnalysisActivitySnapshotImpl::localBegin() const
{
    return m_localEntries.begin();
}

LocalEntryIterator AnalysisActivitySnapshotImpl::localEnd()   const
{
    return m_localEntries.end();
}

QueuedSampleIterator AnalysisActivitySnapshotImpl::queueBegin() const
{
    return m_queuedSamples.begin();
}

QueuedSampleIterator AnalysisActivitySnapshotImpl::queueEnd()   const
{
    return m_queuedSamples.end();
}

class WorklistEntryCollector : public WorklistDirectory::Func
{
public:
    SampleWorklistEntries worklistEntries;

    void execute( const WorklistEntry* wle )
    {
        worklistEntries.push_back( wle );
    }
};


SampleWorklistEntries AnalysisActivitySnapshotImpl::getWorklistEntries( const std::string& sampleDescriptor ) const
{
    WorklistEntryCollector wec;
    m_worklistDirectory->forEach( wec, sampleDescriptor );
    return wec.worklistEntries;
}

}

