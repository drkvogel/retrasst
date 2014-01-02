#ifndef TaskH
#define TaskH

#include <SysUtils.hpp>
#include <vector>

namespace stef
{

class Task;

/*
Register one or more TaskExceptionHandlers with a Task in order to get notifications of task execution exceptions.
*/
class TaskExceptionHandler
{
public:
    TaskExceptionHandler();
    virtual ~TaskExceptionHandler();
    virtual void handleException   ( Task* t, const char* msg ) throw() = 0;
    virtual void handleCancellation( Task* t                  ) throw() = 0;
};

/*
Tasks are what ThreadPool runs.
*/
class Task
{
public:
	Task();
	virtual ~Task();
    void registerExceptionHandler( TaskExceptionHandler* teh );
    /*
    'execute' gets invoked by a thread from the ThreadPool.
    */
	void execute();
    /*
    'notifyCancelled' gets invoked by TaskQueue on a Task that is getting pulled from the queue without getting executed.
    */
    void notifyCancelled();

protected:
    /*
    Subclasses implement 'doStuff' in order to get stuff done.
    */
	virtual void doStuff() = 0;
private:
    typedef std::vector<TaskExceptionHandler*> TaskExceptionHandlers;
    TaskExceptionHandlers m_exceptionHandlers;

    void handleException( const char* msg );
};

}

#endif

