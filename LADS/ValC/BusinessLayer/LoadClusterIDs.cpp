#include "API.h"
#include "LoadClusterIDs.h"
#include "LoggingService.h"
#include "ClusterIDs.h"
#include "StringBuilder.h"
#include "Trace.h"


namespace valc
{

LoadClusterIDs::LoadClusterIDs( ClusterIDs* clusterIDs, paulst::LoggingService* log, DBConnection* con, int localMachineID )
    :
    m_log(log),
    m_con(con),
    m_clusterIDs(clusterIDs),
    m_localMachineID( localMachineID )
{
    trace( "LoadClusterIDs constructor" );
}

LoadClusterIDs::~LoadClusterIDs()
{
    trace( "LoadClusterIDs destructor" );
}

int LoadClusterIDs::releaseReturnValue()
{
    return 1;
}

void LoadClusterIDs::execute()
{
    std::string sql = std::string("SELECT cluster_cid FROM c_cluster_machine WHERE machine_cid = ") << m_localMachineID;

    for ( std::auto_ptr<Cursor> cursor( m_con->executeQuery( sql ) ); ! cursor->endOfRecordSet(); cursor->next() )
    {
        int clusterID = 0;

        cursor->read( 0, clusterID );

        m_clusterIDs->insert( clusterID );
    }

    m_log->log( std::string("Loaded ") <<  m_clusterIDs->size() << " clusterIDs." );
}

}

