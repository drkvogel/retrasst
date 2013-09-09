#ifndef LOADREFERENCEDWORKLISTENTRIESH
#define LOADREFERENCEDWORKLISTENTRIESH

#include "IntList.h"
#include "WorklistDirectory.h"

namespace paulst
{
    class LoggingService;
};

namespace valc
{

class DBConnection;
class ExceptionalDataHandler;
class ResultIndex;
class WorklistEntries;

/*
    The implementation of 'execute' is as follows:
        1) List the IDs of worklist entries that have NOT been loaded but are referenced by worklist entries that HAVE already been loaded.
        2) Insert these IDs into a temporary table
        3) Create a SQL query that joins with this temporary table in order to get this extra worklist data loaded.
        4) Delegate to LoadWorklistEntries to achieve this loading.
        5) Repeat from (1).
*/
class LoadReferencedWorklistEntries : public WorklistDirectory::Func
{
public:
    LoadReferencedWorklistEntries( 
        DBConnection*           con, 
        paulst::LoggingService* log, 
        WorklistEntries*        worklistEntries, 
        ResultIndex*            resultIndex,
        const std::string&      tempTableName,
        const std::string&      worklistSQL,
        const std::string&      worklistRelationSQL,
        ExceptionalDataHandler* exceptionalDataHandler );
    void execute();
    void execute( const WorklistEntry* wle );
private:
    DBConnection*           m_con;
    paulst::LoggingService* m_log;
    WorklistEntries*        m_worklistEntries;
    ResultIndex*            m_resultIndex;
    IntList                 m_idList;
    std::string             m_tempTableName,
                            m_worklistSQL,
                            m_worklistRelationSQL;
    ExceptionalDataHandler* m_exceptionalDataHandler;

    LoadReferencedWorklistEntries( const LoadReferencedWorklistEntries& );
    LoadReferencedWorklistEntries& operator=( const LoadReferencedWorklistEntries& );

    int findReferencedButNotLoaded();
};

};

#endif

