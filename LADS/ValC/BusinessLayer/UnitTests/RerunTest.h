#ifndef RERUNTESTH
#define RERUNTESTH

#include "API.h"
#include "LocalRunIterator.h"
#include "MockConnectionFactory.h"
#include "SnapshotTestFixture.h"
#include <tut.h>

class MockSnapshotObserver : public valc::SnapshotObserver
{
public:
    int updateFailures;
    MockSnapshotObserver() : updateFailures(0)                              {}
    void notifyWorklistEntryChanged ( const valc::WorklistEntry* we )       {}
    void notifyNewWorklistEntry     ( const valc::WorklistEntry* we )       {}
    void notifySampleRunClosedOff   ( const valc::IDToken& runID )          {}
    void notifySampleAddedToQueue   ( const std::string& sampleDescriptor ) {}
    void notifyUpdateFailed         ( const char* errorMsg )                { ++updateFailures; }
    };

namespace tut
{
	class RerunTestFixture : public SnapshotTestFixture
    {
    public:
        static const int LOCAL_MACHINE_ID = -1019349;
        static const int USER_ID          = 1234;

        RerunTestFixture( bool initialise = false, bool suppressLogMessages = true )
            : SnapshotTestFixture( LOCAL_MACHINE_ID, USER_ID, initialise, suppressLogMessages )
        {
        }
        
        valc::SnapshotPtr operator->() const
        {
            return get();
        }

    };

    typedef test_group<RerunTestFixture, 2> RerunTestGroup;
	RerunTestGroup testGroupRerun( "Rerun tests");
	typedef RerunTestGroup::object testRerun;

	template<>
	template<>
	void testRerun::test<1>()
	{
		set_test_name("Queuing a rerun");

		using namespace valc;

        MockSnapshotObserver snapshotObserver;
        MockConnectionFactory::reset();

		MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-832455,reveal,ldb25,\n" );
		MockConnectionFactory::prime( WORKLIST_QRY, 
//rec  machine  barcode   test     group     c sample project p prof                  timestamp           seq s dil   result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,882431,\n")
			;
		MockConnectionFactory::prime( BUDDYDB_QRY,
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed      ,restx,update when        ,
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n")
			;

        MockConnectionFactory::prime( "SQLStmtNextWorklistID", "223,\n" );
        MockConnectionFactory::prime( "QueryForSampleRun", "1,\n" );

        try
        {
            RerunTestFixture s( true, true );

            ensure_equals( s.numLocalRuns(), 1 );
            ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );

            LocalRunIterator localRuns( s->localBegin(), s->localEnd() );

            LocalRun lr = *localRuns;

            Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

            ensure( std::distance( wles.first, wles.second ) == 1U );

            const WorklistEntry* wle = *(wles.first);

            WorklistRelative wr = s->viewRelatively( wle );

            ensure_not( wr.hasChildren() );
            ensure    ( lr.isOpen()      );

            s->runPendingDatabaseUpdates( true );

            s->setObserver( &snapshotObserver );

            HANDLE h = s->queueForRerun( -36845, lr.getRunID(), lr.getSampleDescriptor() );

            DWORD waitResult = WaitForSingleObject( h, 1000 );

            CloseHandle(h);

            ensure_equals( waitResult, WAIT_OBJECT_0 );

            ensure_not   ( snapshotObserver.updateFailures );

            wles = s->getWorklistEntries( lr.getSampleDescriptor() );

            ensure( "There should now be 2 worklist entries for this sample", std::distance( wles.first, wles.second ) == 2U );
            ensure( wr.hasChildren() );

            ensure_not( lr.isOpen() );

            ensure_equals( s.numLocalRuns(), 1 );
            ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 1 );
        }
        catch( const Exception& e )
        {
            printf( "Exception: %s: ", AnsiString( e.Message.c_str() ).c_str() );
        }
    }

    template<>
	template<>
	void testRerun::test<2>()
	{
		set_test_name("Queuing a rerun - when the sample-run hasn't yet been persisted to the database.");

		using namespace valc;

        MockSnapshotObserver snapshotObserver;
        MockConnectionFactory::reset();

		MockConnectionFactory::prime( CLUSTERS_QRY, "-1019430,\n" );
		MockConnectionFactory::prime( PROJECTS_QRY, "-832455,reveal,ldb25,\n" );
		MockConnectionFactory::prime( WORKLIST_QRY, 
//rec  machine  barcode   test     group     c sample project p prof                  timestamp           seq s dil   result
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,882431,\n")
			;
		MockConnectionFactory::prime( BUDDYDB_QRY,
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed      ,restx,update when        ,
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,\n")
			;

        MockConnectionFactory::prime( "SQLStmtNextWorklistID", "223,\n" );
        MockConnectionFactory::prime( "QueryForSampleRun", "0,\n" );

        try
        {
            RerunTestFixture s( true, true );

            ensure_equals( s.numLocalRuns(), 1 );
            ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );

            LocalRunIterator localRuns( s->localBegin(), s->localEnd() );

            LocalRun lr = *localRuns;

            Range<WorklistEntryIterator> wles = s->getWorklistEntries( lr.getSampleDescriptor() );

            ensure( std::distance( wles.first, wles.second ) == 1U );

            const WorklistEntry* wle = *(wles.first);

            WorklistRelative wr = s->viewRelatively( wle );

            ensure_not( wr.hasChildren() );
            ensure    ( lr.isOpen()      );

            s->setObserver( &snapshotObserver );

            HANDLE h = s->queueForRerun( -36845, lr.getRunID(), lr.getSampleDescriptor() );

            DWORD waitResult = WaitForSingleObject( h, 1000 );

            CloseHandle(h);

            ensure_equals( waitResult, WAIT_OBJECT_0 );

            ensure       ( "There should have been notification of failure", snapshotObserver.updateFailures );

            wles = s->getWorklistEntries( lr.getSampleDescriptor() );

            ensure( "There should still be only one worklist entries for this sample", std::distance( wles.first, wles.second ) == 1U );
            ensure_not( wr.hasChildren() );

            ensure    ( lr.isOpen() );

            ensure_equals( s.numLocalRuns(), 1 );
            ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 0 );
        }
        catch( const Exception& e )
        {
            printf( "Exception: %s: ", AnsiString( e.Message.c_str() ).c_str() );
        }
    }

};

#endif

