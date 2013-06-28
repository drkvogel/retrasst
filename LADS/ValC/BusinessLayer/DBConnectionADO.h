#ifndef DBCONNECTIONADOH
#define DBCONNECTIONADOH

#include "API.h"
#include "ADODB_TLB.h"

namespace valc
{

class DBConnectionADO : public DBConnection
{
public:
    DBConnectionADO( const Properties& p );
    ~DBConnectionADO();
    void close();
    Cursor* executeQuery( const std::string& sql );
    void    executeStmt ( const std::string& sql );
private:
    TCOM_Connection m_con;
    bool m_open;
};

};

#endif

