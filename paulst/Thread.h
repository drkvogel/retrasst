#ifndef ThreadH
#define ThreadH

//#include <winbase.h>
#include <windows.h>

namespace paulst
{

class Event
{
    public:
        // Throws exception if construction fails
        Event();
        ~Event();
        unsigned long wait( unsigned long millis ) const;
        bool reset();
        bool set();
        
    private:
        HANDLE m_h;

        Event( const Event& );
        Event& operator=( const Event& );
};

class Runnable
{
    public:
        virtual ~Runnable();
        virtual void run( const Event* stopSignal ) = 0;
};



class Thread
{
    public:
        // Throws exception if construction fails
        Thread(Runnable* r);
		~Thread();
        /*  Returns the thread handle.

            The motivation for making the thread handle available was:

                I had a couple of threads executing in parallel. I wanted to wait till both had finished. With
                access to the HANDLE, I can use WaitForMultipleObjectsEx.

            Note that there is no need to Close this HANDLE. The Thread destructor takes care of that.
        */
		HANDLE getHandle() const;
        unsigned long terminate( unsigned long howLongToWaitMillis );

    private:
        Event m_stopSignal;
        HANDLE m_thread;
        unsigned int m_threadId;
        Runnable* r;

        struct ThreadStartParams
        {
            Runnable* runnable;
            const Event* stopSignal;
        }
        m_threadStartParams;

        Thread( const Thread& );
        Thread& operator=( const Thread& );

        static unsigned int __stdcall threadStart(void *param);

};


};

#endif
