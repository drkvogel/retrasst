#ifndef USERADVISORADAPTERH
#define USERADVISORADAPTERH

#include "AcquireCriticalSection.h"
#include "API.h"
#include "CritSec.h"
#include <string>
#include <vector>

namespace valcui
{

template<class Observer>
class UserAdvisorAdapter : public valc::UserAdvisor
{
public:
    typedef std::vector< std::string > Cache;

    UserAdvisorAdapter()
        :
        m_observer(0)
    {
    }

    void advise( const std::string& warning )
    {
        paulst::AcquireCriticalSection a(m_cs);
        m_cache.push_back( warning );
        if ( m_observer )
        {
            m_observer->notifyNewMessage();
        }
    }

    template<class OutputIterator>
    void drainCache( OutputIterator i )
    {
        paulst::AcquireCriticalSection a(m_cs);
        std::copy( m_cache.begin(), m_cache.end(), i );
        m_cache.clear();
    }

    void setObserver( Observer* o )
    {
        paulst::AcquireCriticalSection a(m_cs);
        m_observer = o;
        if ( m_observer && m_cache.size() )
        {
            m_observer->notifyNewMessage();
        }
    }
        
private:
    paulst::CritSec m_cs;
    Cache m_cache;
    Observer* m_observer;
};

}

#endif

