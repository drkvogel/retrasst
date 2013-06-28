#ifndef QUEUEBUILDERPARAMSH
#define QUEUEBUILDERPARAMSH

#include "ClusterIDs.h"
#include "QueuedSamplesBuilderFunction.h"
#include <set>

namespace valc
{

class BuddyDatabase;
class WorklistDirectory;

/*
    An implementation of QueuedSamplesBuilderFunction::Params. Used by AnalysisActivitySnapshotImpl.
*/
class QueueBuilderParams : public QueuedSamplesBuilderFunction::Params
{
public:
    QueueBuilderParams( const BuddyDatabase* bdb, const WorklistDirectory* wd, const ClusterIDs* clusterIDs );
    bool existsOpenRunForSample( const std::string& sampleDescriptor );
    void forEachWorklistEntry( QueuedSamplesBuilderFunction* f );
    bool isLocalOrCluster( int machineID );

    typedef std::set< std::string > SampleDescriptors;

private:
    int m_localMachineID;
    const ClusterIDs* m_clusterIDs;
    const WorklistDirectory* m_worklistDirectory;
    SampleDescriptors m_sampleDescriptorsWithOpenRuns;
};

};

#endif

