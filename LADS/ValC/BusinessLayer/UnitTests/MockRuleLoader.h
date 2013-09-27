#ifndef MOCKRULELOADERH
#define MOCKRULELOADERH

#include <map>
#include "Require.h"
#include "RuleEngine.h"
#include <string>

class MockRuleLoader : public valc::RuleLoader
{
private:
    typedef std::map< std::string, std::string > Scripts;
    Scripts m_scripts;

public:
    MockRuleLoader() {}

    void addRule( const std::string& ruleName, const std::string& script )
    {
        m_scripts.insert( std::make_pair( ruleName, script ) );
    }

    std::string loadRulesFor(const std::string& ruleName) 
    { 
        Scripts::const_iterator i = m_scripts.find( ruleName );
        require( i != m_scripts.end() );
        return i->second;
    }
};

#endif

