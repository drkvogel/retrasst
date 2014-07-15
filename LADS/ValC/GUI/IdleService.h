#ifndef IDLESERVICEH
#define IDLESERVICEH

#include "CritSec.h"
#include <vector>

namespace valcui
{

class IdleServiceUser
{
public:
    IdleServiceUser();
    virtual ~IdleServiceUser();
    virtual void onIdle() = 0;
private:
    IdleServiceUser( const IdleServiceUser& );
    IdleServiceUser& operator=( const IdleServiceUser& );
};

class IdleService
{
public:
    IdleService();
    void onIdle();
    void registerUser( IdleServiceUser* u );
    void unregisterUser( IdleServiceUser* u );
private:
    paulst::CritSec m_critSec;
    std::vector< IdleServiceUser* > m_serviceUsers;

    IdleService( const IdleService& );
    IdleService& operator=( const IdleService& );
};

}

#endif

