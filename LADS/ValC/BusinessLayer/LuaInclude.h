#ifndef LUA_INCLUDE_H
#define LUA_INCLUDE_H

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "Require.h"

#ifndef lua_open
#define lua_open luaL_newstate
#endif

#endif

