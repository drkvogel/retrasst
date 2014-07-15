#ifndef SAMPLERUNGROUPMODELTESTH
#define SAMPLERUNGROUPMODELTESTH

#include "API.h"
#include <boost/foreach.hpp>
#include <deque>
#include "MockSampleRunGroupIDGenerator.h"
#include "Nullable.h"
#include "Require.h"
#include "SampleRunGroupModel.h"
#include "SampleRunIDResolutionService.h"
#include "SampleRuns.h"
#include <tut.h>

enum SampleRunType { SRT_UNK, SRT_QC };

typedef paulst::Nullable<int> GroupID;

namespace tut
{
	struct SampleRunGroupModelTestFixture
    {
    private: 
        int                             sequencePos;
        MockSampleRunGroupIDGenerator*  mockIDGenerator;
        int                             lastBuilt;
        std::unique_ptr<valc::SampleRunIDResolutionService> sampleRunIDResolutionService;
    public:
        valc::SampleRuns            sampleRuns;
        valc::SampleRunGroupModel   model;

        SampleRunGroupModelTestFixture()
            : 
            sequencePos(0),
            mockIDGenerator( new MockSampleRunGroupIDGenerator() ),
            model(mockIDGenerator),
            lastBuilt(0),
            sampleRunIDResolutionService( new valc::SampleRunIDResolutionService() )
        {
        }

        void build()
        {
            while ( lastBuilt < sampleRuns.size()  )
            {
                const valc::SampleRun& sr = sampleRuns.at(lastBuilt);
                model.assignToGroup( sr.getID(), sr.isQC(), sr.getGroupID() );
                ++lastBuilt;
            }
        }

        int getGroupID( const std::string& runID )
        {
            const valc::IDToken sampleRunID( runID, sampleRunIDResolutionService.get() );
            return model.getGroupID( sampleRunID );
        }

        void primeIDGen( int id )
        {
            mockIDGenerator->prime( id );
        }

        void primeSample( const std::string& runID, SampleRunType rt, int grpID = 0 )
        {
            const valc::IDToken sampleRunID( runID, sampleRunIDResolutionService.get() );
            valc::SampleRun sampleRun( sampleRunID, runID, runID, true, Now(), 0.0, ++sequencePos, grpID ? GroupID(grpID) : GroupID(), rt == SRT_QC );
            sampleRuns.push_back( sampleRun );
        }
    };

    typedef test_group<SampleRunGroupModelTestFixture, 13> SampleRunGroupModelTestGroup;
	SampleRunGroupModelTestGroup testGroupSampleRunGroupModel( "SampleRunGroupModel tests");
	typedef SampleRunGroupModelTestGroup::object testSampleRunGroupModel;

	template<>
	template<>
	void testSampleRunGroupModel::test<1>()
	{
		set_test_name("2 Unknown samples, null GroupID in both cases");

		using namespace valc;

        primeIDGen( 29 );
        primeSample( "a", SRT_UNK );
        primeSample( "b", SRT_UNK );

        build();

        ensure_equals( getGroupID("a"), 29 );
        ensure_equals( getGroupID("b"), 29 );
        ensure_equals( model.countGroups()  , 1  );
    }

    template<>
	template<>
	void testSampleRunGroupModel::test<2>()
	{
		set_test_name("2 Unknown samples, first with a non-null GroupID");

		using namespace valc;

        primeSample( "a", SRT_UNK, 34 );
        primeSample( "b", SRT_UNK );

        build();

        ensure_equals( getGroupID("a"), 34 );
        ensure_equals( getGroupID("b"), 34 );
        ensure_equals( model.countGroups()  , 1  );
    }

    template<>
	template<>
	void testSampleRunGroupModel::test<3>()
	{
		set_test_name("2 Unknown samples, 2nd with a non-null GroupID");

		using namespace valc;

        primeIDGen( 89 );
        primeSample( "a", SRT_UNK );
        primeSample( "b", SRT_UNK, 38 );

        build();

        ensure_equals( getGroupID("a"), 38 );
        ensure_equals( getGroupID("b"), 38 );
        ensure_equals( model.countGroups()  , 1  );
    }

    template<>
	template<>
	void testSampleRunGroupModel::test<4>()
	{
		set_test_name("2 Unknown samples, both with non-null GroupID");

		using namespace valc;

        primeSample( "a", SRT_UNK, 39 );
        primeSample( "b", SRT_UNK, 39 );

        build();

        ensure_equals( getGroupID("a"), 39 );
        ensure_equals( getGroupID("b"), 39 );
        ensure_equals( model.countGroups()  , 1  );
    }

    template<>
	template<>
	void testSampleRunGroupModel::test<5>()
	{
		set_test_name("2 Unknown samples, both with non-null GroupID - different values - error! Shouldn't be adjacent!");

		using namespace valc;

        primeSample( "a", SRT_UNK, 39 );
        primeSample( "b", SRT_UNK, 31 );

        bool exceptionThrown = false;

        try
        {
            build();
        }
        catch( const Exception& e )
        {
            wprintf( L"%s\n", e.Message.c_str() );
            exceptionThrown = true;
        }

        ensure( exceptionThrown );
    }

    template<>
	template<>
	void testSampleRunGroupModel::test<6>()
	{
		set_test_name("1 Unknown sample, 1 QC sample, both with null GroupID");

		using namespace valc;

        primeIDGen( 51 );
        primeIDGen( 52 );
        primeSample( "a", SRT_UNK );
        primeSample( "b", SRT_QC );

        build();

        ensure_equals( getGroupID("a"), 51 );
        ensure_equals( getGroupID("b"), 52 );
        ensure_equals( model.countGroups()  , 2  );
    }

    template<>
	template<>
	void testSampleRunGroupModel::test<7>()
	{
		set_test_name("2 QC samples, both with null GroupID");

		using namespace valc;

        primeIDGen( 51 );
        primeSample( "a", SRT_QC );
        primeSample( "b", SRT_QC );

        build();

        ensure_equals( getGroupID("a"), 51 );
        ensure_equals( getGroupID("b"), 51 );
        ensure_equals( model.countGroups()  , 1  );
    }

    template<>
	template<>
	void testSampleRunGroupModel::test<8>()
	{
		set_test_name("2 QC samples, both with same non-null GroupID");

		using namespace valc;

        primeSample( "a", SRT_QC, 72 );
        primeSample( "b", SRT_QC, 72 );

        build();

        ensure_equals( getGroupID("a"), 72 );
        ensure_equals( getGroupID("b"), 72 );
        ensure_equals( model.countGroups()  , 1  );
    }

    template<>
	template<>
	void testSampleRunGroupModel::test<9>()
	{
		set_test_name("2 QC samples, both with non-null GroupID, but different values");

		using namespace valc;

        primeSample( "a", SRT_QC, 72 );
        primeSample( "b", SRT_QC, 74 );

        build();

        ensure_equals( getGroupID("a"), 72 );
        ensure_equals( getGroupID("b"), 74 );
        ensure_equals( model.countGroups()  , 2  );
    }

    template<>
	template<>
	void testSampleRunGroupModel::test<10>()
	{
		set_test_name("1 QC sample, then an Unknown");

		using namespace valc;

        primeIDGen( 52 );
        primeIDGen( 53 );
        primeSample( "a", SRT_QC );
        primeSample( "b", SRT_UNK );

        build();

        ensure_equals( getGroupID("a"), 52 );
        ensure_equals( getGroupID("b"), 53 );
        ensure_equals( model.countGroups()  , 2  );
    }

    template<>
	template<>
	void testSampleRunGroupModel::test<11>()
	{
		set_test_name("An extended sequence");

		using namespace valc;

        for ( int i = 1; i < 20; ++i )
        {
            primeIDGen( i );
        }

        primeSample( "a", SRT_QC , 72 );
        primeSample( "b", SRT_UNK, 73 );
        primeSample( "c", SRT_UNK, 73 );
        primeSample( "d", SRT_QC , 74 );
        primeSample( "e", SRT_QC , 75 );
        primeSample( "f", SRT_UNK, 76 );
        primeSample( "g", SRT_UNK, 76 );
        primeSample( "h", SRT_QC , 77 );
        primeSample( "i", SRT_UNK );
        primeSample( "j", SRT_UNK );
        primeSample( "k", SRT_QC  );
        primeSample( "l", SRT_UNK );
        primeSample( "m", SRT_UNK );
        primeSample( "n", SRT_QC  );

        build();

        ensure_equals( model.countGroups()  , 10  );
        ensure_equals( getGroupID("i"), 1   );
        ensure_equals( getGroupID("n"), 4   );
    }

    template<>
	template<>
	void testSampleRunGroupModel::test<12>()
	{
		set_test_name("Duplicate run id");

		using namespace valc;

        primeIDGen( 1 );
        primeSample( "a", SRT_UNK );
        primeSample( "a", SRT_UNK );

        bool exceptionThrown = false;

        try
        {
            build();
        }
        catch( const Exception& e )
        {
            wprintf( L"%s\n", e.Message.c_str() );
            exceptionThrown = true;
        }

        ensure( exceptionThrown );
    }

    template<>
	template<>
	void testSampleRunGroupModel::test<13>()
	{
		set_test_name("Demonstrating instability of group ID during assignments.");

		using namespace valc;

        primeIDGen( 29 );
        primeSample( "a", SRT_UNK );
        build();

        ensure_equals( getGroupID("a"), 29 );

        primeSample( "b", SRT_UNK, 62 );
        build();

        ensure_equals( getGroupID("a"), 62 );
    }

}

#endif

