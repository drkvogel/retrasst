#include "CritSec.h"

namespace paulst
{

CritSec::CritSec()
    : m_cs( new CRITICAL_SECTION() )
{
    InitializeCriticalSection(m_cs);
}

CritSec::~CritSec()
{
    DeleteCriticalSection(m_cs);
    delete m_cs;
}

}

