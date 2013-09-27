#include <boost/shared_ptr.hpp>
#include "InclusionRule.h"
#include "LuaInclude.h"
#include "Require.h"
#include <SysUtils.hpp>

namespace valc
{

InclusionRule::InclusionRule( const std::string& script, const std::string& functionName )
    : L(lua_open()), m_functionName( functionName )
{
    luaL_openlibs(L);

    if ( luaL_dostring( L, script.c_str() ) )
    {
        std::string exceptionMsg = lua_tostring( L, -1 );
        lua_close(L);
        L = 0;
        throw Exception( UnicodeString( exceptionMsg.c_str() ) + UnicodeString(L"  Script: ") + UnicodeString(script.c_str()) );
    }
}

InclusionRule::~InclusionRule()
{
    if ( L )
    {
        lua_close(L);
    }
}

void InclusionRule::addParam( const std::string& param )
{
    lua_pushstring( L, param.c_str() );
}

void InclusionRule::addParam( int i )
{
    lua_pushinteger( L, i );
}

void InclusionRule::init()
{
    lua_settop( L, 0 );
    lua_getglobal( L, m_functionName.c_str() );
    require( 1 == lua_gettop(L) );
}

void InclusionRule::run()
{
    const int numParams = lua_gettop( L ) - 1;

    if( lua_pcall(L, numParams, 1, 0) )
    {
        throw Exception( lua_tostring( L, -1 ) );
    }
}

bool InclusionRule::getBooleanResult()
{
    if ( ! lua_isboolean( L, -1 ) )
    {
        throw Exception( "Topmost element on LUA stack is not a boolean value" );
    }

    return lua_toboolean(L, -1);
}

}

