#ifndef BASICTESTSH
#define BASICTESTSH

#include <boost/shared_ptr.hpp>
#include <cstring>
#include <tut.h>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace tut
{
    class BasicTestFixture
    {
    };

    typedef test_group<BasicTestFixture, 10> BasicTestGroup;
    BasicTestGroup testGroupBasic( "Basic tests");
    typedef BasicTestGroup::object testBasic;

    template<>
    template<>
    void testBasic::test<1>()
    {
        set_test_name("Calling a function that takes a single argument and returns a single value");

        lua_State* L = lua_open();
        boost::shared_ptr<void> closeOnExit( L, lua_close );
        luaL_openlibs(L);
        ensure( 0 == lua_gettop(L) );
        ensure( 0 == luaL_dostring( L, "function f (a) return a * 2 end" ) );
        lua_getglobal( L, "f" );
        ensure( 1 == lua_gettop(L) );
        lua_pushinteger( L, 5 );
        ensure( 2 == lua_gettop(L) );
        ensure( 0 == lua_pcall(L, 1, 1, 0) );
        ensure( 10 == lua_tointeger( L, -1 ) );
     }

    template<>
    template<>
    void testBasic::test<2>()
    {
        set_test_name("Badly written script");

        lua_State* L = lua_open();
        boost::shared_ptr<void> closeOnExit( L, lua_close );
        luaL_openlibs(L);
        ensure( 0 == lua_gettop(L) );
        ensure( 0 != luaL_dostring( L, "function f (a) return a * 2 en" ) );
        ensure( 1 == lua_gettop(L) );
        ensure( lua_isstring( L, -1 ) );
        std::printf( "error message: %s\n", lua_tostring( L, -1 ) );
     }

};

#endif

