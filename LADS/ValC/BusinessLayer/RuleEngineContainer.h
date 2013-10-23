#ifndef RULEENGINECONTAINERH
#define RULEENGINECONTAINERH

#include "RuleEngine.h"
#include "RulesConfigUsingMap.h"
#include "RuleLoaderUsingMap.h"

namespace stef
{
    class SerialTaskExecutionFramework;
}

namespace paulstdb
{
    class AbstractConnectionFactory;
}

namespace paulst
{
    class Config;
    class LoggingService;
}

namespace valc
{

class ResultAttributes;

class RuleEngineContainer
{
public:
    RuleEngineContainer( 
        stef::SerialTaskExecutionFramework* stef, 
        const paulst::Config* config,
        paulstdb::AbstractConnectionFactory* connectionFactory,
        paulst::LoggingService* log,
        ResultAttributes* resultAttributes );
	void clearRulesCache();
    bool queue( const UncontrolledResult& r );
    void waitForQueued();
private:
    RuleEngine              m_ruleEngine;
    RulesConfigUsingMap     m_rulesConfig;
    RuleLoaderUsingMap      m_ruleLoader;
    paulst::LoggingService* m_log;
    ResultAttributes*       m_resultAttributes;
};

};

#endif

