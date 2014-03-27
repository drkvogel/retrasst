#ifndef THREADPOOLH
#define THREADPOOLH

#include "SerializedStorage.h"
#include <string>
#include "Task.h"
#include "TaskQueue.h"

namespace stef
{

/*
    Construct ThreadPool instances on the heap and do not delete them, but call 'shutdown' which 
    will do its best to delete the instance, provided it's safe to do so.
*/
class ThreadPool : public TaskExceptionHandler
{
public:
	ThreadPool(int threadMinimum = 0, int threadMaximum = 1);
	~ThreadPool();
    void        addDefaultTaskExceptionHandler( TaskExceptionHandler* teh );
    // Returns false if the task couldn't be scheduled (because shutdown in progress)
	bool        addTask( Task* t );
    void        clearLastError();
    // The last error of which ThreadPool was made aware via the 'handleException' callback.
    std::string getLastError() const;
    // Implementation of the TaskExceptionHandler interface.
	void        handleCancellation( Task* t ) throw() {}
    // Implementation of the TaskExceptionHandler interface.  ThreadPool likes to know about Task execution exceptions.
    void        handleException( Task* t, const char* msg ) throw();
    // Returns false if failed to clear backlog of added Tasks within the specified time limit.
    bool        shutdown( long millis, bool cancelPending );
    // Returns true if queue cleared within the specified time limit
    bool        waitTillQuiet( long millis );
private:
	TP_CALLBACK_ENVIRON     threadPoolCallbackEnv;
	PTP_POOL                threadPool;
	PTP_WORK                threadPoolWork;
	TaskQueue				taskQueue;
    paulst::SerializedStorage<std::string> lastError;
    TaskExceptionHandler*   m_defaultTaskExceptionHandler;

};

}

#endif

