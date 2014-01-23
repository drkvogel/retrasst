#ifndef RULEENGINEH
#define RULEENGINEH

#include "API.h"
#include "ConnectionCache.h"
#include "CritSec.h"
#include "LuaDB.h"
#include <map>
#include <queue>
#include <string>
#include <System.hpp>
#include "Task.h"
#include "UncontrolledResult.h"
#include <utility>
#include <vector>

class lua_State;

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
    class ThreadPool;
}

namespace valc
{

class Gates;
class RuleLoaderInterface;

class RuleResultPublisher
{
public:
    RuleResultPublisher();
    virtual ~RuleResultPublisher();
    /*
        Implementations of publish must be able to handle being called concurrently.
    */
    virtual void publish( const RuleResults& results, int forResult ) = 0;
private:
    RuleResultPublisher( const RuleResultPublisher& );
    RuleResultPublisher& operator=( const RuleResultPublisher& );
};


class Rules
{
public:
    Rules( const RuleDescriptor& rd, const std::string& script, ConnectionFactory cf, void* connectionState, RuleLoaderInterface* rl,
        paulst::LoggingService* l, Gates* g, int test, int machine, int project, const paulst::Config* c );
    ~Rules();
    RuleResults applyTo( const UncontrolledResult& r );
private:
    paulst::CritSec             m_critSec;
    ConnectionFactory           m_connectionFactory;
    void*                       m_connectionState;
    lua_State*                  L;
    paulst::LoggingService*     m_log;
    const RuleDescriptor        m_ruleDescriptor;
    Gates*                      m_gates;

    Rules( const Rules& );
    Rules& operator=( const Rules& );
};

class RulesConfig
{
public:
    RulesConfig();
    virtual ~RulesConfig();
    /*
        Implementations of this method are invoked one-at-a-time,
        i.e. in series, not in parallel.
    */
    virtual int         getRuleIDFor( int test, int machine, int project ) = 0;
    virtual bool        isConfigured( const UncontrolledResult& r ) = 0;
private:
    RulesConfig( const RulesConfig& );
    RulesConfig& operator=( const RulesConfig& );
};

class RulesCache
{
public:
    RulesCache();
    ~RulesCache();
    void  clear();
    Rules* getRulesFor      ( int test, int machine, int project );
    void setConfig          ( const paulst::Config* c );
    void setConnectionCache ( ConnectionCache* cc );
    void setGates           ( Gates* g );
    void setLog             ( paulst::LoggingService* l );
    void setRulesConfig     ( RulesConfig* c );
    void setRuleLoader      ( RuleLoaderInterface* l );
private:
    typedef std::map< int, Rules* > Cache;
    paulst::CritSec         m_critSec;
    RulesConfig*            m_rulesConfig;
    RuleLoaderInterface*    m_ruleLoader;
    Cache                   m_cache;
    ConnectionCache*        m_connectionCache;
    paulst::LoggingService* m_log;
    Gates*                  m_gates;
    const paulst::Config*   m_config;

    RulesCache( const RulesCache& );
    RulesCache& operator=( const RulesCache& );
    Rules* load( int ruleID, int test, int machine, int project );
};

class QCResults;

class ResultAssessmentTask : public stef::Task
{
public:
    ResultAssessmentTask( const UncontrolledResult& result, RulesCache* rulesCache, 
        RuleResultPublisher* resultPublisher, paulst::LoggingService* log );
protected:
    void doStuff();
private:
    const UncontrolledResult    m_result;
    RulesCache*                 m_rulesCache;
    RuleResultPublisher*        m_resultPublisher;
    paulst::LoggingService*     m_log;
};

class RuleEngineQueueListener
{
public:
    virtual ~RuleEngineQueueListener();
    virtual void notifyQueued( const UncontrolledResult& r ) = 0;
};

class RuleEngine
{
public:
    RuleEngine( int maxThreads = 10 );
    ~RuleEngine();
    void clearRulesCache();
    void queue( const UncontrolledResult& r );
    void setConfig( const paulst::Config* c );
    /*
        This connection factory is used when a rule script wants a connection.
        It is invoked indirectly, via ConnectionCache. The latter enforces
        one-at-a-time access to this connection factory.
    */
    void setConnectionFactory( paulstdb::AbstractConnectionFactory* conFac );
    void setDefaultTaskExceptionHandler( stef::TaskExceptionHandler* teh );
    void setGates( Gates* g );
    void setLog( paulst::LoggingService* l );
    void setQueueListener( RuleEngineQueueListener* l );
    void setRuleLoader( RuleLoaderInterface* l );
    void setRulesConfig( RulesConfig* c );
    void setResultPublisher( RuleResultPublisher* p );
    bool waitForQueued( long timeoutMillis );

private:
    stef::ThreadPool*           m_resultAssessor;
    paulst::CritSec             m_cs;
    RuleResultPublisher*        m_publisher;
    RulesCache                  m_rulesCache;
    ConnectionCache             m_connectionCache;
    paulst::LoggingService*     m_log;
    int                         m_pending;
    HANDLE                      m_signalWhenNonPending;
    RuleEngineQueueListener*    m_queueListener;

    RuleEngine( const RuleEngine& );
    RuleEngine& operator=( const RuleEngine& );
    void onNotify();
};

}

#endif

