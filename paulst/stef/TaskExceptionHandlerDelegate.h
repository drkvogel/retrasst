#ifndef TASKEXCEPTIONHANDLERDELEGATEH
#define TASKEXCEPTIONHANDLERDELEGATEH

#include "Task.h"

namespace stef
{

template<class T>
class TaskExceptionHandlerDelegate : public TaskExceptionHandler
{
public:
    TaskExceptionHandlerDelegate( T* delegate )
        : m_delegate( delegate )
    {
    }

    void handleCancellation( stef::Task* t ) throw()
    {
        m_delegate->handleCancellation();
    }

    void handleException( stef::Task* t, const char* msg ) throw()
    {
        m_delegate->handleException(msg);
    }

private:
    T* const m_delegate;
};

}

#endif

