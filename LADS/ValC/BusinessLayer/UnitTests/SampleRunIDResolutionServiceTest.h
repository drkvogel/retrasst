#ifndef SAMPLERUNIDRESOLUTIONSERVICETESTH
#define SAMPLERUNIDRESOLUTIONSERVICETESTH

#include "SampleRunIDResolutionService.h"
#include <tut.h>

namespace tut
{
	class SampleRunIDResolutionServiceTestFixture
    {
    };

    typedef test_group<SampleRunIDResolutionServiceTestFixture, 10> SampleRunIDResolutionServiceTestGroup;
	SampleRunIDResolutionServiceTestGroup testGroupSampleRunIDResolutionService( "SampleRunIDResolutionService tests");
	typedef SampleRunIDResolutionServiceTestGroup::object testSampleRunIDResolutionService;

	template<>
	template<>
	void testSampleRunIDResolutionService::test<1>()
	{
		set_test_name("SampleRunIDResolutionService");

		using namespace valc;

        SampleRunIDResolutionService s;
        s.addMapping( "a", "b" );
        s.addMapping( "c", "d" );
        
        ensure( s.existsAnAlternativeEquivalentFor( "a" ) );
        ensure( s.existsAnAlternativeEquivalentFor( "b" ) );
        ensure( s.existsAnAlternativeEquivalentFor( "c" ) );
        ensure( s.existsAnAlternativeEquivalentFor( "d" ) );

        ensure_not( s.existsAnAlternativeEquivalentFor( "e" ) );


        ensure( s.hasMappingFor( "a" ) );
        ensure( s.hasMappingFor( "c" ) );

        ensure_not( s.hasMappingFor( "b" ) );
        ensure_not( s.hasMappingFor( "d" ) );
        ensure_not( s.hasMappingFor( "e" ) );

        ensure( s.getMappingFor("a") == "b" );
        ensure( s.getMappingFor("c") == "d" );

        ensure( s.getAlternativeEquivalentFor("a") == "b" );
        ensure( s.getAlternativeEquivalentFor("b") == "a" );
        ensure( s.getAlternativeEquivalentFor("c") == "d" );
        ensure( s.getAlternativeEquivalentFor("d") == "c" );

        ensure( s.compareSampleRunIDs( "a", "b" ) );
        ensure( s.compareSampleRunIDs( "c", "d" ) );

        ensure_not( s.compareSampleRunIDs( "a", "d" ) );
	}
};

#endif

