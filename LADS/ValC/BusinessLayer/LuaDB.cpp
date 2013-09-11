#include "LuaDB.h"

#include "LuaInclude.h"

ConnectionStats stats;
ConnectionFactory connectionFactory = 0;

static int openConnection( lua_State* L )
{
    const char* connectionString = lua_tostring( L, -1 );
    lua_pushnumber( L, 123 );
    ++ stats.opened;
    return 1;
}

static int closeConnection( lua_State* L )
{
    int connectionHandle = lua_tonumber( L, -1 );
    ++ stats.closed;
    return 0;
}

void openLuaDB( lua_State* L )
{
    stats.opened = stats.closed = 0;
    lua_register ( L, "openConnection" , openConnection ); 
    lua_register ( L, "closeConnection", closeConnection ); 
}

