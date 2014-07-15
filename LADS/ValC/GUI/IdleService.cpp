#include "AcquireCriticalSection.h"
#include <algorithm>
#include "IdleService.h"
#include "Require.h"

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

void IdleService::unregisterUser( IdleServiceUser* u )
{
    paulst::AcquireCriticalSection a(m_critSec);

    auto i = std::find( m_serviceUsers.begin(), m_serviceUsers.end(), u );

    require( i != m_serviceUsers.end() );

    m_serviceUsers.erase( i );
}

}

