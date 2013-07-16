#ifndef LOADTESTNAMESH
#define LOADTESTNAMESH

#include "TestNames.h"

namespace paulst
{
    class LoggingService;
};

namespace valc
{

class DBConnection;

class LoadTestNames
{
public:
    LoadTestNames( TestNames* testNames, paulst::LoggingService* log, DBConnection* con );
    ~LoadTestNames();
    void execute();
    int releaseReturnValue();
private:
    paulst::LoggingService* m_log;
    DBConnection* m_con;
    TestNames* m_testNames;

    LoadTestNames( const LoadTestNames& );
    LoadTestNames& operator=( const LoadTestNames& );
};

};

#endif

