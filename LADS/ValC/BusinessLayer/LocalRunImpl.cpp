#include "AcquireCriticalSection.h"
#include "LocalRunImpl.h"

namespace valc
{

void LocalRun::Impl::closeOff( const std::string& sampleRunID )
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        m_openRuns.erase( sampleRunID );
    }
}

void LocalRun::Impl::introduce( LocalRun& lr, bool isOpen )
{
    lr.m_impl = this;

    if ( isOpen )
    {
        m_openRuns.insert( lr.getRunID() );
    }
}

bool LocalRun::Impl::isOpen( const std::string& sampleRunID ) const
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        return m_openRuns.count( sampleRunID );
    }
}

}

