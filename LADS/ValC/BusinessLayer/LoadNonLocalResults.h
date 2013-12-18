#ifndef LOADNONLOCALRESULTSH
#define LOADNONLOCALRESULTSH

#include "ClusterIDs.h"
#include "Task.h"

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

namespace paulstdb
{
    class Cursor;
    class DBConnection;
}

namespace valc
{

class ExceptionalDataHandler;
class Projects;
class ResultIndex;

class LoadNonLocalResults : public stef::Task
{
public:
    LoadNonLocalResults( 
        const Projects*         projects,
        paulstdb::DBConnection* con, 
        paulst::LoggingService* log,
        ResultIndex*            resultIndex,
        const std::string&      sql,
        ExceptionalDataHandler* exceptionalDataHandler );
protected:
    void doStuff();
private:
    const ClusterIDs*       m_clusterIDs;
    const Projects*         m_projects; 
    paulstdb::DBConnection* m_con;
    paulst::LoggingService* m_log;
    ResultIndex*            m_resultIndex;
    std::string             m_sql;
    ExceptionalDataHandler* m_exceptionalDataHandler;

    bool loadResult( paulstdb::Cursor& c );
};

};

#endif

