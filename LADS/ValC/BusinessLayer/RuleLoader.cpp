#include <algorithm>
#include <boost/bind.hpp>
#include "ExceptionUtil.h"
#include "RuleLoader.h"

namespace valc
{

RuleLoader::RuleLoader()
{
}

RuleLoader::~RuleLoader()
{
    for ( Scripts::iterator i = m_scripts.begin(); i != m_scripts.end(); ++i )
    {
        delete *i;
    }
}

bool sameRuleName( const std::string& name, const RuleScript* script )
{
    return script->ruleDescriptor.getUniqueName() == name;
}

bool sameRuleID( int ruleID, const RuleScript* script )
{
    return script->ruleDescriptor.getRuleID() == ruleID;
}

void RuleLoader::add( const RuleDescriptor& rd, const std::string& script )
{
    if ( find( rd.getRuleID() ) != m_scripts.end() )  
    {
        paulst::exception( "A rule with an ID value of \%d has already been added", rd.getRuleID() );
    }

    if ( find( rd.getUniqueName() ) != m_scripts.end() ) 
    {
        paulst::exception( "A rule with the name '\%s' has already been added", rd.getUniqueName().c_str() );
    }

    RuleScript* l = new RuleScript();
    l->ruleDescriptor = rd;
    l->script = script;
    m_scripts.push_back( l );
}


RuleDescriptor RuleLoader::getRuleDescriptor( int ruleID )
{
    Scripts::iterator i = std::find_if( m_scripts.begin(), m_scripts.end(), boost::bind( sameRuleID, ruleID, _1 ) );
    
    if ( i == m_scripts.end() )
    {
        paulst::exception( "No rule found with ID '\%d'.", ruleID );
    }

    return (*i)->ruleDescriptor;
}

std::string RuleLoader::loadRulesFor( const std::string& uniqueRuleName )
{
    Scripts::iterator i = std::find_if( m_scripts.begin(), m_scripts.end(), boost::bind( sameRuleName, uniqueRuleName, _1 ) );
    
    if ( i == m_scripts.end() )
    {
        paulst::exception( "No rule found with name '\%s'.", uniqueRuleName.c_str() );
    }

    return (*i)->script;
}

std::string RuleLoader::loadRulesFor( int ruleID )
{
    Scripts::iterator i = std::find_if( m_scripts.begin(), m_scripts.end(), boost::bind( sameRuleID, ruleID, _1 ) );
    
    if ( i == m_scripts.end() )
    {
        paulst::exception( "No rule found with ID '\%d'.", ruleID );
    }

    return (*i)->script;
}

RuleLoader::Scripts::iterator RuleLoader::find( int ruleID )
{
    return std::find_if( m_scripts.begin(), m_scripts.end(), boost::bind( sameRuleID, ruleID, _1 ) );
}

RuleLoader::Scripts::iterator RuleLoader::find( const std::string& ruleName )
{
    return std::find_if( m_scripts.begin(), m_scripts.end(), boost::bind( sameRuleName, ruleName, _1 ) );
}

}

