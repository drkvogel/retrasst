#ifndef AcquireCriticalSectionH
#define AcquireCriticalSectionH

#include <windows.h>

namespace paulst
{

class CritSec;

class AcquireCriticalSection
{
    private:
        CRITICAL_SECTION* m_pcs;

        AcquireCriticalSection( const AcquireCriticalSection& );
        AcquireCriticalSection& operator=( const AcquireCriticalSection& );
    public:
        AcquireCriticalSection(CRITICAL_SECTION *p);
        AcquireCriticalSection(const CritSec& cs);

        ~AcquireCriticalSection();
};

};

#endif

