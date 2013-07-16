#ifndef WINCONSOLECMDREADTHREADH
#define WINCONSOLECMDREADTHREADH

#include "Thread.h"

struct WinConsoleCmdArgs;

namespace paulst
{

class WinConsoleCmdReadThread : Runnable
{
public:
    WinConsoleCmdReadThread(HANDLE h, Event* readyForCmdEvent, char* cmdBuffer, WinConsoleCmdArgs* userArgs);
    ~WinConsoleCmdReadThread();
    void abort();
    void run( const Event* stopSignal );
private:
    Thread m_thread;
    HANDLE m_h;
    Event* m_readyForCmdEvent;
    WinConsoleCmdArgs* m_userArgs;
    char* m_cmdBuffer;
 
    bool isWaitingForCmd();

    static const int ABORT_THREAD_WAIT_TIMEOUT = 3000;
};

};

#endif

