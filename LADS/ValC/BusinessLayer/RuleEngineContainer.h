#ifndef RULEENGINECONTAINERH
#define RULEENGINECONTAINERH

#include "RuleEngine.h"
#include "RuleLoader.h"
#include "RulesConfigUsingMap.h"

namespace paulstdb
{
    class AbstractConnectionFactory;
}

namespace paulst
{
    class Config;
    class LoggingService;
}

namespace stef
{
    class TaskExceptionHandler;
    class ThreadPool;
}

namespace valc
{

class Gates;

class RuleEngineContainer
{
public:
    RuleEngineContainer( 
        stef::ThreadPool* dbQueryRunner,
        const paulst::Config* config,
        paulstdb::AbstractConnectionFactory* connectionFactory,
        paulst::LoggingService* log,
        RuleResultPublisher* resultPublisher,
        Gates* gates,
        stef::TaskExceptionHandler* defaultTaskExceptionHandler );
	void clearRulesCache();
    bool queue( const UncontrolledResult& r );
    bool waitForQueued(long millis);
private:
    RuleEngine              m_ruleEngine;
    RulesConfigUsingMap     m_rulesConfig;
    RuleLoader              m_ruleLoader;
    paulst::LoggingService* m_log;
};

};

#endif

