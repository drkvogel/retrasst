#ifndef LOADWORKLISTENTRIESH
#define LOADWORKLISTENTRIESH

#include "ClusterIDs.h"

namespace paulst
{
    class LoggingService;
};

namespace valc
{

class DBConnection;
class ResultIndex;
class WorklistEntries;

class LoadWorklistEntries
{
public:

    static const std::string SQL_WORKLIST_SELECT;
    static const std::string SQL_WORKLIST_RELATION_SELECT;
    static const std::string SQL_ORDER_BY;

    LoadWorklistEntries( 
        /* input parameter */
        const ClusterIDs* clusterIDs, 
        /* Collection into which worklist entries will be loaded. May already contain some entries. */
        WorklistEntries* worklistEntries, 
        int localMachineID, 
        DBConnection* con, 
        paulst::LoggingService* log, 
        /*  If loaded worklist entries name-check results, then this information will be added to resultIndex. */
        ResultIndex* resultIndex,         
        /*  Optional input parameter. By default, restricts to worklist entries for the local machine or a related cluster and 
            where the status is one of Q, P, C, S, W or X. */
        const std::string& worklistSQL = "", 
        /*  Optional input parameter. By default restricts to those entries in worklist_relation for which either the rerun_id or 
            the parent_id is in the set of worklist entries defined by the default query on the worklist table (see above). */
        const std::string& worklistRelationSQL = "" );
    void execute();
    int releaseReturnValue();
private:
    WorklistEntries* m_worklistEntries;
    const int m_localMachineID;
    DBConnection* m_con;
    paulst::LoggingService* m_log;
    ResultIndex* m_resultIndex;
    std::string m_worklistSQL, m_worklistRelationSQL;
    const ClusterIDs* m_clusterIDs;
};

};

#endif 

