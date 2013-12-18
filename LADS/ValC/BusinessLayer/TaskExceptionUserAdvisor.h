#ifndef TASKEXCEPTIONUSERADVISORH
#define TASKEXCEPTIONUSERADVISORH

#include "Task.h"

namespace paulst
{
    class LoggingService;
}

namespace valc
{

class UserAdvisor;

class TaskExceptionUserAdvisor : public stef::TaskExceptionHandler
{
public:
    TaskExceptionUserAdvisor( UserAdvisor* userAdvisor, paulst::LoggingService* log );
    void handleException( stef::Task* t, const char* msg ) throw();
    void handleCancellation( stef::Task* t ) throw();
private:
    UserAdvisor* m_userAdvisor;
    paulst::LoggingService* m_log;

};

}

#endif

