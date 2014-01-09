#include "AcquireCriticalSection.h"
#include "CompositeRuleResultPublisher.h"

namespace valc
{

CompositeRuleResultPublisher::CompositeRuleResultPublisher()
{
}

void CompositeRuleResultPublisher::add( RuleResultPublisher* rrp )
{
    m_publishers.push_back( rrp );
}

void CompositeRuleResultPublisher::publish( const RuleResults& results, int forResult )
{
    paulst::AcquireCriticalSection a(m_cs);

    for ( Publishers::iterator i = m_publishers.begin(); i != m_publishers.end(); ++i )
    {
        (*i)->publish( results, forResult );
    }
}

}

