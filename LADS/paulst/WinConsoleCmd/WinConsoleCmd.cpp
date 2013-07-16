#include "Require.h"
#include "WinConsoleCmd.h"
#include "WinConsoleCmdReadThread.h"
#include "WinConsoleCmdWriteThread.h"
#include "StrUtil.h"
#include "Thread.h"
#include "Util.h"

#include <algorithm>
#include <cstring>
#include <windows.h>

WinConsoleCmdArgs::WinConsoleCmdArgs()
    : timeoutMillis(DEFAULT_TIMEOUT_VALUE)
{
}

// A pipe has an end for reading and an end for writing.
struct Pipe
{
    HANDLE hRead, hWrite;

    Pipe()
    {
        SECURITY_ATTRIBUTES sa;
        sa.nLength= sizeof(SECURITY_ATTRIBUTES);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;

        throwUnless (CreatePipe(&hRead,&hWrite,&sa,0));
    }

private:
    Pipe( const Pipe& );
    Pipe& operator=( const Pipe& );
};


void launchChild( HANDLE childStdOut, HANDLE childStdIn, HANDLE childStdErr, PROCESS_INFORMATION* pi, const char* connectionString );
void swapForDuplicate( HANDLE& h );



void runWinConsoleCmd( WinConsoleCmdArgs* args )
{
    args->timedOut = false;
    args->exitCode = 0;
    std::string exception;
    std::memset( args->exceptionMsg, 0, MSG_BUF_LEN );
    char cmdBuffer[1024];
    cmdBuffer[0] = '\0';
   
    try
    {
        HANDLE hErrorWrite;
        Pipe childOutputPipe, childInputPipe;

        // Create a duplicate of the output write handle for the std error
        // write handle. This is necessary in case the child application
        // closes one of its std output handles.
        throwUnless (DuplicateHandle(GetCurrentProcess(),childOutputPipe.hWrite,
                   GetCurrentProcess(),&hErrorWrite,0,
                   TRUE,DUPLICATE_SAME_ACCESS));

        // Create new output read handle and the input write handles. Set
        // the Properties to FALSE. Otherwise, the child inherits the
        // properties and, as a result, non-closeable handles to the pipes
        // are created.
        swapForDuplicate( childOutputPipe.hRead );
        swapForDuplicate( childInputPipe.hWrite );

        PROCESS_INFORMATION pi;
        launchChild( childOutputPipe.hWrite, childInputPipe.hRead, hErrorWrite, &pi, args->launchString );

        // Close the parent's copies of these handles, now the child has theirs.
        throwUnless (CloseHandle(childOutputPipe.hWrite));
        throwUnless (CloseHandle(childInputPipe.hRead  ));
        throwUnless (CloseHandle(hErrorWrite ));

        paulst::Event *readyForInput = new paulst::Event();

        // Threads assume ownership of pipe handles
        paulst::WinConsoleCmdReadThread  readThread ( childOutputPipe.hRead, readyForInput, cmdBuffer, args ); 
        paulst::WinConsoleCmdWriteThread writeThread( childInputPipe.hWrite, readyForInput, cmdBuffer, args->log ); 

        if ( WAIT_OBJECT_0 !=  WaitForSingleObject( pi.hProcess, args->timeoutMillis ) )
        {
            args->log( "Timeout. Aborting read- and write-threads..." );
            readThread.abort();
            writeThread.abort();
            args->log( "Terminating process..." );
            TerminateProcess( pi.hProcess, 1 );
            args->timedOut = true; 
        }
        
        throwUnless ( GetExitCodeProcess( pi.hProcess, &(args->exitCode) ) );
        throwUnless ( CloseHandle( pi.hProcess ) );
        throwUnless ( CloseHandle( pi.hThread  ) );
    }
    catch( const Exception& e )
    {
        exception = std::string("Exception. ") + paulst::stdstring( e.Message.c_str() );
    }
    catch( ... )
    {
        exception = "Undefined exception.";
    }

    if ( exception.size() )
    {
        args->log( exception.c_str() );
        exception.resize( MSG_BUF_LEN - 1 );
        std::copy( exception.begin(), exception.end(), args->exceptionMsg );
    }
}

void launchChild( HANDLE childStdOut, HANDLE childStdIn, HANDLE childStdErr, PROCESS_INFORMATION* pi, const char* launchString )
{
    STARTUPINFO si;

    // Set up the start up info struct.
    ZeroMemory(&si,sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
    si.hStdOutput = childStdOut;
    si.hStdInput  = childStdIn;
    si.hStdError  = childStdErr;
    si.wShowWindow = SW_HIDE;

    UnicodeString args(launchString);

    throwUnless (CreateProcess( NULL, args.c_str(), NULL,NULL,TRUE, CREATE_NEW_CONSOLE,NULL,NULL,&si,pi));
}


void swapForDuplicate( HANDLE& h )
{
    HANDLE hDup;

    throwUnless (DuplicateHandle(GetCurrentProcess(),h,
                       GetCurrentProcess(),
                       &hDup, // Address of new handle.
                       0,FALSE, // Make it uninheritable.
                       DUPLICATE_SAME_ACCESS));
    
    throwUnless (CloseHandle(h));

    h = hDup;
}


void removeLastCommand( char* lastCommand, char* commandStack )
{
    int len = std::strlen( commandStack );
    if ( len )
    {
        require( len > 1 );
        require( '\n' == *( commandStack + len - 1 ) );
        require( '\0' == *( commandStack + len ) );
        char* start = commandStack;
        char* end = start + len - 1;
        while (--end >= start && *end != '\n' );
        // There are 2 possibilities now. 'end' == (commandStack - 1) otherwise *end == '\n'
        // Include trailing '\0' when copying to lastCommand
        std::copy( end + 1, commandStack + len + 1, lastCommand );
        *(end + 1) = '\0';
    }
    else
    {
        *lastCommand = '\0';
    }
}


