#ifndef LOADCLUSTERIDSH
#define LOADCLUSTERIDSH

#include "ClusterIDs.h"

namespace paulst
{
    class LoggingService;
};

namespace valc
{

class DBConnection;

class LoadClusterIDs
{
public:
    LoadClusterIDs( ClusterIDs* clusterIDs, paulst::LoggingService* log, DBConnection* con, int localMachineID );
    ~LoadClusterIDs();
    void execute();
    int releaseReturnValue();
private:
    paulst::LoggingService* m_log;
    DBConnection* m_con;
    ClusterIDs* m_clusterIDs;
    const int m_localMachineID;

    LoadClusterIDs( const LoadClusterIDs& );
    LoadClusterIDs& operator=( const LoadClusterIDs& );
};

};

#endif

