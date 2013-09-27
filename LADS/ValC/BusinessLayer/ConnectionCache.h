#ifndef CONNECTIONCACHEH
#define CONNECTIONCACHEH

#include "CritSec.h"
#include <map>
#include <string>

namespace paulstdb
{
    class AbstractConnectionFactory;
    class DBConnection;
}

namespace valc
{

class ConnectionCache
{
public:
    ConnectionCache();
    ~ConnectionCache();
    paulstdb::DBConnection* getConnection( const char* connectionString );
    void setConnectionFactory( paulstdb::AbstractConnectionFactory* conFac );
private:
    paulst::CritSec m_critSec;
    paulstdb::AbstractConnectionFactory* m_connectionFactory;
    typedef std::map< std::string, paulstdb::DBConnection* > Connections;
    Connections m_connections;

    ConnectionCache( const ConnectionCache& );
    ConnectionCache& operator=( const ConnectionCache& );
};

}

#endif

