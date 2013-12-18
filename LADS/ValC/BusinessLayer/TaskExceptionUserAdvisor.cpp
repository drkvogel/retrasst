#include "API.h"
#include "LoggingService.h"
#include "TaskExceptionUserAdvisor.h"

namespace valc
{

TaskExceptionUserAdvisor::TaskExceptionUserAdvisor( UserAdvisor* userAdvisor, paulst::LoggingService* log )
    :
    m_userAdvisor( userAdvisor ),
    m_log        ( log         )
{
}

void TaskExceptionUserAdvisor::handleException( stef::Task* t, const char* msg ) throw()
{
    m_log->log( msg );
    m_userAdvisor->advise( msg );
}

void TaskExceptionUserAdvisor::handleCancellation( stef::Task* t ) throw()
{
    const char* msg = "Task cancelled";
    m_log->log( msg );
    m_userAdvisor->advise( msg );
}

}

