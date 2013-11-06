#ifndef FORCERELOAD_TEST_H 
#define FORCERELOAD_TEST_H

#include <algorithm>
#include "API.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/variant/get.hpp>
#include "ConsoleWriter.h"
#include <cstdio>
#include <cwchar>
#include "CSVIterator.h"
#include <iterator>
#include "LoggingService.h"
#include <map>
#include "MockConnection.h"
#include "MockConnectionFactory.h"
#include "MockConfig.h"
#include "NoLogging.h"
#include <set>
#include "StringBackedCursor.h"
#include "StrUtil.h"
#include <tut.h>
#include <vector>


void pipeSeparatedFieldValues( const std::string& record, paulstdb::RowValues& out )
{
    typedef paulst::CSVIterator<'|'> CSVIter;
	std::copy( CSVIter(record), CSVIter(), std::back_inserter( out ) );
}

/*
These tests use MockConnectionFactory.
*/

bool runIDEquals( const std::string& runID, const valc::TestResult* result )
{
    return runID == result->getSampleRunID();
}

bool hasResultWithRunID( const std::string& runID, const valc::WorklistEntry* wle )
{
    using namespace valc;
    Range<TestResultIterator> resultRange = wle->getTestResults();
    return std::count_if( resultRange.first, resultRange.second, boost::bind( runIDEquals, runID, _1 ) );
}

const valc::TestResult* testResultFor( const valc::WorklistEntry* wle )
{
    using namespace valc;
    Range<TestResultIterator> resultRange = wle->getTestResults();
    unsigned int numResults = std::distance( resultRange.first, resultRange.second );
    if (  numResults != 1 )
    {
        std::string testIDs;
        if ( numResults )
        {
            testIDs = " .TestResult IDs: ";

            for ( TestResultIterator i = resultRange.first; i != resultRange.second; ++i )
            {
                testIDs.append( AnsiString( (*i)->getID() ).c_str() );
                testIDs.append(",");
            }
        }
        char buffer[1024];
        std::sprintf( buffer, "Worklist entry %d has %d test result(s)%s", wle->getID(), numResults, testIDs.c_str() );
        throw Exception( buffer );
    }
    return *(resultRange.first);
}

const valc::WorklistEntry* worklistEntry( valc::Range<valc::WorklistEntryIterator>& worklistEntries, int id )
{
    using namespace valc;
    const WorklistEntry* wle = 0;
    for ( WorklistEntryIterator i = worklistEntries.first; i != worklistEntries.second; ++i )
    {
        if ( (*i)->getID() == id )
        {
            wle = *i;
            break;
        }
    }

    if ( wle == 0 )
    {
        throw Exception( "Failed to find expected worklist entry." );
    }

    return wle;
}

class ExceptionListener : public valc::DBUpdateExceptionHandlingPolicy
{
public:

    void handleException( const std::string& msg )
    {
        m_msgs.push_back( msg );
    }

    bool noExceptions() const
    {
        return m_msgs.empty();
    }

private:
    std::vector< std::string > m_msgs;
};

class UserWarnings : public valc::UserAdvisor
{
public:
    std::vector< std::string > messages;

    UserWarnings() {}

    // NB: might want some concurrency protection here!
    void advise( const std::string& msg ) { messages.push_back( msg ); }
};

bool sortOnBuddySampleID( const valc::BuddyDatabaseEntry& e1, const valc::BuddyDatabaseEntry& e2 )
{
    return e1.buddy_sample_id < e2.buddy_sample_id;
}

bool sortOnWorklistEntryID( const valc::WorklistEntry* w1, const valc::WorklistEntry* w2 )
{
    return w1->getID() < w2->getID();
}

namespace tut
{
	class ForceReloadTestFixture
    {
    public:
        static const int LOCAL_MACHINE_ID = -1019349;
        static const int USER_ID          = 1234;

        valc::SnapshotPtr s;
        paulst::LoggingService*         log;
        valc::MockConfig                config;
        UserWarnings                    userWarnings;
        const bool                      logToConsole;

        ForceReloadTestFixture( bool initialise = false, bool suppressLogMessages = true )
            : log(0), logToConsole( ! suppressLogMessages )
        {
            if ( initialise )
            {
                init();
            }
        }

        void init()
        {
            paulst::Writer* logWriter = 0;
            if ( logToConsole )
            {
                logWriter = new paulst::ConsoleWriter();
            }
            else
            {
                logWriter = new NoLogging();
            }
            log = new paulst::LoggingService( logWriter );
            valc::InitialiseApplicationContext( LOCAL_MACHINE_ID, USER_ID, config.toString(), log );
            s   = valc::Load( &userWarnings );
        }
        
        ~ForceReloadTestFixture()
        {
            valc::DeleteApplicationContext();
            delete log;
        }

        valc::SnapshotPtr operator->() const
        {
            return s;
        }
    };

    typedef test_group<ForceReloadTestFixture, 17> ForceReloadTestGroup;
	ForceReloadTestGroup testGroupForceReload( "ForceReload tests");
	typedef ForceReloadTestGroup::object testForceReload;

	void ensureNumResults( const valc::WorklistEntry* wle, int numExpected )
	{
		valc::Range<valc::TestResultIterator> range = wle->getTestResults();
		ensure( std::distance( range.first, range.second ) == numExpected );
	}


	template<>
	template<>
	void testForceReload::test<1>()
	{
		set_test_name("ForceReload - no data!");

		using namespace valc;

        ForceReloadTestFixture s(true);

		ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );
		ensure_equals( std::distance( s->localBegin(), s->localEnd() ), 0 );
	}

	template<>
	template<>
	void testForceReload::test<2>()
	{

		set_test_name("ForceReload - 3 tests queued, single sample.");

		using namespace valc;

        MockConnectionFactory::reset();

		MockConnectionFactory::clusters = "-1019430,\n";
		MockConnectionFactory::projects = "-832455,reveal,ldb25,\n";
		MockConnectionFactory::worklist = 
//rec  machine  barcode   test     group     c sample project p prof                  timestamp           seq s dil   result
"24110,-1019430,118502164,-1031390,-12744865,0,417128,-832455,0,EDTA_1 analysis other,21-06-2013 11:07:57,39,Q,0.000,0,\n"
"24111,-1019430,118502164,-1031389,-12744865,0,417128,-832455,0,EDTA_1 analysis other,21-06-2013 11:07:57,34,Q,0.000,0,\n"
"24112,-1019430,118502164,-1031388,-12744865,0,417128,-832455,0,EDTA_1 analysis other,21-06-2013 11:07:57,35,Q,0.000,0,\n"
			;

        ForceReloadTestFixture s(true);

		ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 1 );
		ensure_equals( std::distance( s->localBegin(), s->localEnd() ), 0 );

		QueuedSample qs = *(s->queueBegin());

		ensure_equals( qs.getSampleDescriptor(), "417128/-832455" );

		Range<WorklistEntryIterator> wles = s->getWorklistEntries( qs.getSampleDescriptor() );

		ensure( std::distance( wles.first, wles.second ) == 3 );
	}

	template<>
	template<>
	void testForceReload::test<3>()
	{
		set_test_name("ForceReload - 2 samples, one with queued only, other with some results.");

		using namespace valc;

        MockConnectionFactory::reset();

		MockConnectionFactory::clusters = "-1019430,\n";
		MockConnectionFactory::projects = "-832455,reveal,ldb25,\n";
		MockConnectionFactory::worklist =
//rec  machine  barcode   test     group     c sample project p prof                  timestamp           seq s dil   result
"-36832,-1019430,118507097,-1031384,-12750391,0,195199,-832455,0,EDTA_1 analysis other,27-06-2013 10:56:14,11,Q,0.000,0,\n"
"-36831,-1019430,118507097,-1031385,-12750391,0,195199,-832455,0,EDTA_1 analysis other,27-06-2013 10:56:14,19,Q,0.000,0,\n"
"-36830,-1019430,118507097,-1031386,-12750391,0,195199,-832455,0,EDTA_1 analysis other,27-06-2013 10:56:14,10,Q,0.000,0,\n"
"-36829,-1019430,118507097,-1031388,-12750391,0,195199,-832455,0,EDTA_1 analysis other,27-06-2013 10:56:14,12,Q,0.000,0,\n"
"-36828,-1019430,118507097,-1031389,-12750391,0,195199,-832455,0,EDTA_1 analysis other,27-06-2013 10:56:14,13,Q,0.000,0,\n"
"-36827,-1019430,118507097,-1031390,-12750391,0,195199,-832455,0,EDTA_1 analysis other,27-06-2013 10:56:14,14,Q,0.000,0,\n"
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,882431,\n"
"-36847,-1019430,118507091,-1031388,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,12,C,0.000,882429,\n"
"-36846,-1019430,118507091,-1031389,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,13,C,0.000,882430,\n"
"-36848,-1019430,118507091,-1031386,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,10,C,0.000,882427,\n"
// Note that the last 2 have zero for buddy_result_id
//
// the buddy_sample_id for buddy_result_id 882429 is: 882290
			;
		MockConnectionFactory::buddyDB =
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed      ,restx,update when        ,
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882427,-1031386,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882430,-1031389,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,,,,,\n"
			;

        ForceReloadTestFixture s( true );

        const bool blockTillNoPendingUpdates = true;
        ExceptionListener exceptionListener;

        s->runPendingDatabaseUpdates( &exceptionListener, blockTillNoPendingUpdates );

        ensure( exceptionListener.noExceptions() );
        ensure( "Expected 1 new sample-run", 1 == MockConnection::totalNewSampleRuns() );
        const int actualUpdatesOfSampleRunID = MockConnection::totalUpdatesForSampleRunIDOnBuddyDatabase();
        ensure_equals( actualUpdatesOfSampleRunID, 1 );

		ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 1 );
		ensure_equals( std::distance( s->localBegin(), s->localEnd() ), 1 );

		LocalEntry localEntry = *(s->localBegin());

		LocalRun lr = boost::get<LocalRun>(localEntry);

		Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

		ensure( std::distance( wles.first, wles.second ) == 4U );

        // Check that the TestResult runID matches the runID of the local run
        const WorklistEntry* wle = *(wles.first);
        Range<TestResultIterator> testResults = wle->getTestResults();
        ensure( 1 == std::distance( testResults.first, testResults.second ) );
        const TestResult* result = *(testResults.first);
        ensure( result->getSampleRunID() == lr.getRunID() );
		std::for_each( wles.first, wles.second, boost::bind( ensureNumResults, _1, 1 ) );
	}

	template<>
	template<>
	void testForceReload::test<4>()
	{
		set_test_name("ForceReload - 1 sample, 1 test, 2 results. (Use Case 1, Type B)");

		using namespace valc;

		MockConnectionFactory::reset();

		MockConnectionFactory::clusters = "-1019430,\n";
		MockConnectionFactory::projects = "-832455,reveal,ldb25,\n";
		MockConnectionFactory::worklist =
//rec  machine  barcode   test     group     c sample project p prof                  timestamp           seq s dil   result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
			;
		MockConnectionFactory::buddyDB =
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,res ,a,date analysed      ,restx,update when      ,cbw
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.8 ,0,27-06-2013 11:57:47,1.8,27-06-2013 10:57:49,-36845,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882432,-1031390,1.3 ,0,27-06-2013 11:57:47,1.3,27-06-2013 10:57:49,-36845,,,,,,\n"
			;

        ForceReloadTestFixture s( true );

        const bool blockTillNoPendingUpdates = true;
        ExceptionListener exceptionListener;

        s->runPendingDatabaseUpdates( &exceptionListener, blockTillNoPendingUpdates );

        ensure( exceptionListener.noExceptions() );
        ensure( 1 == MockConnection::totalNewSampleRuns() );
        ensure( 1 == MockConnection::totalUpdatesForSampleRunIDOnBuddyDatabase() );

		ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );
		ensure_equals( std::distance( s->localBegin(), s->localEnd() ), 1 );

		LocalEntry localEntry = *(s->localBegin());

		LocalRun lr = boost::get<LocalRun>(localEntry);

		Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

		ensure( std::distance( wles.first, wles.second ) == 1 );

		const WorklistEntry* wle = *(wles.first);

		ensureNumResults( wle, 2 );

		std::set<int> expectedResultIDs, actualResultIDs;
		expectedResultIDs.insert( 882431 );
		expectedResultIDs.insert( 882432 );

		Range< TestResultIterator > testResults = wle->getTestResults();

		for ( TestResultIterator i = testResults.first; i != testResults.second; ++i )
		{
			actualResultIDs.insert(  (*i)->getID() );
		}

		ensure( actualResultIDs.size() == 2 );
		ensure( actualResultIDs == expectedResultIDs );
	}

	template<>
	template<>
	void testForceReload::test<5>()
	{
		set_test_name("Loading test names");

		using namespace valc;

		MockConnectionFactory::reset();

		MockConnectionFactory::testNames =
			"-12711493,A1c-IFmc,\n"
			"-12703509,CD40ccv,\n"
			"-12703493,CD40cm,\n"
			"-12703487,CD40cc,\n"
			"-12703329,CD40scv,\n"
			"-12703327,CD40sm,\n"
			"-12703115,BNPccv,\n"
			"-12703113,BNPcm,\n";

        ForceReloadTestFixture s( true );

		ensure( s->getTestName(-12711493) == "A1c-IFmc" );
		ensure( s->getTestName(-12703329) == "CD40scv" );
		ensure( s->getTestName(-12703113) == "BNPcm" );
	}

    template<>
    template<>
	void testForceReload::test<6>()
    {
        set_test_name("ForceReload - 1 sample, 2 tests (with results), of which 1st is already associated with an open sample run");

        using namespace valc;

        MockConnectionFactory::reset();

        MockConnectionFactory::clusters = "-1019430,\n";
        MockConnectionFactory::projects = "-832455,reveal,ldb25,\n";
        MockConnectionFactory::worklist =
//rec  machine  barcode   test     group      c sample project p prof                  timestamp           seq s dil  result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
"-36846,-1019430,118507091,-1031391,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
            ;

        std::string tests[2] = {
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,res ,a,date analysed      ,restx,update when      ,cbw
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.8 ,0,27-06-2013 11:57:47,1.8,27-06-2013 10:57:49,-36845,",
"882291,118507091,27-06-2013 11:55:36,REVEAL,432560,-1019349,882432,-1031391,1.3 ,0,27-06-2013 11:57:47,1.3,27-06-2013 10:57:49,-36846,"
            };

        //                                runID, isOpen, when created       , when closed, sequence position,fao_level_one
        std::string sampleRunData[2] = { "   12,      1,27-06-2013 11:42:36,,882290,y,",
                                         ",,,,,," };

        MockConnectionFactory::buddyDB =
            tests[0] + sampleRunData[0] + "\n" +
            tests[1] + sampleRunData[1] + "\n"
            ;

		try
		{
            ForceReloadTestFixture s( true );

            const bool blockTillNoPendingUpdates = true;
            ExceptionListener exceptionListener;

            s->runPendingDatabaseUpdates( &exceptionListener, blockTillNoPendingUpdates );

            ensure( exceptionListener.noExceptions() );
            ensure( 0 == MockConnection::totalNewSampleRuns() );
            ensure( 1 == MockConnection::totalUpdatesForSampleRunIDOnBuddyDatabase() );

			ensure( std::distance( s->queueBegin(), s->queueEnd() ) == 0 );
			unsigned int numSampleRuns = std::distance( s->localBegin(), s->localEnd() );
			ensure( numSampleRuns == 1U );

			LocalEntry localEntry = *(s->localBegin());

			LocalRun lr = boost::get<LocalRun>(localEntry);

			Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

			ensure( std::distance( wles.first, wles.second ) == 2 );
        
            ensure(     testResultFor( worklistEntry( wles, -36845 ) )->getSampleRunID() == lr.getRunID() );
            ensure_not( testResultFor( worklistEntry( wles, -36846 ) )->getSampleRunID() == lr.getRunID() );

            ensure( s->compareSampleRunIDs( testResultFor( worklistEntry( wles, -36845 ) )->getSampleRunID(), lr.getRunID() ) );

            ensure( s->compareSampleRunIDs( testResultFor( worklistEntry( wles, -36846 ) )->getSampleRunID(), lr.getRunID() ) );
		}
		catch( const Exception& e )
		{
			std::wprintf( L"Exception: %s\n", e.Message.c_str() );
			ensure( false );
		}
	}

    template<>
    template<>
	void testForceReload::test<7>()
    {
        set_test_name("ForceReload - 1 sample, 2 tests (with results), of which 1st is already associated with a CLOSED sample run");

        using namespace valc;

        MockConnectionFactory::reset();

        MockConnectionFactory::clusters = "-1019430,\n";
        MockConnectionFactory::projects = "-832455,reveal,ldb25,\n";
        MockConnectionFactory::worklist =
//rec  machine  barcode   test     group      c sample project p prof                  timestamp           seq s dil  result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
"-36846,-1019430,118507091,-1031391,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
            ;

        std::string tests[2] = {
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,res ,a,date analysed      ,restx,update when      ,cbw
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.8 ,0,27-06-2013 11:57:47,1.8,27-06-2013 10:57:49,-36845,",
"882291,118507091,27-06-2013 11:55:36,REVEAL,432560,-1019349,882432,-1031391,1.3 ,0,27-06-2013 11:57:47,1.3,27-06-2013 10:57:49,-36846,"
            };

        //                                runID, isOpen, when created       , when closed      ,sequence position,fao_level_one
        std::string sampleRunData[2] = { "   12,      0,27-06-2013 11:42:36,27-06-2013 11:42:36,882290,y,",
                                         ",,,,,," };

        MockConnectionFactory::buddyDB =
            tests[0] + sampleRunData[0] + "\n" +
            tests[1] + sampleRunData[1] + "\n"
            ;

		try
		{
            ForceReloadTestFixture s( true );

            const bool blockTillNoPendingUpdates = true;
            ExceptionListener exceptionListener;

            s->runPendingDatabaseUpdates( &exceptionListener, blockTillNoPendingUpdates );

            ensure( exceptionListener.noExceptions() );
            ensure( 1 == MockConnection::totalNewSampleRuns() );
            ensure( 1 == MockConnection::totalUpdatesForSampleRunIDOnBuddyDatabase() );

			ensure( std::distance( s->queueBegin(), s->queueEnd() ) == 0U );
			unsigned int numSampleRuns = std::distance( s->localBegin(), s->localEnd() );
			ensure( numSampleRuns == 2U );

            for ( LocalEntryIterator i = s->localBegin(); i != s->localEnd(); ++i )
            {
                LocalEntry localEntry = *i;

                LocalRun sampleRun = boost::get<LocalRun>(localEntry);

                Range<WorklistEntryIterator> wles = s->getWorklistEntries( sampleRun.getSampleDescriptor() );

                ensure( std::distance( wles.first, wles.second ) == 2 );
                // Of these 2 worklist entries, only one should have a runID matching the current runID.
                ensure( 1 == std::count_if( wles.first, wles.second, boost::bind( hasResultWithRunID, sampleRun.getRunID(), _1 ) ) );
            }
		}
		catch( const Exception& e )
		{
			std::wprintf( L"Exception: %s\n", e.Message.c_str() );
			ensure( false );
		}
	}

    template<>
    template<>
	void testForceReload::test<8>()
    {
        set_test_name("ForceReload - 1 sample, 2 tests (with results), both already associated with a CLOSED sample run");

        using namespace valc;

        MockConnectionFactory::reset();

        MockConnectionFactory::clusters = "-1019430,\n";
        MockConnectionFactory::projects = "-832455,reveal,ldb25,\n";
        MockConnectionFactory::worklist =
//rec  machine  barcode   test     group      c sample project p prof                  timestamp           seq s dil  result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
"-36846,-1019430,118507091,-1031391,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
            ;

        std::string tests[2] = {
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,res ,a,date analysed      ,restx,update when      ,cbw
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.8 ,0,27-06-2013 11:57:47,1.8,27-06-2013 10:57:49,-36845,",
"882291,118507091,27-06-2013 11:55:36,REVEAL,432560,-1019349,882432,-1031391,1.3 ,0,27-06-2013 11:57:47,1.3,27-06-2013 10:57:49,-36846,"
            };

        //                                runID, isOpen, when created       , when closed      ,sequence position,fao_level_one
        std::string sampleRunData[2] = { "   12,      0,27-06-2013 11:42:36,27-06-2013 11:42:36,882290,y,",
                                         "   12,      0,27-06-2013 11:42:36,27-06-2013 11:42:36,882290,y," };

        MockConnectionFactory::buddyDB =
            tests[0] + sampleRunData[0] + "\n" +
            tests[1] + sampleRunData[1] + "\n"
            ;

		try
		{
            ForceReloadTestFixture s( true );

            const bool blockTillNoPendingUpdates = true;
            ExceptionListener exceptionListener;

            s->runPendingDatabaseUpdates( &exceptionListener, blockTillNoPendingUpdates );

            ensure( exceptionListener.noExceptions() );
            ensure( 0 == MockConnection::totalNewSampleRuns() );
            ensure( 0 == MockConnection::totalUpdatesForSampleRunIDOnBuddyDatabase() );

            ensure( 1 == std::distance( s->localBegin(), s->localEnd() ) );
            BuddyDatabaseEntries buddyDatabaseEntries = s->listBuddyDatabaseEntriesFor( "12" );
            ensure( 2 == buddyDatabaseEntries.size() );
            std::sort( buddyDatabaseEntries.begin(), buddyDatabaseEntries.end(), sortOnBuddySampleID );
            ensure( 882290 == buddyDatabaseEntries.at(0).buddy_sample_id );
            ensure( 882291 == buddyDatabaseEntries.at(1).buddy_sample_id );

	    }
		catch( const Exception& e )
		{
			std::wprintf( L"Exception: %s\n", e.Message.c_str() );
			ensure( false );
		}
	}

    template<>
	template<>
	void testForceReload::test<9>()
	{
		set_test_name("ForceReload - Excluding buddy_database entries.");

		using namespace valc;

        const std::string worklist(
//rec  mac   barcode   test     group     c sample project p prof                  timestamp         seq s dil   result
"-1,-1019430,11850701,-1031390,-12750394,0,432561,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,1,\n"
"-2,-1019430,11850702,-1031390,-12750394,0,432562,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,12,C,0.000,2,\n"
"-3,-1019430,11850703,-1031390,-12750394,0,432563,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,13,C,0.000,3,\n"
"-4,-1019430,11850704,-1031390,-12750394,0,432564,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,10,C,0.000,4,\n");

        const std::string buddyDB(
//bsid ,barcode ,date analysed      ,dbname,sample,macine  ,res id,test,result,a,date analysed      ,restx,update when        ,
"882291,11850701,27-06-2013 11:42:36,REVEAL,432561,-1019349,1,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,,,,,\n"
"882292,11850702,27-06-2013 11:42:36,REVEAL,432562,-1019349,2,-1031390,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,,,,,\n"
"882293,11850703,27-06-2013 11:42:36,REVEAL,432563,-1019349,3,-1031390,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,,,,,\n"
"882294,11850704,27-06-2013 11:42:36,REVEAL,432564,-1019349,4,-1031390,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,,,,,\n");

        const std::string inclusionRule[2] = { 
            "function accept ( b, d, a, f ) return true  end",
            "function accept ( b, d, a, f ) return false end" };

        const int expectedLocalEntries[2] = { 4, 0 }; 

        try
        {
            for ( int iteration = 0; iteration <2; ++iteration )
            {
                MockConnectionFactory::reset();

                MockConnectionFactory::clusters = "-1019430,\n";
                MockConnectionFactory::projects = "-832455,reveal,ldb25,\n";
                MockConnectionFactory::worklist = worklist;
                MockConnectionFactory::buddyDB  = buddyDB;

                ForceReloadTestFixture s;
                s.config.edit( "BuddyDatabaseInclusionRule", inclusionRule[iteration] );

                s.init(); 

                ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );
                ensure_equals( std::distance( s->localBegin(), s->localEnd() ), expectedLocalEntries[iteration] );
            }
        }
        catch( const Exception& e )
        {
            ensure( AnsiString( e.Message.c_str() ).c_str(), false );
        }
        catch( ... )
        {
            ensure( "Unknown exception.", false );
        }
	}

    template<>
	template<>
	void testForceReload::test<10>()
	{
		set_test_name("ForceReload - Loading non-local results.");

		using namespace valc;

        MockConnectionFactory::reset();

		MockConnectionFactory::clusters = "-1019430,\n";
		MockConnectionFactory::projects = "-832455,reveal,ldb25,\n";
		MockConnectionFactory::worklist =
//rec  machine  barcode   test     group     c sample project p prof                  timestamp           seq s dil   result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,882431,\n"
"-36847,-1019430,118507091,-1031388,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,12,C,0.000,882429,\n"
			;
		MockConnectionFactory::buddyDB =
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed      ,restx,update when        ,
"992431,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,,,,,\n"
			;
        MockConnectionFactory::nonLocalResults =
//barcode, machine ,sample,dbname,result_id,test_id , res_value, act_flg,date_analysed      , res_text, update_when        ,rec_no,project
"118507091,-1019329,432560,REVEAL,882429   ,-1031388, 2.9      ,0       ,27-06-2013 11:57:47,2.9      , 27-06-2013 11:57:47,-36847,-832455,\n";  

        ForceReloadTestFixture s( true );

        ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );
        ensure_equals( std::distance( s->localBegin(), s->localEnd() ), 1 );

        LocalEntry localEntry = *(s->localBegin());
        LocalRun   localRun   = boost::get<LocalRun>(localEntry);
        Range<WorklistEntryIterator> worklistEntries = s->getWorklistEntries( localRun.getSampleDescriptor() );
        ensure( 2 == std::distance( worklistEntries.first, worklistEntries.second ) );
        try
        {
            const TestResult* localResult    = testResultFor( worklistEntry( worklistEntries, -36845 ) );
            const TestResult* nonLocalResult = testResultFor( worklistEntry( worklistEntries, -36847 ) );
            ensure( -1019349 == localResult   ->getMachineID() );
            ensure( -1019329 == nonLocalResult->getMachineID() );
            ensure    ( s->compareSampleRunIDs( localRun.getRunID(), localResult   ->getSampleRunID() ) );
            ensure_not( s->compareSampleRunIDs( localRun.getRunID(), nonLocalResult->getSampleRunID() ) );
        }
        catch( const Exception& e )
        {
            AnsiString as( e.Message.c_str() );
            ensure( as.c_str(), false );
        }

    }

    template<>
	template<>
	void testForceReload::test<11>()
	{
    	set_test_name("ForceReload - Worklist entries are missing a project ID.");

		using namespace valc;

		MockConnectionFactory::reset();

		MockConnectionFactory::clusters = "-1019430,\n";
		MockConnectionFactory::projects = "-832455,reveal,ldb25,\n";
        // Data same as for test 2, but project ID is zero
		MockConnectionFactory::worklist =
//rec  machine  barcode   test     group     c sample project p prof           timestamp           seq s dil   result
"24110,-1019430,118502164,-1031390,-12744865,0,417128,0,0,EDTA_1 analysis other,21-06-2013 11:07:57,39,Q,0.000,0,\n"
"24111,-1019430,118502164,-1031389,-12744865,0,417128,0,0,EDTA_1 analysis other,21-06-2013 11:07:57,34,Q,0.000,0,\n"
"24112,-1019430,118502164,-1031388,-12744865,0,417128,0,0,EDTA_1 analysis other,21-06-2013 11:07:57,35,Q,0.000,0,\n"
			;

        ForceReloadTestFixture s;

        s.config.edit( "ExceptionalDataHandler", 
" function canProvideProjectIDFor         () return false end\n"
" function getProjectIDFor                () error( \"Cannot provide project ID\" ) return 0 end\n"
" function notifyBuddyDatabaseEntryIgnored() return true, true end\n"
" function notifyWorklistEntryIgnored     () return true, true end\n"
            );
        s.init();
		ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );
		ensure_equals( std::distance( s->localBegin(), s->localEnd() ), 0 );
        ensure_equals( s.userWarnings.messages.size(), 3 );
    }

    template<>
	template<>
	void testForceReload::test<12>()
	{
    	set_test_name("ForceReload - buddy_database entries are missing a project ID.");

		using namespace valc;

        const std::string CLUSTERS = "-1019430,\n";
        const std::string PROJECTS = "-832455,reveal,ldb25,\n";
        const std::string WORKLIST = 
//rec  machine  barcode   test     group     c sample project p prof                  timestamp           seq s dil   result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,882431,\n"
"-36847,-1019430,118507091,-1031388,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,12,C,0.000,882429,\n"
"-36846,-1019430,118507091,-1031389,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,13,C,0.000,882430,\n"
"-36848,-1019430,118507091,-1031386,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,10,C,0.000,882427,\n";
        const std::string BUDDYDB = // Data same as for test 3, but dbname is 'Unknown'
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed      ,restx,update when        ,
"882290,118507091,27-06-2013 11:42:36,Unknown,432560,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,Unknown,432560,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,Unknown,432560,-1019349,882427,-1031386,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,Unknown,432560,-1019349,882430,-1031389,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,,,,,\n";

        const std::string exceptionalDataHandlers[3] = {
" function canProvideProjectIDFor         () return false end\n"
" function getProjectIDFor                () error( \"Cannot provide project ID\" ) return 0 end\n"
" function notifyBuddyDatabaseEntryIgnored() return true, true end\n"
" function notifyWorklistEntryIgnored     () return true, true end\n"
,
" function canProvideProjectIDFor         () return false end\n"
" function getProjectIDFor                () error( \"Cannot provide project ID\" ) return 0 end\n"
" function notifyBuddyDatabaseEntryIgnored() return true, false end\n" // Don't advise user
" function notifyWorklistEntryIgnored     () return true, true end\n"
,
" function canProvideProjectIDFor         () return true end\n"
" function getProjectIDFor                () return -832455 end\n" // provide a project ID
" function notifyBuddyDatabaseEntryIgnored() return true, true end\n"
" function notifyWorklistEntryIgnored     () return true, true end\n"

            };

        for ( int i = 0; i < 3; ++i )
        {
            MockConnectionFactory::reset();

            MockConnectionFactory::clusters = CLUSTERS;
            MockConnectionFactory::projects = PROJECTS;
            MockConnectionFactory::worklist = WORKLIST;
            MockConnectionFactory::buddyDB  = BUDDYDB;

            ForceReloadTestFixture s;

            s.config.edit( "ExceptionalDataHandler", exceptionalDataHandlers[i] );

            s.init();

            ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );
            ensure_equals( std::distance( s->localBegin(), s->localEnd() ), i == 2 ? 1 : 0 );

            ensure_equals( s.userWarnings.messages.size(), i == 0 ? 4 : 0 );  
        }
    }

    template<>
	template<>
	void testForceReload::test<13>()
	{
    	set_test_name("ForceReload - results without worklist entries.");

		using namespace valc;

        const std::string CLUSTERS = "-1019430,\n";
        const std::string PROJECTS = "-832455,reveal,ldb25,\n";
        const std::string WORKLIST = "";
        const std::string BUDDYDB = // Data same as for test 3
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed      ,restx,update when        ,
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882427,-1031386,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882430,-1031389,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,,,,,\n";

        const char* exceptionalDataHandler = 
" function canProvideProjectIDFor         () return false end\n"
" function getProjectIDFor                () error( \"Cannot provide project ID\" ) return 0 end\n"
" function notifyBuddyDatabaseEntryIgnored() return true, true end\n"
" function notifyWorklistEntryIgnored     () return true, true end\n"
" function notifyCannotAllocateResultToWorklistEntry() return true, %s end\n";

        for ( int i = 0; i < 2; ++i )
        {
            MockConnectionFactory::reset();

            MockConnectionFactory::clusters = CLUSTERS;
            MockConnectionFactory::projects = PROJECTS;
            MockConnectionFactory::worklist = WORKLIST;
            MockConnectionFactory::buddyDB  = BUDDYDB;

            ForceReloadTestFixture s;

            char buffer[2048];
            std::sprintf( buffer, exceptionalDataHandler, i ? "true" : "false" );
            s.config.edit( "ExceptionalDataHandler", buffer );

            s.init();

            ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );
            ensure_equals( std::distance( s->localBegin(), s->localEnd() ), 1 );

            ensure_equals( s.userWarnings.messages.size(), i ? 4 : 0 );  
        }
    }


    template<>
	template<>
	void testForceReload::test<14>()
	{
    	set_test_name("ForceReload - persisting newly-created result-worklist links.");

		using namespace valc;

        const std::string CLUSTERS = "-1019430,\n";
        const std::string PROJECTS = "-832455,reveal,ldb25,\n";
        // Using a subset of the data from test #3 (above)
        const std::string WORKLIST = 
//rec  machine  barcode   test     group     c sample project p prof                  timestamp           seq s dil   result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
"-36847,-1019430,118507091,-1031388,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,12,C,0.000,0,\n"
"-36846,-1019430,118507091,-1031389,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,13,C,0.000,0,\n"
"-36848,-1019430,118507091,-1031386,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,10,C,0.000,0,\n";

        const std::string BUDDYDB = // Data same as for test 3
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed      ,restx,update when        ,
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882427,-1031386,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882430,-1031389,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,,,,,\n";

        MockConnectionFactory::reset();

        MockConnectionFactory::clusters = CLUSTERS;
        MockConnectionFactory::projects = PROJECTS;
        MockConnectionFactory::worklist = WORKLIST;
        MockConnectionFactory::buddyDB  = BUDDYDB;

        ForceReloadTestFixture s( true );

        ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );
        ensure_equals( std::distance( s->localBegin(), s->localEnd() ), 1 );

        const bool blockTillNoPendingUpdates = true;
        ExceptionListener exceptionListener;

        s->runPendingDatabaseUpdates( &exceptionListener, blockTillNoPendingUpdates );

        ensure( exceptionListener.noExceptions() );
        ensure_equals( "There should be no warnings.", s.userWarnings.messages.size(), 0 );
        ensure_equals( "4 rows in buddy_result_float should have been updated.", MockConnection::totalNewResult2WorklistLinks(), 4 );

    }

    template<>
	template<>
	void testForceReload::test<15>()
	{
    	set_test_name("ForceReload - applying rules to a QC result");

		using namespace valc;

        MockConnectionFactory::reset();

		MockConnectionFactory::clusters = "-1019430,\n";
		MockConnectionFactory::projects = "-1234,QCs,ldbqc,\n";
		MockConnectionFactory::worklist =
//rec  machine   barcode    test    grp c samp prj p prof                              time               seq s dil   result
"-36845,-1019349,QCRC100355,-1031390,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,14,C,0.000,882431,\n"
"-36847,-1019349,QCRC100355,-1031388,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,12,C,0.000,882429,\n"
"-36846,-1019349,QCRC100355,-1031389,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,13,C,0.000,882430,\n"
"-36848,-1019349,QCRC100355,-1031386,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,10,C,0.000,882427,\n";

		MockConnectionFactory::buddyDB =
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed ,restx,update when        ,
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882427,-1031386,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882430,-1031389,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,,,,,\n"
			;
        

        MockConnectionFactory::ruleConfig = 
               "-1031390,-1019349,-1234,configRule,\n" 
               "-1031388,-1019349,-1234,configRule,\n" 
               "-1031389,-1019349,-1234,configRule,\n" 
               "-1031386,-1019349,-1234,configRule,\n" ;


        MockConnectionFactory::rules = SerializedRecordset( 
                "configRule|"
                    " local rule                                        "
                    " context = {}                                      "
                    "                                                   "
                    " function onLoad(loadFunc)                         "
                    "   local script = loadFunc('myRule')               "
                    "   rule = load(script)                             "
                    " end                                               "
                    "                                                   "
                    " function applyRules( qc )                         "
                    "   context.qc = qc                                 "
                    "   local result = rule()                           "
                    "   return { result }, result.msg, result.resultCode"
                    " end                                               |\n"
                "myRule|"
                    "return { resultCode = context.qc.testID + 2, rule = 'my rule', msg = 'Bob' }|\n",
                &pipeSeparatedFieldValues);
 
        ForceReloadTestFixture s( true, true );

		ensure_equals( std::distance( s->localBegin(), s->localEnd() ), 1 );

        LocalEntry localEntry = *(s->localBegin());

        LocalRun lr = boost::get<LocalRun>(localEntry);

        Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

        ensure_equals( std::distance( wles.first, wles.second ), 4U );

        for ( WorklistEntryIterator i = wles.first; i != wles.second; ++i )
        {
            const WorklistEntry* wle = *i;
            const TestResult* tr = testResultFor(wle);
            const int resultID = tr->getID();

            ensure( s->hasRuleResults( resultID ) );
            RuleResults rr = s->getRuleResults( resultID );
            ensure_equals( rr.getSummaryResultCode(), tr->getTestID() + 2 );
        }
    }

    template<>
	template<>
	void testForceReload::test<16>()
	{
    	set_test_name("ForceReload - rule config error");

		using namespace valc;

        MockConnectionFactory::reset();

		MockConnectionFactory::clusters = "-1019430,\n";
		MockConnectionFactory::projects = "-1234,QCs,ldbqc,\n";
		MockConnectionFactory::worklist =
//rec  machine   barcode    test    grp c samp prj p prof                              time               seq s dil   result
"-36845,-1019349,QCRC100355,-1031390,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,14,C,0.000,882431,\n";

		MockConnectionFactory::buddyDB =
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed ,restx,update when        ,
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,,,,,\n";
        

        MockConnectionFactory::ruleConfig = "-1031390,-1019349,-1234,configRule,\n" ; // Note: different test id from above.


        MockConnectionFactory::rules = SerializedRecordset( 
                "configRule|"
                    " function onLoad(loadFunc)                         "
                    "   loadFunc('nonExistentRule')                     "
                    " end                                               "
                    "                                                   "
                    " function applyRules( qc )                         "
                    "   local result = { resultCode = 1, rule = 'x', msg = 'msg' }                           "
                    "   return { result }, 'msg', 1                     "
                    " end                                               |\n",
                &pipeSeparatedFieldValues);
 
        ForceReloadTestFixture s( false, true );

        s.config.edit( "RuleEngineErrorCode", "94949494" );

        s.init();

		ensure_equals( std::distance( s->localBegin(), s->localEnd() ), 1 );

        LocalEntry localEntry = *(s->localBegin());

        LocalRun lr = boost::get<LocalRun>(localEntry);

        Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

        ensure_equals( std::distance( wles.first, wles.second ), 1U );

        const WorklistEntry* wle = *(wles.first);
        const TestResult* tr = testResultFor(wle);
        const int resultID = tr->getID();

        ensure( s->hasRuleResults( resultID ) );
        RuleResults rr = s->getRuleResults( resultID );
        ensure_equals( rr.getSummaryResultCode(), 94949494 );
    }

    template<>
	template<>
	void testForceReload::test<17>()
	{
    	set_test_name("ForceReload - multiple result rules");

		using namespace valc;

        MockConnectionFactory::reset();

		MockConnectionFactory::clusters = "-1019430,\n";
		MockConnectionFactory::projects = "-1234,QCs,ldbqc,\n";
		MockConnectionFactory::worklist =
//rec  machine   barcode    test    grp c samp prj p prof                              time               seq s dil   result
"-36845,-1019349,QCRC100355,-1031390,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,14,C,0.000,882431,\n"
"-36847,-1019349,QCRC100355,-1031388,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,12,C,0.000,882429,\n"
"-36846,-1019349,QCRC100355,-1031389,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,13,C,0.000,882430,\n"
"-36848,-1019349,QCRC100355,-1031386,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,10,C,0.000,882427,\n";

		MockConnectionFactory::buddyDB =
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed ,restx,update when        ,
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882427,-1031386,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882430,-1031389,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,,,,,\n";

        MockConnectionFactory::ruleConfig = 
               "-1031390,-1019349,-1234,configRule1,\n" 
               "-1031388,-1019349,-1234,configRule2,\n" 
               "-1031389,-1019349,-1234,configRule3,\n" 
               "-1031386,-1019349,-1234,configRule4,\n" ;

        const char* ruleConfigTemplate =
                "configRule\%d|                                     "
                " loadedRules = {}                                  "
                "                                                   "
                " context = {}                                      "
                "                                                   "
                " ruleNames = { \%s }                               "
                "                                                   "
                " function onLoad(loadFunc)                         "
                "   for i, ruleName in ipairs(ruleNames) do         "
                "      local script = loadFunc(ruleName)            "
                "      loadedRules[ruleName] = load(script)         "
                "   end                                             "
                " end                                               "
                "                                                   "
                " function applyRules( qc )                         "
                "   context.total = 0                               "
                "   local results = {}                              "
                "   for name, rule in pairs(loadedRules) do         "
                "     local result = rule()                         "
                "     table.insert( results, result )               "
                "   end                                             "
                "   return results, 'msg', context.total            "
                " end                                               |\n";
 
        MockConnectionFactory::rules = SerializedRecordset( 
            paulst::format( ruleConfigTemplate, 1, "'plus1','plus5'" ) +
            paulst::format( ruleConfigTemplate, 2, "'plus4','plus6'" ) +
            paulst::format( ruleConfigTemplate, 3, "'plus3','plus1'" ) +
            paulst::format( ruleConfigTemplate, 4, "'plus6','plus7'" ) +
                std::string(
                "plus1| context.total = context.total + 1  return { resultCode = 1, rule = 'plus1', msg = 'OK' }|\n"
                "plus2| context.total = context.total + 2  return { resultCode = 2, rule = 'plus2', msg = 'OK' }|\n"
                "plus3| context.total = context.total + 3  return { resultCode = 3, rule = 'plus3', msg = 'OK' }|\n"
                "plus4| context.total = context.total + 4  return { resultCode = 4, rule = 'plus4', msg = 'OK' }|\n"
                "plus5| context.total = context.total + 5  return { resultCode = 5, rule = 'plus5', msg = 'OK' }|\n"
                "plus6| context.total = context.total + 6  return { resultCode = 6, rule = 'plus6', msg = 'OK' }|\n"
                "plus7| context.total = context.total + 7  return { resultCode = 7, rule = 'plus7', msg = 'OK' }|\n"),
                &pipeSeparatedFieldValues);
 
        ForceReloadTestFixture s( true, true );

		ensure_equals( std::distance( s->localBegin(), s->localEnd() ), 1 );

        LocalEntry localEntry = *(s->localBegin());

        LocalRun lr = boost::get<LocalRun>(localEntry);

        Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

        ensure_equals( std::distance( wles.first, wles.second ), 4U );

        std::map< int, int > expectedResults;
        expectedResults[882431] = /*testID -1031390, configRule1*/ 6  /*plus1 plus5*/;
        expectedResults[882429] = /*testID -1031388, configRule2*/ 10 /*plus4 plus6*/;
        expectedResults[882427] = /*testID -1031386, configRule4*/ 13 /*plus6 plus7*/;
        expectedResults[882430] = /*testID -1031389, configRule3*/ 4  /*plus3 plus1*/;

        for ( WorklistEntryIterator i = wles.first; i != wles.second; ++i )
        {
            const WorklistEntry* wle = *i;
            const TestResult* tr = testResultFor(wle);
            const int resultID = tr->getID();

            ensure( s->hasRuleResults( resultID ) );
            RuleResults rr = s->getRuleResults( resultID );
            ensure_equals( rr.getSummaryResultCode(), expectedResults[resultID] );
        }
    }

};

#endif

