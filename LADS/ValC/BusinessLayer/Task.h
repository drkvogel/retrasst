#ifndef TASKH
#define TASKH

#include <boost/scoped_ptr.hpp>
#include <System.hpp>
#include <SysUtils.hpp>
#include "Thread.h"

namespace valc
{

template <class T, class ReturnValue>
class TaskRunner : public paulst::Runnable
{
public:
    TaskRunner( T* task ) 
        : m_task( task )
    {
    }

    void run( const paulst::Event* )
    {
        try
        {
            m_task->execute();
        }
        catch( const std::string& s )
        {
            m_errorMsg = UnicodeString( s.c_str() );
        }
        catch( const Exception& e )
        {
            m_errorMsg = e.Message;
        }
        catch( ... )
        {
            m_errorMsg = L"Unspecified exception";
        }
    }

    ReturnValue releaseReturnValue()
    {
        if ( ! m_errorMsg.IsEmpty() )
        {
            throw Exception( m_errorMsg );
        }

        return m_task->releaseReturnValue(); 
    }

private:
    boost::scoped_ptr< T > m_task;
    UnicodeString m_errorMsg;
};

template <class T, class ReturnValue>
class Task
{
public:
    Task( T* t ) 
        : m_taskRunner(t) 
    {
    } 

    void start()
    {
        m_thread.reset( new paulst::Thread( &m_taskRunner ) );
    }

    ReturnValue waitFor()
    {
        m_thread.reset( 0 );
        return m_taskRunner.releaseReturnValue();
    }

private:
    boost::scoped_ptr< paulst::Thread > m_thread;
    TaskRunner< T, ReturnValue >  m_taskRunner;
};

};

#endif


