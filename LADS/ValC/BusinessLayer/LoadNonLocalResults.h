#ifndef LOADNONLOCALRESULTSH
#define LOADNONLOCALRESULTSH

#include "ClusterIDs.h"

/*
    Loads results obtained by other analysers if
    those results are associated with 
    worklist entries included by the SQL query 
    that drives LoadWorklistEntries.
*/

namespace paulst
{
    class LoggingService;
};

namespace valc
{

class DBConnection;
class Projects;
class ResultIndex;

class LoadNonLocalResults
{
public:
    LoadNonLocalResults( 
        const Projects*         projects,
        DBConnection*           con, 
        paulst::LoggingService* log,
        ResultIndex*            resultIndex,
        const std::string&      sql );
    void execute();
private:
    const ClusterIDs*       m_clusterIDs;
    const Projects*         m_projects; 
    DBConnection*           m_con;
    paulst::LoggingService* m_log;
    ResultIndex*            m_resultIndex;
    std::string             m_sql;

    void loadResult( Cursor& c );
};

};

#endif

