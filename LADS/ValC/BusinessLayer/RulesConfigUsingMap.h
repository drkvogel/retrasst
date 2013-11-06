#ifndef RULESCONFIGUSINGMAPH
#define RULESCONFIGUSINGMAPH

#include <map>
#include "RuleEngine.h"
#include <string>

namespace valc
{

class RulesConfigUsingMap : public RulesConfig
{
public:
    RulesConfigUsingMap();
    void specify( const std::string ruleName, int forTest, int onMachine, int forProject );
    std::string getRuleNameFor( int test, int machine, int project );
    bool isConfigured( const UncontrolledResult& r );

private:
    struct TestMachine
    {
        std::string s;
        TestMachine( int t = 0, int m = 0, int p = 0 );
        bool operator<( const TestMachine& other ) const; 
    }; 

    typedef std::map< TestMachine, std::string > RuleNamesKeyedOnTestMachine;
    RuleNamesKeyedOnTestMachine m_ruleNames;
};

}

#endif

