#ifndef DBCONNECTIONADOH
#define DBCONNECTIONADOH

#include "CritSec.h"
#include "DBConnection.h"
#include "ADODB_TLB.h"

namespace paulstdb
{

class DBConnectionADO : public DBConnection
{
public:
    DBConnectionADO( const std::string& connectionString, const std::string& sessionReadLockSetting = "" );
    ~DBConnectionADO();
    void close();
    Cursor* executeQuery( const std::string& sql );
    void    executeStmt ( const std::string& sql );
private:
    TCOM_Connection m_con;
    bool m_open;
	paulst::CritSec m_critSec;

	void reportErrors( const UnicodeString& supplementaryInfo );
};

};

#endif

