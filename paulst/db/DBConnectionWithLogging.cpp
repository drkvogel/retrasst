#include "DBConnectionWithLogging.h"
#include "LoggingService.h"

namespace paulstdb
{

DBConnectionWithLogging::DBConnectionWithLogging( DBConnection* nestedInstance, paulst::LoggingService* log )
    :
    m_nestedInstance( nestedInstance ),
    m_log( log )
{
}

DBConnectionWithLogging::~DBConnectionWithLogging()
{
    delete m_nestedInstance;
}

void DBConnectionWithLogging::close()
{
    m_nestedInstance->close();
}

Cursor* DBConnectionWithLogging::executeQuery( const std::string& sql )
{
    m_log->logFormatted( "Executing query: %s", sql.c_str() );
    return m_nestedInstance->executeQuery( sql );
}

void DBConnectionWithLogging::executeStmt ( const std::string& sql )
{
    m_log->logFormatted( "Executing statement: %s", sql.c_str() );
    m_nestedInstance->executeStmt( sql );
}

}

