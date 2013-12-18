#include "AcquireCriticalSection.h"
#include "Require.h"
#include "Task.h"
#include "TaskQueue.h"

namespace stef
{


class CancelledTask : public Task
{
public:
    void notifyCancelled() {}
protected:
    void doStuff() {}
};


TaskQueue::TaskQueue()
    :
    m_signalWhenQuiet( 0 ),
    m_currentlyExecuting( 0 ),
    m_closedToNewTasks( false )
{
}

bool TaskQueue::add( Task* t )
{
    paulst::AcquireCriticalSection a(m_cs);

    if ( m_closedToNewTasks )
	{
        return false;
    }
    else
    {
		tasks.push_back(t);
        return true;
    }
}

void TaskQueue::deleteTask( Task* t )
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        delete t;

        --m_currentlyExecuting;

        if ( ( 0 == m_currentlyExecuting ) && ( m_signalWhenQuiet ) && tasks.empty() )
        {
            SetEvent( m_signalWhenQuiet );
            m_signalWhenQuiet = NULL;
        }
    }
}

Task* TaskQueue::popTask()
{
    paulst::AcquireCriticalSection a(m_cs);

	{
        Task* t = 0;

        if ( tasks.size() )
        {
            t = tasks.front();
            tasks.pop_front();
            ++m_currentlyExecuting;
        }

        return t;
    }
}


void TaskQueue::closeToNewTasks()
{
    paulst::AcquireCriticalSection a(m_cs);

    m_closedToNewTasks = true;
}

void TaskQueue::signalWhenQuiet( HANDLE hEvent, bool cancelPending )
{
    paulst::AcquireCriticalSection a(m_cs);

	{
        if ( m_signalWhenQuiet )
        {
            return; // Notification has already been requested
        }

        m_signalWhenQuiet = hEvent;

        // Cancel pending tasks by replacing with instances of CancelledTask.
        // (Cannot simply clear the queue because threadpool submissions already made.
        if ( cancelPending && tasks.size() )
        {
            const int numTasks = tasks.size();

            for ( int i = 0; i < numTasks; ++i )
            {
                Task* t = tasks.front();
                tasks.pop_front();
                t->notifyCancelled();
                delete t;
                tasks.push_back( new CancelledTask() );
            }
        }

        if ( ( 0 == m_currentlyExecuting ) && tasks.empty() ) // Otherwise allow doneExecuting to do the signalling
        {
            SetEvent( m_signalWhenQuiet );
            m_signalWhenQuiet = NULL;
        }
    }
}


}

