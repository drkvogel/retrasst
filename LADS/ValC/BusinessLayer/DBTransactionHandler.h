#ifndef DBUPDATECONSUMERH
#define DBUPDATECONSUMERH

#include "CritSec.h"
#include "DBTransactionResources.h"
#include "SnapshotUpdateHandle.h"

namespace paulst
{
    class Config;
    class LoggingService;
};

namespace paulstdb
{
    class DBConnection;
}

namespace stef
{
    class Task;
    class TaskExceptionHandler;
    class ThreadPool;
}

namespace valc
{

class DBUpdateTask;


class DBTransactionHandler : public DBTransactionResources
{
public:
    stef::ThreadPool* workerThread;

    DBTransactionHandler( 
        paulstdb::DBConnection*             connection,
        paulst::LoggingService*             log,
        const SnapshotUpdateHandle&         snapshotUpdateHandle, 
        int                                 shutdownTimeoutSecs,
        bool                                cancelPendingUpdatesOnShutdown,
        stef::TaskExceptionHandler*         defaultTaskExceptionHandler,
        const paulst::Config*               config );

    ~DBTransactionHandler();

    const paulst::Config*       getConfig()                 const;
    paulstdb::DBConnection*     getConnection()             const;
    paulst::LoggingService*     getLog()                    const;
    SnapshotUpdateHandle        getSnapshotUpdateHandle()   const;

    void queue( stef::Task* t );
    void setSnapshotUpdateHandle( const SnapshotUpdateHandle& h );
    bool waitForQueued( long millis );
private:
    paulst::CritSec                m_critSec;
    paulstdb::DBConnection* const  m_connection;
    paulst::LoggingService* const  m_log;
    SnapshotUpdateHandle           m_snapshotUpdateHandle;
    const int                      m_shutdownTimeoutSecs;
    const bool                     m_cancelPendingUpdatesOnShutdown;
    const paulst::Config*   const  m_config;
};

}

#endif

