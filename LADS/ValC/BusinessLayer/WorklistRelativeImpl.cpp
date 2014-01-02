#include "WorklistRelativeImpl.h"
#include "WorklistLinks.h"

namespace valc
{
WorklistRelative::Impl::Impl( WorklistLinks* worklistLinks )
    :
    m_worklistLinks( worklistLinks )
{
}

bool WorklistRelative::Impl::hasChildren( int worklistID ) const
{
    return m_worklistLinks->hasChildren( worklistID );
}

bool WorklistRelative::Impl::hasParent( int worklistID ) const
{
    return m_worklistLinks->hasParent( worklistID );
}

WorklistRelative WorklistRelative::Impl::getParent( int worklistID ) const
{
    WorklistRelative wr = m_worklistLinks->getParent( worklistID );
    wr.m_impl = this;
    return wr;
}

std::vector<WorklistRelative> WorklistRelative::Impl::getChildren( int worklistID ) const
{
    std::vector<WorklistRelative> wrs = m_worklistLinks->getChildren( worklistID );

    for ( std::vector<WorklistRelative>::iterator i = wrs.begin(); i != wrs.end(); ++i )
    {
        i->m_impl = this;
    }

    return wrs;
}

WorklistRelative WorklistRelative::Impl::wrap( const WorklistEntry* e ) const
{
    WorklistRelative wr = m_worklistLinks->wrap( e );
    wr.m_impl = this;
    return wr;
}

}

