

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <iostream>
#include <string>
#include <vector>

std::vector<int> myVector;

/* The function we'll call from the lua script */
static int getItemCount(lua_State *L)
{
	lua_pushnumber( L, myVector.size() );
	return 1; // number of results
}

/* The function we'll call from the lua script */
static int getItem(lua_State *L)
{
	int itemIndex = lua_tonumber(L, 1);
	lua_pushnumber(L, myVector[itemIndex]);
	return 1; // number of results
}

static int myWestgardRule(lua_State *L)
{
    lua_getfield (L, -1, "lot");
    int lot = lua_tointeger( L, -1 );
    lua_pop( L, 1 );
    lua_getfield (L, -1, "material");
    std::string material( lua_tostring( L, -1 ) );
    lua_pop( L, 1 );
	lua_pushnumber(L, 48 + lot);
	return 1;
}

void runScript( const std::string& scriptPath )
{
	int result = 0;
	lua_State* L;

	/* initialize Lua */
	L = lua_open();

	/* load Lua base libraries */
	luaL_openlibs(L);

	lua_register(L, "getItemCount", getItemCount);
	lua_register(L, "getItem", getItem);
	lua_register(L, "myWestgardRule", myWestgardRule);

	/* load the script */
	luaL_dofile(L, scriptPath.c_str());
	
	/* the function name */
	lua_getglobal(L, "applyRules");

    lua_newtable( L );
    lua_pushstring( L, "QCRC100401" );
    lua_setfield( L, -2, "barcode" );
    lua_pushinteger( L, 2 );
    lua_setfield( L, -2, "level" );
    lua_pushinteger( L, 4 );
    lua_setfield( L, -2, "lot" );
    lua_pushstring( L, "RC" );
    lua_setfield( L, -2, "material" );
    lua_pushinteger( L, -14369 );
    lua_setfield( L, -2, "testID" );
    lua_pushinteger( L, -4409 );
    lua_setfield( L, -2, "machineID" );
    lua_pushnumber( L, 1.702 );
    lua_setfield( L, -2, "result" );

	/* call the function with 1
	   arguments, return 1 result */
	switch( lua_pcall(L, 1, 1, 0) )
	{
		case 0:
        	
			result = (int)lua_tointeger(L, -1);

			std::cout << "result: " << result << std::endl;

			lua_pop(L, 1);

			break;

		case LUA_ERRRUN: // a runtime error.
		case LUA_ERRMEM: // memory allocation error. For such errors, Lua does not call the error handler function.
		case LUA_ERRERR: // error while running the error handler function

			size_t len = 0;

			const char* errorMsg = lua_tolstring ( L, -1, &len);

			std::cout << "error: " << errorMsg << std::endl;
	}

	/* cleanup Lua */
	lua_close(L);
}

void pause()
{
    std::cout << "[enter] to quit..."  << std::endl;
    char c;
    std::cin >> std::noskipws >> c;
}

int main(int argc, char* argv[])
{
	myVector.push_back( 1 );
	myVector.push_back( 4 );
	myVector.push_back( 6 );
	runScript( "C:\\cvs\\LADS\\LUA\\LUA_explore\\scripts\\my.script" );
    pause();
	return 0;
}
//---------------------------------------------------------------------------
