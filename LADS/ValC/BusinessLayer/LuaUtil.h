#ifndef LUA_UTIL_H
#define LUA_UTIL_H

#include "ExceptionUtil.h"
#include "LuaInclude.h"

// The following are wrappers for functions which, in Lua, are macros.
const char* lua_toString ( lua_State* L, int index );
int         lua_toInteger( lua_State* L, int index );
double      lua_toNumber ( lua_State* L, int index );

template<class DataType, class CheckFunc, class AccessFunc>
DataType retrieveTableValue( lua_State* L, int tableIndex, const std::string& key, CheckFunc check, AccessFunc get )
{
    lua_getfield( L, tableIndex, key.c_str() );
    if( ! check( L, -1 ) ) 
    {
        paulst::exception( "Data-type check failure for table value keyed on '%s'", key.c_str() );
    }
    DataType t = get( L, -1 );
    lua_pop( L, 1 );
    return t;
}

#endif

