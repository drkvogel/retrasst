#include "AcquireCriticalSection.h"
#include "IdleService.h"

namespace valcui
{

IdleServiceUser::IdleServiceUser()
{
}

IdleServiceUser::~IdleServiceUser()
{
}

IdleService::IdleService()
{
}

void IdleService::onIdle()
{
    paulst::AcquireCriticalSection a(m_critSec);

    for ( IdleServiceUser* u : m_serviceUsers )
    {
        u->onIdle();
    }
}

void IdleService::registerUser( IdleServiceUser* u )
{
    paulst::AcquireCriticalSection a(m_critSec);

    m_serviceUsers.push_back( u );
}

}

