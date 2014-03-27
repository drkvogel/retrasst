#include "Require.h"
#include "Thread.h"

#include <process.h>

namespace paulst
{

Runnable::~Runnable()
{
}

Thread::Thread(Runnable* r)
    : m_thread(0), m_threadId(0)
{
    m_threadStartParams.runnable = r;
    m_threadStartParams.stopSignal = &m_stopSignal;

	m_thread = (HANDLE) _beginthreadex( NULL, 0, &threadStart,
						 &m_threadStartParams, 0, &m_threadId );
    throwUnless ( m_thread );
}

Thread::~Thread()
{
    m_stopSignal.set();
    WaitForSingleObject( m_thread, INFINITE );
    CloseHandle( m_thread );
}

HANDLE Thread::getHandle() const
{
	return m_thread;
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
