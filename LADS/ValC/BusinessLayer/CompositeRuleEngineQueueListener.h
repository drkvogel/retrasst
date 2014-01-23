#ifndef COMPOSITERULEENGINEQUEUELISTENERH
#define COMPOSITERULEENGINEQUEUELISTENERH

#include <list>
#include "RuleEngine.h"

namespace valc
{

/*
    Note: does NOT OWN the instances that get added to it. i.e. doesn't delete these instances.
*/
class CompositeRuleEngineQueueListener : public RuleEngineQueueListener
{
public:
    CompositeRuleEngineQueueListener();
    void add( RuleEngineQueueListener* l );
    void notifyQueued( const UncontrolledResult& r );
private:
    typedef std::list< RuleEngineQueueListener* > Listeners;
    Listeners m_listeners;

    CompositeRuleEngineQueueListener( const CompositeRuleEngineQueueListener& );
    CompositeRuleEngineQueueListener& operator=( const CompositeRuleEngineQueueListener& );
};

}

#endif

