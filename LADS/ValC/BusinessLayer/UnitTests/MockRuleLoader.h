#ifndef MOCKRULELOADERH
#define MOCKRULELOADERH

#include "Require.h"
#include "RuleLoaderInterface.h"
#include "RuleLoader.h"

class MockRuleLoader : public valc::RuleLoaderInterface
{
public:
    valc::RuleLoader impl;
    int totalLoadRequests;

    MockRuleLoader() : totalLoadRequests(0) {}

    valc::RuleDescriptor getRuleDescriptor( int ruleID )              { return impl.getRuleDescriptor( ruleID ); }
    std::string          loadRulesFor( int ruleID )                   { ++totalLoadRequests; return impl.loadRulesFor( ruleID );     }
    std::string          loadRulesFor( const std::string& uniqueName ){ ++totalLoadRequests; return impl.loadRulesFor( uniqueName ); }
};

#endif

