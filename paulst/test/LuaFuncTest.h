#ifndef LUAFUNCTESTH
#define LUAFUNCTESTH

#include <tut.h>

#include "LuaFunc.h"


namespace tut
{
	class LuaFuncTestFixture
    {
    };

    typedef test_group<LuaFuncTestFixture, 3> LuaFuncTestGroup;
	LuaFuncTestGroup testGroupLuaFunc( "LuaFunc tests");
	typedef LuaFuncTestGroup::object testLuaFunc;

    template<>
    template<>
	void testLuaFunc::test<1>()
    {
        set_test_name("first stab");

        using namespace paulst;

        LuaValue args[2] = { 45, 12 };

        int i = LuaFunc<int, 2>( "sum", "function sum( a, b ) return a + b end" )( args );

        ensure_equals( 45 + 12, i );
    }

    template<>
    template<>
	void testLuaFunc::test<2>()
    {
        set_test_name("different argument types");

        using namespace paulst;

        LuaValue args[3] = { 45, "pig", 1.023 };

        std::string s = LuaFunc<const char*, 3>( "concat", "function concat( a, b, c ) return a .. b .. c end" )( args );

        ensure_equals( std::string("45pig1.023"), s );
    }

};
 
#endif

