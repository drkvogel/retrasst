#ifndef CONTROLMODELTESTH
#define CONTROLMODELTESTH

#include "API.h"
#include "ControlModel.h"
#include "MockSampleRunGroupIDGenerator.h"
#include "Nullable.h"
#include "RunIDC14n.h"
#include "SampleRunGroupModel.h"
#include "SampleRunIDResolutionService.h"
#include <set>
#include "StrUtil.h"
#include <tut.h>
#include "UncontrolledResult.h"


namespace tut
{
	class ControlModelTestFixture
    {
        valc::ControlModelImpl                  controlModel;
        MockSampleRunGroupIDGenerator*          mockIDGenerator;
        valc::SampleRunGroupModel*              sampleRunGroupModel;
        valc::RunIDC14n*                        runIDC14n;
        int                                     resultIDSequence;
        valc::RuleDescriptor                    ruleDescriptor;
        std::set< std::string >                 runIDsAssignedToGroup;

    public:

        valc::SampleRunIDResolutionService*     sampleRunIDResolutionService;

        static const int TEST_A = 24;
        static const int TEST_B = 27;

        ControlModelTestFixture()
            :
            mockIDGenerator             ( new MockSampleRunGroupIDGenerator() ),
            sampleRunGroupModel         ( new valc::SampleRunGroupModel(mockIDGenerator) ),
            sampleRunIDResolutionService( new valc::SampleRunIDResolutionService() ),
            runIDC14n                   ( 0 ),
            resultIDSequence            ( 0 ),
            ruleDescriptor              ( 1, 1, "1:2s", "1:2s" )
        {
            for ( int i = 1; i < 20; ++i )
            {
                mockIDGenerator->prime(i);
            }
        }

        ~ControlModelTestFixture()
        {
            delete sampleRunGroupModel;
            delete sampleRunIDResolutionService;
            delete runIDC14n;
        }

        void primeQC( const std::string& runID, int testID, valc::ResultCode outcome, int resultID = -1 )
        {
            if ( resultID == -1 )
            {
                resultID = ++resultIDSequence;
            }
            controlModel.notifyQCEvaluationStarted  ( buildUncontrolledResult( resultID, testID, runID ) );
            controlModel.notifyQCEvaluationCompleted( buildRuleResults( outcome ), resultID );
            if ( ! runIDsAssignedToGroup.count( runID ) )
            {
                sampleRunGroupModel->assignToGroup( runID, true, paulst::Nullable<int>() );
                runIDsAssignedToGroup.insert( runID );
            }
        }

        void primeUnk( const std::string& runID )
        {
            sampleRunGroupModel->assignToGroup( runID, false, paulst::Nullable<int>() );
        }

        void run()
        {
            runIDC14n = new valc::RunIDC14n( *sampleRunIDResolutionService );
            controlModel.setRunIDC14n( runIDC14n );
            controlModel.setSampleRunGroupModel( sampleRunGroupModel );
        }

        valc::ControlStatus getControlStatus( int testID, const std::string& runID )
        {
            return controlModel.getControlStatus( testID, runID );
        }

    private:

        valc::UncontrolledResult buildUncontrolledResult( int resultID, int testID, const std::string& runID )
        {
            valc::UncontrolledResult r;
            r.testID = testID;
            r.resultID = resultID;
            r.runID = runID;
            return r;
        }

        valc::RuleResults buildRuleResults( valc::ResultCode outcome )
        {
            valc::RuleResults::RuleResultCollection empty;

            return valc::RuleResults( ruleDescriptor, empty.begin(), empty.end(), outcome, "No message" );
        }
    };

    typedef test_group<ControlModelTestFixture, 8> ControlModelTestGroup;
	ControlModelTestGroup testGroupControlModel( "ControlModel tests");
	typedef ControlModelTestGroup::object testControlModel;

	template<>
	template<>
	void testControlModel::test<1>()
	{
		set_test_name("ControlModel behaviour specification");

		using namespace valc;

        const std::string runID   = "sampleRunID";

        primeQC ( "qcRun1", TEST_A, RESULT_CODE_PASS );
        primeUnk( runID );
        primeQC ( "qcRun2", TEST_A, RESULT_CODE_PASS, 4 );

        run();

        ControlStatus cs = getControlStatus( TEST_A, runID );

        ensure_equals( "Should be a PASS"                       , cs.summaryCode()                  , CONTROL_STATUS_PASS );
        ensure_equals( "Should be 1 preceding QC"               , cs.precedingQCs().size()          , 1 );
        ensure_equals( "ID of preceding QC should be 1"         , cs.precedingQCs()[0].resultID()   , 1 );
        ensure_equals( "Status of preceding QC should be PASS"  , cs.precedingQCs()[0].status()     , RESULT_CODE_PASS );
        ensure_equals( "Should be 1 following QC"               , cs.followingQCs().size()          , 1 );
        ensure_equals( "ID of following QC should be 4"         , cs.followingQCs()[0].resultID()   , 4 );
        ensure_equals(                                            cs.followingQCs()[0].status()     , RESULT_CODE_PASS );

        cs = getControlStatus( TEST_B, runID );

        ensure_equals(                                             cs.summaryCode()                 , CONTROL_STATUS_UNCONTROLLED );
        ensure_equals(                                             cs.precedingQCs().size()         , 0 );
        ensure_equals(                                             cs.followingQCs().size()         , 0 );

        bool exceptionThrown = false;

        try
        {
            cs.precedingQCs()[1];
        }
        catch( const Exception& e )
        {
            exceptionThrown = true;
        }

        ensure( exceptionThrown );

        exceptionThrown = false;

        try
        {
            getControlStatus( TEST_A, "what run ID is this?" );
        }
        catch( const Exception& e )
        {
            exceptionThrown = true;
        }

        ensure( exceptionThrown );

        exceptionThrown = false;

        try
        {
            getControlStatus( TEST_A, "qcRun1" );
        }
        catch( const Exception& e )
        {
            exceptionThrown = true;
        }

        ensure( exceptionThrown );
    }

	template<>
	template<>
	void testControlModel::test<2>()
	{
		set_test_name("When the TestResult.SampleRunID is different from the LocalRun.RunID");

		using namespace valc;
        
        primeQC ( "qcRun1", TEST_A, RESULT_CODE_PASS );
        primeUnk( "sampleRun1" );
        primeQC ( "qcRun2", TEST_A, RESULT_CODE_PASS );

        sampleRunIDResolutionService->addMapping( "sampleRun2", "sampleRun1" );

        run();

        ControlStatus cs;

        try
        {
            cs = getControlStatus( TEST_A, "sampleRun2" );
            ensure_equals( cs.summaryCode(), CONTROL_STATUS_PASS );

            cs = getControlStatus( TEST_A, "sampleRun1" );
            ensure_equals( cs.summaryCode(), CONTROL_STATUS_PASS );
        }
        catch( const Exception& e )
        {
            wprintf( e.Message.c_str() );
            ensure( false );
        }
    }

	template<>
	template<>
	void testControlModel::test<3>()
	{
		set_test_name("When some QCs are good and some are bad.");

		using namespace valc;
        
        primeQC ( "qcRun1", TEST_A, RESULT_CODE_PASS );
        primeUnk( "sampleRun1" );
        primeQC ( "qcRun2", TEST_A, RESULT_CODE_FAIL );
        
        run();

        ControlStatus cs = getControlStatus( TEST_A, "sampleRun1" );
        ensure_equals( cs.summaryCode(), CONTROL_STATUS_FAIL );
    }

    template<>
	template<>
	void testControlModel::test<4>()
	{
		set_test_name("Variation on (3) - more than one QC run (before and after)");

		using namespace valc;
        
        primeQC ( "qcRun1", TEST_A, RESULT_CODE_FAIL );
        primeQC ( "qcRun2", TEST_A, RESULT_CODE_PASS );
        primeUnk( "sampleRun1" );
        primeQC ( "qcRun3", TEST_A, RESULT_CODE_PASS );
        primeQC ( "qcRun4", TEST_A, RESULT_CODE_PASS );
        
        run();

        ControlStatus cs = getControlStatus( TEST_A, "sampleRun1" );
        ensure_equals( cs.summaryCode(), CONTROL_STATUS_FAIL );
    }

    template<>
	template<>
	void testControlModel::test<5>()
	{
		set_test_name("Variation on (4) - 'ERROR' beats 'FAIL'");

		using namespace valc;
        
        primeQC ( "qcRun1", TEST_A, RESULT_CODE_FAIL );
        primeQC ( "qcRun2", TEST_A, RESULT_CODE_PASS );
        primeUnk( "sampleRun1" );
        primeQC ( "qcRun3", TEST_A, RESULT_CODE_PASS );
        primeQC ( "qcRun4", TEST_A, RESULT_CODE_ERROR );
        
        run();

        ControlStatus cs = getControlStatus( TEST_A, "sampleRun1" );
        ensure_equals( cs.summaryCode(), CONTROL_STATUS_ERROR );
    }

	template<>
	template<>
	void testControlModel::test<6>()
	{
		set_test_name("When no preceding QCs.");

		using namespace valc;
        
        primeUnk( "sampleRun1" );
        primeQC ( "qcRun3", TEST_A, RESULT_CODE_PASS );
        primeQC ( "qcRun4", TEST_A, RESULT_CODE_ERROR );

        run();

        ControlStatus cs = getControlStatus( TEST_A, "sampleRun1" );
        ensure_equals( cs.summaryCode()         , CONTROL_STATUS_UNCONTROLLED );
        ensure_equals( cs.precedingQCs().size() , 0 );
        ensure_equals( cs.followingQCs().size() , 2 );
    }

	template<>
	template<>
	void testControlModel::test<7>()
	{
		set_test_name("When no following QCs.");

		using namespace valc;

        primeQC ( "qcRun1", TEST_A, RESULT_CODE_PASS );
        primeQC ( "qcRun2", TEST_A, RESULT_CODE_ERROR );
        primeUnk( "sampleRun1" );

        run();

        ControlStatus cs = getControlStatus( TEST_A, "sampleRun1" );
        ensure_equals( cs.summaryCode()         , CONTROL_STATUS_UNCONTROLLED );
        ensure_equals( cs.precedingQCs().size() , 2 );
        ensure_equals( cs.followingQCs().size() , 0 );
    }

	template<>
	template<>
	void testControlModel::test<8>()
	{
		set_test_name("Several batches.");

		using namespace valc;
        
        try
        {
            primeQC ( "qcRun1", TEST_A, RESULT_CODE_PASS );
            primeQC ( "qcRun1", TEST_B, RESULT_CODE_PASS );

            primeQC ( "qcRun2", TEST_A, RESULT_CODE_PASS );

            primeUnk( "sampleRun1" );

            primeUnk( "sampleRun2" );

            primeQC ( "qcRun3", TEST_A, RESULT_CODE_PASS );
            primeQC ( "qcRun3", TEST_B, RESULT_CODE_FAIL );

            primeQC ( "qcRun4", TEST_A, RESULT_CODE_PASS );

            primeUnk( "sampleRun3" );

            primeUnk( "sampleRun4" );

            primeQC ( "qcRun5", TEST_A, RESULT_CODE_PASS );
            primeQC ( "qcRun5", TEST_B, RESULT_CODE_PASS );

            primeUnk( "sampleRun5" );

            primeQC ( "qcRun6", TEST_B, RESULT_CODE_PASS );

            primeUnk( "sampleRun6" );

            primeQC ( "qcRun7", TEST_B, RESULT_CODE_PASS );

            run();

            ensure_equals( getControlStatus( TEST_A, "sampleRun1" ).summaryCode(), CONTROL_STATUS_PASS );
            ensure_equals( getControlStatus( TEST_B, "sampleRun1" ).summaryCode(), CONTROL_STATUS_FAIL );

            ensure_equals( getControlStatus( TEST_A, "sampleRun4" ).summaryCode(), CONTROL_STATUS_PASS );
            ensure_equals( getControlStatus( TEST_B, "sampleRun4" ).summaryCode(), CONTROL_STATUS_FAIL );

            ensure_equals( getControlStatus( TEST_B, "sampleRun5" ).summaryCode(), CONTROL_STATUS_PASS );

            ensure_equals( getControlStatus( TEST_A, "sampleRun6" ).summaryCode(), CONTROL_STATUS_UNCONTROLLED );
            ensure_equals( getControlStatus( TEST_B, "sampleRun6" ).summaryCode(), CONTROL_STATUS_PASS );
        }
        catch( const Exception& e )
        {
            wprintf( e.Message.c_str() );
            ensure( false );
        }
    }
};

#endif

















