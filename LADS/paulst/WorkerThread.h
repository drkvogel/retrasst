#ifndef WORKERTHREADH
#define WORKERTHREADH

#include <deque>

#include "CritSec.h"
#include "Thread.h"

namespace paulst
{

class WorkerThreadTask
{
public:
    WorkerThreadTask();
    virtual ~WorkerThreadTask() {}
    // Return 'false' to indicate the Thread should die.
    virtual bool execute() = 0;
    void setCancellationFlag();
    bool getCancellationFlag();

private:
    CritSec m_cs;
    bool m_cancelled;

    WorkerThreadTask( const WorkerThreadTask& );
    WorkerThreadTask& operator=( const WorkerThreadTask& );
};


class WorkerThread : public Runnable
{
public:
    WorkerThread();
    void cancelCurrentlyExecuting();
    int  queueSize();
    void queueTask( WorkerThreadTask* );
    void run( const Event* stopSignal );
private:
    std::deque<WorkerThreadTask*> m_queue; 
    CritSec m_cs;
    Thread m_thread;

    WorkerThread( const WorkerThread& );
    WorkerThread& operator=( const WorkerThread& );
    WorkerThreadTask* getFirst();
    void removeFirst();
};

};

#endif

