#ifndef LOADCLUSTERIDSH
#define LOADCLUSTERIDSH

#include "ClusterIDs.h"

namespace paulst
{
    class LoggingService;
};

namespace paulstdb
{
    class DBConnection;
}

namespace valc
{

class LoadClusterIDs
{
public:
    LoadClusterIDs( ClusterIDs* clusterIDs, paulst::LoggingService* log, paulstdb::DBConnection* con, int localMachineID );
    ~LoadClusterIDs();
    void execute();
private:
    paulst::LoggingService* m_log;
    paulstdb::DBConnection* m_con;
    ClusterIDs* m_clusterIDs;
    const int m_localMachineID;

    LoadClusterIDs( const LoadClusterIDs& );
    LoadClusterIDs& operator=( const LoadClusterIDs& );
};

};

#endif

