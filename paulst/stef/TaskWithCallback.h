#ifndef TASKWITHCALLBACKH
#define TASKWITHCALLBACKH

#include "MeetingPlace.h"
#include "Require.h"
#include "TaskExceptionHandlerDelegate.h"
#include "ThreadPool.h"
#include "Trace.h"

namespace stef
{


template<class OutputType>
struct TaskOutput
{
    OutputType out;
    bool completed, cancelled;
    std::string error;

    TaskOutput() : cancelled(false), completed(false) { }

};


template<class OutputType>
struct Callback
{
    TaskOutput<OutputType> output;

    Callback()
        : m_completionSignal( CreateEvent( NULL, TRUE, FALSE, NULL ) )
    {
        throwUnless( m_completionSignal );
    }

    ~Callback()
    {
        CloseHandle( m_completionSignal );
        trace("~Callback");
    }

    HANDLE completionSignal()
    {
        return m_completionSignal;
    }

    void notify( TaskOutput<OutputType>& out ) 
    {
        output = out;

        output.completed = true;

        SetEvent( m_completionSignal );
    }
    
    unsigned long wait( unsigned long millis )
    {
        return event.wait( millis );
    } 

private:
    HANDLE m_completionSignal;

    Callback( const Callback& );
    Callback& operator=( const Callback& );
};


struct TaskProxy
{
    TaskProxy()
    {
    }

    ~TaskProxy()
    {
        trace("~TaskProxy");
    }

    void notify( int& i ) { }

private:
    TaskProxy( const TaskProxy& );
    TaskProxy& operator=( const TaskProxy& );
};


template<class MeetingPlaceType, class AttendeeType, class PartingMsgType>
class AttendanceHandle
{
private:
    MeetingPlaceType*        m_meetingPlace;
    AttendeeType*            m_attendee;

    AttendanceHandle( const AttendanceHandle& );
    AttendanceHandle& operator=( const AttendanceHandle& );

public:
    PartingMsgType partingMsg;

    AttendanceHandle( MeetingPlaceType* meetingPlace, AttendeeType* attendee )
        :
        m_meetingPlace(meetingPlace),
        m_attendee(attendee)
    {
        m_meetingPlace->arrive( attendee );
    }

    ~AttendanceHandle()
    {
        m_meetingPlace->leave( m_attendee, partingMsg );
        delete m_attendee;
    }
};

template<class Function, class AttendanceHandleType>
class TaskWithCallback : public Task
{
public:

    TaskWithCallback( Function& f, AttendanceHandleType* h )
        :
        m_function(f),
        m_exceptionHandlerDelegate( this ),
        m_attendanceHandle(h)
    {
        registerExceptionHandler( &m_exceptionHandlerDelegate );
    }

    ~TaskWithCallback()
    {
        delete m_attendanceHandle; // causes the dispatch of any parting msg
    }

    void handleCancellation()
    {
        m_attendanceHandle->partingMsg.cancelled = true;
    }

    void handleException( const char* msg )
    {
        m_attendanceHandle->partingMsg.error = msg;
    }

protected:
    void doStuff()
    {
        m_function( m_attendanceHandle->partingMsg.out );
    }

private:
    TaskExceptionHandlerDelegate<TaskWithCallback<Function,AttendanceHandleType> > m_exceptionHandlerDelegate;
    Function m_function;
    AttendanceHandleType* m_attendanceHandle;
};


template < class T /* Raw output type*/, class F /* Function that the task will perform*/ >
struct Submission
{
    typedef paulst::MeetingPlace<TaskProxy, Callback<T>, TaskOutput<T>, int> MeetingPlaceType;
    typedef AttendanceHandle<MeetingPlaceType, Callback<T>, int          > AttendanceHandle_Callback;  
    typedef AttendanceHandle<MeetingPlaceType, TaskProxy,   TaskOutput<T> > AttendanceHandle_Task;
private:
    Callback<T>* callback;
    AttendanceHandle_Callback*  callbackHandle;
public:

    Submission( F& func, ThreadPool* pool )
    {
        MeetingPlaceType* meetingPlace = new MeetingPlaceType();

        // The 2 parties to the meeting
        TaskProxy* taskProxy = new TaskProxy();
        callback  = new Callback<T>();

        // Both parties join the meeting
        callbackHandle                         = new AttendanceHandle_Callback( meetingPlace, callback );
        AttendanceHandle_Task* taskProxyHandle = new AttendanceHandle_Task    ( meetingPlace, taskProxy);

        pool->addTask( new TaskWithCallback<F,AttendanceHandle_Task>(func, taskProxyHandle) );
    }

    ~Submission()
    {
        delete callbackHandle;// unregister from meeting place - no null ptr callbacks
    }

    // Only safe to call if completion has been signalled.
    bool completed() const
    {
        return callback->output.completed;
    }
    
    // Only safe to call if completion has been signalled.
    bool cancelled() const
    {
        return callback->output.cancelled;
    }

    // Only safe to call if completion has been signalled.
    std::string error() const
    {
        return callback->output.error;
    }

    // Only safe to call if completion has been signalled.
    const T& returnValue() const
    {
        return callback->output.out;
    }

    HANDLE completionSignal()
    {
        return callback->completionSignal();
    }
};

}

#endif

