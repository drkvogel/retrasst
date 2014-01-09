#ifndef COMPOSITERULERESULTPUBLISHERH
#define COMPOSITERULERESULTPUBLISHERH

#include "CritSec.h"
#include "RuleEngine.h"
#include <vector>

namespace valc
{

class CompositeRuleResultPublisher : public RuleResultPublisher
{
public:
    CompositeRuleResultPublisher();
    void add( RuleResultPublisher* rrp );
    void publish( const RuleResults& results, int forResult );
private:
    typedef std::vector< RuleResultPublisher* > Publishers;
    Publishers m_publishers;
    paulst::CritSec m_cs;

    CompositeRuleResultPublisher( const CompositeRuleResultPublisher& );
    CompositeRuleResultPublisher& operator=( const CompositeRuleResultPublisher& );
};

}

#endif

