#include "Require.h"
#include "Thread.h"

#include <process.h>

namespace paulst
{

Event::Event()
    : m_h(0)
{
    m_h = CreateEvent( NULL, TRUE, FALSE, NULL );
    throwUnless (  m_h );
}

Event::~Event()
{
    CloseHandle( m_h );
}

unsigned long Event::wait( unsigned long millis ) const
{
    return WaitForSingleObject( m_h, millis );
}

bool Event::reset()
{
    return ResetEvent( m_h );
}


bool Event::set()
{
    return SetEvent( m_h );
}

Runnable::~Runnable()
{
}

Thread::Thread(Runnable* r)
    : m_thread(0), m_threadId(0)
{
    m_threadStartParams.runnable = r;
    m_threadStartParams.stopSignal = &m_stopSignal;

    m_thread = (HANDLE)_beginthreadex( NULL, 0, &threadStart,
                         &m_threadStartParams, 0, &m_threadId );
    throwUnless ( m_thread );
}

Thread::~Thread()
{
    m_stopSignal.set();
    WaitForSingleObject( m_thread, INFINITE );
    CloseHandle( m_thread );
}

unsigned long Thread::terminate(unsigned long wait)
{
    m_stopSignal.set();
    CancelSynchronousIo( m_thread );
    return wait ? WaitForSingleObject( m_thread, wait ) : 0;    
}

unsigned int __stdcall Thread::threadStart(void *param)
{
    ThreadStartParams* params = (ThreadStartParams*) param;
    params->runnable->run( params->stopSignal );
    return 1;
}

}
