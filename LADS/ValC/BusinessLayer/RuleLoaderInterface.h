#ifndef RULELOADERINTERFACEH
#define RULELOADERINTERFACEH

#include "API.h"
#include <string>

namespace valc
{

class RuleLoaderInterface
{
public:
    RuleLoaderInterface();
    virtual ~RuleLoaderInterface();
    virtual RuleDescriptor getRuleDescriptor( int ruleID ) = 0;
    virtual std::string loadRulesFor( int ruleID ) = 0;
    virtual std::string loadRulesFor( const std::string& uniqueName ) = 0;
private:
    RuleLoaderInterface( const RuleLoaderInterface& );
    RuleLoaderInterface& operator=( const RuleLoaderInterface& );
};

}

#endif

