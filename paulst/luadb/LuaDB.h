#ifndef LUADBH
#define LUADBH

// Forward declarations
struct lua_State;

namespace paulstdb
{
    class DBConnection;
}

typedef paulstdb::DBConnection* (*ConnectionFactory)( const char* connectionString, void* connectionState );

enum SQLDataType { SQLDataType_int, SQLDataType_string, SQLDataType_real };

/*
    Call openLuaDB to make the following methods available to your Lua script:
        openConnection( connectionString ) : returns a connection handle
        closeConnection( int connectionID )
        executeQuery( sql ) : returns a query handle
        fetchNextRow( queryHandle ) : returns an array of values or nil for end-of-recordset
        closeQuery( queryHandle )

    'connectionFactory' is a callback function that you must supply and which gets called 
    when a LUA script invokes 'openConnection'.

    'connectionState' is a pointer which gets passed as an argument to the 'connectionFactory' callback 
    whenever the latter is invoked (see above).  

    'connectionFactory' and 'connectionState', taken together, represent a closure.

    Note: 

    Usage of LuaDB is thread-bound.  In other words, openLuaDB initialises LuaDB for 
    the calling thread.
*/
void openLuaDB( lua_State* L, ConnectionFactory connectionFactory, void* connectionState );

/*
    Call closeLuaDB to release cached database resources acquired since openLuaDB was called.
    Again, usage is thread-bound.  
*/
void closeLuaDB();

/*
    LuaDB implements the idiom 'Resource Acquisition Is Initialization'
*/

class LuaDB
{
public:
    LuaDB( lua_State* L, ConnectionFactory cf, void* connectionState )
    {
        openLuaDB( L, cf, connectionState );
    }

    ~LuaDB()
    {
        closeLuaDB();
    }
};

#endif

