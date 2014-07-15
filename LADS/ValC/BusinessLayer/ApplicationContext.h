#ifndef APPLICATIONCONTEXTH
#define APPLICATIONCONTEXTH

#include "ClusterIDs.h"
#include <string>
#include "TestNames.h"

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
    class TaskExceptionHandler;
    class ThreadPool;
}

namespace valc
{

class AnalysisActivitySnapshot;
class CompositeRuleEngineQueueListener;
class CompositeRuleResultPublisher;
class ControlModel;
class DBTransactionHandler;
class Projects;
class QCGates;
class ResultAttributes;
class RuleEngineContainer;
class RuleEngineQueueListener;
class RuleResultPublisher;
class SampleRunIDResolutionService;
class UserAdvisor;


struct ApplicationContext
{
    AnalysisActivitySnapshot*                       snapshot;
    int                                             localMachineID;
    int                                             user;
    paulst::Config*                                 config;
    paulst::LoggingService*                         log; // Owned by UI
    paulstdb::AbstractConnectionFactory*            connectionFactory;
    stef::ThreadPool*                               initialisationQueries;
    ResultAttributes*                               resultAttributes;
    RuleEngineContainer*                            ruleEngineContainer;
    QCGates*                                        qcGates;
    Projects*                                       projects;
    ClusterIDs*                                     clusterIDs;
    TestNames*                                      testNames;
    UserAdvisor*                                    userAdvisor; // Owned by UI
    SampleRunIDResolutionService*                   sampleRunIDResolutionService;
    stef::TaskExceptionHandler*                     taskExceptionUserAdvisor;
    CompositeRuleResultPublisher*                   compositeRuleResultPublisher;
    ControlModel*                                   controlModel;
    CompositeRuleEngineQueueListener*               compositeRuleEngineQueueListener;
    RuleEngineQueueListener*                        controlModelQueueListenerAdapter;
    RuleResultPublisher*                            controlModelResultPublisherAdapter;
    DBTransactionHandler*                           dbTransactionHandler;


    ApplicationContext();
    ~ApplicationContext();
    std::string getProperty( const std::string& name) const;
    

private:
    ApplicationContext( const ApplicationContext& );
    ApplicationContext& operator=( const ApplicationContext& );
}; 

}

#endif

