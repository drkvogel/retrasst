#include "DBConnection.h"
#include "DBTransactionHandler.h"
#include "DBUpdateTask.h"
#include <SysUtils.hpp>
#include "ThreadPool.h"

namespace valc
{

DBTransactionHandler::DBTransactionHandler( 
    paulstdb::DBConnection*                 c,
    paulst::LoggingService*                 log,
    SampleRunIDResolutionService*           s,
    int                                     shutdownTimeoutSecs,
    bool                                    cancelPendingUpdatesOnShutdown,
    stef::TaskExceptionHandler*             defaultTaskExceptionHandler,
    const paulst::Config*                   config )
    :
    m_connection                    ( c ),
    m_log                           ( log ),
    m_sampleRunIDResolutionService  ( s ),
    m_threadPool                    ( new stef::ThreadPool(0, 1) ),
    m_shutdownTimeoutSecs           ( shutdownTimeoutSecs ),
    m_cancelPendingUpdatesOnShutdown( cancelPendingUpdatesOnShutdown ),
    m_config                        ( config )
{
    m_threadPool->addDefaultTaskExceptionHandler( defaultTaskExceptionHandler );
}

DBTransactionHandler::~DBTransactionHandler()
{
    m_connection->close();
    m_threadPool->shutdown( 1000 * m_shutdownTimeoutSecs, m_cancelPendingUpdatesOnShutdown );
}

void DBTransactionHandler::queue( DBUpdateTask* t )
{
    t->setConfig                        ( m_config );
    t->setConnection                    ( m_connection );
    t->setLog                           ( m_log );
    t->setSampleRunIDResolutionService  ( m_sampleRunIDResolutionService );

    if ( ! m_threadPool->addTask( t ) )
    {
        throw Exception( L"ThreadPool refused to accept new DBUpdateTask" );
    }
}

bool DBTransactionHandler::waitForQueued( long millis )
{
    return m_threadPool->waitTillQuiet( millis );
}

}

