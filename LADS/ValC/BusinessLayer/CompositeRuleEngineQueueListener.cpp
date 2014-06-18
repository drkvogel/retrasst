#include "CompositeRuleEngineQueueListener.h"

namespace valc
{

CompositeRuleEngineQueueListener::CompositeRuleEngineQueueListener()
{
}

void CompositeRuleEngineQueueListener::add( RuleEngineQueueListener* l )
{
    m_listeners.push_back( l );
}

void CompositeRuleEngineQueueListener::notifyQueued( const UncontrolledResult& r, const IDToken& runID )
{
    for ( Listeners::const_iterator i = m_listeners.begin(); i != m_listeners.end(); ++i )
    {
        (*i)->notifyQueued( r, runID );
    }
}

}

