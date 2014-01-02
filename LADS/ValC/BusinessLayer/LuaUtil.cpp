#include "LuaUtil.h"

const char* lua_toString( lua_State* L, int index )
{
    return lua_tostring( L, index );
}

int lua_toInteger( lua_State* L, int index )
{
    return lua_tointeger( L, index );
}

double lua_toNumber( lua_State* L, int index )
{
    return lua_tonumber( L, index );
}

