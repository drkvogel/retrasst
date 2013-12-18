#include "AcquireCriticalSection.h"

#include "WorkerThread.h"

namespace paulst
{

WorkerThreadTask::WorkerThreadTask()
    : m_cancelled(false)
{
}
 
void WorkerThreadTask::setCancellationFlag()
{
    AcquireCriticalSection a(m_cs);
    m_cancelled = true;
}

bool WorkerThreadTask::getCancellationFlag()
{
    AcquireCriticalSection a(m_cs);

    {
        return m_cancelled;
    }
}

WorkerThread::WorkerThread()
    : m_thread(this)
{
}

void WorkerThread::cancelCurrentlyExecuting()
{
	AcquireCriticalSection a(m_cs);

	if ( m_queue.size() )
	{
		WorkerThreadTask* t = m_queue.front();
		t->setCancellationFlag();
	}
}

int WorkerThread::queueSize()
{
    AcquireCriticalSection a(m_cs);

    {
        return m_queue.size();
    }
}

void WorkerThread::queueTask(  WorkerThreadTask* task )
{
    AcquireCriticalSection a(m_cs);
    
    {
        m_queue.push_back( task );
    }
}

WorkerThreadTask* WorkerThread::getFirst()
{
    AcquireCriticalSection a(m_cs);

    {
        return m_queue.front();
    }
}

void WorkerThread::removeFirst()
{
    AcquireCriticalSection a(m_cs);

    {
        m_queue.pop_front();
    }
}

void WorkerThread::run( const Event* stopSignal )
{
    try
    {
        while ( WAIT_TIMEOUT == stopSignal->wait(10) )
        {
            if ( 0 == queueSize() )
                continue;
            WorkerThreadTask* t = getFirst();
            bool keepWorking = t->execute();
            removeFirst();
            delete t;
            if ( ! keepWorking )
            {
                break;
            }
        }
    }
    catch( ... )
    {
    }

}

void WorkerThread::waitFor()
{
	WaitForSingleObjectEx( m_thread.getHandle(), INFINITE, false );
}

}

