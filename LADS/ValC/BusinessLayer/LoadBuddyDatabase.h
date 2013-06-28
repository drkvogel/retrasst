#ifndef LOADBUDDYDATABASEH
#define LOADBUDDYDATABASEH

#include <memory>

namespace paulst
{
    class LoggingService;
};

namespace valc
{

class BuddyDatabase;
class DBConnection;
class Projects;
class ResultIndex;
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
    LoadBuddyDatabase( int localMachineID, DBConnection* con, paulst::LoggingService* log, 
        ResultIndex* resultIndex, Projects* projects );
    void execute();
    BuddyDatabase* releaseReturnValue();
private:
    std::auto_ptr<BuddyDatabase>  m_buddyDatabase;
    const int       m_localMachineID;
    const Projects* m_projects;
    DBConnection*   m_con;
    ResultIndex*    m_resultIndex;
    paulst::LoggingService* m_log;
};

};

#endif

