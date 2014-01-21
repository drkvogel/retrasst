#include "AbstractConnectionFactory.h"
#include "ApplicationContext.h"
#include "CompositeRuleEngineQueueListener.h"
#include "CompositeRuleResultPublisher.h"
#include "Config.h"
#include "ControlModel.h"
#include "Projects.h"
#include "QCGates.h"
#include "ResultAttributes.h"
#include "RuleEngineContainer.h"
#include "SampleRunIDResolutionService.h"
#include "Task.h"
#include "ThreadPool.h"

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
    initialisationQueries(0),
    resultAttributes(0),
    ruleEngineContainer(0),
    qcGates(0),
    projects(0),
    clusterIDs(0),
    testNames(0),
    sampleRunIDResolutionService(0),
    taskExceptionUserAdvisor(0),
    compositeRuleResultPublisher(0),
    controlModel(0),
    compositeRuleEngineQueueListener(0),
    controlModelQueueListenerAdapter(0),
    controlModelResultPublisherAdapter(0)
{
}

ApplicationContext::~ApplicationContext()
{
    if ( initialisationQueries )
    {
        initialisationQueries->shutdown( 2000, true );
    }

    delete snapshot;
    delete config;
    delete connectionFactory;
    delete clusterIDs;
    delete projects;
    delete qcGates;
    delete resultAttributes;
    delete ruleEngineContainer;
    delete testNames;
    delete sampleRunIDResolutionService;
    delete taskExceptionUserAdvisor;
    delete compositeRuleResultPublisher;
    delete compositeRuleEngineQueueListener;
    delete controlModel;
    delete controlModelQueueListenerAdapter;
    delete controlModelResultPublisherAdapter;
}

std::string ApplicationContext::getProperty( const std::string& name) const
{
    return config->get(name);
}

}

