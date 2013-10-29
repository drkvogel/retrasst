#ifndef QCRULEENGINETESTH
#define QCRULEENGINETESTH

#include "AcquireCriticalSection.h"
#include <algorithm>
#include "API.h"
#include "ConsoleWriter.h"
#include "CritSec.h"
#include <cstdio>
#include <iterator>
#include <iostream>
#include "LoggingService.h"
#include <map>
#include "MockRuleEngineConnectionFactory.h"
#include "MockRuleLoader.h"
#include "NoLogging.h"
#include "RuleEngine.h"
#include "RulesConfigUsingMap.h"
#include <set>
#include "StrUtil.h"
#include <SysUtils.hpp>
#include <tut.h>

struct PrintRuleResult
{
    void operator()( const valc::RuleResult& r )
    {
        std::printf( "%d/%s/%s\n", r.resultCode, r.rule.c_str(), r.msg.c_str() );
    }
};

valc::RuleResult buildRuleResult( int resultCode, const std::string& rule, const std::string& msg )
{
    valc::RuleResult r;
    r.resultCode = resultCode;
    r.rule = rule;
    r.msg = msg;
    return r;
}

std::string serialize( const valc::RuleResult& r )
{
    char buffer[1024];
    std::sprintf( buffer, "%d/%s/%s", r.resultCode, r.rule.c_str(), r.msg.c_str() );
    return buffer;
}

bool compareRuleResults( const valc::RuleResult& r1, const valc::RuleResult& r2 )
{
    return serialize( r1 ) < serialize( r2 );
}

bool equivalentRuleResults( const valc::RuleResult& r1, const valc::RuleResult& r2 )
{
    return serialize( r1 ) == serialize( r2 );
}

namespace tut
{
	class RuleEngineTestFixture : public valc::RuleResultPublisher
    {
    private:
        typedef std::map< int, valc::RuleResults > ResultMap;
        ResultMap resultMap;
        paulst::CritSec m_critSec;

    public:

        MockRuleLoader ruleLoader;
        valc::RulesConfigUsingMap rulesConfig;
        MockRuleEngineConnectionFactory connectionFactory;
        valc::RuleEngine ruleEngine;
        std::vector< int > resultSequence;
        paulst::LoggingService log;
       
        RuleEngineTestFixture(int numThreads = 10, int errorResultCode = 999, bool logging = false )
            : 
            ruleEngine(numThreads, errorResultCode ), 
            log( logging ? (paulst::Writer*) new paulst::ConsoleWriter() : (paulst::Writer*)new NoLogging() )
        {
            ruleEngine.setLog( &log );
            ruleEngine.setRulesConfig( &rulesConfig );
            ruleEngine.setRuleLoader( &ruleLoader );
            ruleEngine.setResultPublisher( this );
            ruleEngine.setConnectionFactory( &connectionFactory );
        }

        int countResults() const
        {
            return resultMap.size();
        }
    
        valc::RuleResults getResultFor( int resultID )
        {
            ResultMap::const_iterator i = resultMap.find( resultID );
            require( i != resultMap.end() );
            return i->second;
        }

        void publish( const valc::RuleResults& results, int forResult ) 
        { 
            paulst::AcquireCriticalSection a(m_critSec);
            {
                resultMap[forResult] = results;
                resultSequence.push_back( forResult );
            }
        }

        void setConnectionCounter( int* opened, int* closed )
        {
            connectionFactory.setConnectionCounter( opened, closed );
        }
    };

    typedef test_group<RuleEngineTestFixture, 8> RuleEngineTestGroup;
	RuleEngineTestGroup testGroupRuleEngine( "RuleEngine tests");
	typedef RuleEngineTestGroup::object testRuleEngine;

	template<>
	template<>
	void testRuleEngine::test<1>()
	{
		set_test_name("Simplest script.  Checking basic engine-script interaction.");

		using namespace valc;

        const std::string ruleScript(// The following script returns
                //      an array of the names of failed rules
                //      an integer value describing the overall result
                " function onLoad() end                     \n"
                "                                           \n"
                " function applyRules( qc )                 \n"
                "   assert( qc.testID     == 2 )            \n"
                "   assert( qc.resultID   == 6 )            \n"
                "   assert( qc.machineID  == 8 )            \n"
                "   assert( qc.projectID  == 4 )            \n"
                "   assert( qc.resultText == 'dog' )        \n"
                "   assert( qc.barcode    == 'chicken' )    \n"
                "   assert( qc.actionFlag == '?' )          \n"
                "   assert( qc.dateAnalysed.year  == 1998 ) \n"
                "   assert( qc.dateAnalysed.month == 10 )   \n"
                "   assert( qc.dateAnalysed.day   == 30 )   \n"
                "   assert( qc.dateAnalysed.hour  == 14 )   \n"
                "   assert( qc.dateAnalysed.min   == 33 )   \n"
                "   assert( qc.dateAnalysed.sec   == 5  )   \n"
                "   assert( qc.dateAnalysed.msec  == 10 )   \n"
                "   local ruleResults = {}                  \n"
                "   local aResult = {resultCode = 42, rule = 'testRule', msg = 'hello'} \n"
                "   table.insert( ruleResults, aResult ) \n"
                "   local anotherResult = {resultCode = 81, rule = 'testRule2', msg = 'bye'} \n"
                "   table.insert( ruleResults, anotherResult ) \n"
                "   return ruleResults, 'ok', 72             \n"
                " end " );  

		RuleEngineTestFixture testFixture;
        testFixture.ruleLoader.addRule( "myRule", ruleScript );
        testFixture.rulesConfig.specify( "myRule", 2, 8, 4 );

        UncontrolledResult result;
        result.testID = 2;
        result.resultID = 6;
        result.machineID = 8;
        result.projectID = 4;
        result.resultValue = 27.8;
        result.resultText = "dog";
        result.barcode = "chicken";
        result.dateAnalysed = TDateTime( 1998, 10, 30, 14, 33, 5, 10 );

        typedef std::vector<RuleResult> RawResults;

        try
        {
            testFixture.ruleEngine.queue( result );
            testFixture.ruleEngine.waitForQueued();
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryResultCode(), 72 );
            ensure_equals( ruleResults.getSummaryMsg(), std::string("ok") );
            RawResults actualRawResults;
            std::copy( ruleResults.begin(), ruleResults.end(), std::back_inserter( actualRawResults ) );
            std::sort( actualRawResults.begin(), actualRawResults.end(), compareRuleResults );
            RawResults expectedRawResults;
            expectedRawResults.push_back( buildRuleResult( 42, "testRule", "hello" ) );
            expectedRawResults.push_back( buildRuleResult( 81, "testRule2", "bye" ) );
            std::sort( expectedRawResults.begin(), expectedRawResults.end(), compareRuleResults );
            ensure_equals( "Expect 2 raw results", actualRawResults  .size(), 2U );
            ensure_equals( "Sanity check",         expectedRawResults.size(), 2U );
            // std::for_each( actualRawResults.begin(), actualRawResults.end(), PrintRuleResult() );
            ensure( "Comparing expected sequence of raw results with actual", 
                std::equal( expectedRawResults.begin(), expectedRawResults.end(), actualRawResults.begin(), equivalentRuleResults ) );
        }
        catch( const Exception& e )
        {
            AnsiString ansiStr( e.Message.c_str() );
            ensure( ansiStr.c_str(), false );
        }
	}

    template<>
	template<>
	void testRuleEngine::test<2>()
	{
		set_test_name("When a script uses a database connection");

		using namespace valc;

        UncontrolledResult result;
        result.testID = 2;
        result.resultID = 6;
        result.machineID = 8;
        result.projectID = 4;
        result.resultValue = 27.8;
        result.resultText = "dog";
        result.barcode = "chicken";
        result.dateAnalysed = TDateTime( 1998, 10, 30, 14, 33, 5, 10 );

        const std::string ruleScript(// The following script uses a connection when it is loaded.
                " globalValue = 58                                          \n"
                "                                                           \n"
			    " sqlType = { integer = 0 }\n"
                "                                                           \n"
                " function onLoad()                                         \n"
                "     local hC = openConnection('myCon')                    \n"
                "     local hQ = executeQuery( '49,\\n', {sqlType.integer}, hC ) \n"
                "     local row = fetchNextRow( hQ )                        \n"
                "     globalValue = globalValue + row[1]                    \n"
                " end                                                       \n"
                "                                                           \n"
                " function applyRules( qc )                                 \n"
                "     local hC = openConnection('myCon')                    \n"
                "     local hQ = executeQuery( qc.resultID .. ',\\n', {sqlType.integer}, hC ) \n"
                "     local row = fetchNextRow( hQ )                        \n"
                "     assert( row[1] == qc.resultID )                       \n"
                "     globalValue = globalValue + 1                         \n"
                "     return {}, 'ok', globalValue                          \n"
                " end                                                       \n"
                "                                                           \n");


        int connectionsOpened = 0;
        int connectionsClosed = 0;

		{
		    RuleEngineTestFixture testFixture;
			testFixture.setConnectionCounter(&connectionsOpened, &connectionsClosed);
			testFixture.ruleLoader.addRule( "myRule", ruleScript );
            testFixture.rulesConfig.specify( "myRule", 2, 8, 4 );

            try
            {
                for ( int resultID = 1; resultID < 10; ++resultID )
                {
                    result.resultID = resultID;

                    testFixture.ruleEngine.queue( result );
                }

                testFixture.ruleEngine.waitForQueued();

                ensure_equals( testFixture.countResults(), 9 );
                ensure_equals( "expect 1 connection constructed", connectionsOpened, 1 );// connection re-use
                ensure_equals( "expect 0 connections closed", connectionsClosed, 0 );

                // Each iteration is expected to use the same instance of the script.
                // Within the script, globalValue is getting incremented. The 
                // expectation is that these increments are being applied to the same
                // instance of global value, so their effects are cumulative.
                std::set<int> expectedResults;

                int tally = 58 + 49; // globalValue is initialised to 58, but onLoad() should have added 49 to it.

                for ( int resultID = 1; resultID < 10; ++resultID )
                {
                    expectedResults.insert( ++tally );
                }

                std::set<int> actualResults;

                for ( int resultID = 1; resultID < 10; ++resultID )
                {
                    RuleResults ruleResults = testFixture.getResultFor( resultID );
                    actualResults.insert( ruleResults.getSummaryResultCode() );
                }

                ensure_equals( actualResults.size(), 9U );
                ensure( "Results as expected?", equal( actualResults.begin(), actualResults.end(), expectedResults.begin() ) );
            }
            catch( const Exception& e )
            {
                AnsiString ansiStr( e.Message.c_str() );
                ensure( ansiStr.c_str(), false );
            }
        }

        // As the RuleEngine's ConnectionCache goes out of scope, the cached connection should have been closed
        ensure_equals( "expect 1 connection instance closed", connectionsClosed, 1 );
	}

    template<>
	template<>
	void testRuleEngine::test<3>()
	{
		set_test_name("When lua throws an error");

		using namespace valc;

        const std::string ruleScript(
                " function onLoad( loadFunc )            \n"
                " end                                    \n"
                " function applyRules( qc )              \n"
                "   error( 'biscuits' )                  \n"
                "   return {}, '', 1                     \n"
                " end " );  

        const int ERROR_CODE = 999;

		RuleEngineTestFixture testFixture;
        testFixture.ruleLoader.addRule( "myRule", ruleScript );
        testFixture.rulesConfig.specify( "myRule", 2, 8, 4 );
        testFixture.ruleEngine.setErrorResultCode( ERROR_CODE );

        UncontrolledResult result;
        result.testID = 2;
        result.resultID = 6;
        result.machineID = 8;
        result.projectID = 4;
        result.resultValue = 27.8;
        result.resultText = "dog";
        result.barcode = "chicken";
        result.dateAnalysed = TDateTime( 1998, 10, 30, 14, 33, 5, 10 );

        try
        {
            testFixture.ruleEngine.queue( result );
            testFixture.ruleEngine.waitForQueued();
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryResultCode(), ERROR_CODE );
            ensure( ruleResults.getSummaryMsg().find("biscuits") != std::string::npos );
        }
        catch( const Exception& e )
        {
            AnsiString ansiStr( e.Message.c_str() );
            ensure( ansiStr.c_str(), false );
        }
	}

    template<>
	template<>
	void testRuleEngine::test<4>()
	{
		set_test_name("When no config for a test/machine");

		using namespace valc;

        const int ERROR_CODE = 999;

		RuleEngineTestFixture testFixture;
        testFixture.ruleEngine.setErrorResultCode( ERROR_CODE );

        UncontrolledResult result;
        result.testID = 2;
        result.resultID = 6;
        result.machineID = 8;
        result.projectID = 4;
        result.resultValue = 27.8;
        result.resultText = "dog";
        result.barcode = "chicken";
        result.dateAnalysed = TDateTime( 1998, 10, 30, 14, 33, 5, 10 );

        try
        {
            testFixture.ruleEngine.queue( result );
            testFixture.ruleEngine.waitForQueued();
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryResultCode(), ERROR_CODE );
        }
        catch( const Exception& e )
        {
            AnsiString ansiStr( e.Message.c_str() );
            ensure( ansiStr.c_str(), false );
        }
	}

    template<>
	template<>
	void testRuleEngine::test<5>()
	{
		set_test_name("Rule loading - the simplest case");

		using namespace valc;

        
        const std::string configRuleScript(
                " loadedRules = {}                                  \n"
                "                                                   \n"
                " context = {}                                      \n"
                "                                                   \n"
                " ruleNames = { 'someRule' }                        \n"
                "                                                   \n"
                " function onLoad(loadFunc)                         \n"
                "   for i, ruleName in ipairs(ruleNames) do         \n"
                "      local script = loadFunc(ruleName)            \n"
                "      loadedRules[ruleName] = load(script)         \n"
                "   end                                             \n"
                "   context.sequence = { 12, 49, 52 }               \n"
                "   context.stdDev   = 1.9                          \n"
                "   context.mean     = 38                           \n"
                " end                                               \n"
                "                                                   \n"
                " function applyRules( qc )                         \n"
                "                                                   \n"
                "   local results = {}                              \n"
                "   context.qc = qc                                 \n"
                "   for name, rule in pairs(loadedRules) do         \n"
                "     local result = rule()                         \n"
                "     table.insert( results, result )               \n"
                "   end                                             \n"
                "                                                   \n"
                "   return results, results[1].msg, results[1].resultCode \n"
                " end                                               \n"
                );  

        const std::string someRule(
                " return { resultCode = context.mean, rule = 'this here rule', msg = 'success' } "
            );

		RuleEngineTestFixture testFixture;
        testFixture.ruleLoader.addRule( "configRule", configRuleScript );
        testFixture.ruleLoader.addRule( "someRule"  , someRule );
        testFixture.rulesConfig.specify( "configRule", 2, 8, 4 );

        UncontrolledResult result;
        result.testID = 2;
        result.resultID = 6;
        result.machineID = 8;
        result.projectID = 4;
        result.resultValue = 27.8;
        result.resultText = "dog";
        result.barcode = "chicken";
        result.dateAnalysed = TDateTime( 1998, 10, 30, 14, 33, 5, 10 );

        try
        {
            testFixture.ruleEngine.queue( result );
            testFixture.ruleEngine.waitForQueued();
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryMsg(), std::string("success" ) );
            ensure_equals( ruleResults.getSummaryResultCode(), 38 );
            RuleResult ruleResult = *(ruleResults.begin());
            ensure_equals( ruleResult.resultCode, 38 );
            ensure_equals( ruleResult.rule, std::string("this here rule") );
            ensure_equals( ruleResult.msg, std::string("success") );
        }
        catch( const Exception& e )
        {
            AnsiString ansiStr( e.Message.c_str() );
            ensure( ansiStr.c_str(), false );
        }
	}

    template<>
	template<>
	void testRuleEngine::test<6>()
	{
		set_test_name("Rule loading - multiple (different) tests");

		using namespace valc;

		RuleEngineTestFixture testFixture;
  
        const int NUM_RESULTS = 50;
        int testIDs[NUM_RESULTS];

        const char* configScriptTemplate  =
                    " local log                                         \n"
                    " local sleep                                       \n"
                    "                                                   \n"
                    " function onLoad(loadFunc, logFunc, sleepFunc)     \n"
                    "   log = logFunc                                   \n"
                    "   sleep = sleepFunc                               \n"
                    "   log( 'onLoad:' .. \%d )                         \n"
                    " end                                               \n"
                    "                                                   \n"
                    " function applyRules( qc )                         \n"
                    "   log( 'applyRules:' .. \%d )                     \n"
                    "   math.randomseed( qc.resultID )                  \n"
                    "   local myRand = math.random()                    \n"
                    "   if ( myRand > 0.5 ) then                        \n"
                    "      log('Sleeping: ' .. qc.resultID)             \n"
                    "      sleep(20)                                    \n"
                    "   end                                             \n"
                    "   return {}, '\%s', \%d                           \n"
                    " end                                               \n";

        for ( int testID = 1; testID <= NUM_RESULTS; ++testID )
        {
            const std::string configName = paulst::format( "configRuleForTest%d", testID );
            char config[1024];
            ensure( std::sprintf( config, configScriptTemplate, testID, testID, configName.c_str(), testID ) );
            testFixture.rulesConfig.specify( configName, testID, 8/*machineID*/, 4/*projectID*/ );
            testFixture.ruleLoader.addRule( configName, config );
        }

        UncontrolledResult result;
        result.testID = 2;
        result.resultID = 6;
        result.machineID = 8;
        result.projectID = 4;
        result.resultValue = 27.8;
        result.resultText = "dog";
        result.barcode = "chicken";
        result.dateAnalysed = TDateTime( 1998, 10, 30, 14, 33, 5, 10 );

        testFixture.log.logFormatted( "Queuing %d results", NUM_RESULTS );

        for ( int testID = 1; testID <= NUM_RESULTS; ++testID )
        {
            result.testID = result.resultID = testID;
            testFixture.ruleEngine.queue( result );
        }

        testFixture.log.log( "Waiting till all done..." );

        testFixture.ruleEngine.waitForQueued();

        testFixture.log.log( "Evaluating quality..." );

        for ( int testID = 1; testID <= NUM_RESULTS; ++testID )
        {
            const int resultID = testID;
            RuleResults ruleResults = testFixture.getResultFor( resultID );
            ensure_equals( ruleResults.getSummaryMsg(), paulst::format( "configRuleForTest%d", testID ) );
            ensure_equals( ruleResults.getSummaryResultCode(), testID );
        }

        // Because of random Sleep patterns (refer to 'applyRules' implementation above), 
        // rule results should not have come through in the same sequence as they were queued.
        ensure_equals( testFixture.resultSequence.size(), NUM_RESULTS );
        std::vector<int> resultSequenceCopy = testFixture.resultSequence;
        ensure_equals( resultSequenceCopy.size(), NUM_RESULTS );
        std::sort( resultSequenceCopy.begin(), resultSequenceCopy.end() );
        ensure_not( equal( resultSequenceCopy.begin(), resultSequenceCopy.end(), testFixture.resultSequence.begin() ) );
    }

    template<>
	template<>
	void testRuleEngine::test<7>()
	{
		set_test_name("Rule caching");

		using namespace valc;

        UncontrolledResult result;
        result.testID = 2;
        result.resultID = 6;
        result.machineID = 8;
        result.projectID = 4;
        result.resultValue = 27.8;
        result.resultText = "dog";
        result.barcode = "chicken";
        result.dateAnalysed = TDateTime( 1998, 10, 30, 14, 33, 5, 10 );

		RuleEngineTestFixture testFixture;

        const char* configScript =
            " local rule                                        \n"
            "                                                   \n"
            " function onLoad(loadFunc)                         \n"
            "   local script = loadFunc('myRule')               \n"
            "   rule = load(script)                             \n"
            " end                                               \n"
            "                                                   \n"
            " function applyRules( qc )                         \n"
            "   local result = rule( qc )                       \n"
            "   return { result }, result.msg, result.resultCode\n"
            " end                                               \n";

        std::string ruleVersions[2] = {
            " return { resultCode = 29, rule = 'my rule', msg = 'Bob' } ",
            " return { resultCode = 72, rule = 'my rule', msg = 'Jim' } "} ;

        testFixture.ruleLoader.addRule( "configRule", configScript );
        testFixture.rulesConfig.specify( "configRule", 2, 8, 4 );

        for ( int i = 0; i < 2; ++i )
        {
            testFixture.ruleLoader.addRule( "myRule", ruleVersions[i] );

            try
            {
                testFixture.ruleEngine.queue( result );
                testFixture.ruleEngine.waitForQueued();
                
                ensure_equals( testFixture.countResults(), 1U );
                RuleResults ruleResults = testFixture.getResultFor( 6 );
                ensure_equals( ruleResults.getSummaryMsg(), std::string("Bob" ) );
                ensure_equals( ruleResults.getSummaryResultCode(), 29 );
            }
            catch( const Exception& e )
            {
                AnsiString ansiStr( e.Message.c_str() );
                ensure( ansiStr.c_str(), false );
            }
        }

        testFixture.ruleEngine.clearRulesCache();

        ensure_equals( testFixture.ruleLoader.loadRulesFor( "myRule" ), ruleVersions[1] );

        try
        {
            testFixture.ruleEngine.queue( result );
            testFixture.ruleEngine.waitForQueued();
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryMsg(), std::string("Jim" ) );
            ensure_equals( ruleResults.getSummaryResultCode(), 72 );
        }
        catch( const Exception& e )
        {
            AnsiString ansiStr( e.Message.c_str() );
            ensure( ansiStr.c_str(), false );
        }


    }

    template<>
	template<>
	void testRuleEngine::test<8>()
	{
		set_test_name("Multi-rule");

		using namespace valc;

        UncontrolledResult result;
        result.testID = 2;
        result.resultID = 6;
        result.machineID = 8;
        result.projectID = 4;
        result.resultValue = 27.8;
        result.resultText = "dog";
        result.barcode = "chicken";
        result.dateAnalysed = TDateTime( 1998, 10, 30, 14, 33, 5, 10 );

        const char* configScript =
            " rules = {}                                        \n"
            " ruleNames = { 'a', 'b', 'c', 'd' }                \n"
            " context = {}                                      \n"
            "                                                   \n"
            " function onLoad(loadFunc)                         \n"
            "   for i, ruleName in ipairs(ruleNames) do         \n"
            "      local script = loadFunc(ruleName)            \n"
            "      rules[ruleName] = load(script)               \n"
            "   end                                             \n"
            " end                                               \n"
            "                                                   \n"
            " function applyRules( qc )                         \n"
            "   local results = {}                              \n"
            "   context.qc = qc                                 \n"
            "   context.resultCode = 0                          \n"
            "   context.msg = ''                                \n"
            "   for name, rule in pairs(rules) do               \n"
            "     local result = rule()                         \n"
            "     table.insert( results, result )               \n"
            "   end                                             \n"
            "                                                   \n"
            "   return results, context.msg, context.resultCode \n"
            " end                                               \n";

        const char* rule =
            " context.resultCode = context.resultCode + 1       \n"
            " context.msg        = context.msg .. 'x'           \n"
            " return { resultCode = 2, rule = '?', msg = '!' }  \n";

		RuleEngineTestFixture testFixture( 10, 999, true );
        testFixture.ruleLoader.addRule( "configRule", configScript );
        testFixture.ruleLoader.addRule( "a", rule );
        testFixture.ruleLoader.addRule( "b", rule );
        testFixture.ruleLoader.addRule( "c", rule );
        testFixture.ruleLoader.addRule( "d", rule );
        testFixture.rulesConfig.specify( "configRule", 2, 8, 4 );
        
        try
        {
            testFixture.ruleEngine.queue( result );
            testFixture.ruleEngine.waitForQueued();
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryMsg(), std::string("xxxx") );
            ensure_equals( ruleResults.getSummaryResultCode(), 4 );
            ensure_equals( std::distance( ruleResults.begin(), ruleResults.end() ), 4U );
        }
        catch( const Exception& e )
        {
            AnsiString ansiStr( e.Message.c_str() );
            ensure( ansiStr.c_str(), false );
        }
    }

};

#endif

