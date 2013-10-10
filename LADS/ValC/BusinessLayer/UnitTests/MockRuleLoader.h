#ifndef MOCKRULELOADERH
#define MOCKRULELOADERH

#include "AcquireCriticalSection.h"
#include "CritSec.h"
#include <map>
#include "Require.h"
#include "RuleEngine.h"
#include <string>

class MockRuleLoader : public valc::RuleLoader
{
private:
    typedef std::map< std::string, std::string > Scripts;
    Scripts m_scripts;
    paulst::CritSec m_critSec;

public:
    MockRuleLoader() {}

    void addRule( const std::string& ruleName, const std::string& script )
    {
        m_scripts[ruleName] = script;
    }

    std::string loadRulesFor(const std::string& ruleName) 
    { 
        paulst::AcquireCriticalSection a(m_critSec);

        {
            Scripts::const_iterator i = m_scripts.find( ruleName );
            require( i != m_scripts.end() );
            return i->second;
        }
    }
};

#endif

