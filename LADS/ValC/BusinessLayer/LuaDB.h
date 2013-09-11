#ifndef LUADBH
#define LUADBH

// Forward declarations
struct lua_State;

namespace valc
{
    class DBConnection;
}

typedef valc::DBConnection* (*ConnectionFactory)( const char* connectionString );

extern struct ConnectionStats
{
    int opened;
    int closed;
} stats;

extern ConnectionFactory connectionFactory;

void openLuaDB( lua_State* L );


#endif

