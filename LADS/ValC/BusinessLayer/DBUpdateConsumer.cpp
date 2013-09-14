#include "DBUpdateConsumer.h"
#include "DBUpdateSchedule.h"
#include "DBUpdateTask.h"
#include <SysUtils.hpp>

namespace valc
{

DBUpdateConsumer::DBUpdateConsumer( 
    paulstdb::DBConnection* connection, 
    paulst::LoggingService* log,
    SampleRunIDResolutionService* s,
    DBUpdateSchedule* updateSchedule,
    DBUpdateExceptionHandlingPolicy* exceptionCallback )
    :
    m_connection                    ( connection        ),
    m_log                           ( log               ),
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
        while ( WAIT_TIMEOUT == stopSignal->wait(10) )
        {
            if ( m_updateSchedule->noMoreUpdates() )
            {
                break;
            }

            std::auto_ptr<DBUpdateTask> t( m_updateSchedule->front() );
            m_updateSchedule->pop_front();

            t->setConnection                    ( m_connection );
            t->setExceptionHandlingPolicy       ( m_exceptionCallback );
            t->setLog                           ( m_log );
            t->setSampleRunIDResolutionService  ( m_sampleRunIDResolutionService );

            const bool continueWithNext = t->execute();

            if ( ! continueWithNext )
            {
                break;
            }
        }
    }
    catch( ... )
    {
    }
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

