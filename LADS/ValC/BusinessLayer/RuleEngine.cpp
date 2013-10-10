#include "AcquireCriticalSection.h"
#include "LoggingService.h"
#include "LuaInclude.h"
#include "LuaDB.h"
#include "LuaUtil.h"
#include "Require.h"
#include "RuleEngine.h"
#include <vector>

namespace valc
{

paulstdb::DBConnection* obtainConnectionFromCache( const char* connectionString, void* connectionState )
{
    ConnectionCache* cache = (ConnectionCache*)connectionState;
    require( cache );
    return cache->getConnection( connectionString );
}

void runTask( PTP_CALLBACK_INSTANCE instance, void* context, PTP_WORK work )
{
    RuleEngine::ThreadTaskContext* c = (RuleEngine::ThreadTaskContext*)context;

    UncontrolledResult uncontrolledResult;

    try
    {
        uncontrolledResult = c->nextQueuedResult();
    }
    catch( const Exception& e )
    {
        c->reportException( e );
    }
    catch( ... )
    {
        c->reportUnspecifiedException();
    }

    RuleResults results;

    try
    {
        Rules* rules = c->getRulesFor( uncontrolledResult.testID, uncontrolledResult.machineID );
        results  = rules->applyTo( uncontrolledResult );
    }
    catch( const Exception& e )
    {
        RuleResults empty;
        results = RuleResults( empty.begin(), empty.end(), c->getErrorResultCode(), AnsiString( e.Message.c_str() ).c_str() );
    }
    catch( ... )
    {
        RuleResults empty;
        results = RuleResults( empty.begin(), empty.end(), c->getErrorResultCode(), "Unspecified Exception" );
    }

    c->publishResults( results, uncontrolledResult.resultID );
}

RuleLoader::RuleLoader()
{
}

RuleLoader::~RuleLoader()
{
}


RuleResultPublisher::RuleResultPublisher()
{
}

RuleResultPublisher::~RuleResultPublisher()
{
}

UncontrolledResult::UncontrolledResult()
    :
    testID(0),
    resultID(0),
    machineID(0),
    resultValue(0),
    projectID(0),
    dateAnalysed(1977, 1, 1, 1, 1, 1, 1)
{
}

int loadRuleScript( lua_State* L )
{
    try
    {
        std::string ruleName = lua_tostring( L, -1 );

        int index = lua_upvalueindex( 1 );

        RuleLoader* ruleLoader = (RuleLoader*) lua_touserdata( L, index );

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
        lua_pushstring( L, "Unspecified exception in loadRuleScript" );
        lua_error( L );
    }

    return 0;
}


Rules::Rules( const std::string& script, ConnectionFactory cf, void* connectionState, RuleLoader* ruleLoader,
    paulst::LoggingService* log )
    : 
    L( lua_open() ),
    m_connectionFactory( cf ),
    m_connectionState( connectionState ),
    m_log(log)
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

        lua_pushcfunction( L, luaSleep );

        if ( lua_pcall( L, 3, 0, 0 ) )
        {
            throw LuaError();
        }
    }
    catch( const LuaError& luaError )
    {
        std::string errorMsg(  lua_tostring( L, -1 ) );
        lua_close(L);
        throw Exception( UnicodeString( errorMsg.c_str() )  + UnicodeString( script.c_str() ) );
    }
}

Rules::~Rules()
{
    lua_close(L);
}

void lua_pushUncontrolledResult( lua_State* L, const UncontrolledResult& r )
{
    lua_newtable( L );
    lua_pushinteger( L,    r.testID  );
    lua_setfield   ( L, -2, "testID" );
    lua_pushinteger( L,    r.resultID  );
    lua_setfield   ( L, -2, "resultID" );
    lua_pushinteger( L,    r.machineID  );
    lua_setfield   ( L, -2, "machineID" );
    lua_pushinteger( L,    r.projectID  );
    lua_setfield   ( L, -2, "projectID" );
    lua_pushnumber ( L,    r.resultValue );
    lua_setfield   ( L, -2, "resultValue" );
    lua_pushstring ( L,    r.resultText.c_str() );
    lua_setfield   ( L, -2, "resultText" );
    lua_pushstring ( L,    r.barcode.c_str() );
    lua_setfield   ( L, -2, "barcode" );
    
    unsigned short year, month, day, hour, min, sec, msec;

    r.dateAnalysed.DecodeDate( &year, &month, &day );
    r.dateAnalysed.DecodeTime( &hour, &min, &sec, &msec );

    lua_newtable( L );
    lua_pushinteger( L,      year );
    lua_setfield   ( L, -2, "year" );
    lua_pushinteger( L,      month );
    lua_setfield   ( L, -2, "month" );
    lua_pushinteger( L,      day );
    lua_setfield   ( L, -2, "day" );
    lua_pushinteger( L,      hour );
    lua_setfield   ( L, -2, "hour" );
    lua_pushinteger( L,      min );
    lua_setfield   ( L, -2, "min" );
    lua_pushinteger( L,      sec );
    lua_setfield   ( L, -2, "sec" );
    lua_pushinteger( L,      msec );
    lua_setfield   ( L, -2, "msec" );
    lua_setfield   ( L, -2, "dateAnalysed" );
}

RuleResults Rules::applyTo( const UncontrolledResult& r )
{
    paulst::AcquireCriticalSection a(m_critSec);

    {
        if ( m_log )
            m_log->logFormatted("Applying rules to result %d", r.resultID );

        LuaDB luaDB( L, m_connectionFactory, m_connectionState );

        lua_getglobal( L, "applyRules" );

        lua_pushUncontrolledResult( L, r );
        
        if ( lua_pcall( L, 1, 3, 0 ) )
        {
            throw Exception( lua_tostring( L, -1 ) );
        }

        // top of the stack is the summary result code
        require( lua_isnumber( L, -1 ) );

        int summaryResultCode = lua_tointeger( L, -1 );

        lua_pop( L, 1 );

        // next is the summary message
        require( lua_isstring( L, -1 ) );

        std::string summaryMsg = lua_tostring( L, -1 );

        lua_pop( L, 1 );

        // Next is an array of raw results.
        // Each raw result is a table consisting of 3 values: resultCode, rule, msg
        std::vector<RuleResult> rawResults;

        require( lua_istable( L, -1 ) );// (In Lua, arrays are tables.)
        const int arraySize = lua_rawlen( L, -1 );
        for ( int i = 1; i <= arraySize; ++i )
        {
            lua_rawgeti( L, -1, i );
            require( lua_istable( L, -1 ) );
            RuleResult rawResult;
            // Top of the stack is now a raw result table
            // Now to retrieve each of 'resultCode', 'rule' and 'msg'
            rawResult.resultCode = retrieveTableValue<int>        ( L, -1, "resultCode", lua_isnumber, lua_toInteger ); 
            rawResult.rule       = retrieveTableValue<std::string>( L, -1, "rule"      , lua_isstring, lua_toString  ); 
            rawResult.msg        = retrieveTableValue<std::string>( L, -1, "msg"       , lua_isstring, lua_toString  ); 

            rawResults.push_back( rawResult );

            lua_pop( L, 1 );
        }

        if ( m_log )
            m_log->logFormatted( "Obtained %d after applying rules to result %d", summaryResultCode, r.resultID );

        return RuleResults( rawResults.begin(), rawResults.end(), summaryResultCode, summaryMsg );
    }
}

ResultQueue::ResultQueue()
{
}

void ResultQueue::queue( const UncontrolledResult& r )
{
    paulst::AcquireCriticalSection a(m_critSec);

    {
        m_queue.push(r);
    }
}

UncontrolledResult ResultQueue::removeNext() 
{
    paulst::AcquireCriticalSection a(m_critSec);

    {
        require( ! m_queue.empty() );
        UncontrolledResult r = m_queue.front();
        m_queue.pop();
        return r;
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
    m_connectionCache(0),
    m_log(0)
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

Rules* RulesCache::getRulesFor( int test, int machine )
{
    paulst::AcquireCriticalSection a(m_critSec);
    
    {
        Rules* rules = 0;

        std::string ruleName = m_rulesConfig->getRuleNameFor( test, machine );

        Cache::const_iterator i = m_cache.find( ruleName );

        if ( i == m_cache.end() )
        {
            rules = load( ruleName );
            m_cache.insert( std::make_pair( ruleName, rules ) );
        }
        else
        {
            rules = i->second;
        }

        return rules;
    }
}

Rules* RulesCache::load( const std::string& ruleName )
{
    m_log->logFormatted( "Loading script for rule '\%s'", ruleName.c_str() );
    std::string script = m_ruleLoader->loadRulesFor( ruleName );
    return new Rules( script, obtainConnectionFromCache, m_connectionCache, m_ruleLoader, m_log );
}

void RulesCache::setConnectionCache( ConnectionCache* cc )
{
    m_connectionCache = cc;
}

void RulesCache::setLog( paulst::LoggingService* log )
{
    m_log = log;
}

void RulesCache::setRulesConfig( RulesConfig* c )
{
    m_rulesConfig = c;
}

void RulesCache::setRuleLoader( RuleLoader* l )
{
    m_ruleLoader = l;
}


ThreadTaskExceptions::ThreadTaskExceptions()
{
}

void ThreadTaskExceptions::add( const Exception& e )
{
    paulst::AcquireCriticalSection a(m_critSec);

    {
        AnsiString ansiMsg( e.Message.c_str() );
        m_messages.push_back( ansiMsg.c_str() );
    }
}

void ThreadTaskExceptions::addUnspecified()
{
    paulst::AcquireCriticalSection a(m_critSec);

    {
        m_messages.push_back( "Unspecified exception." );
    }
}


bool ThreadTaskExceptions::empty() const
{
    paulst::AcquireCriticalSection a(m_critSec);

    {
        return m_messages.empty();
    }
}

std::string ThreadTaskExceptions::getFirstMsg() const
{
    paulst::AcquireCriticalSection a(m_critSec);

    {
        require( ! empty() );
        return m_messages.at(0);
    }
}

RuleEngine::RuleEngine( int maxThreads, int errorResultCode )
    :
    m_publisher(0),
    m_threadPool( 0 ),
    m_threadTaskContext( this ),
    m_threadPoolWork( 0 ),
    m_errorResultCode( errorResultCode )
{
    InitializeThreadpoolEnvironment( &m_threadPoolCallbackEnv );
    m_threadPool = CreateThreadpool(NULL);//CreateThreadpoolWork( runTask, &m_threadTaskContext, NULL );
    require( m_threadPool );
    SetThreadpoolCallbackPool( &m_threadPoolCallbackEnv , m_threadPool );
    SetThreadpoolThreadMaximum( m_threadPool, maxThreads );
    m_threadPoolWork = CreateThreadpoolWork( runTask, &m_threadTaskContext, &m_threadPoolCallbackEnv );
    require( m_threadPoolWork );
    m_rulesCache.setConnectionCache( &m_connectionCache );
}

RuleEngine::~RuleEngine()
{
    CloseThreadpoolWork( m_threadPoolWork );
    CloseThreadpool( m_threadPool );
    DestroyThreadpoolEnvironment( &m_threadPoolCallbackEnv );
}

void RuleEngine::clearRulesCache()
{
    m_rulesCache.clear();
}

int RuleEngine::getErrorResultCode() const
{
    return m_errorResultCode;
}

Rules* RuleEngine::getRulesFor( int test, int machine )
{
    return m_rulesCache.getRulesFor( test, machine );
}

UncontrolledResult RuleEngine::nextQueuedResult()
{
    return m_resultQueue.removeNext();
}

void RuleEngine::publishResults( const RuleResults& r, int forResult )
{
    m_publisher->publish( r, forResult );
}

void RuleEngine::queue( const UncontrolledResult& r )
{
    m_resultQueue.queue( r );
    SubmitThreadpoolWork( m_threadPoolWork );
}

void RuleEngine::setConnectionFactory( paulstdb::AbstractConnectionFactory* conFac )
{
    m_connectionCache.setConnectionFactory( conFac );
}

void RuleEngine::setErrorResultCode( int errorResultCode )
{
    m_errorResultCode = errorResultCode;
}

void RuleEngine::setLog( paulst::LoggingService* l )
{
    m_rulesCache.setLog( l );
}

void RuleEngine::setRulesConfig( RulesConfig* c )
{
    m_rulesCache.setRulesConfig( c );
}

void RuleEngine::setRuleLoader( RuleLoader* l )
{
    m_rulesCache.setRuleLoader( l );
}

void RuleEngine::setResultPublisher( RuleResultPublisher* p )
{
    m_publisher = p;
}

void RuleEngine::reportException( const Exception& e )
{
    m_exceptions.add( e );
}

void RuleEngine::reportUnspecifiedException()
{
    m_exceptions.addUnspecified();
}

void RuleEngine::waitForQueued()
{
    WaitForThreadpoolWorkCallbacks( m_threadPoolWork, false );

    if ( ! m_exceptions.empty() )
    {
        throw Exception( UnicodeString(m_exceptions.getFirstMsg().c_str() ) );
    }
}

RuleEngine::ThreadTaskContext::ThreadTaskContext( RuleEngine* re )
    : 
    m_ruleEngine( re )
{
}

int RuleEngine::ThreadTaskContext::getErrorResultCode() const
{
    return m_ruleEngine->getErrorResultCode();
}

UncontrolledResult RuleEngine::ThreadTaskContext::nextQueuedResult()
{
    return m_ruleEngine->nextQueuedResult();
}

Rules* RuleEngine::ThreadTaskContext::getRulesFor( int test, int machine )
{
    return m_ruleEngine->getRulesFor( test, machine );
}

void RuleEngine::ThreadTaskContext::publishResults( const RuleResults& r, int forResult )
{
    m_ruleEngine->publishResults( r, forResult );
}

void RuleEngine::ThreadTaskContext::reportException( const Exception& e )
{
    m_ruleEngine->reportException( e );
}

void RuleEngine::ThreadTaskContext::reportUnspecifiedException()
{
    m_ruleEngine->reportUnspecifiedException();
}

}

