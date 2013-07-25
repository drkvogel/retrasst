#ifndef FORCERELOAD_TEST_H 
#define FORCERELOAD_TEST_H

#include <algorithm>
#include "API.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/variant/get.hpp>
#include <cwchar>
#include "MockConnectionFactory.h"
#include <set>
#include "StrUtil.h"
#include <tut.h>
#include <vector>

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

namespace tut
{
	class ForceReloadTestFixture
    {
    };

    typedef test_group<ForceReloadTestFixture, 10> ForceReloadTestGroup;
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

		MockConnectionFactory connectionFactory;

		boost::scoped_ptr<valc::DBConnection> connection( connectionFactory.createConnection() );

		boost::scoped_ptr<valc::AnalysisActivitySnapshot> s( valc::SnapshotFactory::load( -1019349, 1234, connection.get() ) );

		ensure( s.get() );
		ensure( s->localBegin() == s->localEnd() );
		ensure( s->queueBegin() == s->queueEnd() );
	}

	template<>
	template<>
	void testForceReload::test<2>()
	{
		set_test_name("ForceReload - 3 tests queued, single sample.");

		using namespace valc;

		MockConnectionFactory connectionFactory;

		connectionFactory.setClusters( "-1019430,\n" );
		connectionFactory.setProjects( "-832455,reveal,ldb25,\n" );
		connectionFactory.setWorklist(
//rec  machine  barcode   test     group     c sample project p prof                  timestamp           seq s dil   result
"24110,-1019430,118502164,-1031390,-12744865,0,417128,-832455,0,EDTA_1 analysis other,21-06-2013 11:07:57,39,Q,0.000,0,\n"
"24111,-1019430,118502164,-1031389,-12744865,0,417128,-832455,0,EDTA_1 analysis other,21-06-2013 11:07:57,34,Q,0.000,0,\n"
"24112,-1019430,118502164,-1031388,-12744865,0,417128,-832455,0,EDTA_1 analysis other,21-06-2013 11:07:57,35,Q,0.000,0,\n"
			);

		boost::scoped_ptr<valc::DBConnection> connection( connectionFactory.createConnection() );

		boost::scoped_ptr<valc::AnalysisActivitySnapshot> s( valc::SnapshotFactory::load( -1019349, 1234, connection.get() ) );

		ensure( s.get() );
		ensure( s->localBegin() == s->localEnd() );
		ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 1 );

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

		MockConnectionFactory connectionFactory;

		connectionFactory.setClusters( "-1019430,\n" );
		connectionFactory.setProjects( "-832455,reveal,ldb25,\n" );
		connectionFactory.setWorklist(
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
			);
		connectionFactory.setBuddyDB(
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed      ,restx,update when        ,
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882427,-1031386,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882430,-1031389,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,,,,,\n"
			);


		boost::scoped_ptr<valc::DBConnection> connection( connectionFactory.createConnection() );

		boost::scoped_ptr<valc::AnalysisActivitySnapshot> s( valc::SnapshotFactory::load( -1019349, 1234, connection.get() ) );
		ensure( s.get() );

        const DBUpdateStats* stats = s->getDBUpdateStats();

        ensure( 1 == stats->totalNewSampleRuns() );
        ensure( 1 == stats->totalUpdatesForSampleRunIDOnBuddyDatabase() );

		ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 1 );
		ensure_equals( std::distance( s->localBegin(), s->localEnd() ), 1 );

		LocalEntry localEntry = *(s->localBegin());

		LocalRun lr = boost::get<LocalRun>(localEntry);

		Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

		ensure( std::distance( wles.first, wles.second ) == 4U );

		std::for_each( wles.first, wles.second, boost::bind( ensureNumResults, _1, 1 ) );
	}

	template<>
	template<>
	void testForceReload::test<4>()
	{
		set_test_name("ForceReload - 1 sample, 1 test, 2 results. (Use Case 1, Type B)");

		using namespace valc;

		MockConnectionFactory connectionFactory;

		connectionFactory.setClusters( "-1019430,\n" );
		connectionFactory.setProjects( "-832455,reveal,ldb25,\n" );
		connectionFactory.setWorklist(
//rec  machine  barcode   test     group     c sample project p prof                  timestamp           seq s dil   result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
			);
		connectionFactory.setBuddyDB(
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,res ,a,date analysed      ,restx,update when      ,cbw
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.8 ,0,27-06-2013 11:57:47,1.8,27-06-2013 10:57:49,-36845,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882432,-1031390,1.3 ,0,27-06-2013 11:57:47,1.3,27-06-2013 10:57:49,-36845,,,,,,\n"
			);


		boost::scoped_ptr<valc::DBConnection> connection( connectionFactory.createConnection() );

		boost::scoped_ptr<valc::AnalysisActivitySnapshot> s( valc::SnapshotFactory::load( -1019349, 1234, connection.get() ) );

		ensure( s.get() );

        const DBUpdateStats* stats = s->getDBUpdateStats();

        ensure( 1 == stats->totalNewSampleRuns() );
        ensure( 1 == stats->totalUpdatesForSampleRunIDOnBuddyDatabase() );


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

		MockConnectionFactory connectionFactory;
		connectionFactory.setTestNames(
			"-12711493,A1c-IFmc,\n"
			"-12703509,CD40ccv,\n"
			"-12703493,CD40cm,\n"
			"-12703487,CD40cc,\n"
			"-12703329,CD40scv,\n"
			"-12703327,CD40sm,\n"
			"-12703115,BNPccv,\n"
			"-12703113,BNPcm,\n");


		boost::scoped_ptr<valc::DBConnection> connection( connectionFactory.createConnection() );

		boost::scoped_ptr<valc::AnalysisActivitySnapshot> s( valc::SnapshotFactory::load( -1019349, 1234, connection.get() ) );

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

        MockConnectionFactory connectionFactory;

        connectionFactory.setClusters( "-1019430,\n" );
        connectionFactory.setProjects( "-832455,reveal,ldb25,\n" );
        connectionFactory.setWorklist(
//rec  machine  barcode   test     group      c sample project p prof                  timestamp           seq s dil  result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
"-36846,-1019430,118507091,-1031391,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
            );

        std::string tests[2] = {
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,res ,a,date analysed      ,restx,update when      ,cbw
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.8 ,0,27-06-2013 11:57:47,1.8,27-06-2013 10:57:49,-36845,",
"882291,118507091,27-06-2013 11:55:36,REVEAL,432560,-1019349,882432,-1031391,1.3 ,0,27-06-2013 11:57:47,1.3,27-06-2013 10:57:49,-36846,"
            };

        //                                runID, isOpen, when created       , when closed, sequence position
        std::string sampleRunData[2] = { "   12,      1,27-06-2013 11:42:36,,882290,",
                                         ",,,,," };

        connectionFactory.setBuddyDB(
            tests[0] + sampleRunData[0] + "\n" +
            tests[1] + sampleRunData[1] + "\n"
            );

        boost::scoped_ptr<valc::DBConnection> connection( connectionFactory.createConnection() );

		try
		{
			boost::scoped_ptr<valc::AnalysisActivitySnapshot> s( valc::SnapshotFactory::load( -1019349, 1234, connection.get() ) );

			ensure( s.get() );

            const DBUpdateStats* stats = s->getDBUpdateStats();

            ensure( 0 == stats->totalNewSampleRuns() );
            ensure( 1 == stats->totalUpdatesForSampleRunIDOnBuddyDatabase() );

			ensure( std::distance( s->queueBegin(), s->queueEnd() ) == 0 );
			unsigned int numSampleRuns = std::distance( s->localBegin(), s->localEnd() );
			ensure( numSampleRuns == 1U );

			LocalEntry localEntry = *(s->localBegin());

			LocalRun lr = boost::get<LocalRun>(localEntry);

			Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

			ensure( std::distance( wles.first, wles.second ) == 2 );

            for ( WorklistEntryIterator i = wles.first; i != wles.second; ++i )
            {
                ensure( hasResultWithRunID( lr.getRunID(), *i ) );
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
	void testForceReload::test<7>()
    {
        set_test_name("ForceReload - 1 sample, 2 tests (with results), of which 1st is already associated with a CLOSED sample run");

        using namespace valc;

        MockConnectionFactory connectionFactory;

        connectionFactory.setClusters( "-1019430,\n" );
        connectionFactory.setProjects( "-832455,reveal,ldb25,\n" );
        connectionFactory.setWorklist(
//rec  machine  barcode   test     group      c sample project p prof                  timestamp           seq s dil  result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
"-36846,-1019430,118507091,-1031391,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
            );

        std::string tests[2] = {
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,res ,a,date analysed      ,restx,update when      ,cbw
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.8 ,0,27-06-2013 11:57:47,1.8,27-06-2013 10:57:49,-36845,",
"882291,118507091,27-06-2013 11:55:36,REVEAL,432560,-1019349,882432,-1031391,1.3 ,0,27-06-2013 11:57:47,1.3,27-06-2013 10:57:49,-36846,"
            };

        //                                runID, isOpen, when created       , when closed      ,sequence position
        std::string sampleRunData[2] = { "   12,      0,27-06-2013 11:42:36,27-06-2013 11:42:36,882290,",
                                         ",,,,," };

        connectionFactory.setBuddyDB(
            tests[0] + sampleRunData[0] + "\n" +
            tests[1] + sampleRunData[1] + "\n"
            );

        boost::scoped_ptr<valc::DBConnection> connection( connectionFactory.createConnection() );

		try
		{
			boost::scoped_ptr<valc::AnalysisActivitySnapshot> s( valc::SnapshotFactory::load( -1019349, 1234, connection.get() ) );

			ensure( s.get() );

            const DBUpdateStats* stats = s->getDBUpdateStats();

            ensure( 1 == stats->totalNewSampleRuns() );
            ensure( 1 == stats->totalUpdatesForSampleRunIDOnBuddyDatabase() );

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

        MockConnectionFactory connectionFactory;

        connectionFactory.setClusters( "-1019430,\n" );
        connectionFactory.setProjects( "-832455,reveal,ldb25,\n" );
        connectionFactory.setWorklist(
//rec  machine  barcode   test     group      c sample project p prof                  timestamp           seq s dil  result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
"-36846,-1019430,118507091,-1031391,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
            );

        std::string tests[2] = {
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,res ,a,date analysed      ,restx,update when      ,cbw
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.8 ,0,27-06-2013 11:57:47,1.8,27-06-2013 10:57:49,-36845,",
"882291,118507091,27-06-2013 11:55:36,REVEAL,432560,-1019349,882432,-1031391,1.3 ,0,27-06-2013 11:57:47,1.3,27-06-2013 10:57:49,-36846,"
            };

        //                                runID, isOpen, when created       , when closed      ,sequence position
        std::string sampleRunData[2] = { "   12,      0,27-06-2013 11:42:36,27-06-2013 11:42:36,882290,",
                                         "   12,      0,27-06-2013 11:42:36,27-06-2013 11:42:36,882290," };

        connectionFactory.setBuddyDB(
            tests[0] + sampleRunData[0] + "\n" +
            tests[1] + sampleRunData[1] + "\n"
            );

        boost::scoped_ptr<valc::DBConnection> connection( connectionFactory.createConnection() );

		try
		{
			boost::scoped_ptr<valc::AnalysisActivitySnapshot> s( valc::SnapshotFactory::load( -1019349, 1234, connection.get() ) );

			ensure( s.get() );

            const DBUpdateStats* stats = s->getDBUpdateStats();

            ensure( 0 == stats->totalNewSampleRuns() );
            ensure( 0 == stats->totalUpdatesForSampleRunIDOnBuddyDatabase() );

	    }
		catch( const Exception& e )
		{
			std::wprintf( L"Exception: %s\n", e.Message.c_str() );
			ensure( false );
		}
	}

};

#endif

