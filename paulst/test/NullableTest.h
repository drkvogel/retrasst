#ifndef NULLABLETESTH
#define NULLABLETESTH

#include <tut.h>

#include "Nullable.h"

typedef paulst::Nullable<int> NullableInt;

namespace tut
{
	class NullableTestFixture
    {
    };

    typedef test_group<NullableTestFixture, 3> NullableTestGroup;
	NullableTestGroup testGroupNullable( "Nullable tests");
	typedef NullableTestGroup::object testNullable;

    template<>
    template<>
	void testNullable::test<1>()
    {
        set_test_name("Default constructor");

        using namespace paulst;

        NullableInt i;

        ensure( i.isNull() );

        bool exceptionThrown = false;

        try
        {
            int x = i;
        }
        catch( const Exception& e )
        {
            exceptionThrown = true;
        }

        ensure( exceptionThrown );
    }

    template<>
    template<>
	void testNullable::test<2>()
    {
        set_test_name("Construction with value");

        using namespace paulst;

        NullableInt i(9);

        ensure_equals( (int)i, 9 );
        ensure_not   ( i.isNull() );
        int y = i;
        ensure_equals( y, 9 );
    }

    template<>
    template<>
	void testNullable::test<3>()
    {
        set_test_name("Assignment");

        using namespace paulst;

        NullableInt x, y(2);

        ensure    ( x != y );
        ensure    ( x.isNull() );
        ensure_not( y.isNull() );

        x = y;

        ensure_not( x.isNull() );
        ensure_equals( (int)x, 2 );

        ensure     ( x == y );

        x = NullableInt();

        ensure     ( x.isNull() );
    }

};
 
#endif

