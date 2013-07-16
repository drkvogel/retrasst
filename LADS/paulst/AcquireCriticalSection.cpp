#include "AcquireCriticalSection.h"
#include "CritSec.h"

namespace paulst
{

AcquireCriticalSection::AcquireCriticalSection(CRITICAL_SECTION *p)
 : m_pcs(p)
{
    EnterCriticalSection(m_pcs);
}

AcquireCriticalSection::AcquireCriticalSection(const CritSec& cs)
 : m_pcs(cs.m_cs)
{
    EnterCriticalSection(m_pcs);
}


AcquireCriticalSection::~AcquireCriticalSection()
{
    LeaveCriticalSection(m_pcs);
}

}

