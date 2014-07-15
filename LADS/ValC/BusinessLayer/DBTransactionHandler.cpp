#include "AcquireCriticalSection.h"
#include "DBConnection.h"
#include "DBTransactionHandler.h"
#include <SysUtils.hpp>
#include "ThreadPool.h"

namespace valc
{

DBTransactionHandler::DBTransactionHandler( 
    paulstdb::DBConnection*                 c,
    paulst::LoggingService*                 log,
    const SnapshotUpdateHandle&             snapshotUpdateHandle,
    int                                     shutdownTimeoutSecs,
    bool                                    cancelPendingUpdatesOnShutdown,
    stef::TaskExceptionHandler*             defaultTaskExceptionHandler,
    const paulst::Config*                   config )
    :
    m_connection                    ( c ),
    m_log                           ( log ),
    m_snapshotUpdateHandle          ( snapshotUpdateHandle ),
    workerThread                    ( new stef::ThreadPool(0, 1) ),
    m_shutdownTimeoutSecs           ( shutdownTimeoutSecs ),
    m_cancelPendingUpdatesOnShutdown( cancelPendingUpdatesOnShutdown ),
    m_config                        ( config )
{
    workerThread->addDefaultTaskExceptionHandler( defaultTaskExceptionHandler );
}

DBTransactionHandler::~DBTransactionHandler()
{
    m_connection->close();
    workerThread->shutdown( 1000 * m_shutdownTimeoutSecs, m_cancelPendingUpdatesOnShutdown );
}

const paulst::Config* DBTransactionHandler::getConfig() const
{
    return m_config;
}

paulstdb::DBConnection* DBTransactionHandler::getConnection() const
{
    return m_connection;
}

paulst::LoggingService* DBTransactionHandler::getLog() const
{
    return m_log;
}

SnapshotUpdateHandle DBTransactionHandler::getSnapshotUpdateHandle() const
{
    paulst::AcquireCriticalSection a(m_critSec);

    return m_snapshotUpdateHandle;
}


void DBTransactionHandler::queue( stef::Task* t )
{
    if ( ! workerThread->addTask( t ) )
    {
        throw Exception( L"ThreadPool refused to accept new DBUpdateTask" );
    }
}

void DBTransactionHandler::setSnapshotUpdateHandle( const SnapshotUpdateHandle& h )
{
    paulst::AcquireCriticalSection a(m_critSec);

    m_snapshotUpdateHandle = h;
}

bool DBTransactionHandler::waitForQueued( long millis )
{
    return workerThread->waitTillQuiet( millis );
}

}

