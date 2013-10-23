#include "ExceptionUtil.h"
#include "RulesConfigUsingMap.h"
#include "StrUtil.h"

namespace valc
{

RulesConfigUsingMap::TestMachine::TestMachine( int t, int m )
    : 
    s ( paulst::format( "\%d:\%d", t, m ) ) 
{
}

bool RulesConfigUsingMap::TestMachine::operator<( const TestMachine& other ) const 
{ 
    return s < other.s; 
}

RulesConfigUsingMap::RulesConfigUsingMap()
{
}

bool RulesConfigUsingMap::isConfigured( const UncontrolledResult& r )
{
    TestMachine key( r.testID, r.machineID );

    return m_ruleNames.count( key );
}

void RulesConfigUsingMap::specify( const std::string ruleName, int forTest, int onMachine )
{
    TestMachine key( forTest, onMachine );

    if ( m_ruleNames.count( key ) )
    {
        paulst::exception( 
            "Cannot specify rule \%s for test \%d for machine \%d. "
            "A rule is already specified: \%s.", ruleName.c_str(), forTest, onMachine, m_ruleNames.find(key)->second.c_str() );
    }

    m_ruleNames.insert( std::make_pair( key, ruleName ) );
}

std::string RulesConfigUsingMap::getRuleNameFor( int test, int machine )
{
    TestMachine key( test, machine );

    if ( m_ruleNames.count( key ) == 0 )
    {
        paulst::exception( "No rules configured for test %d on machine %d", test, machine );
    }

    return m_ruleNames.find( key )->second;
}


}

