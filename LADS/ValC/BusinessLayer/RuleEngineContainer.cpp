#include <boost/bind.hpp>
#include "Config.h"
#include "DBQueryTask.h"
#include "ResultAttributes.h"
#include "RuleEngineContainer.h"
#include "StrUtil.h"
#include "ThreadPool.h"

namespace valc
{

void addConfig( paulstdb::Cursor* c, RulesConfigUsingMap* rulesConfig )
{
    const int         test     = paulstdb::read<int>        ( *c, 0 );
    const int         machine  = paulstdb::read<int>        ( *c, 1 );
    const int         project  = paulstdb::read<int>        ( *c, 2 );
    const int         ruleID   = paulstdb::read<int>        ( *c, 3 );
    rulesConfig->specify( ruleID, test, machine, project );
}

void addRule( paulstdb::Cursor* c, RuleLoader* ruleLoader )
{
    const int         recordID   = paulstdb::read<int>( *c, 0 );
    const int         ruleID     = paulstdb::read<int>( *c, 1 );
    const std::string uniqueName = paulstdb::read<std::string>( *c, 2 );
    const std::string desc       = paulstdb::read<std::string>( *c, 3 );
    const std::string script     = paulstdb::read<std::string>( *c, 4 );
    ruleLoader->add( RuleDescriptor( recordID, ruleID, uniqueName, desc ), script );
}

RuleEngineContainer::RuleEngineContainer( 
    stef::ThreadPool* dbQueryRunner,
    const paulst::Config* config,
    paulstdb::AbstractConnectionFactory* connectionFactory,
    paulst::LoggingService* log,
    ResultAttributes* resultAttributes,
    Gates* gates,
    stef::TaskExceptionHandler* defaultTaskExceptionHandler )
    :
    m_ruleEngine( 10 ),
    m_log( log ),
    m_resultAttributes( resultAttributes )
{
    m_ruleEngine.setConfig( config );
    m_ruleEngine.setConnectionFactory( connectionFactory );
    m_ruleEngine.setDefaultTaskExceptionHandler( defaultTaskExceptionHandler );
    m_ruleEngine.setErrorResultCode( paulst::toInt( config->get("RuleEngineErrorCode") ) );
    m_ruleEngine.setGates( gates );
    m_ruleEngine.setLog( log );
    m_ruleEngine.setRuleLoader( &m_ruleLoader );
    m_ruleEngine.setRulesConfig( &m_rulesConfig );
    m_ruleEngine.setResultPublisher( resultAttributes );

    dbQueryRunner->addTask( new DBQueryTask( "QCRuleConfig", connectionFactory, config, 
        boost::bind( addConfig, _1, &m_rulesConfig ) ) ); 
    dbQueryRunner->addTask( new DBQueryTask( "QCRule"      , connectionFactory, config, 
        boost::bind( addRule, _1, &m_ruleLoader ) ) ); 
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

bool RuleEngineContainer::waitForQueued(long millis)
{
    return m_ruleEngine.waitForQueued( millis );
}

}

