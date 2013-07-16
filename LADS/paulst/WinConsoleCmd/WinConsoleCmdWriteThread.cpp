#include "Require.h"
#include "WinConsoleCmdWriteThread.h"
#include "Util.h"

namespace paulst
{

WinConsoleCmdWriteThread::WinConsoleCmdWriteThread( HANDLE h, Event* readyForCmdEvent, char* cmdBuffer, LogFunc l )
    :
    m_readyForCmdEvent(readyForCmdEvent), 
    m_h(h), 
    m_thread(this), 
    m_cmdBuffer(cmdBuffer),
    log(l) 
{
}

WinConsoleCmdWriteThread::~WinConsoleCmdWriteThread()
{
    if ( ! CloseHandle( m_h ) )
    {
        log( "CloseHandle failed." );
    }
}

void WinConsoleCmdWriteThread::abort()
{
    m_thread.terminate( ABORT_THREAD_WAIT_TIMEOUT );
}

void WinConsoleCmdWriteThread::run( const Event* stopSignal )
{
    try
    {
        while ( WAIT_TIMEOUT == stopSignal->wait(10) )
        {
            switch( m_readyForCmdEvent->wait(10) )
            {
                case WAIT_TIMEOUT:   continue; 
                case WAIT_FAILED:    throwLastError(); break;
                case WAIT_ABANDONED: throwRuntimeError( "WAIT_ABANDONED" ); break;
                case WAIT_OBJECT_0:  break; // i.e. signalled
            }

            DWORD nBytesWritten = 0;
            
            if (  ! WriteFile( m_h, m_cmdBuffer, strlen(m_cmdBuffer), &nBytesWritten,NULL ) )
            {
                throwLastError();
            }

            m_readyForCmdEvent->reset();
        }
    }
    catch( const Exception& e )
    {
        log( stdstring( e.Message.c_str() ).c_str() );
    }
    catch( ... )
    {
        log( "Undefined exception." );
    }

    log( "Thread terminating..." );
}
   
}

