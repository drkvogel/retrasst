#ifndef APPLICATIONCONTEXTH
#define APPLICATIONCONTEXTH

#include <string>

namespace paulst
{
    class Config;
    class LoggingService;
}

namespace paulstdb
{
    class AbstractConnectionFactory;
}

namespace stef
{
    class SerialTaskExecutionFramework;
}

namespace valc
{

class AnalysisActivitySnapshot;
class AsyncInitialisationMonitor;
class ResultAttributes;
class RuleEngineContainer;


struct ApplicationContext
{
    AnalysisActivitySnapshot*               snapshot;
    int                                     localMachineID;
    int                                     user;
    paulst::Config*                         config;
    paulst::LoggingService*                 log;
    paulstdb::AbstractConnectionFactory*    connectionFactory;
    AsyncInitialisationMonitor*             asyncInitialisationMonitor;
    stef::SerialTaskExecutionFramework*     asyncInitialisationTaskList;
    ResultAttributes*                       resultAttributes;
    RuleEngineContainer*                    ruleEngineContainer;

    ApplicationContext();
    ~ApplicationContext();
    std::string getProperty( const std::string& name) const;
    

private:
    ApplicationContext( const ApplicationContext& );
    ApplicationContext& operator=( const ApplicationContext& );
}; 

}

#endif

