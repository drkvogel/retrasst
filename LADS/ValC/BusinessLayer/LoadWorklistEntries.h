#ifndef LOADWORKLISTENTRIESH
#define LOADWORKLISTENTRIESH

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

    LoadWorklistEntries( 
        /* Collection into which worklist entries will be loaded. May already contain some entries. */
        WorklistEntries* worklistEntries, 
        DBConnection* con, 
        paulst::LoggingService* log, 
        /*  If loaded worklist entries name-check results, then this information will be added to resultIndex. */
        ResultIndex* resultIndex,         
        const std::string& worklistSQL, 
        const std::string& worklistRelationSQL );
    void execute();
private:
    WorklistEntries* m_worklistEntries;
    DBConnection* m_con;
    paulst::LoggingService* m_log;
    ResultIndex* m_resultIndex;
    const std::string m_worklistSQL, m_worklistRelationSQL;
};

};

#endif 

