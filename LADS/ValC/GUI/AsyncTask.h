#ifndef ASYNCTASKH
#define ASYNCTASKH

#include "ModelEventConstants.h"
#include "ModelEventListener.h"
#include <string>
#include <windows.h>

namespace stef
{
    class ThreadPool;
}

namespace valcui
{

class ModelEventListener;

class AsyncTask
{
public:
    AsyncTask();
    virtual ~AsyncTask();
    virtual std::string getDesc() const = 0;
    /*
    Returns a HANDLE which becomes signalled on completion.
    Expectation is that this method is invoked on the main UI thread.
    */
    virtual HANDLE queue( stef::ThreadPool* tp ) = 0;
    /*
    Invoked on receiving notification that the HANDLE returned by 'queue' is 
    in a signalled state, or following a time-out - i.e. when the wait for this transition to the signalled 
    state is taking too long.
    Expectation is that this method is invoked on the main UI thread, which ensures that callbacks to ModelEventListeners are 
    also on the main UI thread.
    */
    virtual void callback( ModelEventListener* eventSink ) = 0;

private:
    AsyncTask( const AsyncTask& );
    AsyncTask& operator=( const AsyncTask& );
};

template<typename TaskSubmission>
bool succeeded( TaskSubmission* task, ModelEventListener* eventSink )
{
    bool ok = false;

    if ( ! task->completed() )
    {
        eventSink->notify( MODEL_EVENT::ASYNC_TASK_ERROR, std::string("Operation did not complete") );
    }
    else if ( task->cancelled() )
    {
        eventSink->notify( MODEL_EVENT::ASYNC_TASK_ERROR, std::string("Operation cancelled") );
    }
    else if ( task->error().size() )
    {
        eventSink->notify( MODEL_EVENT::ASYNC_TASK_ERROR, task->error() );
    }
    else
    {
        ok = true;
    }

    return ok;
}

}

#endif

