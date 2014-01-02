#ifndef ABSTRACTCONNECTIONFACTORYH
#define ABSTRACTCONNECTIONFACTORYH

#include <string>

namespace paulstdb
{

class DBConnection;

class AbstractConnectionFactory
{
public:
    virtual ~AbstractConnectionFactory();
    virtual DBConnection* createConnection( const std::string& connectionString, const std::string& sessionReadLockSetting ) = 0;
};

}

#endif

