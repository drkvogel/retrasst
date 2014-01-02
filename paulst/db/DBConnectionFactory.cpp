#include "DBConnectionADO.h"
#include "DBConnectionFactory.h"

namespace paulstdb
{


paulstdb::DBConnection* DBConnectionFactory::createConnection( const std::string& connectionString,
    const std::string& sessionReadLockSetting )
{
    return new paulstdb::DBConnectionADO( connectionString, sessionReadLockSetting );
}



}

