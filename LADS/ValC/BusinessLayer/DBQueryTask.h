#ifndef DBQUERYTASKH
#define DBQUERYTASKH

#include <memory>
#include <string>
#include "../../../paulst/stef/Task.h"

namespace paulstdb
{
    class AbstractConnectionFactory;
}

namespace valc
{

class CursorConsumer;

class DBQueryTask : public stef::Task
{
public:
    DBQueryTask( const std::string& name = "DBQueryTask" );
    ~DBQueryTask();
    void setConnectionFactory( paulstdb::AbstractConnectionFactory* cf );
    void setConnectionString( const std::string& connectionString );
    void setCursorConsumer( CursorConsumer* cc, bool assumeOwnershipOfConsumer = true );
    void setSessionReadLockSetting( const std::string& sessionReadLockSetting );
protected:
    void doStuff();
private:
    CursorConsumer* m_cursorConsumer;
    paulstdb::AbstractConnectionFactory* m_connectionFactory;
    std::string m_connectionString;
    std::string m_sessionReadLockSetting;
    bool m_deleteCursorConsumer;
    
};

}

#endif

