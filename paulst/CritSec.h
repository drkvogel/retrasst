#ifndef CRITSECH
#define CRITSECH

#include <windows.h>

namespace paulst
{

/** A wrapper for a CRITICAL_SECTION object (a Microsoft implementation of a lock
  * for use on a critical section of code). 
  *
  * Following the RAII style, 
  * construction of a CritSec object calls InitializeCriticalSection,
  * and destruction calls DeleteCriticalSection.
  *
  * To protect its use as a lock for a critical section of code, instances of this
  * class cannot be copied.
  *
  * This class is useful in conjunction with its friend, the AcquireCriticalSection class,
  * which handles uses of a CritSec lock. For an example of it in use, see the
  * comments for the AcquireCriticalSection class.  
  */
class CritSec
{
public:
    CritSec();
    ~CritSec();

    friend class AcquireCriticalSection;

private:
    CRITICAL_SECTION* m_cs;                // the lock itself 

    CritSec( const CritSec& );             // unimplemented private copy constructor 
    CritSec& operator=( const CritSec& );  // and = operator  prevents an instance of
	                                       // this class being copied
};

};

#endif

