#include "SnapshotUpdateTask.h"
#include "SnapshotUpdateThread.h"
#include "ThreadPool.h"

namespace valc
{

SnapshotUpdateThread::SnapshotUpdateThread( 
        DBTransactionHandler*           dbth, 
        SnapshotUpdateHandle            snapshot,
        paulst::LoggingService*         log,
        stef::TaskExceptionHandler*     defaultExceptionHandler )
    :
    m_snapshotObserver(0),
    m_dbTransactionHandler(dbth),
    m_log(log),
    m_snapshot(snapshot),
    m_threadPool( new stef::ThreadPool( 0, 1 ) )
{
    m_threadPool->addDefaultTaskExceptionHandler( defaultExceptionHandler );
}

SnapshotUpdateThread::~SnapshotUpdateThread()
{
    m_threadPool->shutdown( 2000, true );
}

bool SnapshotUpdateThread::add( SnapshotUpdateTask* t )
{
    t->setDBTransactionHandler( m_dbTransactionHandler );
    t->setLog( m_log );
    t->setSnapshot( m_snapshot );
    t->setSnapshotObserver( m_snapshotObserver );

    return m_threadPool->addTask( t );
}

void SnapshotUpdateThread::setSnapshotObserver( SnapshotObserver* so )
{
    m_snapshotObserver = so;
}

bool SnapshotUpdateThread::waitTillQuiet( long millis )
{
    return m_threadPool->waitTillQuiet( millis );
}

}

