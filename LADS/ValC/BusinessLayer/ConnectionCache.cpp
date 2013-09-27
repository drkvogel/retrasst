#include "AbstractConnectionFactory.h"
#include "AcquireCriticalSection.h"
#include "ConnectionCache.h"
#include "DBConnection.h"

namespace valc
{

class ConnectionProxy : public paulstdb::DBConnection
{
public:
    ConnectionProxy( paulstdb::DBConnection* underlyingConnection )
        :
        m_c( underlyingConnection )
    {}

    void              close()                                {} // Does NOT close the underlying connection, which is owned by the Cache.
    paulstdb::Cursor* executeQuery( const std::string& sql ) { return m_c->executeQuery( sql ); }
    void              executeStmt ( const std::string& sql ) { m_c->executeStmt( sql );         }

private:
    paulstdb::DBConnection* m_c;

    ConnectionProxy( const ConnectionProxy& );
    ConnectionProxy& operator=( const ConnectionProxy& );
};

ConnectionCache::ConnectionCache()
    :
    m_connectionFactory( 0 )
{
}

ConnectionCache::~ConnectionCache()
{
    for( Connections::iterator i = m_connections.begin(); i != m_connections.end(); ++i )
    {
        paulstdb::DBConnection* connection = i->second;
        connection->close();
        delete connection;
    }
}

paulstdb::DBConnection* ConnectionCache::getConnection( const char* connectionString )
{
    paulst::AcquireCriticalSection a(m_critSec);

    {
        paulstdb::DBConnection* connection = 0;

        Connections::iterator i = m_connections.find( connectionString );

        if ( i == m_connections.end() )
        {
            connection = m_connectionFactory->createConnection( connectionString, "" );
            m_connections.insert( std::make_pair( std::string(connectionString), connection ) );
        }
        else
        {
            connection = i->second;
        }

        return new ConnectionProxy( connection );
    }
}

void ConnectionCache::setConnectionFactory( paulstdb::AbstractConnectionFactory* conFac )
{
    m_connectionFactory = conFac;
}


}

