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

namespace valc
{

class AnalysisActivitySnapshot;


struct ApplicationContext
{
    AnalysisActivitySnapshot*               snapshot;
    int                                     localMachineID;
    int                                     user;
    paulst::Config*                         config;
    paulst::LoggingService*                 log;
    paulstdb::AbstractConnectionFactory*    connectionFactory;

    ApplicationContext();
    ~ApplicationContext();
    std::string getProperty( const std::string& name) const;
    

private:
    ApplicationContext( const ApplicationContext& );
    ApplicationContext& operator=( const ApplicationContext& );
}; 

}

#endif

