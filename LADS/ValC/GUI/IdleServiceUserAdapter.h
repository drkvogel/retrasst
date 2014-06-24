#ifndef IDLESERVICEUSERADAPTERH
#define IDLESERVICEUSERADAPTERH

#include "IdleService.h"

namespace valcui
{

template<typename T>
class IdleServiceUserAdapter : public IdleServiceUser
{
public:
    IdleServiceUserAdapter( T* t )
        :
        m_t( t )
    {
    }

    void onIdle()
    {
        m_t->onIdle();
    }

private:
    T* m_t;
}; 

}

#endif

