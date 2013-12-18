#ifndef ThreadPoolTESTH
#define ThreadPoolTESTH

#include "AcquireCriticalSection.h"
#include <algorithm>
#include "CritSec.h"
#include "StrUtil.h"
#include "Task.h"
#include <tut.h>
#include <vector>

#include "ThreadPool.h"

class MyCallback : public stef::TaskExceptionHandler
{
public:
    volatile int ok;
    volatile int cancelled;
    std::vector< std::string > exceptions;
    std::vector<int>           runSequence;

    MyCallback()
        : 
        ok(0),
        cancelled(0)
    {
    }

    void notifyOK()
    {   
        paulst::AcquireCriticalSection a(m_cs);
        ++ok;
    }

    void notifyRunning( int id )
    {
        paulst::AcquireCriticalSection a(m_cs);
        runSequence.push_back( id );
    }

    void handleException( stef::Task* t, const char* msg )
    {
        paulst::AcquireCriticalSection a(m_cs);
        exceptions.push_back( msg );
    }

    void handleCancellation( stef::Task* t )
    {
        paulst::AcquireCriticalSection a(m_cs);
        ++cancelled;
    }

private:
    paulst::CritSec m_cs;
};



class MyTask : public stef::Task
{
public:
	MyTask(
        MyCallback* callback,
        int         id,
        bool        throwException = false, 
        int         takeLongTime   = 0 )
        :
        m_id            ( id),
        m_throwException( throwException ),
        m_takeLongTime  ( takeLongTime ),
        m_callback      ( callback )
	{
        registerExceptionHandler( callback );
	}

    ~MyTask()
    {
		//fputs( "~MyTask\n", stdout );
    }

protected:

    void doStuff()
    {
		//printf( "%d:doStuff\n", m_id );

        if ( m_takeLongTime )
        {
            Sleep( m_takeLongTime );
        }

        m_callback->notifyRunning( m_id );

        if ( m_throwException )
        {
            throw Exception("tigger");
        }

        m_callback->notifyOK();
	}

private:
    const bool        m_throwException;
    const int         m_takeLongTime;
    const int         m_id;
    MyCallback*       m_callback;
};


namespace tut
{
	class ThreadPoolTestFixture
    {
    };

    typedef test_group<ThreadPoolTestFixture, 6> ThreadPoolTestGroup;
	ThreadPoolTestGroup testGroupThreadPool( "ThreadPool tests");
	typedef ThreadPoolTestGroup::object testThreadPool;

    template<>
    template<>
	void testThreadPool::test<1>()
    {
        set_test_name("10 quick tasks");

        using namespace stef;

        ThreadPool* s = new ThreadPool();

        const int NUM_TASKS = 10;

        MyCallback callback;

        for ( int i = 0; i < NUM_TASKS; ++i )
        {
            s->addTask( new MyTask(&callback, i) );
        }

        ensure( s->shutdown( 1000, false ) );

        ensure_equals( callback.ok, NUM_TASKS );
        ensure_equals( callback.exceptions.size(), 0 );
    }

    template<>
    template<>
	void testThreadPool::test<2>()
    {
        set_test_name("Task throws exception.");

        using namespace stef;

        ThreadPool* s = new ThreadPool();

        const int NUM_TASKS = 10;
        const int BAD_EGG = 6;
        const std::string expectedErrorMsg("tigger");

        MyCallback callback;


        for ( int i = 0; i < NUM_TASKS; ++i )
        {
            s->addTask( new MyTask( &callback, i, i == BAD_EGG )  );
        }

        ensure( s->waitTillQuiet( 1000 ) );

        ensure_equals( s->getLastError(), expectedErrorMsg );
        s->clearLastError();
        ensure_equals( s->getLastError(), std::string() );

        ensure_equals( callback.ok, NUM_TASKS - 1 );
        ensure_equals( callback.exceptions.size(), 1 );
        ensure_equals( callback.exceptions[0], expectedErrorMsg );

        ensure( s->shutdown( 50, false ) );
    }

    template<>
    template<>
	void testThreadPool::test<3>()
    {
        set_test_name("Task takes a long time.");

        using namespace stef;

        ThreadPool* s = new ThreadPool();

        const int NUM_TASKS = 10;
        const int BAD_EGG = 6;

        MyCallback callback;

        for ( int i = 0; i < NUM_TASKS; ++i )
        {
            s->addTask( new MyTask( &callback, i, false, i == BAD_EGG ? 2000 : 0 ) );
        }

        ensure_not( s->shutdown( 1000, false ) );

        ensure_equals( callback.ok, BAD_EGG );
        ensure_equals( callback.exceptions.size(), 0 );

        Sleep(2000);
        ensure_equals( callback.ok, NUM_TASKS );
    }

    template<>
    template<>
	void testThreadPool::test<4>()
    {
        set_test_name("Variation on 3, with cancellation.");

        using namespace stef;

        ThreadPool* s = new ThreadPool();

        const int NUM_TASKS = 10;
        const int BAD_EGG = 6;

        MyCallback callback;
        ensure_not( callback.cancelled );

        for ( int i = 0; i < NUM_TASKS; ++i )
        {
            s->addTask( new MyTask( &callback, i, false, i == BAD_EGG ? 1000 : 0 ) );
        }

        ensure( s->shutdown( 3000, true ) );

        ensure          ( callback.ok < NUM_TASKS );
        ensure_equals   ( callback.exceptions.size(), 0 );
        ensure          ( callback.cancelled );

        ensure_equals( callback.ok + callback.cancelled, NUM_TASKS );
    }

    template<>
    template<>
	void testThreadPool::test<5>()
    {
        set_test_name("Evidence of interleaving.");

        using namespace stef;

        ThreadPool* tp = new ThreadPool(5,10);

        const int NUM_TASKS = 10;
        const int BAD_EGG = 6;

        MyCallback callback;

        for ( int i = 0; i < NUM_TASKS; ++i )
        {
            tp->addTask( new MyTask( &callback, i, false, i == BAD_EGG ? 20 : 0 ) );
        }

        ensure( tp->shutdown( 1000, false ) );

        ensure_equals( callback.ok, NUM_TASKS );
        ensure_equals( callback.exceptions.size(), 0 );

        ensure_equals( callback.runSequence.size(), NUM_TASKS );
        int sortedOrder[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        ensure_not( std::equal( callback.runSequence.begin(), callback.runSequence.end(), sortedOrder ) );
    }

    template<>
    template<>
	void testThreadPool::test<6>()
    {
        set_test_name("Waiting on empty queue.");

        using namespace stef;

        ThreadPool* tp = new ThreadPool(0,2);

        MyCallback callback;

        ensure    ( tp->waitTillQuiet( 5   ) );

        for ( int i = 0; i < 10; ++i )
        {
            tp->addTask( new MyTask( &callback, i, false, 50 ) );
        }

        ensure_not( tp->waitTillQuiet( 2   ) );
        ensure    ( tp->waitTillQuiet( 500 ) );

        ensure_equals( callback.ok, 10 );
        ensure_equals( callback.exceptions.size(), 0 );

        for ( int i = 10; i < 20; ++i )
        {
            tp->addTask( new MyTask( &callback, i, false, 50 ) );
        }

        ensure_not   ( tp->waitTillQuiet( 2   ) );
        ensure       ( tp->waitTillQuiet( 500 ) );
        ensure_equals( callback.ok, 20 );
        ensure_equals( callback.exceptions.size(), 0 );
    }

};
 
#endif

