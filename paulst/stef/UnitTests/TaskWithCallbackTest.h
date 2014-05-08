#ifndef TASKWITHCALLBACKTESTH
#define TASKWITHCALLBACKTESTH

#include <boost/shared_ptr.hpp>
#include <iostream>
#include "TaskWithCallback.h"
#include <tut.h>

struct ErrFunc
{
    std::string errorMsg;

    ErrFunc( const std::string& msg ) : errorMsg(msg) {}

    void operator()( int& i ) 
    { 
        throw Exception(errorMsg.c_str()); 
    }
};

struct StrFunc
{
    std::string in;

    StrFunc( const std::string& s ) : in(s) {}

    void operator()( std::string& out )
    {
        out = in;
    }
};

struct SlowFunc
{
    unsigned long millis;

    SlowFunc( unsigned long howSlowMillis ) : millis( howSlowMillis) {}

    void operator()( unsigned long& out )
    {
        Sleep(millis);
        out = millis;
        std::cout << "Phew. Got there in the end!" << std::endl;
    }
};
        

void shutdownPool( stef::ThreadPool* p )
{
    p->shutdown( 100, true );
}

unsigned long waitFor( HANDLE event, unsigned long millis )
{
    return WaitForSingleObject( event, millis );
}

namespace tut
{
	class TaskWithCallbackTestFixture
    {
    };

    typedef test_group<TaskWithCallbackTestFixture, 3> TaskWithCallbackTestGroup;
	TaskWithCallbackTestGroup testGroupTaskWithCallback( "TaskWithCallback tests");
	typedef TaskWithCallbackTestGroup::object testTaskWithCallback;

    template<>
    template<>
	void testTaskWithCallback::test<1>()
    {
        set_test_name("simple test");

        using namespace stef;

        StrFunc func("Hey!");

        ThreadPool* pool = new ThreadPool();

        boost::shared_ptr<void> shutdownOnExit(pool,shutdownPool);

        Submission<std::string,StrFunc> s( func, pool );

        ensure_equals( waitFor( s.completionSignal(), 1000 ), WAIT_OBJECT_0  );
        ensure    ( s.completed() );
        ensure_not( s.cancelled() );
        ensure    ( s.error().empty() );
        const std::string& returnValue = s.returnValue();
        ensure_equals( returnValue, "Hey!" );
    }

    template<>
    template<>
	void testTaskWithCallback::test<2>()
    {
        set_test_name("exception thrown");

        using namespace stef;

        ErrFunc func("Oh dear!");

        ThreadPool* pool = new ThreadPool();

        boost::shared_ptr<void> shutdownOnExit(pool,shutdownPool);

        Submission<int,ErrFunc> s( func, pool );

        ensure_equals( waitFor( s.completionSignal(), 1000 ), WAIT_OBJECT_0  );
        ensure    ( s.completed() );
        ensure_not( s.cancelled() );
        ensure    ( s.error().size() );
        ensure_equals( s.error(), "Oh dear!" );
    }

    template<>
    template<>
	void testTaskWithCallback::test<3>()
    {
        set_test_name("slow function");

        using namespace stef;

        SlowFunc func(1000 * 10);

        ThreadPool* pool = new ThreadPool();

        boost::shared_ptr<void> shutdownOnExit(pool,shutdownPool);

        Submission<unsigned long,SlowFunc> s( func, pool );

        ensure_equals( waitFor( s.completionSignal(), 1000 ), WAIT_TIMEOUT  );
    }

}

#endif

