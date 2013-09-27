#ifndef LUA_UTIL_H
#define LUA_UTIL_H

#include "LuaInclude.h"

// A wrapper for lua_tostring, because, in Lua, the latter is not a 1st class function (is some kind of macro, perhaps?)
const char* lua_toString( lua_State* L, int index );
// A wrapper for lua_tointeger, because, in Lua, the latter is not a 1st class function (is some kind of macro, perhaps?)
int lua_toInteger( lua_State* L, int index );

template<class DataType, class CheckFunc, class AccessFunc>
DataType retrieveTableValue( lua_State* L, int tableIndex, const std::string& key, CheckFunc check, AccessFunc get )
{
    lua_getfield( L, tableIndex, key.c_str() );
    require( check( L, -1 ) );
    DataType t = get( L, -1 );
    lua_pop( L, 1 );
    return t;
}

#endif

