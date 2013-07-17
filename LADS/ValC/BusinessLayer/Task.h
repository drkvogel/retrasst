#ifndef TASKH
#define TASKH

#include <boost/scoped_ptr.hpp>
#include <System.hpp>
#include <SysUtils.hpp>
#include "Thread.h"
#include "ThreadExceptionMsgs.h"

namespace valc
{

/*
Usage: Construct a ThreadTask, call 'start' and wait on the returned HANDLE.

TaskDef must implement 'execute()'.
*/
template <class TaskDef>
class ThreadTask : public paulst::Runnable
{
public:
	ThreadTask( TaskDef* t, ThreadExceptionMsgs * exceptionMsgsOut )
		:
		m_taskDef( t ),
		m_threadExceptionMsgs( exceptionMsgsOut )
    {
    } 

	void run( const paulst::Event* )
	{
		try
		{
			m_taskDef->execute();
		}
		catch( const std::string& s )
		{
			m_threadExceptionMsgs->add( s );
		}
		catch( const Exception& e )
		{
			m_threadExceptionMsgs->add( e.Message );
		}
		catch( ... )
		{
			m_threadExceptionMsgs->add( "Unknown exception." );
		}
	}

	HANDLE start()
    {
		m_thread.reset( new paulst::Thread( this ) );
		return m_thread->getHandle();
	}

private:
    boost::scoped_ptr< paulst::Thread > m_thread;
	boost::scoped_ptr< TaskDef > 		m_taskDef;
	ThreadExceptionMsgs* 				m_threadExceptionMsgs;
};

};

#endif


