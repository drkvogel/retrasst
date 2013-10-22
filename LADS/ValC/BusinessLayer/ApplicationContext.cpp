#include "AbstractConnectionFactory.h"
#include "ApplicationContext.h"
#include "AsyncInitialisationMonitor.h"
#include "Config.h"
#include "ResultAttributes.h"
#include "RuleEngineContainer.h"
#include "STEF.h"

namespace valc
{

ApplicationContext::ApplicationContext()
    : 
    snapshot(0),
    localMachineID(0),
    user(0),
    config(0),
    log(0),
    connectionFactory(0),
    asyncInitialisationMonitor(0),
    asyncInitialisationTaskList(0),
    resultAttributes(0),
    ruleEngineContainer(0)
{
}

ApplicationContext::~ApplicationContext()
{
    delete snapshot;
    delete config;
    delete connectionFactory;
    delete asyncInitialisationMonitor;
    delete asyncInitialisationTaskList;
    delete resultAttributes;
    delete ruleEngineContainer;
}

std::string ApplicationContext::getProperty( const std::string& name) const
{
    return config->get(name);
}

}

