#ifndef CONNECTIONFACTORYWITHLOGGINGH
#define CONNECTIONFACTORYWITHLOGGINGH

#include "AbstractConnectionFactory.h"

namespace paulst
{
    class LoggingService;
}

namespace paulstdb
{

class ConnectionFactoryWithLogging : public AbstractConnectionFactory
{
public:
    ConnectionFactoryWithLogging( AbstractConnectionFactory* nestedInstance, paulst::LoggingService* log );
    ~ConnectionFactoryWithLogging();
    DBConnection* createConnection( const std::string& connectionString, const std::string& sessionReadLockSetting );
private:
    AbstractConnectionFactory* m_nestedInstance;
    paulst::LoggingService* m_log;
};

}

#endif

