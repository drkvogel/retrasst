#ifndef SNAPSHOTUPDATETHREADH
#define SNAPSHOTUPDATETHREADH

#include "SnapshotUpdateHandle.h"

namespace paulst
{
    class LoggingService;
}

namespace stef
{
    class TaskExceptionHandler;
    class ThreadPool;
}

namespace valc
{

class DBTransactionHandler;
class SnapshotObserver;
class SnapshotUpdateTask;

class SnapshotUpdateThread
{
public:
    SnapshotUpdateThread( 
        DBTransactionHandler*           dbth, 
        SnapshotUpdateHandle            snapshot,
        paulst::LoggingService*         log,
        stef::TaskExceptionHandler*     defaultUpdateExceptionHandler );
    ~SnapshotUpdateThread();
    bool add( SnapshotUpdateTask* t );
    void setSnapshotObserver( SnapshotObserver* so );
    bool waitTillQuiet( long millis );
private:
    DBTransactionHandler*           m_dbTransactionHandler;
    SnapshotUpdateHandle            m_snapshot;
    SnapshotObserver*               m_snapshotObserver;
    paulst::LoggingService*         m_log;
    stef::ThreadPool*               m_threadPool;
};

}

#endif

