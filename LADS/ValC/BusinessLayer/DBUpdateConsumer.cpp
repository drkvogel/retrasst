#include "AbstractConnectionFactory.h"
#include "API.h"
#include "ApplicationContext.h"
#include "DBUpdateConsumer.h"
#include "DBUpdateSchedule.h"
#include "DBUpdateTask.h"
#include "LoggingService.h"
#include <SysUtils.hpp>

namespace valc
{

DBUpdateConsumer::DBUpdateConsumer( 
    ApplicationContext*                 appContext,
    SampleRunIDResolutionService*       s,
    DBUpdateSchedule*                   updateSchedule,
    DBUpdateExceptionHandlingPolicy*    exceptionCallback )
    :
    m_appContext                    ( appContext        ),
    m_sampleRunIDResolutionService  ( s                 ),
    m_updateSchedule                ( updateSchedule    ),
    m_exceptionCallback             ( exceptionCallback ),
    m_thread                        ( this              )
{
}

void DBUpdateConsumer::run( const paulst::Event* stopSignal )
{
    try
    {
        const std::string connectionString = m_appContext->getProperty( "DBUpdateConnectionString" );

        std::auto_ptr<paulstdb::DBConnection> connection( m_appContext->connectionFactory->createConnection( connectionString, "" ) );

        while ( WAIT_TIMEOUT == stopSignal->wait(10) )
        {
            if ( m_updateSchedule->noMoreUpdates() )
            {
                break;
            }

            std::auto_ptr<DBUpdateTask> t( m_updateSchedule->front() );

            m_updateSchedule->pop_front();

            t->setConnection                    ( connection.get() );
            t->setExceptionHandlingPolicy       ( m_exceptionCallback );
            t->setLog                           ( m_appContext->log );
            t->setSampleRunIDResolutionService  ( m_sampleRunIDResolutionService );

            const bool continueWithNext = t->execute();

            if ( ! continueWithNext )
            {
                break;
            }
        }
    }
    catch( const Exception& e )
    {
        m_exceptionCallback->handleException( AnsiString( e.Message.c_str() ).c_str() );
    }
    catch( ... )
    {
        m_exceptionCallback->handleException( "Unspecified Exception handled by DBUpdateConsumer." );
    }

    m_appContext->log->log( "DBUpdate consumer thread terminating..." );
}

void DBUpdateConsumer::waitFor()
{
    DWORD waitResult = WaitForSingleObjectEx( m_thread.getHandle(), INFINITE, false );

    if ( waitResult != WAIT_OBJECT_0 )
    {
        throw Exception( L"Unexpected return value from waiting on completion of the Database Update Consumer thread." );
    }
}

}

