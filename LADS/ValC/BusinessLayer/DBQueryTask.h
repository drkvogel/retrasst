#ifndef DBQUERYTASKH
#define DBQUERYTASKH

#include "AbstractConnectionFactory.h"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "Config.h"
#include "Cursor.h"
#include "DBConnection.h"
#include <string>
#include "Task.h"


namespace valc
{

void releaseConnection( paulstdb::DBConnection* c );
void releaseCursor( paulstdb::Cursor* c );

typedef boost::function<void (paulstdb::Cursor*)> CursorConsumer;

class DBQueryTask : public stef::Task
{
public:
    
    DBQueryTask( const std::string& name, paulstdb::AbstractConnectionFactory* f, const paulst::Config* config, CursorConsumer cc )
        :
        m_connectionFactory(f),
        m_cursorConsumer(cc),
        m_name( name )
    {
        m_connectionString       = config->get( name + "ConnectionString" );
        m_sessionReadLockSetting = config->get( name + "SessionReadLockSetting" );
        m_sql                    = config->get( name + "Query" );
    }

    std::string getName() const { return m_name; }

protected:
    void doStuff()
    {
        paulstdb::DBConnection* con = m_connectionFactory->createConnection( m_connectionString, m_sessionReadLockSetting );

        boost::shared_ptr<void> releaseConnectionOnBlockExit( con, releaseConnection );

        {
            paulstdb::Cursor* c = con->executeQuery( m_sql );
            boost::shared_ptr<void> releaseCursorOnBlockExit( c, releaseCursor );
            
            while ( *c )
            {
                m_cursorConsumer( c );
                c->next();
            }
        }
    }

private:
    paulstdb::AbstractConnectionFactory*    m_connectionFactory;
    std::string                             m_connectionString;
    std::string                             m_sessionReadLockSetting;
    std::string                             m_sql;
    CursorConsumer                          m_cursorConsumer;
    std::string                             m_name;
    
};

}

#endif

