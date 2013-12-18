#ifndef TaskQueueH
#define TaskQueueH

#include "CritSec.h"
#include <deque>

namespace stef
{

class Task;

class TaskQueue
{
private:
	typedef std::deque<Task*> Tasks;
	Tasks           tasks;
    paulst::CritSec m_cs;
    volatile HANDLE m_signalWhenQuiet;
    volatile int    m_currentlyExecuting;
    volatile bool   m_closedToNewTasks;

public:
	TaskQueue();
    // Returns false if the task cannot be added (refer to closeToNewTasks)
	bool  add( Task* t );
    // Called by worker thread to signal it has no further use for a task.
    void  deleteTask( Task* t );
    // Should never return NULL in practice (refer to ThreadPool.cpp)
	Task* popTask();
    // Has the effect that 'add' no longer adds, returning false instead of true.
    void closeToNewTasks();
    // Signals hEvent when queue is empty and nothing currently executing.
    void signalWhenQuiet( HANDLE hEvent, bool cancelPending );
};

}
#endif

