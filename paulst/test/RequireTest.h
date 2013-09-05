#ifndef REQUIRETEST_H
#define REQUIRETEST_H

#include <tut.h>

#include "Require.h"

namespace tut
{
	class RequireTestFixture
    {
    };

    typedef test_group<RequireTestFixture, 5> RequireTestGroup;
	RequireTestGroup testGroupRequire("Require tests");
	typedef RequireTestGroup::object testRequire;

    template<>
    template<>
	void testRequire::test<1>()
    {
        using namespace paulst;
        set_test_name("require");

        int a = 1;
        int b = 2;
        int c = 3;

        UnicodeString exceptionMsg;

        try
        {
            require( a == b );
        }
        catch( const Exception& e )
        {
            exceptionMsg = e.Message;
        }

        ensure( exceptionMsg.Pos( L"Assertion failure" ) );
        ensure( exceptionMsg.Pos( L"a == b" ) );
    }
     
    template<>
    template<>
	void testRequire::test<2>()
    {
        using namespace paulst;
        set_test_name("throwException");

        UnicodeString exceptionMsg;

        try
        {
            throwException( "Oh dear!" )
        }
        catch( const Exception& e )
        {
            exceptionMsg = e.Message;
        }

        ensure( exceptionMsg.Pos( L"Error" ) );
        ensure( exceptionMsg.Pos( L"Oh dear!" ) );
    }

    template<>
    template<>
	void testRequire::test<3>()
    {
        using namespace paulst;
        set_test_name("throwRuntimeError");

        UnicodeString exceptionMsg;

        try
        {
            throwRuntimeError( "Oh dear!" )
        }
        catch( const Exception& e )
        {
            exceptionMsg = e.Message;
        }

        ensure( exceptionMsg.Pos( L"Runtime Error:" ) );
        ensure( exceptionMsg.Pos( L"Oh dear!" ) );
    }

    template<>
    template<>
	void testRequire::test<4>()
    {
        using namespace paulst;
        set_test_name("throwIf");

        UnicodeString exceptionMsg;

        int a = 1;
        int b = 2;

        try
        {
            throwIf( a != b )
        }
        catch( const Exception& e )
        {
            exceptionMsg = e.Message;
        }

        ensure( exceptionMsg.Pos( L"Runtime Error:" ) );
        ensure( exceptionMsg.Pos( L"a != b" ) );
    }

    template<>
    template<>
	void testRequire::test<5>()
    {
        using namespace paulst;
        set_test_name("throwUnless");

        UnicodeString exceptionMsg;

        int a = 1;
        int b = 2;

        try
        {
            throwUnless( a == b )
        }
        catch( const Exception& e )
        {
            exceptionMsg = e.Message;
        }

        ensure( exceptionMsg.Pos( L"Runtime Error:" ) );
        ensure( exceptionMsg.Pos( L"! (a == b)" ) );
    }

};

#endif
