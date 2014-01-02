#include "Cursor.h"
#include "DBConnection.h"
#include "LuaDB.h"
#include "LuaInclude.h"
#include <map>
#include "Require.h"
#include <SysUtils.hpp>
#include <vector>
#include <windows.h>

// types


class OpenQuery
{
public:

    OpenQuery() 
        : 
        cursor(0)
    {
    }

    ~OpenQuery()
    {
        if ( cursor )
        {
            cursor->close();
            delete cursor;
        }
    }

    void addColumn( SQLDataType dataType )
    {
        columnDataTypes.push_back( dataType );
    }

    int columnCount() const
    {
        return columnDataTypes.size();
    }

    bool endOfRecordSet() const
    {
        require( cursor );
        return cursor->endOfRecordSet();
    }

    std::string getSQL() const
    {
        return m_sql;
    }

    void moveToNextRow()
    {
        require( cursor );
        cursor->next();
    }

    void pushValueForColumn( lua_State* L, int col )
    {
        require( cursor );
        
        if ( cursor->isNull( col ) )
        {
            lua_pushnil( L );
        }
        else
        {
            using namespace paulstdb;

            const SQLDataType colDataType = columnDataTypes.at(col);
            switch ( colDataType )
            {
                case SQLDataType_int:
                    lua_pushinteger( L, read<int>       ( *cursor, col ) );
                    break;
                case SQLDataType_string:
                    lua_pushstring( L,  read<std::string>( *cursor, col ).c_str() );
                    break;
                case SQLDataType_real:
                    lua_pushnumber( L,  read<float>      ( *cursor, col ) );
                    break;
                default:
                    throw Exception( UnicodeString(L"Unrecognised SQL data type: ") + UnicodeString(colDataType) );
            }
        }
    }

    void setCursor( paulstdb::Cursor* c )
    {
        cursor = c;
    }

    void setSQL( const std::string& sql )
    {
        m_sql = sql;
    }

private:
    OpenQuery( const OpenQuery& );
    OpenQuery& operator=( const OpenQuery& );

    std::vector<SQLDataType>  columnDataTypes;
    paulstdb::Cursor*   cursor;
    std::string m_sql;
};



typedef std::map< int, OpenQuery* > OpenQueries;
typedef std::map< int, paulstdb::DBConnection* > Connections;


class TLS_struct
{
public:

    TLS_struct( ConnectionFactory cf, void* connectionState )
        : connectionFactory(cf), connectionState(connectionState), lastConnectionHandle(0), lastQueryHandle(0)
    {
    }

    ~TLS_struct()
    {
        for ( OpenQueries::iterator i = openQueries.begin(); i != openQueries.end(); ++i )
        {
            delete i->second;
        }


        while ( connections.size() )
        {
            const int connectionHandle = connections.begin()->first;
            closeConnection( connectionHandle );
        }
    }

    void closeConnection( int connectionHandle )
    {
        Connections::iterator i = connections.find( connectionHandle );
        require( i != connections.end() );
        paulstdb::DBConnection* connection = i->second;
        connection->close();
        delete connection;
        connections.erase( i );
    }

    void closeQuery( int queryHandle )
    {
        OpenQueries::iterator i = openQueries.find( queryHandle );
        require( i != openQueries.end() );
        OpenQuery* q = i->second;
        delete q;
        openQueries.erase( i );
    }


    int createConnection( const char* connectionString )
    {
        connections.insert( std::make_pair( ++lastConnectionHandle, connectionFactory( connectionString, connectionState ) ) );
        return lastConnectionHandle;
    }

    OpenQuery* getQuery( int queryHandle )
    {
        OpenQueries::iterator i = openQueries.find( queryHandle );

        require( i != openQueries.end() );

        return i->second;
    }

    int runQuery( OpenQuery* q, int connectionHandle )
    {
        Connections::iterator i = connections.find( connectionHandle );
        require( i != connections.end() );
        paulstdb::DBConnection* connection = i->second;
        q->setCursor( connection->executeQuery( q->getSQL() ) );
        openQueries.insert( std::make_pair( ++lastQueryHandle, q ) );
        return lastQueryHandle;
    } 

private:
    ConnectionFactory connectionFactory;
    void* connectionState;
    int lastConnectionHandle;
    int lastQueryHandle;
    Connections connections;
    OpenQueries openQueries;
    
};


// Definitions (storage)

const DWORD TLS_index = TlsAlloc();

const int MAX_COLS = 100;

// Functions

TLS_struct* getThreadLocalData()
{
    TLS_struct* s = reinterpret_cast<TLS_struct*>( TlsGetValue( TLS_index ) );
    // require( s );
    return s;
}

void setThreadLocalData( TLS_struct* s )
{
	TLS_struct* existing = getThreadLocalData();

	if ( existing )
	{
		delete existing;
	}

	TlsSetValue( TLS_index, s );
}

void raiseLuaError( lua_State* L, const std::string& msg )
{
    lua_pushstring( L, msg.c_str() );
    lua_error( L );
}

void raiseLuaError( lua_State* L, const Exception& e )
{
    AnsiString aString( e.Message.c_str() );
    raiseLuaError( L, aString.c_str() );
}

static int openConnection( lua_State* L )
{
    try
    {
        const char* connectionString = lua_tostring( L, -1 );
        TLS_struct* s = getThreadLocalData();
        lua_pushnumber( L, s->createConnection( connectionString ) );
    }
    catch ( const Exception& e )
    {
        raiseLuaError( L, e );
    }
    catch ( ... )
    {
        raiseLuaError( L, "Unspecified error." );
    }

    return 1;
}

static int closeConnection( lua_State* L )
{
    TLS_struct* s = getThreadLocalData();
    const int connectionHandle = lua_tonumber( L, -1 );
    s->closeConnection( connectionHandle );
    return 0;
}


/*
    3 args expected:
        1) sql query string
        2) a table of sql data types
        3) connection handle (an integer)

    Return values:
        1) query handle (an integer)
*/
static int executeQuery( lua_State* L )
{
    try
    {
        TLS_struct* s = getThreadLocalData();

        std::auto_ptr<OpenQuery> query( new OpenQuery() );

        require( lua_isstring( L, -3 ) );
        require( lua_istable ( L, -2 ) );
        require( lua_isnumber( L, -1 ) );

        const int   connectionHandle = lua_tointeger( L, -1 );
        query->setSQL( lua_tostring ( L, -3 ) );
        

        // Getting the list of sql data types from the table at stack position -2
        for ( int i = 1; i <= MAX_COLS; ++i )
        {
            lua_rawgeti( L, -2, i );  // Push onto the stack the value at index i in the table

            if ( lua_isnil( L, -1 ) ) // end of table values
            {
                break;
            }
            else
            {
                require( lua_isnumber( L, -1 ) );
                query->addColumn( static_cast<SQLDataType>( lua_tointeger( L, -1 ) ) );
                lua_pop( L, 1 );
            }
        }

        const int queryHandle = s->runQuery( query.release(), connectionHandle );
        lua_pushnumber( L, queryHandle );
    } 
    catch ( const Exception& e )
    {
        raiseLuaError( L, e );
    }
    catch ( ... )
    {
        raiseLuaError( L, "Unspecified error." );
    }

    return 1;
}

/*
    1 argument expected: query handle (an integer)
    Return values: none
*/
static int closeQuery( lua_State* L )
{
    try
    {
        TLS_struct* s = getThreadLocalData();
        require( lua_isnumber( L, -1 ) );
        const int queryHandle = lua_tointeger( L, -1 );
        s->closeQuery( queryHandle );
    } 
    catch ( const Exception& e )
    {
        raiseLuaError( L, e );
    }
    catch ( ... )
    {
        raiseLuaError( L, "Unspecified error." );
    }

    return 0;
}

/*
    Expected arguments:
        1) queryHandle (an integer)
    Returns:
        1) a table of values, or nil for end-of-recordset

*/
static int fetchNextRow( lua_State* L )
{
    try
    {
        require( lua_isnumber( L, -1 ) );
        const int queryHandle = lua_tointeger( L, -1 );

        TLS_struct* s = getThreadLocalData();

        OpenQuery* q = s->getQuery( queryHandle );

        if ( q->endOfRecordSet() )
        {
            lua_pushnil( L );
        }
        else
        {
            lua_newtable( L );

            const int stackPositionOfTable = lua_gettop( L );

            for ( int col = 0; col < q->columnCount(); ++col )
            {
                // Push col value onto the stack
                q->pushValueForColumn( L, col );
                
                // Pop col value from the stack, adding it to the new table
                lua_rawseti( L, -2, col + 1 ); 
            }

            require( lua_gettop(L) == stackPositionOfTable ); // i.e. table should be topmost (still)

            q->moveToNextRow();
        }
    } 
    catch ( const Exception& e )
    {
        raiseLuaError( L, e );
    }
    catch ( ... )
    {
        raiseLuaError( L, "Unspecified error." );
    }

    return 1;
}

void openLuaDB( lua_State* L, ConnectionFactory cf, void* connectionState )
{
	require( TLS_index != TLS_OUT_OF_INDEXES );

	setThreadLocalData( new TLS_struct(cf,connectionState) );

	lua_register ( L, "openConnection" , openConnection  );
	lua_register ( L, "closeConnection", closeConnection );
	lua_register ( L, "executeQuery"   , executeQuery    );
	lua_register ( L, "fetchNextRow"   , fetchNextRow    );
	lua_register ( L, "closeQuery"     , closeQuery      );
}

void closeLuaDB()
{
	setThreadLocalData( NULL );
}

