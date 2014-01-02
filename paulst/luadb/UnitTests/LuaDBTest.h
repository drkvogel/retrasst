#ifndef LUADBTESTH 
#define LUADBTESTH

#include "AcquireCriticalSection.h"
#include <boost/shared_ptr.hpp>
#include "CritSec.h"
#include "DBConnection.h"
#include "LuaDB.h"
#include "LuaInclude.h"
#include "Require.h"
#include "StringBackedCursor.h"
#include "Thread.h"

#include <tut.h>
#include <vector>

// Types

class LuaThreadTask : public paulst::Runnable
{
public:
    LuaThreadTask( const std::string& script, const std::string& function, const std::string& arg, ConnectionFactory cf,
        void* connectionState, lua_CFunction callback )
        : m_script( script ), m_function(function), m_arg(arg), m_connectionFactory(cf), 
        m_connectionState( connectionState), m_callback(callback), m_thread(this)
    {
    }

    bool error() const
    {
        return m_errorMsg.size();
    }

    std::string errorMsg() const
    {
        return m_errorMsg;
    }

    HANDLE getHandle() const
    {
        return m_thread.getHandle();
    }

    void run( const paulst::Event* )
    {
		lua_State* L = lua_open();
		boost::shared_ptr<void> closeOnExit( L, lua_close );
		luaL_openlibs( L );

		LuaDB s( L, m_connectionFactory, m_connectionState );

        lua_register( L, "callback", m_callback );

		if( luaL_dostring( L, m_script.c_str() ) )
		{
			m_errorMsg =  lua_tostring( L, -1 );
		}
        else
        {
            lua_getglobal( L, m_function.c_str() );
            lua_pushstring( L, m_arg.c_str() );
            if ( lua_pcall( L, 1, 0, 0 ) )
            {
			    m_errorMsg =  lua_tostring( L, -1 );
            }
        }
    }

private:
    std::string m_script, m_function, m_arg, m_errorMsg;
    paulst::Thread m_thread;
    ConnectionFactory m_connectionFactory;
    void* m_connectionState;
    lua_CFunction m_callback;
};


typedef boost::shared_ptr<LuaThreadTask> LuaThreadTaskPtr;


class MyConnectionForTesting : public paulstdb::DBConnection
{
private:
    std::string m_queryData;
    int* m_openConnections;
public:
    MyConnectionForTesting( const char* connectionString, int* openConnections ) 
        : 
        m_queryData( connectionString ),
        m_openConnections( openConnections ) 
    {
        ++(*openConnections);
    }

    void close() 
    { 
        --(*m_openConnections); 
    }

    paulstdb::Cursor* executeQuery( const std::string& sql ) 
    { 
        return new paulstdb::StringBackedCursor( m_queryData ); 
    }

    void executeStmt( const std::string& sql ) 
    {
    }
};


class MyConnectionFactory
{
public:
    int openConnections;

    MyConnectionFactory() 
        : openConnections(0) 
    {}

    paulstdb::DBConnection* newConnection( const char* connectionString ) 
    { 
        return new MyConnectionForTesting( connectionString, &openConnections ); 
    }
};

// Definitions (storage)

paulst::CritSec critSec;
std::string callbacks;
std::vector< LuaThreadTaskPtr > luaThreads;


// Functions

static int callback( lua_State* L )
{
    paulst::AcquireCriticalSection a( critSec );

    require( lua_isstring( L, -1 ) );
    callbacks.append( lua_tostring( L, -1 ) );

    return 0;
}

paulstdb::DBConnection* mockConnectionFactory( const char* connectionString, void* connectionState )
{
    return ((MyConnectionFactory*)connectionState)->newConnection( connectionString );
}

void runLuaFunc( PTP_CALLBACK_INSTANCE instance, void* context, PTP_WORK work )
{
    lua_State* L = (lua_State*)context;
    MyConnectionFactory cf;
    LuaDB s( L, &mockConnectionFactory, &cf );

    // Push the function onto the Lua stack
    lua_getglobal( L, "useConnection" );

    // Run the function
    lua_pcall( L, 0, 1, 0 );

    // Pop the return value
    lua_pop( L, 1 );
}



HANDLE runScriptOnThread( const std::string& script, const std::string threadID, void* connectionState )
{
    LuaThreadTaskPtr ptr( new LuaThreadTask( script, "loopWithCallbacks", threadID, mockConnectionFactory, connectionState, callback ) );
    luaThreads.push_back(ptr);
    return ptr->getHandle();
}


// Unit tests

namespace tut
{
	class LuaDBTestFixture
    {
    };

	typedef test_group<LuaDBTestFixture, 7> LuaDBTestGroup;
	LuaDBTestGroup testGroupLuaDB( "LuaDB tests");
	typedef LuaDBTestGroup::object testLuaDB;

	template<>
	template<>
	void testLuaDB::test<1>()
	{
		set_test_name("LuaDB - query for single row, single value (integer)");

		const char* script =
			"local sqlType = { integer = 0 }\n"
			"local connectionHandle = openConnection('49,\\n')\n"
			"local queryHandle = executeQuery( 'select 2', {sqlType.integer}, connectionHandle )\n"
			"local row = fetchNextRow( queryHandle )\n"
			"colValue = row[1]\n"
			"closeQuery( queryHandle )\n"
			"closeConnection( connectionHandle )\n"
            "return colValue\n";

		lua_State* L = lua_open();
		boost::shared_ptr<void> closeOnExit( L, lua_close );
		luaL_openlibs( L );
        MyConnectionFactory c;
		LuaDB s( L, &mockConnectionFactory, &c );

		if( luaL_dostring( L, script ) )
		{
			std::puts( lua_tostring( L, -1 ) );
			ensure( false );
		}

		ensure(lua_isnumber( L, -1 ));
		ensure( 49 == lua_tointeger( L, -1 ) );
	}

	template<>
	template<>
	void testLuaDB::test<2>()
	{
		set_test_name("LuaDB - query for single row, single value (string)");

		const char* script =
			"local sqlType = { integer = 0, string = 1 }\n"
			"local connectionHandle = openConnection('rabbits,\\n')\n"
			"local queryHandle = executeQuery( 'select 2', {sqlType.string}, connectionHandle )\n"
			"local row = fetchNextRow( queryHandle )\n"
			"colValue = row[1]\n"
			"closeQuery( queryHandle )\n"
			"closeConnection( connectionHandle )\n"
            "return colValue\n";

		lua_State* L = lua_open();
		boost::shared_ptr<void> closeOnExit( L, lua_close );
		luaL_openlibs( L );
        MyConnectionFactory cf;
		LuaDB s( L, &mockConnectionFactory, &cf );

		if( luaL_dostring( L, script ) )
		{
			std::puts( lua_tostring( L, -1 ) );
			ensure( false );
		}

		ensure(lua_isstring( L, -1 ));
		std::string result = lua_tostring( L, -1 );
		ensure( result == "rabbits" );
	}

    template<>
	template<>
	void testLuaDB::test<3>()
	{
		set_test_name("LuaDB - query for single row, single value (real)");

		const char* script =
			"local sqlType = { integer = 0, string = 1, real = 2 }\n"
			"local connectionHandle = openConnection('57.823,\\n')\n"
			"local queryHandle = executeQuery( 'select 2', {sqlType.real}, connectionHandle )\n"
			"local row = fetchNextRow( queryHandle )\n"
			"colValue = row[1]\n"
			"closeQuery( queryHandle )\n"
			"closeConnection( connectionHandle )\n"
            "return colValue\n";

		lua_State* L = lua_open();
		boost::shared_ptr<void> closeOnExit( L, lua_close );
		luaL_openlibs( L );
        MyConnectionFactory cf;
		LuaDB s( L, &mockConnectionFactory, &cf );

		if( luaL_dostring( L, script ) )
		{
			std::puts( lua_tostring( L, -1 ) );
			ensure( false );
		}

		ensure(lua_isnumber( L, -1 ));
		double result = lua_tonumber( L, -1 );
		ensure( ( result > 57.7 ) && ( result < 57.9 ) );
	}

    template<>
	template<>
	void testLuaDB::test<4>()
	{
		set_test_name("LuaDB - query for several rows");

		const char* script =
			"local sqlType = { integer = 0, string = 1, real = 2 }\n"
			"local connectionHandle = openConnection('5,C,\\n6,a,\\n3,t,\\n7,s,\\n')\n"
			"local queryHandle = executeQuery( 'my sql query', {sqlType.integer, sqlType.string}, connectionHandle )\n"
			"local row = fetchNextRow( queryHandle )\n"
            "local sum = 0\n"
            "local concat = ''\n"
			"while row do\n"
            "  sum    = sum     + row[1]\n"
            "  concat = concat .. row[2]\n" 
            "  row = fetchNextRow( queryHandle )\n"
            "end\n"
			"closeQuery( queryHandle )\n"
			"closeConnection( connectionHandle )\n"
            "return sum, concat\n";

		lua_State* L = lua_open();
		boost::shared_ptr<void> closeOnExit( L, lua_close );
		luaL_openlibs( L );
        MyConnectionFactory cf;
		LuaDB s( L, &mockConnectionFactory, &cf );

		if( luaL_dostring( L, script ) )
		{
			std::puts( lua_tostring( L, -1 ) );
			ensure( false );
		}

		ensure(lua_isstring( L, -1 ));
		ensure(lua_isnumber( L, -2 ));
        const std::string concat = lua_tostring( L, -1 );
        const int sum = lua_tointeger( L, -2 );
		ensure_equals( concat, std::string("Cats") );
        ensure_equals( sum   , 5 + 6 + 3 + 7 );
	}

    template<>
	template<>
	void testLuaDB::test<5>()
	{
		set_test_name("LuaDB - evidence of parallelism");

        std::string queryResults;

        for ( int i = 0; i < 100000; ++i )
        {
            char buffer[10];
            std::sprintf( buffer, "%d", i );
            queryResults.append( buffer );
            queryResults.append( ",\\n" );
        }

        std::string script = std::string(
            " function loopWithCallbacks(threadID) \n"
			"   local sqlType = { integer = 0, string = 1, real = 2 }\n"
			"   local connectionHandle = openConnection('") + queryResults + std::string( "')\n"
			"   local queryHandle = executeQuery( 'my sql query', {sqlType.integer}, connectionHandle )\n"
			"   local row = fetchNextRow( queryHandle )\n"
			"   while row do\n"
            "       callback( threadID )\n"
            "       row = fetchNextRow( queryHandle )\n"
            "   end\n"
			"   closeQuery( queryHandle )\n"
			"   closeConnection( connectionHandle )\n"
            " end\n");


        HANDLE threads[2];
        MyConnectionFactory cf;
        threads[0] = runScriptOnThread( script, "A", &cf );
        threads[1] = runScriptOnThread( script, "B", &cf );

	    DWORD waitResult = WaitForMultipleObjectsEx( 2, threads, true, INFINITE, false );

        ensure( "Wait on thread termination.",
            ( waitResult >= WAIT_OBJECT_0 			) &&
            ( waitResult < WAIT_OBJECT_0 + 2 	) );

        ensure( "No Lua errors", !  (  luaThreads[0]->error() || luaThreads[1]->error() ) );

        bool evidenceOfInterleaving =  
            ( callbacks.find_first_of( "A" )  < callbacks.find_last_of( "B" )  ) &&
            ( callbacks.find_first_of( "B" )  < callbacks.find_last_of( "A" )  );

        ensure( "Evidence of interleaving", evidenceOfInterleaving );
	}

    template<>
	template<>
	void testLuaDB::test<6>()
	{
		set_test_name("LuaDB - OK to close without having opened");

        lua_State* L = lua_open();
		boost::shared_ptr<void> closeOnExit( L, lua_close );
		luaL_openlibs( L );

		if( luaL_dostring( L, " local value = 4 " ) )
		{
			std::puts( lua_tostring( L, -1 ) );
			ensure( false );
		}

		closeLuaDB();
    }

    template<>
	template<>
	void testLuaDB::test<7>()
	{
		set_test_name("LuaDB - running a script twice");

        lua_State* L = lua_open();
		boost::shared_ptr<void> closeOnExit( L, lua_close );
		luaL_openlibs( L );

        const char* script = 
        " total = 0 \n" 
        " function useConnection() \n" 
			"local sqlType = { integer = 0 }\n"
			"local connectionHandle = openConnection('49,\\n')\n"
			"local queryHandle = executeQuery( 'select 2', {sqlType.integer}, connectionHandle )\n"
			"local row = fetchNextRow( queryHandle )\n"
			"colValue = row[1]\n"
			"closeQuery( queryHandle )\n"
			//"closeConnection( connectionHandle )\n"
            "_ENV.total = _ENV.total + colValue\n"
            "return colValue\n"
        " end\n";

        // Note how the LuaDB functions aren't registered with Lua but dostring has no problem!

        // Perform the function assignment
        if( luaL_dostring( L, script ) )
        {
            ensure( lua_tostring( L, -1 ), false );
        }

        MyConnectionFactory cf;

        // Run the function twice
        for ( int i = 0; i < 2; ++i )
        {
            {
                LuaDB s( L, &mockConnectionFactory, &cf );

                // Push the function onto the Lua stack
                lua_getglobal( L, "useConnection" );

                // Run the function
                if ( lua_pcall( L, 0, 1, 0 ) )
                {
                    ensure( lua_tostring( L, -1 ), false );
                }

                // Test its return value
                ensure(lua_isnumber( L, -1 ));
                ensure( 49 == lua_tointeger( L, -1 ) );
                lua_pop( L, 1 );

                ensure( "The script opened a connection but didn't close it.", cf.openConnections == 1 );
		    }

            // As the LuaDB object went out of scope,it cleaned up, closing open connections.
            ensure( cf.openConnections == 0 );
        }

        lua_getglobal( L, "total" );

        ensure( "Should be a number", lua_isnumber( L, -1 ));
        ensure_equals( "Value should be 2 * 49", lua_tointeger( L, -1 ), 2 * 49);

        // Run the function on a different thread
        PTP_WORK pTP_Work = CreateThreadpoolWork( runLuaFunc, L, NULL );
        SubmitThreadpoolWork( pTP_Work );
        WaitForThreadpoolWorkCallbacks( pTP_Work, false );
        CloseThreadpoolWork( pTP_Work );

        lua_getglobal( L, "total" );

        ensure( "Should be a number", lua_isnumber( L, -1 ));
        ensure_equals( "Value should be 3 * 49", lua_tointeger( L, -1 ), 3 * 49);
    }
};

#endif

