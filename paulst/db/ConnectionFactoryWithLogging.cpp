#include "ConnectionFactoryWithLogging.h"
#include "DBConnectionWithLogging.h"
#include "LoggingService.h"

namespace paulstdb
{


ConnectionFactoryWithLogging::ConnectionFactoryWithLogging( AbstractConnectionFactory* nestedInstance, paulst::LoggingService* log )
    :
    m_nestedInstance( nestedInstance ),
    m_log( log )
{
}

ConnectionFactoryWithLogging::~ConnectionFactoryWithLogging()
{
    delete m_nestedInstance;
}

DBConnection* ConnectionFactoryWithLogging::createConnection( const std::string& connectionString, const std::string& sessionReadLockSetting )
{
    m_log->logFormatted( "Establishing database connection using '%s'", connectionString.c_str() );

    return new DBConnectionWithLogging( m_nestedInstance->createConnection( connectionString, sessionReadLockSetting ), m_log );
}


}

