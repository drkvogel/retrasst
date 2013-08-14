#ifndef BUDDYSAMPLEIDKEYEDONSAMPLERUNIDTESTH
#define BUDDYSAMPLEIDKEYEDONSAMPLERUNIDTESTH

#include <algorithm>
#include <boost/scoped_array.hpp>
#include "BuddySampleIDKeyedOnSampleRunID.h"
#include "SampleRunIDResolutionService.h"
#include <tut.h>

namespace tut
{
	class BuddySampleIDKeyedOnSampleRunIDTestFixture
    {
    };

    typedef test_group<BuddySampleIDKeyedOnSampleRunIDTestFixture, 10> BuddySampleIDKeyedOnSampleRunIDTestGroup;
	BuddySampleIDKeyedOnSampleRunIDTestGroup testGroupBuddySampleIDKeyedOnSampleRunID( "BuddySampleIDKeyedOnSampleRunID tests");
	typedef BuddySampleIDKeyedOnSampleRunIDTestGroup::object testBuddySampleIDKeyedOnSampleRunID;

	template<>
	template<>
	void testBuddySampleIDKeyedOnSampleRunID::test<1>()
	{
		set_test_name("testing 'addEntry'");

		using namespace valc;

        SampleRunIDResolutionService s;
        BuddySampleIDKeyedOnSampleRunID map(&s);

        ensure      ( map.addEntry( "a", 1 ) );
        ensure      ( map.addEntry( "a", 2 ) );
        ensure      ( map.addEntry( "a", 3 ) );
        ensure      ( map.addEntry( "b", 4 ) );
        ensure      ( map.addEntry( "b", 5 ) );
        ensure      ( map.addEntry( "b", 6 ) );
        ensure_not  ( map.addEntry( "a", 3 ) );
        ensure_not  ( map.addEntry( "b", 4 ) );
        ensure_not  ( "4 is associated with runID b", map.addEntry( "a", 4 ) );
	}

    template<>
	template<>
	void testBuddySampleIDKeyedOnSampleRunID::test<2>()
	{
		set_test_name("testing 'getBuddySampleIDsFor'");

		using namespace valc;

        SampleRunIDResolutionService s;
        BuddySampleIDKeyedOnSampleRunID map(&s);

        ensure( map.addEntry( "a", 7 ) );
        ensure( map.addEntry( "a", 9 ) );
        ensure( map.addEntry( "b", 4 ) );
        
        int bufferSize = 10;
        boost::scoped_array<int> buffer( new int[bufferSize] );
        map.getBuddySampleIDsFor( "a", buffer.get(), bufferSize );
        ensure( bufferSize == 2 );
        std::sort( buffer.get(), buffer.get() + 2 );
        ensure( buffer[0] == 7 );
        ensure( buffer[1] == 9 );

        s.addMapping( "a", "b" );

        bufferSize = 10;
        map.getBuddySampleIDsFor( "a", buffer.get(), bufferSize );
        ensure( bufferSize == 3 );
        std::sort( buffer.get(), buffer.get() + 3 );
        ensure( buffer[0] == 4 );
        ensure( buffer[1] == 7 );
        ensure( buffer[2] == 9 );

	}

    template<>
	template<>
	void testBuddySampleIDKeyedOnSampleRunID::test<3>()
	{
		set_test_name("Similar to last test but when the mapping is from 'b' to 'a'");

		using namespace valc;

        SampleRunIDResolutionService s;
        BuddySampleIDKeyedOnSampleRunID map(&s);

        ensure( map.addEntry( "a", 7 ) );
        ensure( map.addEntry( "a", 9 ) );
        ensure( map.addEntry( "b", 4 ) );
        
        int bufferSize = 10;
        boost::scoped_array<int> buffer( new int[bufferSize] );
        map.getBuddySampleIDsFor( "a", buffer.get(), bufferSize );
        ensure( bufferSize == 2 );
        std::sort( buffer.get(), buffer.get() + 2 );
        ensure( buffer[0] == 7 );
        ensure( buffer[1] == 9 );

        s.addMapping( "b", "a" );

        bufferSize = 10;
        map.getBuddySampleIDsFor( "a", buffer.get(), bufferSize );
        ensure( bufferSize == 3 );
        std::sort( buffer.get(), buffer.get() + 3 );
        ensure( buffer[0] == 4 );
        ensure( buffer[1] == 7 );
        ensure( buffer[2] == 9 );
    }

    template<>
	template<>
	void testBuddySampleIDKeyedOnSampleRunID::test<4>()
	{
		set_test_name("When the buffer is too small");

		using namespace valc;

        SampleRunIDResolutionService s;
        BuddySampleIDKeyedOnSampleRunID map(&s);

        for ( int buddySampleID = 11; buddySampleID <= 20; ++buddySampleID )
        {
            map.addEntry( "a", buddySampleID );
        }
        
        int bufferSize = 2;
        boost::scoped_array<int> buffer( new int[bufferSize] );
        map.getBuddySampleIDsFor( "a", buffer.get(), bufferSize );
        ensure( bufferSize == 10 );
        buffer.reset( new int[bufferSize] );
        map.getBuddySampleIDsFor( "a", buffer.get(), bufferSize );
        ensure( bufferSize == 10 );
        std::sort( buffer.get(), buffer.get() + 10 );
        ensure( buffer[0] == 11 );
        ensure( buffer[9] == 20 );
    }
};

#endif

