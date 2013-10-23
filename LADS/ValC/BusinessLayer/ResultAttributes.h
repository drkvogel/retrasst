#ifndef RESULTATTRIBUTESH
#define RESULTATTRIBUTESH

#include "CritSec.h"
#include <map>
#include "RuleEngine.h"

namespace valc
{

class ResultAttributes : public RuleResultPublisher
{
public:
    RuleResults getRuleResults( int forResult ) const;
    bool hasRuleResults( int forResult ) const;
    void publish( const RuleResults& results, int forResult );
private:
    paulst::CritSec m_critSec;
    typedef std::map< int, RuleResults > RuleDerivedAttributes;
    RuleDerivedAttributes m_ruleDerivedAttributes;
};

}

#endif

