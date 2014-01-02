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

    return m_ruleIDs.count( key );
}

void RulesConfigUsingMap::specify( int ruleID, int forTest, int onMachine, int forProject )
{
    TestMachine key( forTest, onMachine, forProject );

    if ( m_ruleIDs.count( key ) )
    {
        paulst::exception( 
            "Cannot specify rule \%d for test \%d for machine \%d for project \%d. "
            "A rule is already specified: \%d.", ruleID, forTest, onMachine, forProject, m_ruleIDs.find(key)->second );
    }

    m_ruleIDs.insert( std::make_pair( key, ruleID ) );
}

int RulesConfigUsingMap::getRuleIDFor( int test, int machine, int project )
{
    TestMachine key( test, machine, project );

    if ( m_ruleIDs.count( key ) == 0 )
    {
        paulst::exception( "No rules configured for test \%d on machine \%d for project \%d", test, machine, project );
    }

    return m_ruleIDs.find( key )->second;
}


}

