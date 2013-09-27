#ifndef MOCKRULESCONFIGH
#define MOCKRULESCONFIGH

#include <cstdio>
#include <map>
#include "Require.h"
#include "RuleEngine.h"
#include <string>

class MockRulesConfig : public valc::RulesConfig
{
private:
	typedef std::map< std::string, std::string > RuleNames;
	RuleNames m_ruleNames;

	std::string makeKey( int test, int machine )
	{
		char buffer[1024];
		std::sprintf( buffer, "%d:%d", test, machine );
		return buffer;
	}

public:
    void addMapping( int test, int machine, const std::string& ruleName )
	{
		m_ruleNames.insert( std::make_pair( makeKey( test, machine ), ruleName ) );
    }

    std::string getRuleNameFor( int test, int machine ) 
    { 
		RuleNames::const_iterator i = m_ruleNames.find( makeKey( test, machine ) );
        require( i != m_ruleNames.end() );
        return i->second;
    }
};


#endif

