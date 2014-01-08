#ifndef DBUPDATECONSUMERH
#define DBUPDATECONSUMERH

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
    class TaskExceptionHandler;
    class ThreadPool;
}

namespace valc
{

class DBUpdateTask;


class DBTransactionHandler
{
public:
    DBTransactionHandler( 
        paulstdb::DBConnection*             connection,
        paulst::LoggingService*             log,
        const SnapshotUpdateHandle&         snapshotUpdateHandle, 
        int                                 shutdownTimeoutSecs,
        bool                                cancelPendingUpdatesOnShutdown,
        stef::TaskExceptionHandler*         defaultTaskExceptionHandler,
        const paulst::Config*               config );
    ~DBTransactionHandler();
    void queue( DBUpdateTask* t );
    bool waitForQueued( long millis );
private:
    paulstdb::DBConnection*             m_connection;
    paulst::LoggingService*             m_log;
    SnapshotUpdateHandle                m_snapshotUpdateHandle;
    stef::ThreadPool*                   m_threadPool;
    const int                           m_shutdownTimeoutSecs;
    const bool                          m_cancelPendingUpdatesOnShutdown;
    const paulst::Config*               m_config;
};

}

#endif

