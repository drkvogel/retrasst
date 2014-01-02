#ifndef QCRULELOADERTESTH
#define QCRULELOADERTESTH

#include "RuleLoader.h"
#include <tut.h>


namespace tut
{
	class RuleLoaderTestFixture
    {
    };

    typedef test_group<RuleLoaderTestFixture, 1> RuleLoaderTestGroup;
	RuleLoaderTestGroup testGroupRuleLoader( "RuleLoader tests");
	typedef RuleLoaderTestGroup::object testRuleLoader;

	template<>
	template<>
	void testRuleLoader::test<1>()
	{
		set_test_name("RuleLoader behaviour specification");

		using namespace valc;

        RuleLoader rl;
        rl.add( RuleDescriptor( 1/*recordID*/, 2/*ruleID*/, "name", "nameForUsers" ), "ruleScript" );

        ensure_equals( rl.loadRulesFor( "name" ), std::string("ruleScript") );
        ensure_equals( rl.loadRulesFor( 2 )     , std::string("ruleScript") );

        bool exception = false;

        try
        {
            rl.loadRulesFor( 1 );
        }
        catch( ... )
        {
            exception = true;
        }

        ensure( "Rules are keyed on ruleID, not recordID", exception );
        
        RuleDescriptor ruleDescriptor = rl.getRuleDescriptor( 2 );
        ensure_equals( ruleDescriptor.getRecordID(), 1 );
        ensure_equals( ruleDescriptor.getRuleID(), 2 );

        exception = false;

        try
        {
             rl.getRuleDescriptor( 1 );
        }
        catch( ... )
        {
            exception = true;
        }

        ensure( "No rule with an ID value of 1 has been added", exception );

        exception = false;

        try
        {
            rl.add( RuleDescriptor( 3/*recordID*/, 4/*ruleID*/, "name", "blah blah" ), "anotherScriptBody" );
        }
        catch( const Exception& e )
        {
            exception = true;
        }
    
        ensure( "Cannot add a rule called 'name' because a rule with this name has already been added", exception );

        exception = false;

        try
        {
            rl.loadRulesFor( "nameForUsers" );
        }
        catch( const Exception& e )
        {
            exception = true;
        }

        ensure( "No script called 'nameForUsers' has been added", exception );

        exception = false;

        try
        {
            rl.add( RuleDescriptor( 5/*recordID*/, 2/*ruleID*/, "uniqueName", "blah" ), "uniqueScriptBody" );
        }
        catch( const Exception& e )
        {
            exception = true;
        }

        ensure( "Cannot add a rule with an ID of '2' because a rule with this ID has already been added", exception );
    }
};

#endif

