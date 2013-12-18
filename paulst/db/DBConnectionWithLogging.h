#ifndef DBCONNECTIONWITHLOGGINGH
#define DBCONNECTIONWITHLOGGINGH

#include "DBConnection.h"

namespace paulst
{
    class LoggingService;
}

namespace paulstdb
{

class DBConnectionWithLogging : public DBConnection
{
public:
    DBConnectionWithLogging( DBConnection* nestedInstance, paulst::LoggingService* log );
    ~DBConnectionWithLogging();
    void    close();
    Cursor* executeQuery( const std::string& sql );
    void    executeStmt ( const std::string& sql );
private:
    DBConnection*           const m_nestedInstance;
    paulst::LoggingService* const m_log;
};

}

#endif

