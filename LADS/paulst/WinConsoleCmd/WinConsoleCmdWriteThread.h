#ifndef SQLTERMWRITETHREADH
#define SQLTERMWRITETHREADH

#include "Thread.h"
#include "WinConsoleCmd.h"

namespace paulst
{

class WinConsoleCmdWriteThread : public Runnable
{
public:
    WinConsoleCmdWriteThread( HANDLE h, Event* readyForCmdEvent, char* cmdBuffer, LogFunc l );
    ~WinConsoleCmdWriteThread();
    void abort();
    void run( const Event* stopSignal );
       
private:
    Thread m_thread;
    HANDLE m_h;
    Event* m_readyForCmdEvent;
    char* m_cmdBuffer;
    LogFunc log;

    static const unsigned long ABORT_THREAD_WAIT_TIMEOUT = 3000;
};

};

#endif

