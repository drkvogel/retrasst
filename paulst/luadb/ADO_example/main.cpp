#include <vcl.h>
#include <windows.h>

#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>

#include <boost/shared_ptr.hpp>
#include "DBConnectionADO.h"
#include "LuaDB.h"
#include "LuaInclude.h"
#include "Require.h"
#include <string>
#include "StrUtil.h"


paulstdb::DBConnection* newADOConnection( const char* connectionString )
{
    return new paulstdb::DBConnectionADO( connectionString );
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		require( argc == 2 );
		std::string scriptFilename = argv[1];
		std::string script = paulst::loadContentsOf( scriptFilename );

		lua_State* L = lua_open();
		boost::shared_ptr<void> closeOnExit( L, lua_close );
		luaL_openlibs( L );
		openLuaDB( L, &newADOConnection );
		boost::shared_ptr<void> closeLuaDBOnExit( L, closeLuaDB );

		if( luaL_dostring( L, script.c_str() ) )
		{
			std::printf( "Script error: %s\n", lua_tostring( L, -1 ) );
		}
	}
	catch( const Exception& e )
	{
		AnsiString msg( e.Message.c_str() );
		std::printf( "Exception: %s\n", msg.c_str() );
	}
	catch( ... )
	{
		std::puts( "Unspecified exception." );
	}
	return 0;
}

