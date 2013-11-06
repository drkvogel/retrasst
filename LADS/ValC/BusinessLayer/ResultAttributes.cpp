#include "AcquireCriticalSection.h"
#include "ExceptionUtil.h"
#include "ResultAttributes.h"

namespace valc
{

void ResultAttributes::clearRuleResults()
{
    m_ruleDerivedAttributes.clear();
}

/*
    Notice that there is no critical section for this method.  That is 
    because it is not expected that attempts will be made to 
    get results while other threads are inserting results.
*/
RuleResults ResultAttributes::getRuleResults( int forResult ) const
{
    RuleDerivedAttributes::const_iterator i = m_ruleDerivedAttributes.find( forResult );

    if ( i == m_ruleDerivedAttributes.end() )
    {
        paulst::exception( "No rule results found for result \%d", forResult );
    }

    return i->second;
}

bool ResultAttributes::hasRuleResults( int forResult ) const
{
    return m_ruleDerivedAttributes.count(forResult);
}


void ResultAttributes::publish( const RuleResults& results, int forResult )
{
    paulst::AcquireCriticalSection a(m_critSec);

    {
        m_ruleDerivedAttributes.insert( std::make_pair( forResult, results ) );
    }
}

}

