#include "AbstractConnectionFactory.h"
#include <boost/shared_ptr.hpp>
#include "Cursor.h"
#include "CursorConsumer.h"
#include "DBConnection.h"
#include "DBQueryTask.h"
#include "Require.h"

namespace valc
{

void releaseConnection( paulstdb::DBConnection* c )
{
    if ( c )
    {
        try
        {
            c->close();
        }
        catch( ... )
        {
        }
        delete c;
    }
}

void releaseCursor( paulstdb::Cursor* c )
{
    if ( c )
    {
        try
        {
            c->close();
        }
        catch( ... )
        {
        }
        delete c;
    }
}

DBQueryTask::DBQueryTask( const std::string& name  )
    : 
    stef::Task( name ),
    m_cursorConsumer(0),
    m_connectionFactory(0),
    m_deleteCursorConsumer(true)
{
}

DBQueryTask::~DBQueryTask()
{
    if ( m_deleteCursorConsumer )
    {
        delete m_cursorConsumer;
    }
}

void DBQueryTask::setConnectionFactory( paulstdb::AbstractConnectionFactory* cf )
{
    m_connectionFactory = cf;
}

void DBQueryTask::setConnectionString( const std::string& connectionString )
{
    m_connectionString = connectionString;
}

void DBQueryTask::setCursorConsumer( CursorConsumer* cc, bool assumeOwnership )
{
    m_cursorConsumer = cc;
    m_deleteCursorConsumer = assumeOwnership;
}

void DBQueryTask::setSessionReadLockSetting( const std::string& sessionReadLockSetting )
{
    m_sessionReadLockSetting = sessionReadLockSetting;
}

void DBQueryTask::doStuff()
{
    require( m_connectionFactory );
    require( m_cursorConsumer );

    paulstdb::DBConnection* con = m_connectionFactory->createConnection( m_connectionString, m_sessionReadLockSetting );

    boost::shared_ptr<void> releaseConnectionOnBlockExit( con, releaseConnection );

    {
        paulstdb::Cursor* c = con->executeQuery( m_cursorConsumer->getSQL() );
        boost::shared_ptr<void> releaseCursorOnBlockExit( c, releaseCursor );
        
        m_cursorConsumer->consume( c );
    }
}

}

