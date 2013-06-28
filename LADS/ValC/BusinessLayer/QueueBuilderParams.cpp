#include <algorithm>
#include <boost/bind.hpp>
#include "BuddyDatabase.h"
#include "QueueBuilderParams.h"
#include "SampleRun.h"

namespace valc
{

void storeSampleDescriptorIfRunIsOpen( const SampleRun& sr, QueueBuilderParams::SampleDescriptors& sds )
{
    if ( sr.isOpen() )
    {
        sds.insert( sr.getSampleDescriptor() );
    }
}

QueueBuilderParams::QueueBuilderParams( 
    const BuddyDatabase* bdb, const WorklistDirectory* wd, const ClusterIDs* clusterIDs )
    :
    m_localMachineID( bdb->getLocalMachineID() ),
    m_clusterIDs( clusterIDs ),
    m_worklistDirectory( wd )
{
    std::for_each( bdb->begin(), bdb->end(), boost::bind( storeSampleDescriptorIfRunIsOpen, _1, m_sampleDescriptorsWithOpenRuns ) );
}

bool QueueBuilderParams::existsOpenRunForSample( const std::string& sampleDescriptor )
{
    return m_sampleDescriptorsWithOpenRuns.count( sampleDescriptor );
}

void QueueBuilderParams::forEachWorklistEntry( QueuedSamplesBuilderFunction* f )
{
    WorklistDirectoryFunctionAdapter func(f);
    m_worklistDirectory->forEach( func );
}

bool QueueBuilderParams::isLocalOrCluster( int machineID )
{
    return ( machineID == m_localMachineID ) || m_clusterIDs->count( machineID );
}

}

