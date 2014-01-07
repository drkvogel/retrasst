#include "AcquireCriticalSection.h"
#include "LocalRunImpl.h"
#include "Require.h"
#include "SampleRunGroupModel.h"

namespace valc
{

LocalRun::Impl::Impl()
    :
    m_sampleRunGroupModel(0)
{
}

void LocalRun::Impl::closeOff( const std::string& sampleRunID )
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        m_openRuns.erase( sampleRunID );
    }
}

int LocalRun::Impl::getGroupID( const std::string& sampleRunID )
{
    require( m_sampleRunGroupModel );
    return m_sampleRunGroupModel->getGroupID( sampleRunID );
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

void LocalRun::Impl::setSampleRunGroupModel( SampleRunGroupModel* m )
{
    m_sampleRunGroupModel = m;
}

}

