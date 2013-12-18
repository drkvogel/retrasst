#ifndef LOADWORKLISTENTRIESH
#define LOADWORKLISTENTRIESH

#include "Task.h"

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

class DBConnection;
class ExceptionalDataHandler;
class QCSampleDescriptorDerivationStrategy;
class ResultIndex;
class WorklistEntries;

class LoadWorklistEntries : public stef::Task
{
public:

    LoadWorklistEntries( 
        /* Collection into which worklist entries will be loaded. May already contain some entries. */
        WorklistEntries* worklistEntries, 
        paulstdb::DBConnection* con, 
        paulst::LoggingService* log, 
        /*  If loaded worklist entries name-check results, then this information will be added to resultIndex. */
        ResultIndex* resultIndex,         
        const std::string& worklistSQL, 
        const std::string& inclusionRule,
        ExceptionalDataHandler* exceptionalDataHandler,
        QCSampleDescriptorDerivationStrategy* qcsdds );
protected:
    void doStuff();
private:
    WorklistEntries* m_worklistEntries;
    paulstdb::DBConnection* m_con;
    paulst::LoggingService* m_log;
    ResultIndex* m_resultIndex;
    const std::string   m_worklistSQL, 
                        m_inclusionRule;
    ExceptionalDataHandler* m_exceptionalDataHandler;
    QCSampleDescriptorDerivationStrategy* m_QCSampleDescriptorDerivationStrategy;
};

};

#endif 

