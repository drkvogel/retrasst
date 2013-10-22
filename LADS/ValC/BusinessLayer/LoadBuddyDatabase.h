#ifndef LOADBUDDYDATABASEH
#define LOADBUDDYDATABASEH

#include <memory>

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

class BuddyDatabase;
class DBUpdateSchedule;
class ExceptionalDataHandler;
class Projects;
class ResultIndex;
class RuleEngineContainer;
class SampleRunIDResolutionService;
/*  Queries for local buddy_database entries, joined with sample_run and with buddy_result_float. These joins are OUTER joins. Thus 
    a row will be obtained for each local buddy_database entry, regardless of whether that entry can be joined to a row in sample_run or rows 
    in buddy_result_float.

    In fact uses buddy_result_float_valc, not buddy_result_float. Thus only picks up results with an action_flag value of '0' or 'X'.

    The return value, following the invocation of 'execute', is a new instance of BuddyDatabase. TestResult instances may also have been
    added to resultIndex.
*/
class LoadBuddyDatabase
{
public:
    LoadBuddyDatabase( int localMachineID, paulstdb::DBConnection* con, paulst::LoggingService* log, 
		ResultIndex* resultIndex, Projects* projects, BuddyDatabase** out, DBUpdateSchedule* dbUpdateSchedule,
        SampleRunIDResolutionService* sampleRunIDResolutionService, const std::string& sql,
        const std::string& inclusionRule, ExceptionalDataHandler* exceptionalDataHandler,
        RuleEngineContainer* ruleEngine );
	void execute();
private:
    BuddyDatabase**                 m_buddyDatabase;
    const int                       m_localMachineID;
    const Projects*                 m_projects;
    paulstdb::DBConnection*         m_con;
    ResultIndex*                    m_resultIndex;
    paulst::LoggingService*         m_log;
    DBUpdateSchedule*               m_dbUpdateSchedule;
    SampleRunIDResolutionService*   m_sampleRunIDResolutionService;
    const std::string               m_sql;
    const std::string               m_inclusionRule;
    ExceptionalDataHandler*         m_exceptionalDataHandler;
    RuleEngineContainer*            m_ruleEngine;
};

};

#endif

