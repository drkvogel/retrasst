#include "Require.h"
#include "SampleRunGroup.h"

namespace valc
{

SampleRunGroup::SampleRunGroup( bool isQC, int groupID, bool canAcceptIDRevision )
    :
    m_isQC( isQC ),
    m_id( groupID ),
    m_canAcceptIDRevision( canAcceptIDRevision )
{
}

bool SampleRunGroup::canAcceptIDRevision( int newID ) const
{
    return ( newID == m_id ) || m_canAcceptIDRevision;
}

int  SampleRunGroup::getID() const
{
    return m_id;
}

bool SampleRunGroup::isQC() const
{
    return m_isQC;
}

void SampleRunGroup::listRunIDs( std::vector< std::string >& out ) const
{
    out.insert( out.end(), m_members.begin(), m_members.end() );
}

void SampleRunGroup::push_back( const std::string& sampleRunID )
{
    m_members.push_back( sampleRunID );
}

void SampleRunGroup::reviseGroupID( int newGroupID )
{
    require( canAcceptIDRevision( newGroupID ) );

    m_id = newGroupID;

    m_canAcceptIDRevision = false;
}

}

