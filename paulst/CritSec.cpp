#include "CritSec.h"

namespace paulst
{

/** Constructs a CritSec object by initialising a fresh new CRITICAL_SECTION 
  * object as well as calling the InitializeCriticalSection function on it.
  */
CritSec::CritSec()
    : m_cs( new CRITICAL_SECTION() )
{
    InitializeCriticalSection(m_cs);  // a Windows operation for CRITICAL_SECTION objects
}

/** Destroys the lock and its CritSec wrapper. */
CritSec::~CritSec()
{
    DeleteCriticalSection(m_cs); // a Windows operation for CRITICAL_SECTION objects
    delete m_cs;
}

}

