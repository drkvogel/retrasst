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
#include <utility>
#include <vector>

class lua_State;

namespace paulstdb
{
    class AbstractConnectionFactory;
}

namespace valc
{

class RuleLoader
{
public:
    RuleLoader();
    virtual ~RuleLoader();
    virtual std::string loadRulesFor( const std::string& ruleName ) = 0;
private:
    RuleLoader( const RuleLoader& );
    RuleLoader& operator=( const RuleLoader& );
};


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


struct UncontrolledResult
{
    int         testID;
    int         resultID;
    int         machineID;
    double      resultValue;
    std::string resultText;
    std::string barcode;
    int         projectID;
    TDateTime   dateAnalysed;

    UncontrolledResult();
};


void lua_pushUncontrolledResult( lua_State* L, const UncontrolledResult& r );

class Rules
{
public:
    Rules( const std::string& script, ConnectionFactory cf, void* connectionState, RuleLoader* rl );
    ~Rules();
    RuleResults applyTo( const UncontrolledResult& r );
private:
    paulst::CritSec     m_critSec;
    ConnectionFactory   m_connectionFactory;
    void*               m_connectionState;
    lua_State*          L;

    Rules( const Rules& );
    Rules& operator=( const Rules& );
};

class ResultQueue
{
public:
    ResultQueue();
    void queue( const UncontrolledResult& r );
    UncontrolledResult removeNext();
private:
    std::queue< UncontrolledResult > m_queue;
    paulst::CritSec m_critSec;

    ResultQueue( const ResultQueue& );
    ResultQueue& operator=( const ResultQueue& );
};

class RulesConfig
{
public:
    RulesConfig();
    virtual ~RulesConfig();
    virtual std::string getRuleNameFor( int test, int machine ) = 0;
private:
    RulesConfig( const RulesConfig& );
    RulesConfig& operator=( const RulesConfig& );
};

class RulesCache
{
public:
    RulesCache();
    ~RulesCache();
    Rules* getRulesFor      ( int test, int machine );
    void setConnectionCache ( ConnectionCache* cc );
    void setRulesConfig     ( RulesConfig* c );
    void setRuleLoader      ( RuleLoader* l );
private:
    typedef std::map< std::string, Rules* > Cache;
    paulst::CritSec     m_critSec;
    RulesConfig*        m_rulesConfig;
    RuleLoader*         m_ruleLoader;
    Cache               m_cache;
    ConnectionCache*    m_connectionCache;

    RulesCache( const RulesCache& );
    RulesCache& operator=( const RulesCache& );
    Rules* load( const std::string& ruleName );
};

class ThreadTaskExceptions
{
public:
    ThreadTaskExceptions();
    void add( const Exception& e );
    void addUnspecified();
    bool empty() const;
    std::string getFirstMsg() const;
private:
    paulst::CritSec m_critSec;
    typedef std::vector< std::string > Messages;
    Messages m_messages;
};

class RuleEngine
{
public:
    RuleEngine( int maxThreads = 10, int errorResultCode = 999 );
    ~RuleEngine();
    int  getErrorResultCode() const;
    void queue( const UncontrolledResult& r );
    /*
        This connection factory is used when a rule script wants a connection.
        It is invoked indirectly, via ConnectionCache. The latter enforces
        one-at-a-time access to this connection factory.
    */
    void setConnectionFactory( paulstdb::AbstractConnectionFactory* conFac );
    void setErrorResultCode( int errorCode );
    void setRuleLoader( RuleLoader* l );
    void setRulesConfig( RulesConfig* c );
    void setResultPublisher( RuleResultPublisher* p );
    /*
        'waitForQueued' will throw an Exception if an exception has been reported by 
        a worker thread.
    */
    void waitForQueued();

    friend class ThreadTaskContext;

    class ThreadTaskContext
    {
    public:
        ThreadTaskContext( RuleEngine* qcre );
        UncontrolledResult  nextQueuedResult();
        int                 getErrorResultCode() const;
        Rules*              getRulesFor( int test, int machine );
        void                publishResults( const RuleResults& r, int forResult );
        void                reportException( const Exception& e );
        void                reportUnspecifiedException();
    private:
        RuleEngine* m_ruleEngine;
    }; 

private:
    RuleResultPublisher*    m_publisher;
    TP_CALLBACK_ENVIRON     m_threadPoolCallbackEnv;
    PTP_POOL                m_threadPool;
    PTP_WORK                m_threadPoolWork;
    ResultQueue             m_resultQueue;
    RulesCache              m_rulesCache;
    ThreadTaskExceptions    m_exceptions;
    ThreadTaskContext       m_threadTaskContext;
    ConnectionCache         m_connectionCache;
    int                     m_errorResultCode;

    RuleEngine( const RuleEngine& );
    RuleEngine& operator=( const RuleEngine& );
    Rules*              getRulesFor( int test, int machine );
    UncontrolledResult  nextQueuedResult();
    void                publishResults( const RuleResults& r, int forResult );
    void                reportException( const Exception& e );
    void                reportUnspecifiedException();
};

}

#endif

