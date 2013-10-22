#ifndef RULELOADERUSINGMAPH
#define RULELOADERUSINGMAPH

#include <map>
#include "RuleEngine.h"
#include <string>

namespace valc
{

class RuleLoaderUsingMap : public RuleLoader
{
public:
    RuleLoaderUsingMap();
    void addRule( const std::string& name, const std::string& script );
    std::string loadRulesFor( const std::string& ruleName );
private:
    typedef std::map< std::string, std::string > ScriptsKeyedOnName;
    ScriptsKeyedOnName m_scripts;

    RuleLoaderUsingMap( const RuleLoaderUsingMap& );
    RuleLoaderUsingMap& operator=( const RuleLoaderUsingMap& );
};

}

#endif

