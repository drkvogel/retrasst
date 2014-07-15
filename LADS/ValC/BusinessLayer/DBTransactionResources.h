#ifndef DBTRANSACTIONRESOURCESH
#define DBTRANSACTIONRESOURCESH

#include "SnapshotUpdateHandle.h"

namespace paulst
{
    class Config;
    class LoggingService;
}

namespace paulstdb
{
    class DBConnection;
}

namespace valc
{

class DBTransactionResources
{
public:
    DBTransactionResources();
    virtual ~DBTransactionResources();

    virtual const paulst::Config*       getConfig()                 const = 0;
    virtual paulstdb::DBConnection*     getConnection()             const = 0;
    virtual paulst::LoggingService*     getLog()                    const = 0;
    virtual SnapshotUpdateHandle        getSnapshotUpdateHandle()   const = 0;
};

}

#endif

