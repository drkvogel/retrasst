#include "ExceptionUtil.h"
#include "RuleLoaderUsingMap.h"

namespace valc
{

RuleLoaderUsingMap::RuleLoaderUsingMap()
{
}

void RuleLoaderUsingMap::addRule( const std::string& name, const std::string& script )
{
    if ( m_scripts.count( name ) )
    {
        paulst::exception( "Cannot add a rule called '\%s'.  A rule with this name has already been added.", name.c_str() );    
    }

    m_scripts.insert( std::make_pair( name, script ) );
}

std::string RuleLoaderUsingMap::loadRulesFor( const std::string& ruleName )
{
    ScriptsKeyedOnName::iterator i = m_scripts.find( ruleName );

    if ( i == m_scripts.end() )
    {
        paulst::exception( "No rule script for '\%s'", ruleName.c_str() );
    }

    return i->second;
}


}

