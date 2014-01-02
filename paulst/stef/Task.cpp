#include "Task.h"

namespace stef
{

TaskExceptionHandler::TaskExceptionHandler()
{
}

TaskExceptionHandler::~TaskExceptionHandler()
{
}

Task::Task()
{
}

Task::~Task()
{
}

void Task::registerExceptionHandler( TaskExceptionHandler* teh )
{
    m_exceptionHandlers.push_back(teh);
}

void Task::execute()
{
    try
    {
        doStuff();
    }
    catch( const Exception& e )
    {
         handleException( AnsiString( e.Message.c_str() ).c_str() );
    }
    catch( ... )
    {
        handleException( "Unspecified exception" );
    }
}

void Task::handleException( const char* msg )
{
    for ( TaskExceptionHandlers::const_iterator i = m_exceptionHandlers.begin(); i != m_exceptionHandlers.end(); ++i )
    {
        (*i)->handleException( this, msg );
    }
}

void Task::notifyCancelled()
{
    for ( TaskExceptionHandlers::const_iterator i = m_exceptionHandlers.begin(); i != m_exceptionHandlers.end(); ++i )
    {
        (*i)->handleCancellation( this );
    }
    
}

}

