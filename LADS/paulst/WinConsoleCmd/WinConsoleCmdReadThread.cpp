#include "Require.h"
#include "WinConsoleCmd.h"
#include "WinConsoleCmdReadThread.h"
#include "Util.h"

#include <vector>

namespace paulst
{

WinConsoleCmdReadThread::WinConsoleCmdReadThread(HANDLE h, Event* readyForCmdEvent, char* cmdBuffer, WinConsoleCmdArgs* userArgs )
    :   
    m_readyForCmdEvent(readyForCmdEvent), 
    m_h(h), 
    m_thread(this), 
    m_userArgs( userArgs ),
    m_cmdBuffer(cmdBuffer)
{
}

WinConsoleCmdReadThread::~WinConsoleCmdReadThread()
{
    if ( ! CloseHandle( m_h ) )
    {
        m_userArgs->log( "CloseHandle failed." );
    }
}

void WinConsoleCmdReadThread::run( const Event* stopSignal )
{
    try
    {
        CHAR lpBuffer[256];
        DWORD nBytesRead;
        DWORD nCharsWritten;

        while ( WAIT_TIMEOUT == stopSignal->wait(10) )
        {
            if (!ReadFile( m_h,lpBuffer,sizeof(lpBuffer), &nBytesRead,NULL) || !nBytesRead)
            {
                if (GetLastError() == ERROR_BROKEN_PIPE)
                {
                    m_userArgs->log("ReadThread: ERROR_BROKEN_PIPE - breaking out");
                    break; // pipe done - normal exit path.
                }
                else
                {
                    throwLastError();
                }
            }
            else
            {
                std::string s( lpBuffer, lpBuffer + nBytesRead );

                if ( m_userArgs->dialogLogic( s.c_str(), m_cmdBuffer, m_userArgs->userData ) )
                {
                    m_readyForCmdEvent->set();
                }
            }
        }
    }
    catch( const Exception& e )
    {
        m_userArgs->log( stdstring( e.Message.c_str() ).c_str() );
    }
    catch( ... )
    {
        m_userArgs->log( "Undefined exception." );
    }
    m_userArgs->log( "Thread terminating..." );
} 

void WinConsoleCmdReadThread::abort()
{
    m_thread.terminate( ABORT_THREAD_WAIT_TIMEOUT );
}

}

