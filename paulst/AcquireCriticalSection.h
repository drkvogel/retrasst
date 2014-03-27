#ifndef AcquireCriticalSectionH
#define AcquireCriticalSectionH

#include <windows.h>

namespace paulst
{

class CritSec;   // a friend, can access private members of this class

/** An instance of this class implements the acquiring a lock for a critical section of code.
  * The period from entering the critical section of code to leaving it
  * matches the lifetime of the AcquireCriticalSection object.
  *
  * This class is a wrapper for the EnterCriticalSection and LeaveCriticalSection
  * functions on CRITICAL_SECTION objects. To use it, you could for example declare
  * a private CritSec object cs in the declaration of a class, and then use it 
  * like this in the functions for that class:
  *
  * <pre>
  * void functionName() {
  *     AcquireCriticalSection a(cs);
  *     ... code forming the critical section
  * }
  * </pre> 
  *
  * As the variable a goes out of scope at the end of the method, it gets destroyed,
  * thus releasing the lock that was held whilst the thread was in that critical section.
  */  
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

