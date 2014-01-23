#ifndef QCRULEENGINETESTH
#define QCRULEENGINETESTH

#include "AcquireCriticalSection.h"
#include <algorithm>
#include "API.h"
#include "Config.h"
#include "ConsoleWriter.h"
#include "CritSec.h"
#include <cstdio>
#include "Gate.h"
#include "Gates.h"
#include <iterator>
#include <iostream>
#include "LoggingService.h"
#include <map>
#include <memory>
#include "MockConfig.h"
#include "MockConnectionFactory.h"
#include "MockRuleEngineConnectionFactory.h"
#include "MockRuleLoader.h"
#include "NoLogging.h"
#include "RuleEngine.h"
#include "RuleLoader.h"
#include "RulesConfigUsingMap.h"
#include <set>
#include "StrUtil.h"
#include <SysUtils.hpp>
#include <tut.h>

void waitForQueued( valc::RuleEngine& ruleEngine, long millis = 5000 )
{
    if ( ! ruleEngine.waitForQueued( millis ) )
    {
        throw Exception( L"WaitForSingleObject returned a value other than WAIT_OBJECT_0" );
    }
}

valc::UncontrolledResult aResult()
{
    valc::UncontrolledResult r;
    r.testID = 2;
    r.resultID = 6;
    r.machineID = 8;
    r.projectID = 4;
    r.resultValue = 27.8;
    r.resultText = "dog";
    r.barcode = "chicken";
    r.dateAnalysed = TDateTime( 1998, 10, 30, 14, 33, 5, 10 );
    r.actionFlag = '0';
    return r;
}

const char* ruleThreeTwoS =
            " local result = { resultCode = 1, rule = '3:2s', msg = '' }                                                \n"
            " local tooHigh = function ( result, mean, stddev ) return ( result > ( mean + ( 2 * stddev ) ) ) end       \n"
            " local tooLow  = function ( result, mean, stddev ) return ( result < ( mean - ( 2 * stddev ) ) ) end       \n"
            " local isFailure = nil                                                                                          \n"
            " if ( tooHigh( context.qc.resultValue, context.gate.mean, context.gate.stdDev ) ) then isFailure = tooHigh end  \n"
            " if ( tooLow ( context.qc.resultValue, context.gate.mean, context.gate.stdDev ) ) then assert( not(isFailure) ) isFailure = tooLow end \n"
            "                                                                                                           \n"
            " if ( isFailure ) then                                                                                     \n"
            "    local beyondFirst = false                                                                              \n"
            "    local failures = 1                                                                                     \n"
            "    local passes   = 0                                                                                     \n"
            "    local tested   = 1                                                                                     \n"
            "    for i, elem in ipairs( context.sequence ) do                                                           \n"
            "           if (  beyondFirst ) then                                                                        \n"
            "               tested = tested + 1                                                                         \n"
            "               log( string.format( 'Testing result \%d...', elem.resultID ) )                              \n"
            "               if ( isFailure( elem.resultValue, elem.mean, elem.stdDev ) ) then                           \n"
            "                   failures = failures + 1                                                                 \n"
            "                   if ( failures == 3 ) then                                                               \n"
            "                       break                                                                               \n"
            "                   end                                                                                     \n"
            "               else                                                                                        \n"
            "                   log( string.format( 'Passed: value \%f; mean: \%f; stddev: \%f', elem.resultValue, elem.mean, elem.stdDev ) )\n"
            "                   passes = passes + 1                                                                     \n"
            "                   break -- No need to continue if a result has passed the test                            \n"
            "               end                                                                                         \n"
            "           else                                                                                            \n"
            "               beyondFirst = ( elem.resultID == context.qc.resultID )                                      \n"
            "           end                                                                                             \n"
            "    end                                                                                                    \n"
            "    log ( string.format( 'failures: \%d; passes: \%d; tested: \%d', failures, passes, tested) )            \n"
            "    if ( failures == 3 ) then                                                                              \n"
            "       result.resultCode = FAIL                                                                            \n"
            "    elseif ( ( tested < 3 ) and ( passes == 0 ) ) then                                                     \n"
            "       result.resultCode = ERROR                                                                           \n"
            "       result.msg = 'Too few previous QC results available to apply rule'                                  \n"
            "    end                                                                                                    \n"
            " end                                                                                                       \n"
            " return result                                                                                             \n";

const char* ruleConfigScriptApplyingRuleThreeTwoS =
            " rule = function () end                            \n"
            " context = {}                                      \n"
            " sequenceSQL = 'QC result sequence \%d \%d'        \n"
            " sqlType = { integer = 0, string = 1, real = 2 }   \n"
            " sqlTypes = { sqlType.integer, sqlType.string, sqlType.real, sqlType.string, \n"
            "               sqlType.string,  sqlType.string, sqlType.integer, sqlType.real, sqlType.real, sqlType.integer }  \n"
            "                                                   \n"
            " function onLoad(loadFunc)                         \n"
            "      local script = loadFunc('3:2s')              \n"
            "      rule, errorMsg = load(script)                \n"
            "      if ( errorMsg ) then error( errorMsg ) end   \n"
            "      local connectionHandle = openConnection( config('db connection string') ) \n"
            "      local sql = string.format( sequenceSQL, TEST_ID, MACHINE_ID )      \n"
            "      local queryHandle = executeQuery( sql, sqlTypes, connectionHandle ) \n"
            "      local row = fetchNextRow( queryHandle )      \n"
            "      context.sequence = {}                        \n"
            "      while row do                                 \n"
            "          local elem = {}                          \n"
            "          elem.resultID    = row[1]                \n"
            "          elem.barcode     = row[2]                \n"
            "          elem.resultValue = row[3]                \n"
            "          elem.resultText  = row[4]                \n"
            "          elem.actionFlag  = row[5]                \n"
            "          elem.dbName      = row[6]                \n"
            "          elem.gateID      = row[7]                \n"
            "          elem.mean        = row[8]                \n"
            "          elem.stdDev      = row[9]                \n"
            "          elem.source      = row[10]               \n"
            "          elem.testID      = TEST_ID               \n"
            "          elem.machineID   = MACHINE_ID            \n"
            "          table.insert( context.sequence, elem )   \n"
            "          row = fetchNextRow( queryHandle )        \n"
            "      end                                          \n"
            "      closeQuery( queryHandle )                    \n"
            "      closeConnection( connectionHandle )          \n"
            " end                                               \n"
            "                                                   \n"
            " function applyRules( qc, currentGateIfAny )       \n"
            "   assert( currentGateIfAny )                      \n"
            "   local results = {}                              \n"
            "   context.qc = qc                                 \n"
            "   context.gate = currentGateIfAny                 \n"
            "   local result = rule()                           \n"
            "   table.insert( results, result )                 \n"
            "   return { results, result.msg, result.resultCode, context.gate.id } \n"
            " end                                               \n";


struct PrintRuleResult
{
    void operator()( const valc::RuleResult& r )
    {
        std::printf( "%d/%s/%s\n", r.resultCode, r.rule.c_str(), r.msg.c_str() );
    }
};

valc::RuleResult buildRuleResult( valc::ResultCode resultCode, const std::string& rule, const std::string& msg )
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


class MockGates : public valc::Gates
{
private:
    std::auto_ptr<valc::Gate> m_gate;
public:
    std::vector< valc::UncontrolledResult > resultsForWhichGatesRequested;
    MockGates() {}
    void setGate( const valc::Gate& g ) { m_gate.reset( new valc::Gate(g) ); }
    const valc::Gate* getActiveGateFor( const valc::UncontrolledResult& r ) { resultsForWhichGatesRequested.push_back( r ); return m_gate.get(); }
};

namespace tut
{
	class RuleEngineTestFixture : public valc::RuleResultPublisher
    {
    private:
        typedef std::map< int, valc::RuleResults > ResultMap;
        ResultMap resultMap;
        paulst::CritSec m_critSec;


    public:

        valc::RuleLoader                ruleLoader;
        valc::RulesConfigUsingMap       rulesConfig;
        MockRuleEngineConnectionFactory connectionFactory;
        valc::RuleEngine                ruleEngine;
        std::vector< int >              resultSequence;
        paulst::LoggingService          log;
        valc::MockConfig                m_mockConfig;
        paulst::Config                  m_config;
       
        RuleEngineTestFixture(int numThreads = 10, bool logging = false, valc::RuleLoaderInterface* ruleLoaderOverride = NULL )
            : 
            ruleEngine(numThreads ), 
            log( logging ? (paulst::Writer*) new paulst::ConsoleWriter() : (paulst::Writer*)new NoLogging() ),
            m_config( m_mockConfig.toString() )
        {
            ruleEngine.setLog( &log );
            ruleEngine.setRulesConfig( &rulesConfig );
            ruleEngine.setRuleLoader( ruleLoaderOverride ? ruleLoaderOverride : (valc::RuleLoaderInterface*)&ruleLoader );
            ruleEngine.setResultPublisher( this );
            ruleEngine.setConnectionFactory( &connectionFactory );
            ruleEngine.setConfig( &m_config );
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

    typedef test_group<RuleEngineTestFixture, 15> RuleEngineTestGroup;
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
                "   assert( qc.actionFlag == '0' )          \n"
                "   assert( qc.dateAnalysed.year  == 1998 ) \n"
                "   assert( qc.dateAnalysed.month == 10 )   \n"
                "   assert( qc.dateAnalysed.day   == 30 )   \n"
                "   assert( qc.dateAnalysed.hour  == 14 )   \n"
                "   assert( qc.dateAnalysed.min   == 33 )   \n"
                "   assert( qc.dateAnalysed.sec   == 5  )   \n"
                "   assert( qc.dateAnalysed.msec  == 10 )   \n"
                "   local ruleResults = {}                  \n"
                "   local aResult = {resultCode = 2, rule = 'testRule', msg = 'hello'} \n"
                "   table.insert( ruleResults, aResult ) \n"
                "   local anotherResult = {resultCode = 1, rule = 'testRule2', msg = 'bye'} \n"
                "   table.insert( ruleResults, anotherResult ) \n"
                "   return { ruleResults, 'ok', 72, 'extra info', 99 }         \n"
                " end " );  

		RuleEngineTestFixture testFixture;
        testFixture.ruleLoader.add( RuleDescriptor( 1, 121, "myRule","" ), ruleScript );
        testFixture.rulesConfig.specify( 121, 2, 8, 4 );

        UncontrolledResult result = aResult();

        typedef std::vector<RuleResult> RawResults;

        try
        {
            testFixture.ruleEngine.queue( result );
            waitForQueued(testFixture.ruleEngine);
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryResultCode(), 72 );
            ensure_equals( ruleResults.getSummaryMsg(), std::string("ok") );
            ensure_equals( ruleResults.numExtraValues(), 2 );
            ensure_equals( ruleResults.getExtraValue(0), std::string("extra info") );
            ensure_equals( ruleResults.getExtraValue(1), std::string("99") );
            RawResults actualRawResults;
            std::copy( ruleResults.begin(), ruleResults.end(), std::back_inserter( actualRawResults ) );
            std::sort( actualRawResults.begin(), actualRawResults.end(), compareRuleResults );
            RawResults expectedRawResults;
            expectedRawResults.push_back( buildRuleResult( RESULT_CODE_BORDERLINE, "testRule", "hello" ) );// was 42
            expectedRawResults.push_back( buildRuleResult( RESULT_CODE_PASS, "testRule2", "bye" ) );// was 81
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

        UncontrolledResult result = aResult();

        MockConnectionFactory::reset();
        MockConnectionFactory::prime( "yoghurt", "49,\n" );
        for ( int resultID = 1; resultID < 10; ++resultID )
        {
            MockConnectionFactory::prime( paulst::format("resultID\%d", resultID )  , paulst::format("\%d,\n", resultID) );
        }

        const std::string ruleScript(// The following script uses a connection when it is loaded.
                " globalValue = 58                                          \n"
                "                                                           \n"
			    " sqlType = { integer = 0 }\n"
                "                                                           \n"
                " function onLoad()                                         \n"
                "     local hC = openConnection('myCon')                    \n"
                "     local hQ = executeQuery( 'yoghurt', {sqlType.integer}, hC ) \n"
                "     local row = fetchNextRow( hQ )                        \n"
                "     globalValue = globalValue + row[1]                    \n"
                " end                                                       \n"
                "                                                           \n"
                " function applyRules( qc )                                 \n"
                "     local hC = openConnection('myCon')                    \n"
                "     local hQ = executeQuery( 'resultID' .. qc.resultID, {sqlType.integer}, hC ) \n"
                "     local row = fetchNextRow( hQ )                        \n"
                "     assert( row[1] == qc.resultID )                       \n"
                "     globalValue = globalValue + 1                         \n"
                "     return { {}, 'ok', globalValue }                      \n"
                " end                                                       \n"
                "                                                           \n");


        int connectionsOpened = 0;
        int connectionsClosed = 0;

		{
		    RuleEngineTestFixture testFixture;
			testFixture.setConnectionCounter(&connectionsOpened, &connectionsClosed);
			testFixture.ruleLoader.add( RuleDescriptor( 1, 121, "myRule", "" ), ruleScript );
            testFixture.rulesConfig.specify( 121, 2, 8, 4 );

            try
            {
                for ( int resultID = 1; resultID < 10; ++resultID )
                {
                    result.resultID = resultID;

                    testFixture.ruleEngine.queue( result );
                }

                waitForQueued(testFixture.ruleEngine);

                ensure_equals( "There should be 9 results", testFixture.countResults(), 9 );
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

                ensure_equals( "Expected 9 distinct result codes", actualResults.size(), 9U );
                ensure( "Results as expected?", equal( actualResults.begin(), actualResults.end(), expectedResults.begin() ) );
            }
            catch( const Exception& e )
            {
                AnsiString ansiStr( e.Message.c_str() );
                printf( "Exception: %s\n", ansiStr.c_str() );
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
                "   return { {}, '', 1 }                 \n"
                " end " );  

		RuleEngineTestFixture testFixture;
        testFixture.ruleLoader.add( RuleDescriptor( 1, 121, "myRule", "" ), ruleScript );
        testFixture.rulesConfig.specify( 121, 2, 8, 4 );

        UncontrolledResult result = aResult();

        try
        {
            testFixture.ruleEngine.queue( result );
            waitForQueued(testFixture.ruleEngine);
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryResultCode(), RESULT_CODE_ERROR );
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

		RuleEngineTestFixture testFixture;

        UncontrolledResult result = aResult();

        try
        {
            testFixture.ruleEngine.queue( result );
            waitForQueued(testFixture.ruleEngine);
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryResultCode(), RESULT_CODE_ERROR );
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
                "   return { results, results[1].msg, results[1].resultCode } \n"
                " end                                               \n"
                );  

        const std::string someRule(
                " return { resultCode = context.mean, rule = 'this here rule', msg = 'success' } "
            );

		RuleEngineTestFixture testFixture;
        testFixture.ruleLoader.add( RuleDescriptor( 1, 121, "configRule", "" ), configRuleScript );
        testFixture.ruleLoader.add( RuleDescriptor( 2, 122, "someRule"  , "" ), someRule         );
        testFixture.rulesConfig.specify( 121, 2, 8, 4 );

        UncontrolledResult result = aResult();

        try
        {
            testFixture.ruleEngine.queue( result );
            waitForQueued(testFixture.ruleEngine);
            
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

		RuleEngineTestFixture testFixture( 10, false );
  
        const int NUM_RESULTS = 50;

        const char* configScriptTemplate  =
                    " function onLoad(loadFunc)                         \n"
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
                    "   return { {}, '\%s', \%d }                       \n"
                    " end                                               \n";

        for ( int testID = 1; testID <= NUM_RESULTS; ++testID )
        {
            const int ruleID = testID;
            const std::string configName = paulst::format( "configRuleForTest%d", testID );
            char config[1024];
            ensure( std::sprintf( config, configScriptTemplate, testID, testID, configName.c_str(), testID ) );
            testFixture.rulesConfig.specify( ruleID, testID, 8, 4 );// 8:machineID, 4:projectID
            testFixture.ruleLoader.add( RuleDescriptor( 1, ruleID, configName, "" ), config );
        }

        UncontrolledResult result = aResult();

        testFixture.log.logFormatted( "Queuing %d results", NUM_RESULTS );

        for ( int testID = 1; testID <= NUM_RESULTS; ++testID )
        {
            result.testID = result.resultID = testID;
            testFixture.ruleEngine.queue( result );
        }

        testFixture.log.log( "Waiting till all done..." );

        waitForQueued(testFixture.ruleEngine);

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

        UncontrolledResult result = aResult();

        MockRuleLoader ruleLoaderOverride;

		RuleEngineTestFixture testFixture( 10, false, &ruleLoaderOverride );

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
            "   return { { result }, result.msg, result.resultCode }\n"
            " end                                               \n";


        ruleLoaderOverride.impl.add( RuleDescriptor( 1, 121, "configScript", ""), configScript );
        ruleLoaderOverride.impl.add( RuleDescriptor( 2, 122, "myRule"      , "" ), 
            " return { resultCode = 29, rule = 'my rule', msg = 'Bob' } " );
        
        testFixture.rulesConfig.specify( 121, 2, 8, 4 );

        for ( int i = 0; i < 20; ++i )
        {
            testFixture.ruleEngine.queue( result );
            waitForQueued(testFixture.ruleEngine);
        }

        ensure_equals( ruleLoaderOverride.totalLoadRequests, 2 ); // ie one request for configScript and one for myRule

        testFixture.ruleEngine.clearRulesCache();

        testFixture.ruleEngine.queue( result );
        waitForQueued(testFixture.ruleEngine);

        ensure_equals( ruleLoaderOverride.totalLoadRequests, 4 ); // having cleared cache, had to reload configScript and myRule
    }

    template<>
	template<>
	void testRuleEngine::test<8>()
	{
		set_test_name("Multi-rule");

		using namespace valc;

        UncontrolledResult result = aResult();

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
            "   return { results, context.msg, context.resultCode } \n"
            " end                                               \n";

        const char* rule =
            " context.resultCode = context.resultCode + 1       \n"
            " context.msg        = context.msg .. 'x'           \n"
            " return { resultCode = 2, rule = '?', msg = '!' }  \n";

		RuleEngineTestFixture testFixture( 10, false );
        testFixture.ruleLoader.add( RuleDescriptor( 1, 121, "configRule", "" ), configScript );
        testFixture.ruleLoader.add( RuleDescriptor( 2, 122, "a", "" ), rule );
        testFixture.ruleLoader.add( RuleDescriptor( 3, 123, "b", "" ), rule );
        testFixture.ruleLoader.add( RuleDescriptor( 4, 124, "c", "" ), rule );
        testFixture.ruleLoader.add( RuleDescriptor( 5, 125, "d", "" ), rule );
        testFixture.rulesConfig.specify( 121, 2, 8, 4 );
        
        try
        {
            testFixture.ruleEngine.queue( result );
            waitForQueued(testFixture.ruleEngine);
            
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

    template<>
	template<>
	void testRuleEngine::test<9>()
	{
		set_test_name("Westgard 1:2s");
        // (Reject when control measurement exceeds the same mean plus 2s or the same mean minus 2s control limit.)
        // http://www.westgard.com/westgard-rules-and-multirules.htm

		using namespace valc;

        UncontrolledResult result = aResult();

        const char* configScript =
            " rule = function () end                            \n"
            " ruleNames = { '1:2s' }                            \n"
            " context = {}                                      \n"
            "                                                   \n"
            " function onLoad(loadFunc)                         \n"
            "      local script = loadFunc('1:2s')              \n"
            "      rule, errorMsg = load(script)                \n"
            "      if ( errorMsg ) then error( errorMsg ) end   \n"
            " end                                               \n"
            "                                                   \n"
            " function applyRules( qc )                         \n"
            "   local results = {}                              \n"
            "   context.qc = qc                                 \n"
            "   context.gate = getActiveGate(qc)                      \n"
            "   assert( math.floor(context.gate.mean) == 3 )    \n"
            "   assert( math.floor(context.gate.stdDev) == 0 )  \n"
            "   assert( context.gate.id == 23 )                 \n"
            "   assert( context.gate.source == 1 )              \n"
            "   local result = rule()                           \n"
            "   table.insert( results, result )                 \n"
            "   return { results, result.msg, result.resultCode, context.gate.id } \n"
            " end                                               \n";

        const char* rule =
            " local result = { resultCode = 1, rule = '1:2s', msg = '' }                                                        \n"
            " local tooHigh = function ( result, mean, stddev ) return ( result > ( mean + ( 2 * stddev ) ) ) end               \n"
            " local tooLow  = function ( result, mean, stddev ) return ( result < ( mean - ( 2 * stddev ) ) ) end               \n"
            " if ( tooHigh( context.qc.resultValue, context.gate.mean, context.gate.stdDev ) or                                 \n"
            "      tooLow ( context.qc.resultValue, context.gate.mean, context.gate.stdDev ) ) then result.resultCode = 0 end   \n"
            " return result                                                                                                     \n";

		RuleEngineTestFixture testFixture;
        testFixture.ruleLoader.add( RuleDescriptor( 1, 121, "configRule", "" ), configScript );
        testFixture.ruleLoader.add( RuleDescriptor( 2, 122, "1:2s"      , "" ), rule         );
        testFixture.rulesConfig.specify( 121, 2, 8, 4 );

        MockGates gates;
        gates.setGate( Gate( 3.4, 0.2, 23, 1 ) );
        testFixture.ruleEngine.setGates( &gates );

        try
        {
            testFixture.ruleEngine.queue( result );
            waitForQueued(testFixture.ruleEngine);
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryResultCode(), 0 );
            ensure_equals( std::distance( ruleResults.begin(), ruleResults.end() ), 1U );
            RuleResult ruleResult = *(ruleResults.begin());
            ensure_equals( ruleResult.rule, std::string("1:2s") );
            ensure_equals( ruleResults.getExtraValue(0), "23" );// the ID of the Gate
            ensure_equals( gates.resultsForWhichGatesRequested.size(), 2U );
            UncontrolledResult resultObtainedViaLUA = gates.resultsForWhichGatesRequested.at(0);
            ensure_equals( resultObtainedViaLUA.testID         , 2 );
            ensure_equals( resultObtainedViaLUA.resultID       , 6 );
            ensure_equals( resultObtainedViaLUA.machineID      , 8 );
            ensure_equals( resultObtainedViaLUA.resultValue    , 27.8 );
            ensure_equals( resultObtainedViaLUA.barcode        , std::string("chicken") );
            ensure_equals( resultObtainedViaLUA.projectID      , 4 );
            ensure_equals( resultObtainedViaLUA.actionFlag     , '0' );
            ensure( resultObtainedViaLUA.dateAnalysed == TDateTime( 1998, 10, 30, 14, 33, 5, 10 ) );
        }
        catch( const Exception& e )
        {
            AnsiString ansiStr( e.Message.c_str() );
            ensure( ansiStr.c_str(), false );
        }
    }

    template<>
	template<>
	void testRuleEngine::test<10>()
	{
		set_test_name("Westgard 1:2s - alternative to test case 9");
        // (Reject when control measurement exceeds the same mean plus 2s or the same mean minus 2s control limit.)
        // http://www.westgard.com/westgard-rules-and-multirules.htm

		using namespace valc;

        UncontrolledResult result = aResult();

        const char* configScript =
            " rule = function () end                            \n"
            " ruleNames = { '1:2s' }                            \n"
            " context = {}                                      \n"
            "                                                   \n"
            " function onLoad(loadFunc)                         \n"
            "      local script = loadFunc('1:2s')              \n"
            "      rule, errorMsg = load(script)                \n"
            "      if ( errorMsg ) then error( errorMsg ) end   \n"
            " end                                               \n"
            "                                                   \n"
            " function applyRules( qc, gate )                         \n"
            "   local results = {}                              \n"
            "   context.qc = qc                                 \n"
            "   context.gate = gate                             \n"
            "   assert( math.floor(gate.mean) == 3 )            \n"
            "   assert( math.floor(gate.stdDev) == 0 )          \n"
            "   assert( gate.id == 23 )                         \n"
            "   assert( gate.source == 1 )                      \n"
            "   local result = rule()                           \n"
            "   table.insert( results, result )                 \n"
            "   return { results, result.msg, result.resultCode, gate.id } \n"
            " end                                               \n";

        const char* rule =
            " local result = { resultCode = 1, rule = '1:2s', msg = '' }                                                        \n"
            " local tooHigh = function ( result, mean, stddev ) return ( result > ( mean + ( 2 * stddev ) ) ) end               \n"
            " local tooLow  = function ( result, mean, stddev ) return ( result < ( mean - ( 2 * stddev ) ) ) end               \n"
            " if ( tooHigh( context.qc.resultValue, context.gate.mean, context.gate.stdDev ) or                                 \n"
            "      tooLow ( context.qc.resultValue, context.gate.mean, context.gate.stdDev ) ) then result.resultCode = 0 end   \n"
            " return result                                                                                                     \n";

		RuleEngineTestFixture testFixture;
        testFixture.ruleLoader.add( RuleDescriptor( 1, 121, "configRule", "" ), configScript );
        testFixture.ruleLoader.add( RuleDescriptor( 2, 122, "1:2s"      , "" ), rule         );
        testFixture.rulesConfig.specify( 121, 2, 8, 4 );

        MockGates gates;
        gates.setGate( Gate( 3.4, 0.2, 23, 1 ) );
        testFixture.ruleEngine.setGates( &gates );

        try
        {
            testFixture.ruleEngine.queue( result );
            waitForQueued(testFixture.ruleEngine);
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryResultCode(), 0 );
            ensure_equals( std::distance( ruleResults.begin(), ruleResults.end() ), 1U );
            RuleResult ruleResult = *(ruleResults.begin());
            ensure_equals( ruleResult.rule, std::string("1:2s") );
            ensure_equals( ruleResults.getExtraValue(0), "23" );// the ID of the Gate
            ensure_equals( gates.resultsForWhichGatesRequested.size(), 1U );
        }
        catch( const Exception& e )
        {
            AnsiString ansiStr( e.Message.c_str() );
            ensure( ansiStr.c_str(), false );
        }
    }

    template<>
	template<>
	void testRuleEngine::test<11>()
	{
		set_test_name("Rule script queries and caches a sequence of results.");

		using namespace valc;

        UncontrolledResult result = aResult();

        MockConnectionFactory::prime( "QC result sequence 2 8",
            // resultID,barcode,result value, result text, action flag, db name, gate id, mean, stddev, source
            "  6       ,chicken,27.0        , 27.8       , 0          , ldbqc  , 972    , 24.3, 2.7   , 1,\n"
            "  7       ,kebab  ,32.0        , 32.7       , 0          , ldbqc  , 1023   , 30.4, 1.6   , 1,\n" );

        const char* configScript =
            " context = {}                                      \n"
            " sequenceSQL = 'QC result sequence \%d \%d'        \n"
            " sqlType = { integer = 0, string = 1, real = 2 }   \n"
            " sqlTypes = { sqlType.integer, sqlType.string, sqlType.real, sqlType.string, \n"
            "               sqlType.string,  sqlType.string, sqlType.integer, sqlType.real, sqlType.real, sqlType.integer }  \n"
            "                                                   \n"
            " function onLoad(loadFunc)                         \n"
            "      local connectionHandle = openConnection( 'my connection string' ) \n"
            "      local sql = string.format( sequenceSQL, TEST_ID, MACHINE_ID )      \n"
            "      assert( sql == 'QC result sequence 2 8' )    \n"
            "      local queryHandle = executeQuery( sql, sqlTypes, connectionHandle ) \n"
            "      local row = fetchNextRow( queryHandle )      \n"
            "      context.sequence = {}                        \n"
            "      while row do                                 \n"
            "          local elem = {}                          \n"
            "          elem.resultID    = row[1]                \n"
            "          elem.barcode     = row[2]                \n"
            "          elem.resultValue = row[3]                \n"
            "          elem.resultText  = row[4]                \n"
            "          elem.actionFlag  = row[5]                \n"
            "          elem.dbName      = row[6]                \n"
            "          elem.gateID      = row[7]                \n"
            "          elem.mean        = row[8]                \n"
            "          elem.stdDev      = row[9]                \n"
            "          elem.source      = row[10]               \n"
            "          elem.testID      = TEST_ID               \n"
            "          elem.machineID   = MACHINE_ID            \n"
            "          table.insert( context.sequence, elem )   \n"
            "          row = fetchNextRow( queryHandle )        \n"
            "      end                                          \n"
            "      closeQuery( queryHandle )                    \n"
            "      closeConnection( connectionHandle )          \n"
            " end                                               \n"
            "                                                   \n"
            " function applyRules( qc )                         \n"
            "   local resultCode = 0                            \n"
            "   local resultMsg = ''                            \n"
            "   for i, elem in ipairs( context.sequence ) do    \n"
            "       resultCode = resultCode + elem.resultValue  \n"    
            "       resultMsg = resultMsg .. elem.barcode       \n"
            "   end                                             \n"
            "   return { {}, resultMsg, resultCode }            \n"
            " end                                               \n";

		RuleEngineTestFixture testFixture;
        testFixture.ruleLoader.add( RuleDescriptor( 1, 121, "configRule", "" ), configScript );
        testFixture.rulesConfig.specify( 121, 2, 8, 4 );

        try
        {
            testFixture.ruleEngine.queue( result );
            waitForQueued(testFixture.ruleEngine);
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryResultCode(), 27 + 32 );
            ensure_equals( ruleResults.getSummaryMsg(), std::string("chickenkebab") );
            ensure_equals( std::distance( ruleResults.begin(), ruleResults.end() ), 0U );
        }
        catch( const Exception& e )
        {
            AnsiString ansiStr( e.Message.c_str() );
            ensure( ansiStr.c_str(), false );
        }
    }

    template<>
	template<>
	void testRuleEngine::test<12>()
	{
		set_test_name("Building on test case 11: applying a rule (3:2s) to a sequence - take 1: when the sequence is too small");
        // (Reject when the last 3 control measurement exceeds the same mean plus 2s or the same mean minus 2s control limit.)
        // http://www.westgard.com/westgard-rules-and-multirules.htm

		using namespace valc;

        UncontrolledResult result = aResult();

        MockConnectionFactory::reset();

        MockConnectionFactory::prime( "QC result sequence 2 8",
            // resultID,barcode,result value, result text, action flag, db name, gate id, mean, stddev, source
            "  6       ,chicken,27.8        , 27.8       , 0          , ldbqc  , 23     , 3.4 , 0.2   , 1,\n" );
                
		RuleEngineTestFixture testFixture( 10, false );
        testFixture.ruleLoader.add( RuleDescriptor( 1, 121, "configRule", "" ), ruleConfigScriptApplyingRuleThreeTwoS );
        testFixture.ruleLoader.add( RuleDescriptor( 2, 122, "3:2s"      , "" ), ruleThreeTwoS );
        testFixture.rulesConfig.specify( 121, 2, 8, 4 );

        MockGates gates;
        gates.setGate( Gate( 3.4, 0.2, 23, 1 ) );
        testFixture.ruleEngine.setGates( &gates );

        try
        {
            testFixture.ruleEngine.queue( result );
            waitForQueued(testFixture.ruleEngine);
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryResultCode(), 3  );//RuleResults::RESULT_CODE_ERROR
            ensure_equals( ruleResults.getSummaryMsg(), std::string("Too few previous QC results available to apply rule") );
            ensure_equals( std::distance( ruleResults.begin(), ruleResults.end() ), 1U );
            RuleResult ruleResult = *(ruleResults.begin());
            ensure_equals( ruleResult.rule, std::string("3:2s") );
        }
        catch( const Exception& e )
        {
            AnsiString ansiStr( e.Message.c_str() );
            ensure( ansiStr.c_str(), false );
        }
    }

    template<>
	template<>
	void testRuleEngine::test<13>()
	{
		set_test_name("Building on test case 11: applying a rule (3:2s) to a sequence - take 2: when the first fails and the second succeeds.");
        // (Reject when the last 3 control measurement exceeds the same mean plus 2s or the same mean minus 2s control limit.)
        // http://www.westgard.com/westgard-rules-and-multirules.htm

		using namespace valc;

        UncontrolledResult result = aResult();

        MockConnectionFactory::reset();

        MockConnectionFactory::prime( "QC result sequence 2 8",
            // resultID,barcode,result value, result text, action flag, db name, gate id, mean, stddev, source
            "  7       ,tofu   ,27.8        , 27.8       , 0          , ldbqc  , 22     , 3.4 , 0.2   , 1,\n" 
            "  6       ,chicken,27.8        , 27.8       , 0          , ldbqc  , 23     , 3.4 , 0.2   , 1,\n" 
            "  5       ,lamb   ,22.8        , 22.8       , 0          , ldbqc  , 24     , 22.8, 0.2   , 1,\n" 
            "  4       ,fish   ,22.8        , 22.8       , 0          , ldbqc  , 25     , 22.8, 0.2   , 1,\n" 
            );
                
		RuleEngineTestFixture testFixture( 10, false );
        testFixture.ruleLoader.add( RuleDescriptor( 1, 121, "configRule", "" ), ruleConfigScriptApplyingRuleThreeTwoS );
        testFixture.ruleLoader.add( RuleDescriptor( 2, 122, "3:2s"      , "" ), ruleThreeTwoS );
        testFixture.rulesConfig.specify( 121, 2, 8, 4 );

        MockGates gates;
        gates.setGate( Gate( 3.4, 0.2, 23, 1 ) );
        testFixture.ruleEngine.setGates( &gates );

        try
        {
            testFixture.ruleEngine.queue( result );
            waitForQueued(testFixture.ruleEngine);
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryResultCode(), 1  );//RuleResults::RESULT_CODE_PASS
            ensure_equals( std::distance( ruleResults.begin(), ruleResults.end() ), 1U );
            RuleResult ruleResult = *(ruleResults.begin());
            ensure_equals( ruleResult.rule, std::string("3:2s") );
        }
        catch( const Exception& e )
        {
            AnsiString ansiStr( e.Message.c_str() );
            ensure( ansiStr.c_str(), false );
        }
    }

    template<>
	template<>
	void testRuleEngine::test<14>()
	{
		set_test_name("Building on test case 11: applying a rule (3:2s) to a sequence - take 3: three consecutive failures.");
        // (Reject when the last 3 control measurement exceeds the same mean plus 2s or the same mean minus 2s control limit.)
        // http://www.westgard.com/westgard-rules-and-multirules.htm

		using namespace valc;

        UncontrolledResult result = aResult();

        MockConnectionFactory::reset();

        MockConnectionFactory::prime( "QC result sequence 2 8",
            // resultID,barcode,result value, result text, action flag, db name, gate id, mean, stddev, source
            "  7       ,tofu   ,27.8        , 27.8       , 0          , ldbqc  , 22     , 3.4 , 0.2   , 1,\n" 
            "  6       ,chicken,27.8        , 27.8       , 0          , ldbqc  , 23     , 3.4 , 0.2   , 1,\n" 
            "  5       ,lamb   ,22.8        , 22.8       , 0          , ldbqc  , 24     , 21.8, 0.2   , 1,\n" 
            "  4       ,fish   ,22.8        , 22.8       , 0          , ldbqc  , 25     , 21.8, 0.2   , 1,\n" 
            );
                
		RuleEngineTestFixture testFixture( 10, false );
        testFixture.ruleLoader.add( RuleDescriptor( 1, 121, "configRule", "" ), ruleConfigScriptApplyingRuleThreeTwoS );
        testFixture.ruleLoader.add( RuleDescriptor( 2, 122, "3:2s"      , "" ), ruleThreeTwoS );
        testFixture.rulesConfig.specify( 121, 2, 8, 4 );

        MockGates gates;
        gates.setGate( Gate( 3.4, 0.2, 23, 1 ) );
        testFixture.ruleEngine.setGates( &gates );

        try
        {
            testFixture.ruleEngine.queue( result );
            waitForQueued(testFixture.ruleEngine);
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryResultCode(), 0  );//RuleResults::RESULT_CODE_FAIL
            ensure_equals( std::distance( ruleResults.begin(), ruleResults.end() ), 1U );
            RuleResult ruleResult = *(ruleResults.begin());
            ensure_equals( ruleResult.rule, std::string("3:2s") );
        }
        catch( const Exception& e )
        {
            AnsiString ansiStr( e.Message.c_str() );
            ensure( ansiStr.c_str(), false );
        }
    }

    template<>
	template<>
	void testRuleEngine::test<15>()
	{
		set_test_name("Building on test case 11: applying a rule (3:2s) to a sequence - take 3: third is other side of mean.");
        // (Reject when the last 3 control measurement exceeds the same mean plus 2s or the same mean minus 2s control limit.)
        // http://www.westgard.com/westgard-rules-and-multirules.htm

		using namespace valc;

        UncontrolledResult result = aResult();

        MockConnectionFactory::reset();

        MockConnectionFactory::prime( "QC result sequence 2 8",
            // resultID,barcode,result value, result text, action flag, db name, gate id, mean, stddev, source
            "  7       ,tofu   ,27.8        , 27.8       , 0          , ldbqc  , 22     , 3.4 , 0.2   , 1,\n" 
            "  6       ,chicken,27.8        , 27.8       , 0          , ldbqc  , 23     , 3.4 , 0.2   , 1,\n" 
            "  5       ,lamb   ,22.8        , 22.8       , 0          , ldbqc  , 24     , 21.8, 0.2   , 1,\n" 
            "  4       ,fish   ,20.8        , 20.8       , 0          , ldbqc  , 25     , 21.8, 0.2   , 1,\n" 
            );
                
		RuleEngineTestFixture testFixture( 10, false );
        testFixture.ruleLoader.add( RuleDescriptor( 1, 121, "configRule", "" ), ruleConfigScriptApplyingRuleThreeTwoS );
        testFixture.ruleLoader.add( RuleDescriptor( 2, 122, "3:2s"      , "" ), ruleThreeTwoS );
        testFixture.rulesConfig.specify( 121, 2, 8, 4 );

        MockGates gates;
        gates.setGate( Gate( 3.4, 0.2, 23, 1 ) );
        testFixture.ruleEngine.setGates( &gates );

        try
        {
            testFixture.ruleEngine.queue( result );
            waitForQueued(testFixture.ruleEngine);
            
            ensure_equals( testFixture.countResults(), 1U );
            RuleResults ruleResults = testFixture.getResultFor( 6 );
            ensure_equals( ruleResults.getSummaryResultCode(), 1  );//RuleResults::RESULT_CODE_PASS
            ensure_equals( std::distance( ruleResults.begin(), ruleResults.end() ), 1U );
            RuleResult ruleResult = *(ruleResults.begin());
            ensure_equals( ruleResult.rule, std::string("3:2s") );
        }
        catch( const Exception& e )
        {
            AnsiString ansiStr( e.Message.c_str() );
            ensure( ansiStr.c_str(), false );
        }
    }
};

#endif

