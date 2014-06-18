#include "AcquireCriticalSection.h"
#include <algorithm>
#include <boost/bind.hpp>
#include <functional>
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

void LocalRun::Impl::closeOff( const IDToken& sampleRunID )
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        auto i = m_openRuns.find( sampleRunID );

        while ( i != m_openRuns.end() )
        {
            m_openRuns.erase(i);
            i = m_openRuns.find( sampleRunID );
        }
    }
}

int LocalRun::Impl::getGroupID( const IDToken& sampleRunID )
{
    require( m_sampleRunGroupModel );
    return m_sampleRunGroupModel->getGroupID( sampleRunID );
}

void LocalRun::Impl::introduce( LocalRun& lr, bool isOpen )
{
    lr.m_impl = this;

    if  ( isOpen && ! m_openRuns.contains( lr.getRunID() ) )
    {
        m_openRuns.push_back( lr.getRunID() );
    }
}

bool LocalRun::Impl::isOpen( const IDToken& sampleRunID ) const
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        return m_openRuns.contains( sampleRunID );
    }
}

void LocalRun::Impl::setSampleRunGroupModel( SampleRunGroupModel* m )
{
    m_sampleRunGroupModel = m;
}

}

