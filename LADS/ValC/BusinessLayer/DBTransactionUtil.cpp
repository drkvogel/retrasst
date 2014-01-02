#include "DBConnection.h"
#include "DBTransactionUtil.h"
#include <string>
#include "StrUtil.h"

namespace valc
{

void commit( paulstdb::DBConnection* c )
{
    c->executeStmt( "commit" );
}

void rollback( paulstdb::DBConnection* c )
{
    c->executeStmt( "rollback" );
}

void setAutoCommit( paulstdb::DBConnection* c, bool on )
{
    const std::string sql = paulst::format( "set autocommit %s", on ? "on" : "off" );
    c->executeStmt( sql );
}

}

