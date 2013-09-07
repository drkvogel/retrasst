#ifndef CRITSECH
#define CRITSECH

#include <windows.h>

namespace paulst
{

class CritSec
{
public:
    CritSec();
    ~CritSec();

    friend class AcquireCriticalSection;

private:
    CRITICAL_SECTION* m_cs;

    CritSec( const CritSec& );
    CritSec& operator=( const CritSec& );
};

};

#endif

