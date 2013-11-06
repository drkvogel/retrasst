#include "ExceptionUtil.h"
#include "RulesConfigUsingMap.h"
#include "StrUtil.h"

namespace valc
{

RulesConfigUsingMap::TestMachine::TestMachine( int t, int m, int p )
    : 
    s ( paulst::format( "\%d:\%d:\%d", t, m, p ) ) 
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
    TestMachine key( r.testID, r.machineID, r.projectID );

    return m_ruleNames.count( key );
}

void RulesConfigUsingMap::specify( const std::string ruleName, int forTest, int onMachine, int forProject )
{
    TestMachine key( forTest, onMachine, forProject );

    if ( m_ruleNames.count( key ) )
    {
        paulst::exception( 
            "Cannot specify rule \%s for test \%d for machine \%d for project \%d. "
            "A rule is already specified: \%s.", ruleName.c_str(), forTest, onMachine, forProject, m_ruleNames.find(key)->second.c_str() );
    }

    m_ruleNames.insert( std::make_pair( key, ruleName ) );
}

std::string RulesConfigUsingMap::getRuleNameFor( int test, int machine, int project )
{
    TestMachine key( test, machine, project );

    if ( m_ruleNames.count( key ) == 0 )
    {
        paulst::exception( "No rules configured for test \%d on machine \%d for project \%d", test, machine, project );
    }

    return m_ruleNames.find( key )->second;
}


}

