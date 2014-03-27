#include "ThreadPool.h"
#include "Task.h"

namespace stef
{

VOID CALLBACK executeNextTask(
  PTP_CALLBACK_INSTANCE Instance,
  PVOID                 Context,
  PTP_WORK              Work )
{
	TaskQueue* tq = (TaskQueue*)Context;
	
	Task* task = tq->popTask();
    
    if ( task ) // There should always be a task, but just in case...
    {
        task->execute();

        tq->deleteTask( task );
    }
}

ThreadPool::ThreadPool(int threadMinimum, int threadMaximum)
	:
    threadPool(0),
    threadPoolWork(0),
    m_defaultTaskExceptionHandler(0)
{
	InitializeThreadpoolEnvironment( &threadPoolCallbackEnv );
	threadPool = CreateThreadpool(NULL);
	SetThreadpoolCallbackPool( &threadPoolCallbackEnv , threadPool );
	SetThreadpoolThreadMinimum( threadPool, threadMinimum );
	SetThreadpoolThreadMaximum( threadPool, threadMaximum );
	threadPoolWork = CreateThreadpoolWork( executeNextTask, &taskQueue, &threadPoolCallbackEnv );
}

ThreadPool::~ThreadPool()
{
	CloseThreadpoolWork( threadPoolWork );
	CloseThreadpool( threadPool );
	DestroyThreadpoolEnvironment( &threadPoolCallbackEnv );
}

void ThreadPool::addDefaultTaskExceptionHandler( TaskExceptionHandler* teh )
{
    m_defaultTaskExceptionHandler = teh;
}

bool ThreadPool::addTask( Task* task )
{
    task->registerExceptionHandler(this);

    if ( m_defaultTaskExceptionHandler )
    {
        task->registerExceptionHandler( m_defaultTaskExceptionHandler );
    }

    if ( taskQueue.add(task) )
    {
        SubmitThreadpoolWork( threadPoolWork );
        return true;
    }
    else
    {
        return false;
    }
}

void ThreadPool::clearLastError()
{
    lastError.store( std::string() );
}

std::string ThreadPool::getLastError() const
{
    std::string errorString;
    lastError.load( errorString );
    return errorString;
}

void ThreadPool::handleException( Task* t, const char* msg ) throw()
{
    lastError.store( msg );
}

bool ThreadPool::shutdown( long millis, bool cancelPending )
{
    taskQueue.closeToNewTasks();

    HANDLE h = CreateEvent( NULL, false, false, NULL );

    taskQueue.signalWhenQuiet( h, cancelPending );

    DWORD waitResult = WaitForSingleObject( h, millis );

    CloseHandle( h );

    if ( waitResult == WAIT_OBJECT_0 )
    {
        delete this;
        return true;
    }
    else
    {
        return false;
    }
}

bool ThreadPool::waitTillQuiet( long millis )
{
    HANDLE h = CreateEvent( NULL, false, false, NULL );

    taskQueue.signalWhenQuiet( h, false );

    DWORD waitResult = WaitForSingleObject( h, millis );

    CloseHandle( h );

    return ( waitResult == WAIT_OBJECT_0 );
}

}


