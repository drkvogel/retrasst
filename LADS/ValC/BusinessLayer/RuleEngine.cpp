#include "AbstractConnectionFactory.h"
#include "AcquireCriticalSection.h"
#include "Config.h"
#include "ExceptionUtil.h"
#include "Gate.h"
#include "Gates.h"
#include "LoggingService.h"
#include "LuaInclude.h"
#include "LuaDB.h"
#include "LuaUtil.h"
#include "Require.h"
#include "RuleEngine.h"
#include "RuleLoaderInterface.h"
#include "ThreadPool.h"
#include <vector>

namespace valc
{

class RuleApplicationFailure
{
public:
    RuleApplicationFailure( const RuleDescriptor& rd, const std::string& errorMsg )
        :
        m_ruleDescriptor( rd ),
        m_errorMsg( errorMsg )
    {
    }

    RuleDescriptor getRuleDescriptor() const { return m_ruleDescriptor; }
    std::string    getMessage()        const { return m_errorMsg;       }
private:
    const RuleDescriptor m_ruleDescriptor;
    const std::string    m_errorMsg;
};

paulstdb::DBConnection* obtainConnectionFromCache( const char* connectionString, void* connectionState )
{
    paulstdb::AbstractConnectionFactory* conFac = (paulstdb::AbstractConnectionFactory*)connectionState;
    return conFac->createConnection( connectionString, "" );
}

ResultAssessmentTask::ResultAssessmentTask( const UncontrolledResult& result, RulesCache* rulesCache, 
    RuleResultPublisher* resultPublisher, paulst::LoggingService* log )
    :
    m_result            ( result ),
    m_rulesCache        ( rulesCache ),
    m_resultPublisher   ( resultPublisher ),
    m_log               ( log )
{
}

void ResultAssessmentTask::doStuff()
{
    RuleResults results, empty;

    try
    {
        Rules* rules = m_rulesCache->getRulesFor( m_result.testID, m_result.machineID, m_result.projectID );
        results  = rules->applyTo( m_result );
    }
    catch( const RuleApplicationFailure& raf )
    {
        results = RuleResults( raf.getRuleDescriptor(), empty.begin(), empty.end(), RESULT_CODE_ERROR, raf.getMessage() );
    }
    catch( const Exception& e )
    {
        results = RuleResults( RuleDescriptor(), empty.begin(), empty.end(), RESULT_CODE_ERROR, AnsiString( e.Message.c_str() ).c_str() );
    }
    catch( ... )
    {
        results = RuleResults( RuleDescriptor(), empty.begin(), empty.end(), RESULT_CODE_ERROR, "Unspecified Exception" );
    }

    m_resultPublisher->publish( results, m_result.resultID );
}

RuleResultPublisher::RuleResultPublisher()
{
}

RuleResultPublisher::~RuleResultPublisher()
{
}



int loadRuleScript( lua_State* L )
{
    try
    {
        std::string ruleName = lua_tostring( L, -1 );

        int index = lua_upvalueindex( 1 );

        RuleLoaderInterface* ruleLoader = (RuleLoaderInterface*) lua_touserdata( L, index );

        std::string script = ruleLoader->loadRulesFor( ruleName );

        lua_pushstring( L, script.c_str() );
    }
    catch( const Exception& e )
    {
        lua_pushstring( L, AnsiString( e.Message.c_str() ).c_str() );
        lua_error( L );
    }
    catch( ... )
    {
        lua_pushstring( L, "Unspecified exception in loadRuleScript" );
        lua_error( L );
    }

    return 1;
}

int luaSleep( lua_State* L )
{
    try
    {
        if ( lua_isnumber( L, -1 ) )
        {
            int msecs = lua_tointeger( L, -1 );
            Sleep(msecs);
        }
    }
    catch( const Exception& e )
    {
        lua_pushstring( L, AnsiString( e.Message.c_str() ).c_str() );
        lua_error( L );
    }
    catch( ... )
    {
        lua_pushstring( L, "Unspecified exception in loadRuleScript" );
        lua_error( L );
    }

    return 0;
}

int luaConfig( lua_State* L )
{
    try
    {
        std::string property = lua_tostring( L, -1 );

        int index = lua_upvalueindex( 1 );

        const paulst::Config* config = (const paulst::Config*) lua_touserdata( L, index );

        require ( config );
    
        if ( config->contains( property ) )
        {
            lua_pushstring( L, config->get( property ).c_str() );
        }
        else
        {
            lua_pushstring( L, paulst::format( "Property '\%s' not found", property.c_str() ).c_str() );
        }
    }
    catch( const Exception& e )
    {
        lua_pushstring( L, AnsiString( e.Message.c_str() ).c_str() );
        lua_error( L );
    }
    catch( ... )
    {
        lua_pushstring( L, "Unspecified exception in luaLogger" );
        lua_error( L );
    }

    return 1;

}

int luaLogger( lua_State* L )
{
    try
    {
        std::string msg = lua_tostring( L, -1 );

        int index = lua_upvalueindex( 1 );

        paulst::LoggingService* logger = (paulst::LoggingService*) lua_touserdata( L, index );

        require ( logger );

        logger->log( msg );
    }
    catch( const Exception& e )
    {
        lua_pushstring( L, AnsiString( e.Message.c_str() ).c_str() );
        lua_error( L );
    }
    catch( ... )
    {
        lua_pushstring( L, "Unspecified exception in luaLogger" );
        lua_error( L );
    }

    return 0;
}

int luaGetActiveGate( lua_State* L )
{
    try
    {
        int index = lua_upvalueindex( 1 );

        Gates* gates = (Gates*) lua_touserdata( L, index );

        require( gates );

        UncontrolledResult r = popUncontrolledResult( L, -1 );

        const Gate* gate = gates->getActiveGateFor( r );

        if ( gate )
        {
            lua_pushGate( L, *gate );
        }
        else
        {
            lua_pushnil( L );
        }
    }
    catch( const Exception& e )
    {
        lua_pushstring( L, AnsiString( e.Message.c_str() ).c_str() );
        lua_error( L );
    }
    catch( ... )
    {
        lua_pushstring( L, "Unspecified exception in luaLogger" );
        lua_error( L );
    }

    return 1;
}

/*
    Construction of rules is one-at-a-time because they are instantiated by RulesCache::Load, which is invoked in the 
    context of a critical section.
*/
Rules::Rules( const RuleDescriptor& rd, const std::string& script, ConnectionFactory cf, void* connectionState, RuleLoaderInterface* ruleLoader,
    paulst::LoggingService* log, Gates* gates, int test, int machine, int project, const paulst::Config* config )
    : 
    L( lua_open() ),
    m_connectionFactory( cf ),
    m_connectionState( connectionState ),
    m_log(log),
    m_ruleDescriptor( rd ),
    m_gates( gates )
{
    luaL_openlibs( L );

    class LuaError {};

    try
    {
        require( log );

        LuaDB luaDB( L, m_connectionFactory, m_connectionState );

        if ( luaL_dostring( L, script.c_str() ) )
        {
            throw LuaError();
        }

        lua_getglobal( L, "onLoad" );

        lua_pushlightuserdata( L, ruleLoader );

        lua_pushcclosure( L, loadRuleScript, 1 );

        lua_pushlightuserdata( L, log );

        lua_pushcclosure( L, luaLogger, 1 );

        lua_setglobal( L, "log" );

        lua_pushlightuserdata( L, gates );

        lua_pushcclosure( L, luaGetActiveGate, 1 );

        lua_setglobal( L, "getActiveGate" );

        lua_pushlightuserdata( L, (void*)config );

        lua_pushcclosure( L, luaConfig, 1 );

        lua_setglobal( L, "config" );

        lua_pushcfunction( L, luaSleep );

        lua_setglobal( L, "sleep" );

        const int integerConstants[] = { test, machine, project, RESULT_CODE_FAIL, RESULT_CODE_PASS,
            RESULT_CODE_BORDERLINE, RESULT_CODE_ERROR, RESULT_CODE_NO_RULES_APPLIED, RESULT_CODE_NULL };

        const std::string constantNames[] = { "TEST_ID", "MACHINE_ID", "PROJECT_ID", "FAIL", "PASS",
            "BORDERLINE", "ERROR", "NO_RULES_APPLIED", "NULL" };

        const int numConstants = 9;

        for ( int i = 0; i < numConstants; ++i )
        {
            lua_pushinteger( L, integerConstants[i] );

            lua_setglobal  ( L, constantNames[i].c_str() );
        }

        if ( lua_pcall( L, 1, 0, 0 ) )
        {
            throw LuaError();
        }
    }
    catch( const LuaError& luaError )
    {
        std::string errorMsg(  lua_tostring( L, -1 ) );
        lua_close(L);
        m_log->logFormatted( "Error: \%s", errorMsg.c_str() );
        throw Exception( UnicodeString( errorMsg.c_str() )  + UnicodeString( script.c_str() ) );
    }
}

Rules::~Rules()
{
    lua_close(L);
}


RuleResults Rules::applyTo( const UncontrolledResult& r )
{
    try
    {
        if ( m_log )
            m_log->logFormatted("Waiting to apply rules to result %d", r.resultID );

        paulst::AcquireCriticalSection a(m_critSec);

        {
            if ( m_log )
                m_log->logFormatted("Applying rules to result %d", r.resultID );

            LuaDB luaDB( L, m_connectionFactory, m_connectionState );

            lua_getglobal( L, "applyRules" );

            lua_pushUncontrolledResult( L, r );

            const Gate* gate = m_gates ? m_gates->getActiveGateFor( r ) : NULL;

            if ( gate )
            {
                lua_pushGate( L, *gate );
            }
            else
            {
                lua_pushnil( L );
            }
            
            if ( lua_pcall( L, 2, 1, 0 ) )
            {
                throw Exception( lua_tostring( L, -1 ) );
            }

            require( lua_istable( L, -1 ) );

            const int numTableValues = lua_rawlen( L, -1 );

            if ( numTableValues < 3 )
            {
                paulst::exception( "Only \%d values in the table returned from LUA. Wanted a minimum of 3.", numTableValues );
            }

            // First is an array of raw results.
            // Each raw result is a table consisting of 3 values: resultCode, rule, msg
            std::vector<RuleResult> rawResults;

            lua_rawgeti( L, -1, 1 );

            require( lua_istable( L, -1 ) );// (In Lua, arrays are tables.)

            const int arraySize = lua_rawlen( L, -1 );
            for ( int i = 1; i <= arraySize; ++i )
            {
                lua_rawgeti( L, -1, i );
                require( lua_istable( L, -1 ) );
                RuleResult rawResult;
                // Top of the stack is now a raw result table
                // Now to retrieve each of 'resultCode', 'rule' and 'msg'
                rawResult.resultCode = static_cast<ResultCode>(
                                       retrieveTableValue<int>        ( L, -1, "resultCode", lua_isnumber, lua_toInteger ) ); 
                rawResult.rule       = retrieveTableValue<std::string>( L, -1, "rule"      , lua_isstring, lua_toString  ); 
                rawResult.msg        = retrieveTableValue<std::string>( L, -1, "msg"       , lua_isstring, lua_toString  ); 

                rawResults.push_back( rawResult );

                lua_pop( L, 1 );
            }

            lua_pop( L, 1 );

            // next is the summary message
            lua_rawgeti( L, -1, 2 );

            require( lua_isstring( L, -1 ) );

            std::string summaryMsg = lua_tostring( L, -1 );

            lua_pop( L, 1 );

            // next is the summary result code
            lua_rawgeti( L, -1, 3 );

            require( lua_isnumber( L, -1 ) );

            ResultCode summaryResultCode = static_cast<ResultCode>(lua_tointeger( L, -1 ));

            lua_pop( L, 1 );

            std::vector< std::string > extraValues;

            for ( int i = 4; i <= numTableValues; ++i )
            {
                lua_rawgeti( L, -1, i );
                extraValues.push_back( lua_tostring( L, -1 ) );
                lua_pop( L, 1 );
            }

            if ( m_log )
                m_log->logFormatted( "Obtained %d after applying rules to result %d", summaryResultCode, r.resultID );

            return RuleResults( m_ruleDescriptor, rawResults.begin(), rawResults.end(), summaryResultCode, summaryMsg, extraValues );
        }
    }
    catch( const Exception& e )
    {
        RuleApplicationFailure failure( m_ruleDescriptor, AnsiString( e.Message.c_str() ).c_str() );
        if ( m_log )
            m_log->logFormatted( "Failed to apply rules for result \%d: \%s", r.resultID, failure.getMessage().c_str() );
        throw failure;
    }
    catch( ... )
    {
        if ( m_log )
            m_log->logFormatted( "Unspecified failure when applying rules for result \%d.", r.resultID );
        throw RuleApplicationFailure( m_ruleDescriptor, "Unspecified Exception" );
    }
}
    
RulesConfig::RulesConfig()
{
}

RulesConfig::~RulesConfig()
{
}

RulesCache::RulesCache()
    :
    m_rulesConfig(0),
    m_ruleLoader(0),
    m_connectionFactory(0),
    m_log(0),
    m_gates(0),
    m_config(0)
{
}

RulesCache::~RulesCache()
{
    clear();
}

void RulesCache::clear()
{
    paulst::AcquireCriticalSection a(m_critSec);
    
    {

        for ( Cache::const_iterator i = m_cache.begin(); i != m_cache.end(); ++i )
        {
            Rules* rules = i->second;
            delete rules;
        }

        m_cache.clear();
    }
}

Rules* RulesCache::getRulesFor( int test, int machine, int project )
{
    paulst::AcquireCriticalSection a(m_critSec);
    
    {
        Rules* rules = 0;

        int ruleID = m_rulesConfig->getRuleIDFor( test, machine, project );

        Cache::const_iterator i = m_cache.find( ruleID );

        if ( i == m_cache.end() )
        {
            rules = load( ruleID, test, machine, project );
            m_cache.insert( std::make_pair( ruleID, rules ) );
        }
        else
        {
            rules = i->second;
        }

        return rules;
    }
}

Rules* RulesCache::load( int ruleID, int test, int machine, int project )
{
    m_log->logFormatted( "Loading script for rule ID '\%d'", ruleID );
    std::string script = m_ruleLoader->loadRulesFor( ruleID );
    return new Rules( 
        m_ruleLoader->getRuleDescriptor( ruleID ), script, obtainConnectionFromCache, m_connectionFactory, m_ruleLoader, m_log, m_gates,
                            test, machine, project, m_config );
}

void RulesCache::setConfig( const paulst::Config* c )
{
    m_config = c;
}

void RulesCache::setConnectionFactory( paulstdb::AbstractConnectionFactory* cc )
{
    m_connectionFactory = cc;
}

void RulesCache::setGates( Gates* g )
{
    m_gates = g;
}

void RulesCache::setLog( paulst::LoggingService* log )
{
    m_log = log;
}

void RulesCache::setRulesConfig( RulesConfig* c )
{
    m_rulesConfig = c;
}

void RulesCache::setRuleLoader( RuleLoaderInterface* l )
{
    m_ruleLoader = l;
}

RuleEngineQueueListener::~RuleEngineQueueListener()
{
}

RuleEngine::RuleEngine( int maxThreads)
    :
    m_publisher(0),
    m_resultAssessor( new stef::ThreadPool( 0, maxThreads ) ),
    m_queueListener(0)
{
}

RuleEngine::~RuleEngine()
{
    m_resultAssessor->shutdown( 1000, true );
}

void RuleEngine::clearRulesCache()
{
    m_rulesCache.clear();
}
    
void RuleEngine::queue( const UncontrolledResult& r, const IDToken& runID )
{
    // Notify BEFORE queueing.  Don't want any risk of notification of result to precede notification of being queued.
    if ( m_queueListener )
    {
        m_queueListener->notifyQueued( r, runID );
    }

    m_resultAssessor->addTask( new ResultAssessmentTask( r, &m_rulesCache, m_publisher, m_log ) );
}

void RuleEngine::setConfig( const paulst::Config* c )
{
    m_rulesCache.setConfig( c );
}

void RuleEngine::setConnectionFactory( paulstdb::AbstractConnectionFactory* conFac )
{
    m_rulesCache.setConnectionFactory( conFac );
}

void RuleEngine::setDefaultTaskExceptionHandler( stef::TaskExceptionHandler* teh )
{
    m_resultAssessor->addDefaultTaskExceptionHandler( teh );
}

void RuleEngine::setGates( Gates* g )
{
    m_rulesCache.setGates( g );
}

void RuleEngine::setLog( paulst::LoggingService* l )
{
    m_rulesCache.setLog( l );
    m_log = l;
}

void RuleEngine::setQueueListener( RuleEngineQueueListener* l )
{
    m_queueListener = l;
}

void RuleEngine::setRulesConfig( RulesConfig* c )
{
    m_rulesCache.setRulesConfig( c );
}

void RuleEngine::setRuleLoader( RuleLoaderInterface* l )
{
    m_rulesCache.setRuleLoader( l );
}

void RuleEngine::setResultPublisher( RuleResultPublisher* p )
{
    m_publisher = p;
}

bool RuleEngine::waitForQueued( long timeoutMillis )
{
    return m_resultAssessor->waitTillQuiet( timeoutMillis );
}

}

