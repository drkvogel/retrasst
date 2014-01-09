#ifndef FORCERELOAD_TEST_H 
#define FORCERELOAD_TEST_H

#include <algorithm>
#include "API.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/variant/get.hpp>
#include <cstdio>
#include <cwchar>
#include "CSVIterator.h"
#include <iterator>
#include "LocalRunIterator.h"
#include <map>
#include "MockConnection.h"
#include "MockConnectionFactory.h"
#include <set>
#include "SnapshotTestFixture.h"
#include "StringBackedCursor.h"
#include "StrUtil.h"
#include <tut.h>
#include <vector>


bool sameRun( valc::SnapshotPtr s, const std::string& runA, const std::string& runB )
{
    return s->compareSampleRunIDs( runA, runB );
}

void pipeSeparatedFieldValues( const std::string& record, paulstdb::RowValues& out )
{
    typedef paulst::CSVIterator<'|'> CSVIter;
	std::copy( CSVIter(record), CSVIter(), std::back_inserter( out ) );
}

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
	class ForceReloadTestFixture : public SnapshotTestFixture
    {
    public:
        static const int LOCAL_MACHINE_ID = -1019349;
        static const int USER_ID          = 1234;

        ForceReloadTestFixture( bool initialise = false, bool suppressLogMessages = true )
            : SnapshotTestFixture( LOCAL_MACHINE_ID, USER_ID, initialise, suppressLogMessages )
        {
        }
        
        valc::SnapshotPtr operator->() const
        {
            return get();
        }
    };

    typedef test_group<ForceReloadTestFixture, 26> ForceReloadTestGroup;
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

		MockConnectionFactory::prime( CLUSTERS_QRY,  "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-832455,reveal,ldb25,\n");
		MockConnectionFactory::prime( WORKLIST_QRY, 
//rec  machine  barcode   test     group     c sample project p prof                  timestamp           seq s dil   result
"24110,-1019430,118502164,-1031390,-12744865,0,417128,-832455,0,EDTA_1 analysis other,21-06-2013 11:07:57,39,Q,0.000,0,\n"
"24111,-1019430,118502164,-1031389,-12744865,0,417128,-832455,0,EDTA_1 analysis other,21-06-2013 11:07:57,34,Q,0.000,0,\n"
"24112,-1019430,118502164,-1031388,-12744865,0,417128,-832455,0,EDTA_1 analysis other,21-06-2013 11:07:57,35,Q,0.000,0,\n")
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

		MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-832455,reveal,ldb25,\n" );
		MockConnectionFactory::prime( WORKLIST_QRY, 
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
"-36848,-1019430,118507091,-1031386,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,10,C,0.000,882427,\n")
// Note that the last 2 have zero for buddy_result_id
//
// the buddy_sample_id for buddy_result_id 882429 is: 882290
			;
		MockConnectionFactory::prime( BUDDYDB_QRY,
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed      ,restx,update when        ,
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882427,-1031386,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882430,-1031389,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,\n")
			;

        ForceReloadTestFixture s( true );

        const bool blockTillNoPendingUpdates = true;

        s->runPendingDatabaseUpdates( blockTillNoPendingUpdates );

        ensure( "Expected 1 new sample-run", 1 == MockConnection::totalNewSampleRuns() );
        const int actualUpdatesOfSampleRunID = MockConnection::totalUpdatesForSampleRunIDOnBuddyDatabase();
        ensure_equals( actualUpdatesOfSampleRunID, 1 );

		ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 1 );
		ensure_equals( s.numLocalRuns(), 1 );

        LocalRunIterator lri( s->localBegin(), s->localEnd() );

		LocalRun lr = *lri;

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

		MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-832455,reveal,ldb25,\n" );
		MockConnectionFactory::prime( WORKLIST_QRY, 
//rec  machine  barcode   test     group     c sample project p prof                  timestamp           seq s dil   result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n" )
			;
		MockConnectionFactory::prime( BUDDYDB_QRY,
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,res ,a,date analysed      ,restx,update when      ,cbw
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.8 ,0,27-06-2013 11:57:47,1.8,27-06-2013 10:57:49,-36845,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882432,-1031390,1.3 ,0,27-06-2013 11:57:47,1.3,27-06-2013 10:57:49,-36845,,\n")
			;

        ForceReloadTestFixture s( true );

        const bool blockTillNoPendingUpdates = true;

        s->runPendingDatabaseUpdates( blockTillNoPendingUpdates );

        ensure( 1 == MockConnection::totalNewSampleRuns() );
        ensure( 1 == MockConnection::totalUpdatesForSampleRunIDOnBuddyDatabase() );

		ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );

        LocalRunIterator begin( s->localBegin(), s->localEnd() ), end;

		ensure_equals( std::distance( begin, end ), 1 );

		LocalRun lr = *begin;

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

		MockConnectionFactory::prime( TESTNAMES_QRY, 
			"-12711493,A1c-IFmc,\n"
			"-12703509,CD40ccv,\n"
			"-12703493,CD40cm,\n"
			"-12703487,CD40cc,\n"
			"-12703329,CD40scv,\n"
			"-12703327,CD40sm,\n"
			"-12703115,BNPccv,\n"
			"-12703113,BNPcm,\n" );

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

        MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
        MockConnectionFactory::prime( PROJECTS_QRY, "-832455,reveal,ldb25,\n" );
        MockConnectionFactory::prime( WORKLIST_QRY, 
//rec  machine  barcode   test     group      c sample project p prof                  timestamp           seq s dil  result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
"-36846,-1019430,118507091,-1031391,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n" )
            ;

        std::string tests[2] = {
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,res ,a,date analysed      ,restx,update when      ,cbw
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.8 ,0,27-06-2013 11:57:47,1.8,27-06-2013 10:57:49,-36845,",
"882291,118507091,27-06-2013 11:55:36,REVEAL,432560,-1019349,882432,-1031391,1.3 ,0,27-06-2013 11:57:47,1.3,27-06-2013 10:57:49,-36846,"
            };

        //                                runID, isOpen, when created       , when closed, sequence position,fao_level_one
        std::string sampleRunData[2] = { "   12,      1,27-06-2013 11:42:36,,882290,y,,",
                                         ",,,,,,," };

        MockConnectionFactory::prime( BUDDYDB_QRY, 
            tests[0] + sampleRunData[0] + "\n" +
            tests[1] + sampleRunData[1] + "\n" )
            ;

		try
		{
            ForceReloadTestFixture s( true );

			ensure( "The queue should be empty", std::distance( s->queueBegin(), s->queueEnd() ) == 0 );

            LocalRunIterator begin( s->localBegin(), s->localEnd() ), end;

			unsigned int numSampleRuns = std::distance( begin, end );

			ensure( "There should be just the one sample run", numSampleRuns == 1U );

			LocalRun lr = *begin;

			Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

			ensure( "There should be 2 worklist entries", std::distance( wles.first, wles.second ) == 2 );
        
            for ( int iteration = 0; iteration < 2; ++iteration )
            {
                ensure(     testResultFor( worklistEntry( wles, -36845 ) )->getSampleRunID() == lr.getRunID() );
                ensure_not( testResultFor( worklistEntry( wles, -36846 ) )->getSampleRunID() == lr.getRunID() );
                ensure(     testResultFor( worklistEntry( wles, -36846 ) )->getSampleRunID() == 
                                           worklistEntry( wles, -36846 )  ->getSampleDescriptor()    ); 
                ensure(     testResultFor( worklistEntry( wles, -36845 ) )->getSampleRunID() == "12" );
                ensure    ( s->compareSampleRunIDs( testResultFor( worklistEntry( wles, -36845 ) )->getSampleRunID(), lr.getRunID() ) );
                ensure    ( s->compareSampleRunIDs( testResultFor( worklistEntry( wles, -36846 ) )->getSampleRunID(), lr.getRunID() ) );

                const bool blockTillNoPendingUpdates = true;

                s->runPendingDatabaseUpdates( blockTillNoPendingUpdates );
            }

            ensure( 0 == MockConnection::totalNewSampleRuns() );
            ensure( 1 == MockConnection::totalUpdatesForSampleRunIDOnBuddyDatabase() );
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

        MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
        MockConnectionFactory::prime( PROJECTS_QRY, "-832455,reveal,ldb25,\n" );
        MockConnectionFactory::prime( WORKLIST_QRY, 
//rec  machine  barcode   test     group      c sample project p prof                  timestamp           seq s dil  result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
"-36846,-1019430,118507091,-1031391,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n")
            ;

        std::string tests[2] = {
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,res ,a,date analysed      ,restx,update when      ,cbw
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.8 ,0,27-06-2013 11:57:47,1.8,27-06-2013 10:57:49,-36845,",
"882291,118507091,27-06-2013 11:55:36,REVEAL,432560,-1019349,882432,-1031391,1.3 ,0,27-06-2013 11:57:47,1.3,27-06-2013 10:57:49,-36846,"
            };

        //                                runID, isOpen, when created       , when closed      ,sequence position,fao_level_one,group_id
        std::string sampleRunData[2] = { "   12,      0,27-06-2013 11:42:36,27-06-2013 11:42:36,882290,y,,",
                                         ",,,,,,," };

        MockConnectionFactory::prime( BUDDYDB_QRY,
            tests[0] + sampleRunData[0] + "\n" +
            tests[1] + sampleRunData[1] + "\n" )
            ;

		try
		{
            ForceReloadTestFixture s( true );

            const bool blockTillNoPendingUpdates = true;

            s->runPendingDatabaseUpdates( blockTillNoPendingUpdates );

            ensure( 1 == MockConnection::totalNewSampleRuns() );
            ensure( 1 == MockConnection::totalUpdatesForSampleRunIDOnBuddyDatabase() );

			ensure( std::distance( s->queueBegin(), s->queueEnd() ) == 0U );

            LocalRunIterator begin( s->localBegin(), s->localEnd() ), end;

			unsigned int numSampleRuns = std::distance( begin, end );
			ensure( numSampleRuns == 2U );

            for ( ; begin != end; ++begin )
            {
                LocalRun sampleRun = *begin;

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

        MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
        MockConnectionFactory::prime( PROJECTS_QRY, "-832455,reveal,ldb25,\n" );
        MockConnectionFactory::prime( WORKLIST_QRY,
//rec  machine  barcode   test     group      c sample project p prof                  timestamp           seq s dil  result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
"-36846,-1019430,118507091,-1031391,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n" )
            ;

        std::string tests[2] = {
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,res ,a,date analysed      ,restx,update when      ,cbw
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.8 ,0,27-06-2013 11:57:47,1.8,27-06-2013 10:57:49,-36845,",
"882291,118507091,27-06-2013 11:55:36,REVEAL,432560,-1019349,882432,-1031391,1.3 ,0,27-06-2013 11:57:47,1.3,27-06-2013 10:57:49,-36846,"
            };

        //                                runID, isOpen, when created       , when closed      ,sequence position,fao_level_one,group_id
        std::string sampleRunData[2] = { "   12,      0,27-06-2013 11:42:36,27-06-2013 11:42:36,882290,y,,",
                                         "   12,      0,27-06-2013 11:42:36,27-06-2013 11:42:36,882290,y,," };

        MockConnectionFactory::prime( BUDDYDB_QRY,
            tests[0] + sampleRunData[0] + "\n" +
            tests[1] + sampleRunData[1] + "\n" )
            ;

		try
		{
            ForceReloadTestFixture s( true );

            const bool blockTillNoPendingUpdates = true;

            s->runPendingDatabaseUpdates( blockTillNoPendingUpdates );

            ensure( 0 == MockConnection::totalNewSampleRuns() );
            ensure( 0 == MockConnection::totalUpdatesForSampleRunIDOnBuddyDatabase() );

            LocalRunIterator begin( s->localBegin(), s->localEnd() ), end;

            ensure_equals( std::distance( begin, end ), 1U );
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
"882291,11850701,27-06-2013 11:42:36,REVEAL,432561,-1019349,1,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n"
"882292,11850702,27-06-2013 11:42:36,REVEAL,432562,-1019349,2,-1031390,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,\n"
"882293,11850703,27-06-2013 11:42:36,REVEAL,432563,-1019349,3,-1031390,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,\n"
"882294,11850704,27-06-2013 11:42:36,REVEAL,432564,-1019349,4,-1031390,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,\n");

        const std::string inclusionRule[2] = { 
            "function accept ( b, d, a, f ) return true  end",
            "function accept ( b, d, a, f ) return false end" };

        const int expectedLocalEntries[2] = { 4, 0 }; 

        try
        {
            for ( int iteration = 0; iteration <2; ++iteration )
            {
                MockConnectionFactory::reset();

                MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
                MockConnectionFactory::prime( PROJECTS_QRY, "-832455,reveal,ldb25,\n" );
                MockConnectionFactory::prime( WORKLIST_QRY, worklist );
                MockConnectionFactory::prime( BUDDYDB_QRY,  buddyDB );

                ForceReloadTestFixture s;
                s.config.edit( "BuddyDatabaseInclusionRule", inclusionRule[iteration] );

                s.init(); 

                ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );
                LocalRunIterator begin( s->localBegin(), s->localEnd() ), end;
                ensure_equals( std::distance( begin, end ), expectedLocalEntries[iteration] );
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

		MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-832455,reveal,ldb25,\n" );
		MockConnectionFactory::prime( WORKLIST_QRY,
//rec  machine  barcode   test     group     c sample project p prof                  timestamp           seq s dil   result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,882431,\n"
"-36847,-1019430,118507091,-1031388,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,12,C,0.000,882429,\n" )
			;
		MockConnectionFactory::prime( BUDDYDB_QRY, 
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed      ,restx,update when        ,
"992431,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n")
			;
        MockConnectionFactory::prime( NONLOCALRESULTS_QRY,
//barcode, machine ,sample,dbname,result_id,test_id , res_value, act_flg,date_analysed      , res_text, update_when        ,rec_no,project
"118507091,-1019329,432560,REVEAL,882429   ,-1031388, 2.9      ,0       ,27-06-2013 11:57:47,2.9      , 27-06-2013 11:57:47,-36847,-832455,\n" )
            ;  

        ForceReloadTestFixture s( true );

        ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );
        ensure_equals( s.numLocalRuns()                               , 1 );

        LocalRunIterator begin( s->localBegin(), s->localEnd() );

        LocalRun   localRun   = *begin;
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

		MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-832455,reveal,ldb25,\n" );
        // Data same as for test 2, but project ID is zero
		MockConnectionFactory::prime( WORKLIST_QRY,
//rec  machine  barcode   test     group     c sample project p prof           timestamp           seq s dil   result
"24110,-1019430,118502164,-1031390,-12744865,0,417128,0,0,EDTA_1 analysis other,21-06-2013 11:07:57,39,Q,0.000,0,\n"
"24111,-1019430,118502164,-1031389,-12744865,0,417128,0,0,EDTA_1 analysis other,21-06-2013 11:07:57,34,Q,0.000,0,\n"
"24112,-1019430,118502164,-1031388,-12744865,0,417128,0,0,EDTA_1 analysis other,21-06-2013 11:07:57,35,Q,0.000,0,\n" )
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
"882290,118507091,27-06-2013 11:42:36,Unknown,432560,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n"
"882290,118507091,27-06-2013 11:42:36,Unknown,432560,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,\n"
"882290,118507091,27-06-2013 11:42:36,Unknown,432560,-1019349,882427,-1031386,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,\n"
"882290,118507091,27-06-2013 11:42:36,Unknown,432560,-1019349,882430,-1031389,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,\n";

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

            MockConnectionFactory::prime( CLUSTERS_QRY, CLUSTERS );
            MockConnectionFactory::prime( PROJECTS_QRY, PROJECTS );
            MockConnectionFactory::prime( WORKLIST_QRY, WORKLIST );
            MockConnectionFactory::prime( BUDDYDB_QRY,  BUDDYDB  );

            ForceReloadTestFixture s;

            s.config.edit( "ExceptionalDataHandler", exceptionalDataHandlers[i] );

            s.init();

            ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );
            ensure_equals( s.numLocalRuns(), i == 2 ? 1 : 0 );
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
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882427,-1031386,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882430,-1031389,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,\n";

        const char* exceptionalDataHandler = 
" function canProvideProjectIDFor         () return false end\n"
" function getProjectIDFor                () error( \"Cannot provide project ID\" ) return 0 end\n"
" function notifyBuddyDatabaseEntryIgnored() return true, true end\n"
" function notifyWorklistEntryIgnored     () return true, true end\n"
" function notifyCannotAllocateResultToWorklistEntry() return true, %s end\n";

        for ( int i = 0; i < 2; ++i )
        {
            MockConnectionFactory::reset();

            MockConnectionFactory::prime( CLUSTERS_QRY, CLUSTERS );
            MockConnectionFactory::prime( PROJECTS_QRY, PROJECTS );
            MockConnectionFactory::prime( WORKLIST_QRY, WORKLIST );
            MockConnectionFactory::prime( BUDDYDB_QRY,  BUDDYDB );

            ForceReloadTestFixture s;

            char buffer[2048];
            std::sprintf( buffer, exceptionalDataHandler, i ? "true" : "false" );
            s.config.edit( "ExceptionalDataHandler", buffer );

            s.init();

            ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );
            ensure_equals( s.numLocalRuns(), 1 );
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
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882427,-1031386,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882430,-1031389,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,\n";

        MockConnectionFactory::reset();

        MockConnectionFactory::prime( CLUSTERS_QRY, CLUSTERS );
        MockConnectionFactory::prime( PROJECTS_QRY, PROJECTS );
        MockConnectionFactory::prime( WORKLIST_QRY, WORKLIST );
        MockConnectionFactory::prime( BUDDYDB_QRY,  BUDDYDB  );

        ForceReloadTestFixture s( true );

        ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );
        ensure_equals( s.numLocalRuns(), 1 );

        const bool blockTillNoPendingUpdates = true;

        s->runPendingDatabaseUpdates( blockTillNoPendingUpdates );

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

		MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-1234,QCs,ldbqc,\n" );
		MockConnectionFactory::prime( WORKLIST_QRY, 
//rec  machine   barcode    test    grp c samp prj p prof                              time               seq s dil   result
"-36845,-1019349,QCRC100355,-1031390,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,14,C,0.000,882431,\n"
"-36847,-1019349,QCRC100355,-1031388,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,12,C,0.000,882429,\n"
"-36846,-1019349,QCRC100355,-1031389,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,13,C,0.000,882430,\n"
"-36848,-1019349,QCRC100355,-1031386,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,10,C,0.000,882427,\n" );

		MockConnectionFactory::prime( BUDDYDB_QRY,
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed ,restx,update when        ,
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n"
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,\n"
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882427,-1031386,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,\n"
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882430,-1031389,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,\n" )
			;
        

        MockConnectionFactory::prime( RULECONFIG_QRY, 
               "-1031390,-1019349,-1234,121,\n" 
               "-1031388,-1019349,-1234,121,\n" 
               "-1031389,-1019349,-1234,121,\n" 
               "-1031386,-1019349,-1234,121,\n" );


        MockConnectionFactory::prime( RULES_QRY, SerializedRecordset( 
                "1|121|configRule|a configuration rule|"
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
                    "   return { { result }, result.msg, result.resultCode } "
                    " end                                               |\n"
                "2|122|myRule|a rule|"
                    "return { resultCode = context.qc.testID + 2, rule = 'my rule', msg = 'Bob' }|\n",
                &pipeSeparatedFieldValues) );
 
        ForceReloadTestFixture s( true, true );

        LocalRunIterator begin( s->localBegin(), s->localEnd() ), end;
		ensure_equals( std::distance( begin, end ), 1 );

        LocalRun lr = *begin;

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

		MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-1234,QCs,ldbqc,\n" );
		MockConnectionFactory::prime( WORKLIST_QRY, 
//rec  machine   barcode    test    grp c samp prj p prof                              time               seq s dil   result
"-36845,-1019349,QCRC100355,-1031390,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,14,C,0.000,882431,\n" );

		MockConnectionFactory::prime( BUDDYDB_QRY, 
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed ,restx,update when        ,
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n" );
        

        MockConnectionFactory::prime( RULECONFIG_QRY, "-1031390,-1019349,-1234,121,\n" ); // Note: different test id from above.


        MockConnectionFactory::prime( RULES_QRY, SerializedRecordset( 
                "1|121|configRule|a configuration rule|"
                    " function onLoad(loadFunc)                         "
                    "   loadFunc('nonExistentRule')                     "
                    " end                                               "
                    "                                                   "
                    " function applyRules( qc )                         "
                    "   local result = { resultCode = 1, rule = 'x', msg = 'msg' }                           "
                    "   return { { result }, 'msg', 1 }                 "
                    " end                                               |\n",
                &pipeSeparatedFieldValues) );
 
        ForceReloadTestFixture s( false, true );

        s.config.edit( "RuleEngineErrorCode", "94949494" );

        s.init();

        LocalRunIterator begin( s->localBegin(), s->localEnd() ), end;
		ensure_equals( std::distance( begin, end ), 1 );

        LocalRun lr = *begin;

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

		MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-1234,QCs,ldbqc,\n" );
		MockConnectionFactory::prime( WORKLIST_QRY,
//rec  machine   barcode    test    grp c samp prj p prof                              time               seq s dil   result
"-36845,-1019349,QCRC100355,-1031390,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,14,C,0.000,882431,\n"
"-36847,-1019349,QCRC100355,-1031388,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,12,C,0.000,882429,\n"
"-36846,-1019349,QCRC100355,-1031389,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,13,C,0.000,882430,\n"
"-36848,-1019349,QCRC100355,-1031386,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,10,C,0.000,882427,\n" );

		MockConnectionFactory::prime( BUDDYDB_QRY,
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed ,restx,update when        ,
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n"
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,\n"
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882427,-1031386,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,\n"
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882430,-1031389,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,\n" );

        MockConnectionFactory::prime( RULECONFIG_QRY,
               "-1031390,-1019349,-1234,1,\n" 
               "-1031388,-1019349,-1234,2,\n" 
               "-1031389,-1019349,-1234,3,\n" 
               "-1031386,-1019349,-1234,4,\n" );

        const char* ruleConfigTemplate =
                "\%d|\%d|configRule\%d|config rule \%d|             "
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
                "   return { results, 'msg', context.total }        "
                " end                                               |\n";
 
        MockConnectionFactory::prime( RULES_QRY, SerializedRecordset( 
            paulst::format( ruleConfigTemplate, 10, 1, 1, 1, "'plus1','plus5'" ) +
            paulst::format( ruleConfigTemplate, 20, 2, 2, 2, "'plus4','plus6'" ) +
            paulst::format( ruleConfigTemplate, 30, 3, 3, 3, "'plus3','plus1'" ) +
            paulst::format( ruleConfigTemplate, 40, 4, 4, 4, "'plus6','plus7'" ) +
                std::string(
                "101|5|plus1|plus 1| context.total = context.total + 1  return { resultCode = 1, rule = 'plus1', msg = 'OK' }|\n"
                "109|6|plus2|plus 2| context.total = context.total + 2  return { resultCode = 2, rule = 'plus2', msg = 'OK' }|\n"
                "102|7|plus3|plus 3| context.total = context.total + 3  return { resultCode = 3, rule = 'plus3', msg = 'OK' }|\n"
                "107|8|plus4|plus 4| context.total = context.total + 4  return { resultCode = 4, rule = 'plus4', msg = 'OK' }|\n"
                "103|9|plus5|plus 5| context.total = context.total + 5  return { resultCode = 5, rule = 'plus5', msg = 'OK' }|\n"
                "105|10|plus6|plus 6| context.total = context.total + 6  return { resultCode = 6, rule = 'plus6', msg = 'OK' }|\n"
                "108|11|plus7|plus 7| context.total = context.total + 7  return { resultCode = 7, rule = 'plus7', msg = 'OK' }|\n"),
                &pipeSeparatedFieldValues) );
 
        ForceReloadTestFixture s( true, true );

        LocalRunIterator begin( s->localBegin(), s->localEnd() ), end;

		ensure_equals( std::distance( begin, end ), 1 );

        LocalRun lr = *begin;

        Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

        ensure_equals( std::distance( wles.first, wles.second ), 4U );

        std::map< int, int > expectedResults;

        expectedResults[882431] = /*testID -1031390, configRule1*/ 6  /*plus1 plus5*/;
        expectedResults[882429] = /*testID -1031388, configRule2*/ 10 /*plus4 plus6*/;
        expectedResults[882427] = /*testID -1031386, configRule4*/ 13 /*plus6 plus7*/;
        expectedResults[882430] = /*testID -1031389, configRule3*/ 4  /*plus3 plus1*/;

        std::map< int, int > expectedRuleRecordIDs;

        expectedRuleRecordIDs[882431] = /*configRule1*/ 10;
        expectedRuleRecordIDs[882429] = /*configRule2*/ 20;
        expectedRuleRecordIDs[882427] = /*configRule4*/ 40;
        expectedRuleRecordIDs[882430] = /*configRule3*/ 30;

        for ( WorklistEntryIterator i = wles.first; i != wles.second; ++i )
        {
            const WorklistEntry* wle = *i;
            const TestResult* tr = testResultFor(wle);
            const int resultID = tr->getID();

            ensure( s->hasRuleResults( resultID ) );
            RuleResults rr = s->getRuleResults( resultID );
            ensure_equals( rr.getSummaryResultCode(), expectedResults[resultID] );

            RuleDescriptor rd = rr.getRuleDescriptor();
            ensure_equals( rd.getRecordID(), expectedRuleRecordIDs[resultID] );
        }
    }

    template<>
	template<>
	void testForceReload::test<18>()
	{
    	set_test_name("ForceReload - 'same' QC but each result has a different buddy_database entry.");

		using namespace valc;

        MockConnectionFactory::reset();

		MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-1234,QCs,ldbqc,\n" );
		MockConnectionFactory::prime( WORKLIST_QRY, 
//rec  machine   barcode    test    grp c samp prj p prof                              time               seq s dil   result
"-36845,-1019349,QCRC100355,-1031390,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,14,C,0.000,882431,\n"
"-36847,-1019349,QCRC100355,-1031388,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,12,C,0.000,882429,\n" );

		MockConnectionFactory::prime( BUDDYDB_QRY, 
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed ,restx,update when        ,
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n"
"882291,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,\n" );

        ForceReloadTestFixture s(true);

        const int expectedNumLocalRuns = 2;

        int expectedWorklistIDSequence[expectedNumLocalRuns] = { -36845, -36847 };

        int counter = 0;

        std::set< std::string > localRunIDs;

        for ( LocalRunIterator localRuns( s->localBegin(), s->localEnd() ), end; localRuns != end; ++localRuns )
        {
            LocalRun lr = *localRuns;

            localRunIDs.insert( lr.getRunID() );

            Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

            ensure_equals( "There should be only one worklist entry for each local run", 
                std::distance( wles.first, wles.second ), 1 );

            ensure( "Only expecting 2 local runs", counter < expectedNumLocalRuns );

            const int expectedWorklistID = expectedWorklistIDSequence[counter];

            const WorklistEntry* wle = *(wles.first);

            ensure_equals( wle->getID(), expectedWorklistID );

            ensure( "The test result for worklist entry should be local to this run",
                s->compareSampleRunIDs( testResultFor( wle )->getSampleRunID(), lr.getRunID() ) );

            ++counter;
        }

        ensure_equals( counter, expectedNumLocalRuns );
        ensure_equals( localRunIDs.size(), expectedNumLocalRuns );

        // Each runID should be unique
        BOOST_FOREACH( std::string runID, localRunIDs )
        {
            ensure_equals( 1, std::count_if( localRunIDs.begin(), localRunIDs.end(), boost::bind(sameRun, s.get(), runID, _1) ) );
        } 
    }

    template<>
	template<>
	void testForceReload::test<19>()
	{
    	set_test_name("ForceReload - as 18, but both QC results share the same  buddy_database entry.");

		using namespace valc;

        MockConnectionFactory::reset();

		MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-1234,QCs,ldbqc,\n" );
		MockConnectionFactory::prime( WORKLIST_QRY, 
//rec  machine   barcode    test    grp c samp prj p prof                              time               seq s dil   result
"-36845,-1019349,QCRC100355,-1031390,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,14,C,0.000,882431,\n"
"-36847,-1019349,QCRC100355,-1031388,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,12,C,0.000,882429,\n" );

		MockConnectionFactory::prime( BUDDYDB_QRY,
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed ,restx,update when        ,
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n"
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,\n" );

        ForceReloadTestFixture s(true);

        LocalRunIterator begin( s->localBegin(), s->localEnd() ), end;

		ensure_equals( "There should be only one local run", std::distance( begin, end ), 1 );

        LocalRun lr = *begin;

        Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

        ensure_equals( "There should be 2 worklist entries for this run", 2, std::distance( wles.first, wles.second ) );

        for ( WorklistEntryIterator i = wles.first; i != wles.second; ++i )
        {
            const WorklistEntry* wle = *i;
            ensure( "The test result for the worklist entry should be local to this run",
                s->compareSampleRunIDs( testResultFor( wle )->getSampleRunID(), lr.getRunID() ) );
        }
    }

    // Next: when there's an existing sample-run that is open, .. closed

    template<>
	template<>
	void testForceReload::test<20>()
	{
    	set_test_name("ForceReload - as 19, but the 2nd got picked up on a 2nd force-reload, meaning the first has a persisted entry in the sample_run table.");

		using namespace valc;

        MockConnectionFactory::reset();

		MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-1234,QCs,ldbqc,\n" );
		MockConnectionFactory::prime( WORKLIST_QRY, 
//rec  machine   barcode    test    grp c samp prj p prof                              time               seq s dil   result
"-36845,-1019349,QCRC100355,-1031390,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,14,C,0.000,882431,\n"
"-36847,-1019349,QCRC100355,-1031388,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,12,C,0.000,882429,\n" );

        std::string tests[2] = {
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed ,restx,update when        ,
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,",
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,"
            };

        //                                runID, isOpen, when created       , when closed, sequence position,fao_level_one,group_id
        std::string sampleRunData[2] = { "   12,      1,27-06-2013 11:42:36,,882290,y,,",
                                         ",,,,,,," };

        MockConnectionFactory::prime( BUDDYDB_QRY, 
            tests[0] + sampleRunData[0] + "\n" +
            tests[1] + sampleRunData[1] + "\n" )
            ;

        ForceReloadTestFixture s(true);

        LocalRunIterator begin( s->localBegin(), s->localEnd() ), end;
		ensure_equals( "There should be only one local run", std::distance( begin, end ), 1 );

        LocalRun lr = *begin;

        Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

        ensure_equals( "There should be 2 worklist entries for this run", 2, std::distance( wles.first, wles.second ) );

        for ( WorklistEntryIterator i = wles.first; i != wles.second; ++i )
        {
            const WorklistEntry* wle = *i;
            ensure( "The test result for the worklist entry should be local to this run",
                s->compareSampleRunIDs( testResultFor( wle )->getSampleRunID(), lr.getRunID() ) );
        }
		
		ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );
    }

    template<>
	template<>
	void testForceReload::test<21>()
	{
    	set_test_name("ForceReload - as 18, but the 2nd got picked up on a 2nd force-reload, meaning the first has a persisted entry in the sample_run table.");

		using namespace valc;

        MockConnectionFactory::reset();

		MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-1234,QCs,ldbqc,\n" );
		MockConnectionFactory::prime( WORKLIST_QRY,
//rec  machine   barcode    test    grp c samp prj p prof                              time               seq s dil   result
"-36845,-1019349,QCRC100355,-1031390,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,14,C,0.000,882431,\n"
"-36847,-1019349,QCRC100355,-1031388,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,12,C,0.000,882429,\n" );

        std::string tests[2] = {
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed ,restx,update when        ,
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,",
"882291,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,"
            };

        //                                runID, isOpen, when created       , when closed, sequence position,fao_level_one,group_id
        std::string sampleRunData[2] = { "   12,      1,27-06-2013 11:42:36,,882290,y,,",
                                         ",,,,,,," };

        MockConnectionFactory::prime( BUDDYDB_QRY, 
            tests[0] + sampleRunData[0] + "\n" +
            tests[1] + sampleRunData[1] + "\n" )
            ;

        ForceReloadTestFixture s(true);

		ensure_equals( s.numLocalRuns(), 2 );
    }


    template<>
	template<>
	void testForceReload::test<22>()
	{
    	set_test_name("ForceReload - QC worklist entry at status 'P'.");

		using namespace valc;

        MockConnectionFactory::reset();

		MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-1234,QCs,ldbqc,\n" );
		MockConnectionFactory::prime( WORKLIST_QRY, 
//rec  machine   barcode    test    grp c samp prj p prof                              time               seq s dil   result
"-36845,-1019349,QCRC100355,-1031390,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,14,C,0.000,882431,\n"
"-36847,-1019349,QCRC100355,-1031388,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,12,P,0.000,0,\n" );

		MockConnectionFactory::prime( BUDDYDB_QRY,
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed ,restx,update when        ,
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n" );

        ForceReloadTestFixture s(true);

		ensure_equals( s.numLocalRuns(), 1 );

        LocalRunIterator localRuns( s->localBegin(), s->localEnd() );

        LocalRun lr = *localRuns;

        Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

        ensure_equals( "There should be 2 worklist entries for this run", 2, std::distance( wles.first, wles.second ) );

        Range<TestResultIterator> results = worklistEntry( wles, -36845 )->getTestResults();

        ensure_equals( "There should be 1 result for worklist entry -36845", 1, std::distance( results.first, results.second ) );

        results = worklistEntry( wles, -36847 )->getTestResults();

        ensure_equals( "There should be no results for worklist entry -36847", 0, std::distance( results.first, results.second ) );
    }

    template<>
	template<>
	void testForceReload::test<23>()
	{
    	set_test_name("ForceReload - QC worklist entry at status 'P'. Cut-down version of 22.");

		using namespace valc;

        MockConnectionFactory::reset();

		MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-1234,QCs,ldbqc,\n" );
		MockConnectionFactory::prime( WORKLIST_QRY,
//rec  machine   barcode    test    grp c samp prj p prof                              time               seq s dil   result
"-36847,-1019349,QCRC100355,-1031388,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,12,P,0.000,0,\n" );

		MockConnectionFactory::prime( BUDDYDB_QRY, 
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed ,restx,update when        ,
"882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,,,,,,,,,,,,,,\n" );

        ForceReloadTestFixture s(true);

		ensure_equals( "There should be only one local run", s.numLocalRuns(), 1 );

        LocalRunIterator localRuns( s->localBegin(), s->localEnd() );

        LocalRun lr = *localRuns;

        Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

        ensure_equals( "There should be only one worklist entry", 1, std::distance( wles.first, wles.second ) );

        Range<TestResultIterator> results = worklistEntry( wles, -36847 )->getTestResults();

        ensure_equals( "There should be no result for worklist entry -36847", 0, std::distance( results.first, results.second ) );
    }

    template<>
	template<>
	void testForceReload::test<24>()
	{
    	set_test_name(
        "ForceReload - a 'P' QC will go with the latest buddy_database entry, unless the latest has a result already for the same test.");

		using namespace valc;

        const int idOfFirstTest [] = { -1031390, -1031390, -1031388 };
        const int idOfSecondTest[] = { -1031390, -1031388, -1031388 };
        const int testIDForPWorklistEntry =      -1031388;


        for ( int testCase = 0; testCase < 3; ++testCase )
        {
            MockConnectionFactory::reset();

            MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
            MockConnectionFactory::prime( PROJECTS_QRY, "-1234,QCs,ldbqc,\n" );
            MockConnectionFactory::prime( WORKLIST_QRY, 
    paulst::format(
    //rec  machine   barcode    test    grp c samp prj p prof                              time               seq s dil   result
    "-36845,-1019349,QCRC100355,\%d     ,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,14,C,0.000,882431,\n"
    "-36847,-1019349,QCRC100355,\%d     ,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,12,C,0.000,882429,\n"
    "-36849,-1019349,QCRC100355,\%d     ,0,0,0,-1234,0,Randox custom QC QC level 1 707UNCM,27-06-2013 10:57:49,12,P,0.000,0,\n",
        idOfSecondTest[testCase], idOfFirstTest[testCase], testIDForPWorklistEntry ) );

            MockConnectionFactory::prime( BUDDYDB_QRY,
    paulst::format(
    //bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed ,restx,update when        ,
    "882290,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882429,\%d     ,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n"
    "882291,QCRC100355,27-06-2013 11:42:36,ldbqc,0,-1019349,882431,\%d     ,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,\n",
        idOfFirstTest[testCase], idOfSecondTest[testCase] ) );

            ForceReloadTestFixture s(true);

            ensure_equals( "There should be 2 local runs", s.numLocalRuns(), 2 );

            int listPosition = 0;
            int expectedWorklistEntries [2];

            switch( testCase )
            {
                case 0: 
                    expectedWorklistEntries[0] = 1;
                    expectedWorklistEntries[1] = 2;
                    break;
                case 1:
                    expectedWorklistEntries[0] = 2;
                    expectedWorklistEntries[1] = 1;
                    break;
                case 2:
                    expectedWorklistEntries[0] = 1;
                    expectedWorklistEntries[1] = 1;
                    break;
            }

            for ( LocalRunIterator localRuns( s->localBegin(), s->localEnd() ), end; localRuns != end; ++localRuns )
            {
                ensure( listPosition < 2 );

                LocalRun lr = *localRuns;

                Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );
                
                ensure_equals( paulst::format( 
                    "Test iteration: \%d. LocalRun#\%d. There should be \%d worklist entry/entries", 
                    testCase, listPosition, expectedWorklistEntries[listPosition] ).c_str(),
                    std::distance( wles.first, wles.second ), expectedWorklistEntries[listPosition]  );

                ++listPosition;
            }

            ensure_equals( listPosition, 2 );
        }
    }
 
    template<>
	template<>
	void testForceReload::test<25>()
	{
		set_test_name("ForceReload - loading from worklist_relation.");

		using namespace valc;

        std::vector<WorklistRelative> children;

        const std::string worklist(
//rec  mac   barcode   test     group     c sample project p prof                  timestamp         seq s dil   result
"-1,-1019430,11850701,-1031390,-12750394,0,432561,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,1,\n"
"-2,-1019430,11850702,-1031390,-12750394,0,432562,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,12,C,0.000,2,\n"
"-3,-1019430,11850703,-1031390,-12750394,0,432563,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,13,C,0.000,3,\n"
"-4,-1019430,11850704,-1031390,-12750394,0,432564,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,10,C,0.000,4,\n");

        const std::string buddyDB(
//bsid ,barcode ,date analysed      ,dbname,sample,macine  ,res id,test,result,a,date analysed      ,restx,update when        ,
"882291,11850701,27-06-2013 11:42:36,REVEAL,432561,-1019349,1,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n"
"882292,11850702,27-06-2013 11:42:36,REVEAL,432562,-1019349,2,-1031390,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,\n"
"882293,11850703,27-06-2013 11:42:36,REVEAL,432563,-1019349,3,-1031390,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,\n"
"882294,11850704,27-06-2013 11:42:36,REVEAL,432564,-1019349,4,-1031390,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,\n");

        MockConnectionFactory::reset();

        MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
        MockConnectionFactory::prime( PROJECTS_QRY, "-832455,reveal,ldb25,\n" );
        MockConnectionFactory::prime( WORKLIST_QRY, worklist );
        MockConnectionFactory::prime( BUDDYDB_QRY,  buddyDB );
        MockConnectionFactory::prime( "LoadWorklistRelation",  
            "-2,-4,r,\n"
            "-1,-2,r,\n"
            "-4,-5,r,\n"
            );

        ForceReloadTestFixture s;
        s.config.edit( "WorklistRelationQuery", "LoadWorklistRelation" );
        s.init(); 

        ensure_equals( s.numLocalRuns(), 4 );

        LocalRunIterator localRuns( s->localBegin(), s->localEnd() );

        LocalRun lr = *localRuns;

		Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

        const WorklistEntry* e = worklistEntry( wles, -1 );

        ensure_equals( e->getID(), -1 );

        WorklistRelative wr = s->viewRelatively( e );

        ensure_equals( wr.getID(), -1 );
        ensure       ( wr.isBoundToWorklistEntryInstance() );
        ensure_equals( wr->getID(), -1 );
        ensure       ( wr.hasChildren() );
        children = wr.getChildren();
        ensure_equals( children.size(), 1 );
        wr = children.at(0);
        ensure_equals( wr.getID(), -2 );
        ensure       ( wr.isBoundToWorklistEntryInstance() );
        ensure_equals( wr->getID(), -2 );
        ensure       ( wr.hasChildren() );
        children = wr.getChildren();
        ensure_equals( children.size(), 1 );
        wr = children.at(0);
        ensure_equals( wr.getID(), -4 );
        ensure       ( wr.isBoundToWorklistEntryInstance() );
        ensure_equals( wr->getID(), -4 );
        ensure       ( wr.hasChildren() );
        children = wr.getChildren();
        ensure_equals( children.size(), 1 );
        wr = children.at(0);
        ensure_equals( wr.getID(), -5 );
        ensure_not   ( wr.isBoundToWorklistEntryInstance() );
        ensure_not   ( wr.hasChildren() );

        localRuns += 2;
        lr = *localRuns;

		wles = s->getWorklistEntries( lr.getSampleDescriptor() );

        wr = s->viewRelatively( worklistEntry( wles, -3 ) );

        ensure_equals( wr.getID(), -3 );
        ensure       ( wr.isBoundToWorklistEntryInstance() );
        ensure_equals( wr->getID(), -3 );
        ensure_not   ( wr.hasChildren() );
	}
 
    template<>
	template<>
	void testForceReload::test<26>()
	{
		set_test_name("ForceReload - loading buddy_sample_run.group_id");

        using namespace valc;

        MockConnectionFactory::reset();

        MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
        MockConnectionFactory::prime( PROJECTS_QRY, "-832455,reveal,ldb25,\n" );
        MockConnectionFactory::prime( WORKLIST_QRY, 
//rec  machine  barcode   test     group      c sample project p prof                  timestamp           seq s dil  result
"-36846,-1019430,118507091,-1031391,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n" );

        std::string testResult = 
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,res ,a,date analysed      ,restx,update when      ,cbw
"882291,118507091,27-06-2013 11:55:36,REVEAL,432560,-1019349,882432,-1031391,1.3 ,0,27-06-2013 11:57:47,1.3,27-06-2013 10:57:49,-36846,";

        //                            runID, isOpen, when created       , when closed, sequence position,fao_level_one,group_id
        std::string sampleRunData =  "   12,      1,27-06-2013 11:42:36,,882290,y,87,";

        MockConnectionFactory::prime( BUDDYDB_QRY, testResult + sampleRunData + "\n" );

        ForceReloadTestFixture s( true );

        ensure_equals( "There should be just the one sample run", s.numLocalRuns(), 1U );

        LocalRunIterator localRuns( s->localBegin(), s->localEnd() );

        LocalRun lr = *localRuns;

        ensure_equals( lr.getGroupID(), 87 );
    }
 
};

#endif

