#ifndef USERADVISORADAPTERH
#define USERADVISORADAPTERH

#include "AcquireCriticalSection.h"
#include "API.h"
#include "CritSec.h"
#include <string>
#include <vector>

namespace valcui
{

class UserAdvisorAdapter : public valc::UserAdvisor
{
public:
    typedef std::vector< std::string > Cache;

    UserAdvisorAdapter()
    {
    }

    void advise( const std::string& warning )
    {
        paulst::AcquireCriticalSection a(m_cs);
        m_cache.push_back( warning );
    }

    template<class OutputIterator>
    void drainCache( OutputIterator i )
    {
        paulst::AcquireCriticalSection a(m_cs);
        std::copy( m_cache.begin(), m_cache.end(), i );
        m_cache.clear();
    }

private:
    paulst::CritSec m_cs;
    Cache m_cache;
};

}

#endif

