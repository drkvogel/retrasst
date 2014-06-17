#include "SnapshotUpdateTask.h"

namespace valc
{

void setSignal( HANDLE h )
{
    SetEvent( h );
}

SnapshotUpdateTask::SnapshotUpdateTask()
    :
    dbTransactionHandler(0),
    snapshotObserver(0),
    log(0),
    doneSignal( CreateEvent( NULL, false, false, NULL ) )
{
}

HANDLE SnapshotUpdateTask::getDoneSignal()
{
    return doneSignal;
}

void SnapshotUpdateTask::setDBTransactionHandler( DBTransactionHandler* dbth )
{
    dbTransactionHandler = dbth;
}

void SnapshotUpdateTask::setLog( paulst::LoggingService* l )
{
    log = l;
}

void SnapshotUpdateTask::setSnapshot( SnapshotUpdateHandle s )
{
    snapshot = s;
}

void SnapshotUpdateTask::setSnapshotObserver( SnapshotObserver* so )
{
    snapshotObserver = so;
}

}

