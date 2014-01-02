#ifndef DBTRANSACTIONUTILH
#define DBTRANSACTIONUTILH

namespace paulstdb
{
    class DBConnection;
}

namespace valc
{

void commit( paulstdb::DBConnection* c );
void rollback( paulstdb::DBConnection* c );
void setAutoCommit( paulstdb::DBConnection* c, bool on );

}

#endif

