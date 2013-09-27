#ifndef MOCKRULEENGINECONNECTIONFACTORYH
#define MOCKRULEENGINECONNECTIONFACTORYH

#include "AbstractConnectionFactory.h"
#include "DBConnection.h"
#include "StringBackedCursor.h"

class MockRuleEngineConnection : public paulstdb::DBConnection
{
public:
    MockRuleEngineConnection( int* opened, int* closed )
        :
        m_opened(opened),
        m_closed(closed)
    {
        if ( m_opened )
            ++(*m_opened);
    }

    void close()
    {
        if ( m_closed )
            ++(*m_closed);
    }

    paulstdb::Cursor* executeQuery( const std::string& sql )
    {
        return new paulstdb::StringBackedCursor( sql );
    }

    void executeStmt( const std::string& ) 
    {
    }
private:
    int* m_opened;
    int* m_closed;
};


class MockRuleEngineConnectionFactory : public paulstdb::AbstractConnectionFactory
{
private:
    int* m_opened;
    int* m_closed;
public:
    MockRuleEngineConnectionFactory()
        :
        m_opened(0),
        m_closed(0)
    {
    }

    void setConnectionCounter( int* opened, int* closed )
    {
        m_opened = opened;
        m_closed = closed;
    }

    paulstdb::DBConnection* createConnection( const std::string& connectionString, const std::string& )
    {
        return new MockRuleEngineConnection(m_opened, m_closed);
    }
};

#endif

