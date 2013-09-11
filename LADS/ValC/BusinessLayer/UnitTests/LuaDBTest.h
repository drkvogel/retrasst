#ifndef LUADBTESTH 
#define LUADBTESTH

#include <boost/shared_ptr.hpp>
#include "LuaDB.h"
#include "LuaInclude.h"

#include <tut.h>

extern ConnectionStats stats;

namespace tut
{
	class LuaDBTestFixture
    {
    };

    typedef test_group<LuaDBTestFixture, 1> LuaDBTestGroup;
	LuaDBTestGroup testGroupLuaDB( "LuaDB tests");
	typedef LuaDBTestGroup::object testLuaDB;

    template<>
    template<>
	void testLuaDB::test<1>()
    {
        set_test_name("LuaDB");

        using namespace valc;

        const char* script = 
            "local connectionHandle = openConnection('paul')\n"
            "closeConnection( connectionHandle )\n";

        lua_State* L = lua_open();
        boost::shared_ptr<void> closeOnExit( L, lua_close );
        luaL_openlibs( L );
        openLuaDB( L );


        ensure( 0 == stats.opened );
        ensure( 0 == stats.closed  );

        ensure( 0 == luaL_dostring( L, script ) );

        ensure( 1 == stats.opened );
        ensure( 1 == stats.closed  );
    }

};

#endif

