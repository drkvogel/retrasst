#include "API.h"
#include <cstdio>
#include "ExceptionalDataHandlerImpl.h"
#include "LoggingService.h"
#include "LuaInclude.h"
#include "Require.h"

namespace valc
{

ExceptionalDataHandlerImpl::ExceptionalDataHandlerImpl( const std::string& luaScript, UserAdvisor* userAdvisor, paulst::LoggingService* log )
    : L(lua_open()), m_userAdvisor( userAdvisor ), m_log( log )
{
    luaL_openlibs(L);

    if ( luaL_dostring( L, luaScript.c_str() ) )
    {
        std::string exceptionMsg = lua_tostring( L, -1 );
        lua_close(L);
        L = 0;
        throw Exception( exceptionMsg.c_str() );
    }
}

ExceptionalDataHandlerImpl::~ExceptionalDataHandlerImpl()
{
    if ( L )
    {
        lua_close(L);
    }
}

bool ExceptionalDataHandlerImpl::canProvideProjectIDFor( const std::string& barcode )
{
    lua_settop( L, 0 );

    lua_getglobal( L, "canProvideProjectIDFor" );

    lua_pushstring( L, barcode.c_str() );

    if( lua_pcall(L, 1, 1, 0) )
    {
        throw Exception( lua_tostring( L, -1 ) );
    }

    if ( ! lua_isboolean( L, -1 ) )
    {
        throw Exception( "Topmost element on LUA stack is not a boolean value" );
    }

    return lua_toboolean(L, -1);
}

int  ExceptionalDataHandlerImpl::getProjectIDFor( const std::string& barcode )
{
    lua_settop( L, 0 );

    lua_getglobal( L, "getProjectIDFor" );

    lua_pushstring( L, barcode.c_str() );

    if( lua_pcall(L, 1, 1, 0) )
    {
        throw Exception( lua_tostring( L, -1 ) );
    }

    if ( ! lua_isnumber( L, -1 ) )
    {
        throw Exception( "Topmost element on LUA stack is not an integer value" );
    }

    return lua_tointeger(L, -1);
}

bool ExceptionalDataHandlerImpl::notifyBuddyDatabaseEntryIgnored( int buddySampleID, const std::string& reason )
{
    char msgBuffer[1024];
    std::sprintf( msgBuffer, "Ignoring buddy_database entry %d.", buddySampleID ); 
    return runNotifyMethod( "notifyBuddyDatabaseEntryIgnored", msgBuffer, reason );
}

bool ExceptionalDataHandlerImpl::notifyCannotAllocateResultToWorklistEntry( int resultID, const std::string& reason )
{
    char msgBuffer[1024];
    std::sprintf( msgBuffer, "Cannot allocate result %d to a worklist entry.", resultID ); 
    return runNotifyMethod( "notifyCannotAllocateResultToWorklistEntry", msgBuffer, reason );
}

bool ExceptionalDataHandlerImpl::notifyWorklistEntryIgnored( int recordNo, const std::string& reason )
{
    char msgBuffer[1024];
    std::sprintf( msgBuffer, "Ignoring worklist entry %d.", recordNo ); 
    return runNotifyMethod( "notifyWorklistEntryIgnored", msgBuffer, reason );
}

bool ExceptionalDataHandlerImpl::runNotifyMethod( const std::string& methodName, const std::string& failure, const std::string& reason )
{
    lua_settop( L, 0 );

    lua_getglobal( L, methodName.c_str() );

    require( 1 == lua_gettop(L) );

    if( lua_pcall(L, 0, 2, 0) )
    {
        throw Exception( lua_tostring( L, -1 ) );
    }

    if ( ! ( lua_isboolean( L, -1 )  && lua_isboolean( L, -2 ) ) )
    {
        throw Exception( "Topmost 2 elements on LUA stack are not boolean values" );
    }

    const bool continueProcessing = lua_toboolean(L, -2);
    const bool adviseUser         = lua_toboolean(L, -1);

    const std::string msg = failure + std::string(" ") + reason;

    LOG( msg );

    if ( adviseUser && m_userAdvisor )
    {
        m_userAdvisor->advise( msg );
    }

    return continueProcessing;
}

}

