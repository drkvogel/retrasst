#ifndef RULELOADERH
#define RULELOADERH

#include "API.h"
#include "RuleLoaderInterface.h"
#include <vector>

namespace valc
{

struct RuleScript
{
RuleDescriptor  ruleDescriptor;
std::string     script;
};

class RuleLoader : public RuleLoaderInterface
{
public:
    RuleLoader();
    ~RuleLoader();
    void add( const RuleDescriptor& rd, const std::string& script );
    RuleDescriptor getRuleDescriptor( int ruleID );
    std::string loadRulesFor( int ruleID );
    std::string loadRulesFor( const std::string& uniqueName );
private:
    typedef std::vector< const RuleScript* > Scripts;
    Scripts m_scripts;
    
    RuleLoader( const RuleLoader& );
    RuleLoader& operator=( const RuleLoader& );

    Scripts::iterator find( int ruleID );
    Scripts::iterator find( const std::string& ruleName );
};

}

#endif

