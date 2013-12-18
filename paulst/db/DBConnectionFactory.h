#ifndef DBCONNECTIONFACTORYH
#define DBCONNECTIONFACTORYH

#include "AbstractConnectionFactory.h"

namespace paulstdb
{

class DBConnectionFactory : public AbstractConnectionFactory
{
public:
    DBConnection* createConnection( const std::string& connectionString, const std::string& sessionReadLockSetting );
};

}

#endif

