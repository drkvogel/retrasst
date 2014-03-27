#ifndef LUAFUNCH
#define LUAFUNCH

#include <boost/variant.hpp>
#include "Require.h"
#include <string>
#include <SysUtils.hpp>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#ifndef lua_open
#define lua_open luaL_newstate
#endif

namespace paulst
{

typedef boost::variant< int, double, std::string > LuaValue;

template<typename T> struct ReturnType { T operator()( lua_State* L ) { return T(); } };

template<> struct ReturnType<int>         { int         operator()( lua_State* L ) { return lua_tointeger( L, -1 ); } };
template<> struct ReturnType<double>      { double      operator()( lua_State* L ) { return lua_tonumber ( L, -1 ); } };
template<> struct ReturnType<const char*> { const char* operator()( lua_State* L ) { return lua_tostring ( L, -1 ); } };
template<> struct ReturnType<bool>        { bool        operator()( lua_State* L ) { return lua_toboolean( L, -1 ); } };
template<> struct ReturnType<std::string> { std::string operator()( lua_State* L ) { std::string s = lua_tostring ( L, -1 ); return s; } };


/*
    For usage, refer to paulst/test/LuaFuncTest.h
*/
template< typename return_type, int num_args > class LuaFunc  
    : public boost::static_visitor<>
{
public:

    LuaFunc( const char* functionName, const std::string& script )
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

    ~LuaFunc()
    {
        if ( L )
        {
            lua_close(L);
        }
    }

    return_type operator()( LuaValue* args )
    {
        lua_settop( L, 0 );

        lua_getglobal( L, m_functionName.c_str() );

        require( 1 == lua_gettop(L) );

		for ( int i = 0; i < num_args; ++i )
        {
            boost::apply_visitor( *this, args[i] );
        }

        const int numParams = lua_gettop( L ) - 1;

        if( lua_pcall(L, numParams, 1, 0) )
        {
            throw Exception( lua_tostring( L, -1 ) );
        }

        return ReturnType<return_type>()(L);
    }


    void operator()( int i )               { lua_pushinteger( L, i ); }

    void operator()( double d )            { lua_pushnumber( L, d ); }

    void operator()( const std::string s ) { lua_pushstring( L, s.c_str() ); }

private:
    const std::string m_functionName;
    lua_State* L;

};

}

#endif

