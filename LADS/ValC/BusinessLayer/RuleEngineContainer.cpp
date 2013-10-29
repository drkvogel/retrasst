#include "Config.h"
#include "DBQueryTask.h"
#include "ResultAttributes.h"
#include "RuleEngineContainer.h"
#include "RulesConfigInitialiser.h"
#include "RuleLoaderInitialiser.h"
#include "STEF.h"
#include "StrUtil.h"

namespace valc
{

RuleEngineContainer::RuleEngineContainer( 
    stef::SerialTaskExecutionFramework* stef, 
    const paulst::Config* config,
    paulstdb::AbstractConnectionFactory* connectionFactory,
    paulst::LoggingService* log,
    ResultAttributes* resultAttributes )
    :
    m_ruleEngine( 10 ),
    m_log( log ),
    m_resultAttributes( resultAttributes )
{
    m_ruleEngine.setConnectionFactory( connectionFactory );
    m_ruleEngine.setErrorResultCode( paulst::toInt( config->get("RuleEngineErrorCode") ) );
    m_ruleEngine.setLog( log );
    m_ruleEngine.setRuleLoader( &m_ruleLoader );
    m_ruleEngine.setRulesConfig( &m_rulesConfig );
    m_ruleEngine.setResultPublisher( resultAttributes );

    DBQueryTask* task = new DBQueryTask( "QC Rule Config" );
    task->setConnectionFactory( connectionFactory );
    task->setConnectionString( config->get("QCRuleConfigConnectionString") );
    task->setSessionReadLockSetting( config->get("QCRuleConfigConnectionSessionReadLockSetting") );
    RulesConfigInitialiser* configInitialiser = new RulesConfigInitialiser( config->get("LoadRuleConfig") );
    configInitialiser->setInitialisationTarget( &m_rulesConfig ); 
    task->setCursorConsumer( configInitialiser, true );
    stef->addTask( task );

    task = new DBQueryTask( "QC Rule Scripts" );
    task->setConnectionFactory( connectionFactory );
    task->setConnectionString( config->get("QCRuleConfigConnectionString") );
    task->setSessionReadLockSetting( config->get("QCRuleConfigConnectionSessionReadLockSetting") );
    RuleLoaderInitialiser* loaderInitialiser = new RuleLoaderInitialiser( config->get("LoadRules") );
    loaderInitialiser->setInitialisationTarget( &m_ruleLoader );
    task->setCursorConsumer( loaderInitialiser, true );
    stef->addTask( task );
}

void RuleEngineContainer::clearRulesCache()
{
    m_ruleEngine.clearRulesCache();
}

bool RuleEngineContainer::queue( const UncontrolledResult& r )
{
    bool queued = false;

    do
    {
        if ( ! m_rulesConfig.isConfigured( r ) )
        {
            break;
        }

        m_ruleEngine.queue( r );

        queued = true;
    }
    while( false );

    return queued;
}

void RuleEngineContainer::waitForQueued()
{
    m_ruleEngine.waitForQueued();
}

}

