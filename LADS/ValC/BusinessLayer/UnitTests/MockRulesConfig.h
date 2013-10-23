#ifndef MOCKRULESCONFIGH
#define MOCKRULESCONFIGH

#include <cstdio>
#include <map>
#include "Require.h"
#include "RuleEngine.h"
#include <string>
#include "StrUtil.h"

class MockRulesConfig : public valc::RulesConfig
{
private:
	typedef std::map< std::string, std::string > RuleNames;
	RuleNames m_ruleNames;


public:
    void addMapping( int test, int machine, const std::string& ruleName )
	{
		const std::string key = paulst::format( "%d %d", test, machine );

		m_ruleNames.insert( std::make_pair( key, ruleName ) );
    }

    std::string getRuleNameFor( int test, int machine ) 
	{
		const std::string key = paulst::format( "%d %d", test, machine );
		RuleNames::const_iterator i = m_ruleNames.find( key );
        require( i != m_ruleNames.end() );
        return i->second;
    }

    bool isConfigured( const valc::UncontrolledResult& r )
    {
		const std::string key = paulst::format( "%d %d", r.testID, r.machineID );
        return m_ruleNames.count( key );
    }
};


#endif

